#include "../../core/framework/SwiftWorker.h"
#include "../../core/AsynExec.h"
#include "common.h"
#include "param.h"
#include "access_method.h"
using namespace swift_snails;
using namespace fms;

struct rcd_t {
    typedef std::vector<std::pair<index_t, bool>> feas_t;
    feas_t feas;
};

class SkipGram : public BaseAlgorithm<index_t, Word2VecParam,  Word2VecGrad, rcd_t > {
public:
    typedef index_t          key_t;
    typedef Word2VecParam   val_t;
    typedef Word2VecGrad    grad_t;
    using param_cache_t = GlobalParamCache<key_t, val_t, grad_t> ;

    SkipGram(int num_iters, int len_vec, int window, int negative) : \
        _num_iters(num_iters),
        window(window), 
        negative(negative), 
        len_vec(len_vec),
        param_cache(global_param_cache<key_t, val_t, grad_t>())
    { 
        //_num_iters = global_config().get_config("num_iters").to_int32();
        learning_rate = global_config().get_config("learning_rate").to_float();
    }

    virtual void train() {
        // init local keys
        //{ rwlock_read_guard lk(param_cache.rwlock());
            for(auto& item : param_cache.params()) {
                local_keys.push_back(item.first);
            }
        //}

        for(int i = 0; i < _num_iters; i ++) {
            LOG(WARNING) << i << " th iteration";
            train_iter(_async_channel_thread_num);
            param_cache.inc_num_iters();
        }
    }

    rcd_t parse_record(const std::string &line) {
        rcd_t res;
        auto fields = std::move(split(line, " "));
        try {
            for(std::string & f : fields) {
                res.feas.push_back(std::make_pair(std::stoi(f), true));
            }
        } catch (...) {
            //RAW_LOG(INFO, "wrong record detected!");
        }
        if(res.feas.size() < 5) {
            res.feas.clear();
        }
        return std::move(res);
    }

private:
    /*
     * data format:
     *  wordid wordid wordid
     */
    void train_iter(int thread_num) {
        LOG(INFO) << "train file with " << thread_num << " threads";
        const int line_buffer_queue_capacity = global_config().get_config("line_buffer_queue_capacity").to_int32();
        queue_with_capacity<std::string> queue(line_buffer_queue_capacity);
        FILE* file = std::fopen(data_path().c_str(), "r");
        CHECK_NOTNULL(file);

        auto trainner = [this, &queue] {
            for(;;) {
                std::string line;
                queue.wait_and_pop(line);
                if(line.empty()) break;
                rcd_t rcd = parse_record(line);
                // TODO add config for shortest sentence
                if(rcd.feas.size() > 4) {
                    learn_record(std::move(rcd.feas));
                }
            }
        };
        auto producer = [this, &queue, file, thread_num] {

            LineFileReader line_reader; 

            while(line_reader.getline(file)) {
                std::string line = line_reader.get();
                queue.push(std::move(line));
                //RAW_DLOG(INFO, "line_reader push a line to queue");
            }
            queue.end_input(thread_num, "");
        };
        std::thread t (std::move(producer));

        async_exec(thread_num, std::move(trainner), async_channel());
        //line_trainner();
        //RAW_LOG(INFO, ">  after asyn_exec, to join producer thread");
        // display parameters
        /*
        for(auto& it :  param_cache.params()) {
            RAW_DLOG(INFO,  "%lu:\t%s", it.first, it.second.v().to_str().c_str());
        }
        */
        t.join();
        std::fclose(file);
    }

    void learn_record(rcd_t::feas_t && wids) {
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> rand(0, window);
        Vec labels(negative + 1);
        labels[0] = 1.0;


        for(int pos = 0; pos < wids.size(); pos ++) {
            int reduced_window = rand(rng);
            int start = std::max(0, pos - window + reduced_window);
            key_t& word = wids[pos].first;

            for(size_t pos2 = start; pos2 < pos + window + 1 - reduced_window; pos2 ++){
                key_t& word2 = wids[pos2].first;

                if( ! pos2 == pos) {
                    train_sg_pair(word, word2, labels);
                }
            }
        }
    }

    void train_sg_pair(key_t word, key_t word2, const Vec& labels, bool train_w1=true, bool train_w2=true) {
        CHECK_GT(local_keys.size(), 0) << "local_keys should be inited";
        //RWLock& rwlock = param_cache.rwlock();
        Vec l1;
        //{ rwlock_read_guard lk(rwlock);
            l1 = param_cache.params()[word2].v();
        //}

        Vec neu1e = Vec(len_vec);
        // generate noise         
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> rand(0, local_keys.size());

        std::vector<key_t> word_indices;
        word_indices.push_back(word);
        while(word_indices.size() < negative + 1) {
            key_t id = rand(rng);
            key_t w = local_keys[id];
            if (w != word) word_indices.push_back(w);
        }
        // prepare l2b
        // negative hidden vector of the selected words
        std::vector<Vec> l2b;
        for(size_t i = 0; i < word_indices.size(); ++i ) {
            key_t id = word_indices[i];
            Vec h;
            //{ rwlock_read_guard lk(rwlock);
            h = param_cache.params()[id].h();
            //}

            //RAW_DLOG(INFO,  "h: %s", h.to_str().c_str());
            l2b.push_back(std::move(h));
            //CHECK_GT(l2b[i].size(), 0) << "after push_back((h)";
        }
        // prepare fb
        Vec fb(word_indices.size());
        for(size_t i = 0 ; i < word_indices.size(); i++) {
            CHECK_GT(l2b[i].size(), 0);
            fb[i] = 1.0 / (1.0 + 1.0 / exp(l1.dot(l2b[i])));
        }

        //CHECK_EQ(labels.size(), fb.size());
        Vec gb = (labels - fb);// * learning_rate;

        //RAW_DLOG(INFO, "%s", gb.to_str().c_str());

        auto Outer = outer(gb, l1);

        if(train_w1) {
            for(size_t i = 0; i < word_indices.size(); i++) {
                const auto wid = word_indices[i];
                // update word vector
                auto &_h = param_cache.params()[wid].h();
                CHECK_EQ(_h.size(), Outer[i].size()); 

                //{ rwlock_write_guard lk(rwlock); 
                    _h += Outer[i] * learning_rate;
                    param_cache.grads()[wid].accu_h(Outer[i]);
                //}
            }
        }

        Vec gb_dot_l2b(len_vec);
        for(size_t i = 0; i < len_vec; i++) {
            Vec l2b_T(word_indices.size());
            for(size_t j = 0; j < word_indices.size(); j++) {
                l2b_T[j] = l2b[j][i]; 
            }
            gb_dot_l2b[i] = gb.dot(l2b_T);
        }
        neu1e += gb_dot_l2b;

        if(train_w2) {
            //rwlock_write_guard(param_cache.rwlock());
            // update v
            param_cache.params()[word2].v() += neu1e * learning_rate;
            param_cache.grads()[word2].accu_v(neu1e);
        }
    }


private:
    int window{0};
    int negative{0};
    int len_vec{0};
    int _num_iters{0};
    std::random_device rd;
    param_cache_t &param_cache;
    std::vector<key_t> local_keys;
    float learning_rate = 0.01;

};  // class Word2Vec




