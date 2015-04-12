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
    using param_cache_t = GlobalParamCache<key_t, val_t, grad_t>;
    using pull_access_t = GlobalPullAccess<key_t, val_t, grad_t>;
    using push_access_t = GlobalPushAccess<key_t, val_t, grad_t>;

    SkipGram(int num_iters, int len_vec, int window, int negative) : \
        _num_iters(num_iters),
        window(window), 
        negative(negative), 
        len_vec(len_vec),
        pull_access(global_pull_access<key_t, val_t, grad_t>()),
        push_access(global_push_access<key_t, val_t, grad_t>())
    { 
        learning_rate = global_config().get_config("learning_rate").to_float();
        sample = global_config().get_config("sample").to_float();
        minibatch = global_config().get_config("minibatch").to_int32();
    }

    virtual void train() {
        get_word_freq(_async_channel_thread_num);
        init_unigram_table();

        for(int i = 0; i < _num_iters; i ++) {
            LOG(WARNING) << i << " th iteration";
            train_iter(_async_channel_thread_num);
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
        // file read buffer queue
        queue_with_capacity<std::string> queue(line_buffer_queue_capacity);
        // computation channel
        FILE* file = std::fopen(data_path().c_str(), "r");
        CHECK_NOTNULL(file);

        double global_error = 0;
        size_t error_counter = 0;
        size_t bad_error_counter = 0;

        auto trainer = [this, &global_error, &error_counter, &bad_error_counter] 
        (std::vector<rcd_t::feas_t> &batch, param_cache_t &param_cache, std::unordered_set<key_t> &local_keys) 
        {
            double error;
            pull_access.pull_with_barrier(local_keys, param_cache);
            for(auto& feas : batch) {
                error = learn_record(std::move(feas), param_cache);
                global_error += error;
                error_counter ++;
            }
            // finish mini-batch training 
            // and PUSH the grads
            push_access.push_with_barrier(local_keys, param_cache);
            local_keys.clear();
            batch.clear();
            param_cache.clear();
        };

        auto minibatch_task = [this, &queue, &global_error, &error_counter, &trainer] {
            std::vector<rcd_t::feas_t> _minibatch;
            std::unordered_set<key_t> local_keys;
            param_cache_t param_cache;  // local param
            
            for(;;) {
                // get one line from queue
                std::string line;
                queue.wait_and_pop(line);
                if(line.empty()) break;

                rcd_t rcd = parse_record(line);
                // TODO add config for shortest sentence
                if(rcd.feas.size() > 4) {
                    for(const auto& key : rcd.feas) {
                        local_keys.insert(key.first);
                    }
                    _minibatch.push_back(std::move(rcd.feas));
                    if(_minibatch.size() == minibatch) {
                        // pull parameters
                        trainer(_minibatch, param_cache, local_keys);
                    }
                }
            } // end for

            if(! _minibatch.empty()) {
                trainer(_minibatch, param_cache, local_keys);
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

        async_exec(thread_num, std::move(minibatch_task), async_channel());

        t.join();
        std::fclose(file);

        RAW_LOG(INFO, ">  train error:\t%f\tbad_error:\t", global_error/error_counter, bad_error_counter);
    }

    double learn_record(rcd_t::feas_t && sen, param_cache_t &param_cache) {
        long long a, b, d, word, last_word;
        size_t sentence_length = sen.size(), 
                sentence_position = 0;
        long long word_count = 0, last_word_count = 0;
        long long l1, l2, c, target, label;
        double f, g;
        clock_t now;

        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> int_rand(0, table_size);
        std::default_random_engine float_gen;
        std::uniform_real_distribution<double> float_rand(0.0, 1.0);

        Vec neu1(len_vec);
        Vec neu1e(len_vec);

        double global_error = 0;
        size_t error_counter = 0;

        while(true) {
            word =  sen[sentence_position].first;

            if(sample > 0) {
                double keep = sqrt(sample / word_freg[word]) + sample / word_freg[word];
                double rand = float_rand(float_gen);
                if( keep <  rand) continue;
            }

            for (c = 0; c < len_vec; c++) neu1[c] = 0;
            for (c = 0; c < len_vec; c++) neu1e[c] = 0;

            b = int_rand(rng) % window;

            for (a = b; a < window * 2 + 1 - b; a++) if (a != window) {
                c = sentence_position - window + a;
                if (c < 0) continue;
                if (c >= sentence_length) continue;
                last_word = sen[c].first;
                //if (last_word == -1) continue;
                Vec &v1 = param_cache.params()[last_word].v();
                for (c = 0; c < len_vec; c++) neu1e[c] = 0;

                for (d = 0; d < negative + 1; d++) {
                    // prepare label and vector
                    if (d == 0) {
                        target = word;
                        label = 1;
                    } else {
                        int target_indent = int_rand(rng);
                        if (target_indent == 0) target_indent = int_rand(rng);
                        target = table2word_freq[ table[target_indent] ].first;
                        if (target == word) continue;
                        label = 0;
                    }

                    Vec &v2 = param_cache.params()[target].h();
                    f = 1.0 / ( 1 + exp(- v1.dot(v2)));
                    g = (label - f);
                    // calculate error
                    double error = label == 0 ? - log(1-f) : -log(f);
                    if(error > 0.00000001 && error < 9000) {
                        global_error += error;
                        error_counter ++;
                    } else {
                        //bad_error_counter ++;
                    }
                    neu1e += (g * v2); // * learning_rate

                    Vec grad_v2 = std::move(g * v1);
                    v2 += learning_rate * grad_v2;
                    param_cache.grads()[target].accu_h(grad_v2);
                } // end for
                // Learn weights input -> hidden
                v1 += learning_rate * neu1e;
                param_cache.grads()[last_word].accu_v(neu1e);
            }
            sentence_position ++;
            if (sentence_position >= sentence_length) {
                break;
            }
        }   
        return error_counter == 0 ? 0 : global_error / error_counter;
    }


    void get_word_freq(int thread_num) {
        LOG(WARNING) << "... get_word_freq";
        DLOG(INFO) << "start " << thread_num << " threads to gather keys";
        CHECK_GT(thread_num, 0);
        // make sure the following task wait for the init period
        FILE* file = std::fopen(data_path().c_str(), "r");
        CHECK(file) << "file: open " << data_path() << " failed!";
        std::mutex file_mut;

        //std::set<key_t> keys;
        std::mutex keys_mut;

        std::function<void(const std::string& line)> handle_line \
            = [this, &keys_mut] (const std::string& line) {
                auto rcd = parse_record(line);
                std::lock_guard<std::mutex> lk(keys_mut);
                for(auto &item : rcd.feas) {
                    word_freg[item.first] ++;
                }
            };

        AsynExec::task_t task = [file, &file_mut, handle_line] {
            auto _handle_line = handle_line;
            scan_file_by_line(file, file_mut, std::move(_handle_line) );
        };

        async_exec(thread_num, std::move(task), async_channel());
        std::fclose(file);
    }

    void init_unigram_table() {
        LOG(INFO) << "... init_unigram_table";
        CHECK_GT(word_freg.size(), 0) << "word_freg should be inited before";
        int a, i;
        double train_words_pow = 0;
        double d1, power = 0.75;
        table.reset( new int[table_size]);
        table2word_freq.reset( new std::pair<key_t, int>[word_freg.size()]);
        
        i = 0;
        for(const auto& item : word_freg) {
            table2word_freq[i++] = item;
            train_words_pow += std::pow(item.second, power);
        }
        i = 0;
        d1 = pow(table2word_freq[i].second, power) / (double)train_words_pow;
        for(a = 0; a < table_size; a++) {
            table[a] = i;
            if(a / (double)table_size > d1) {
                i++;
                d1 += pow(table2word_freq[i].second, power) / (double)train_words_pow;
            }
            if(i >= word_freg.size()) i = word_freg.size() - 1;
        }
    }

private:
    int window{0};
    int negative{0};
    int len_vec{0};
    int _num_iters{0};
    std::random_device rd;
    //param_cache_t &param_cache;
    pull_access_t &pull_access;
    push_access_t &push_access;
    //std::vector<key_t> local_keys;
    float learning_rate = 0.01;
    std::map<key_t, int> word_freg;
    std::unique_ptr<int[]> table;
    std::unique_ptr<std::pair<key_t, int>[]> table2word_freq;
    int table_size = 1e8;
    float sample = 0;
    int minibatch = 0;
};  // class Word2Vec
