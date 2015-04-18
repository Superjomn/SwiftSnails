#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "../../utils/all.h"
#include "./param.h"
#include "./access_method_3.h"
#include "../../core/parameter/global_param_cache.h"

#define MAX_STRING 100
#define EXP_TABLE_SIZE 1000
#define MAX_EXP 6
#define MAX_SENTENCE_LENGTH 1000
#define MAX_CODE_LENGTH 40
using namespace std;
using namespace swift_snails;
typedef float real_t;                    // Precision of float numbers
const int table_size = 1e8;
using namespace std;

// total number of words in dataset
size_t train_words = 0;
long long file_size = 0;
long long word_count_actual = 0;
clock_t start;

std::vector<size_t> parse_record(const std::string &line, int min_len = 4) {
	std::vector<size_t> res;
	auto fields = std::move(split(line, " "));
	try {
		for (std::string & f : fields) {
			res.push_back(std::stoi(f));
		}
	} catch (...) {
		//RAW_LOG(INFO, "wrong record detected!");
	}
	if (res.size() < min_len) {
		res.clear();
	}
	return std::move(res);
}

struct Random {
    Random(unsigned long long seed) {
        next_random = seed;
    }

    unsigned long long operator() () {
        next_random = next_random * (unsigned long long) 25214903917 + 11;
        return next_random;
    }

private:
    unsigned long long next_random  = 0;
};


void ReadWord(char *word, FILE *fin) {
	int a = 0, ch;
	while (!feof(fin)) {
		ch = fgetc(fin);
		if (ch == 13)
			continue;
		if ((ch == ' ') || (ch == '\t') || (ch == '\n')) {
			if (a > 0) {
				if (ch == '\n')
					ungetc(ch, fin);
				break;
			}
			if (ch == '\n') {
				strcpy(word, (char * )"</s>");
				return;
			} else
				continue;
		}
		word[a] = ch;
		a++;
		if (a >= MAX_STRING - 1)
			a--;   // Truncate too long words
	}
	word[a] = 0;
}

long ReadWordKey(FILE *file) {
    char word[MAX_STRING];
    ReadWord(word, file);
    //RAW_LOG(INFO, "word:\t%s", word);
    //RAW_LOG(INFO, "char:\t%s", word);
    if (feof(file)) return -1;
    char *end;
    if (strcmp(word, "</s>") == 0) return -2;
    long key  = strtol(word, &end, 10);
    return key;
}


struct Error {
    real_t data = 0;
    size_t counter = 0;

    void accu(real_t e) {
        data += e;
        counter ++;
    }
    
    real_t norm() {
        real_t error = data / counter;
        data = 0;
        counter = 0;
        return error;
    }
};

struct GlobalVocab {
	typedef size_t id_t;
    typedef index_t key_t;
	struct WordItem {
		WordItem() {
		}
		int count = 0;
		id_t id = std::numeric_limits<id_t>::max();
	};  // end struct WordItem

	std::pair<key_t, WordItem>* worditems = NULL;

	dense_hash_map<key_t, WordItem> vocab;

    int *table = NULL;

	//std::unique_ptr<int[]> table;

	GlobalVocab() {
		vocab.set_empty_key(std::numeric_limits<id_t>::max());
		len_vec = global_config().get_config("len_vec").to_int32();
		min_reduce = global_config().get_config("min_reduce").to_int32();
		min_len = global_config().get_config("min_len").to_int32();
		CHECK_GT(len_vec, 0);
		CHECK_GT(min_reduce, 0);
		CHECK_GT(min_len, 0);
	}

    ~GlobalVocab() {
        if(worditems != NULL) {
            worditems = NULL;
            delete worditems;
        }
    }

    void reset_worditems(size_t size) {
        CHECK_GT(size, 0);
        if(worditems_size == size) return;
        if(worditems != NULL) {
            delete worditems;
            worditems_size = 0;
        }
        worditems = new std::pair<key_t, WordItem>[size];
        worditems_size = size;
    }

    void operator() (const string& path) 
    {
        // init vocab from dataset
        init(path);
        init_unigram_table();
    }

    WordItem& operator[] (key_t key) {
        return vocab[key];
    }

    id_t word2id(key_t key) {
        return vocab[key].id;
    }

    key_t id2word(id_t id) {
        return worditems[ table[id] ].first;
    }

	size_t size() const {
		return vocab.size();
	}

protected:
	// init from a file
	void init(const std::string& path) {
		FILE *file = fopen(path.c_str(), "rb");
		if (file == NULL) {
			fprintf(stderr, "no such file or directory: %s", path.c_str());
			exit(1);
		}
        long word = 0;
        while(true) {
            word = ReadWordKey(file);
            //RAW_LOG(INFO, "read %d", word);
            if(feof(file)) break;
            if(word == -1 || word == -2) continue;
            inc_word(word);
            train_words++;
            if (train_words % 1000 == 0) {
                printf("%lldK%c", train_words / 1000, 13);
                fflush(stdout);
            }
        }
        LOG(INFO) << "file_size:\t" << file_size;
		file_size = ftell(file);
        DLOG(INFO) << "to create worditems";
		//worditems.reset(new std::pair<key_t, WordItem>[vocab.size()]);
        reset_worditems(vocab.size());
		for (const auto& item : vocab) {
			worditems[item.second.id] = item;
		}
		LOG(INFO)<< "vocabulary load\t" << vocab.size() << "\t words";
		LOG(INFO)<< "dataset has\t" << train_words << "\twords";
		fclose(file);
	}

	void inc_word(key_t key) {
		if (vocab.count(key) == 0) {
			vocab[key] = WordItem();
			vocab[key].id = word_counter++;
		}
		vocab[key].count++;
	}

	// clean the unusual words
	void clean_vocab() {
		if (min_reduce > 0) {
			for (auto it = vocab.begin();; it != vocab.end()) {
				auto& item = it->second;
				if (item.count < min_reduce)
					vocab.erase(it++);
			}
		}
	}

	void init_unigram_table() {
		LOG(INFO)<< "... init_unigram_table";
		CHECK_GT(vocab.size(), 0) << "word_freq should be inited before";
        CHECK( ! table) << "table has been inited before";
		int a, i;
		double train_words_pow = 0;
		double d1, power = 0.75;
		//table.reset(new int[table_size]);
        if(table == NULL) table = new int[table_size];

		i = 0;
		for(const auto& item : vocab) {
			train_words_pow += std::pow(item.second.count, power);
		}
		i = 0;
		d1 = pow(worditems[i].second.count, power) / (double)train_words_pow;
		for(a = 0; a < table_size; a++) {
			table[a] = worditems[i].first;
            //CHECK(table[a] != 19987);
			if(a / (double)table_size > d1) {
				i++;
				d1 += pow(worditems[i].second.count, power) / (double)train_words_pow;
			}
			if(i >= vocab.size()) i = vocab.size() - 1;
		}
	}


	//std::unique_ptr<real[]> feas;
	size_t word_counter = 0;
	int len_vec = 0;
	int min_reduce = 0;
	int min_len = 0;
    size_t worditems_size = 0;
	const id_t maxid = std::numeric_limits<id_t>::max();
}; // end struct GlobalVocab



class GlobalCache {
public:
    typedef index_t          key_t;
    typedef Word2VecParam val_t;
    typedef Word2VecGrad grad_t;
    using param_cache_t = GlobalParamCache<key_t, val_t, grad_t>;
    using pull_access_t = GlobalPullAccess<key_t, val_t, grad_t>;
    using push_access_t = GlobalPushAccess<key_t, val_t, grad_t>;

    GlobalCache(GlobalVocab &vocab) :
        pull_access(global_pull_access<key_t, val_t, grad_t>()),
        push_access(global_push_access<key_t, val_t, grad_t>()), 
        global_vocab(vocab)
    { }

    void init() {
        LOG(INFO) << "... GlobalVocab init";
        CHECK(local_keys.empty());
        CHECK(!global_vocab.vocab.empty());
        for(auto& item : global_vocab.vocab) {
            local_keys.insert(item.first);
        }
    }

    void pull() {
        DLOG(INFO) << "to pull from PS";
        CHECK(! local_keys.empty());
        DLOG(INFO) << "pull " << local_keys.size() << " keys";
        pull_access.pull_with_barrier(local_keys, param_cache);
        DLOG(INFO) << "get pulled param " << param_cache.size();
    }

    void push() {
        DLOG(INFO) << "to push to PS";
        if(local_keys.empty()) {
            RAW_DLOG(INFO, "local_keys empty, do not push");
            return;
        }
        push_access.push_with_barrier(local_keys, param_cache);
    }

    param_cache_t& cache() {
        return param_cache;
    }


    GlobalVocab& global_vocab;
    GlobalParamCache<key_t, val_t, grad_t> param_cache;
    std::unordered_set<key_t> local_keys;
    pull_access_t &pull_access;
    push_access_t &push_access;
};

    
class MiniBatch {
public:
    typedef index_t          key_t;
    typedef Word2VecParam val_t;
    typedef Word2VecGrad grad_t;
    using param_cache_t = GlobalParamCache<key_t, val_t, grad_t>;
    using pull_access_t = GlobalPullAccess<key_t, val_t, grad_t>;
    using push_access_t = GlobalPushAccess<key_t, val_t, grad_t>;

    MiniBatch(GlobalCache &global_cache) :
        global_cache(global_cache),
        pull_access(global_pull_access<key_t, val_t, grad_t>()),
        push_access(global_push_access<key_t, val_t, grad_t>())
    { 
        num_sents_to_cache = global_config().get_config("num_sents_to_cache").to_int32();
        CHECK_GT(num_sents_to_cache, 0);
    }

    void collect_keys(FILE *file, size_t word_count, size_t word_task) {
        long cur_pos = ftell(file);
        size_t word_counter = 0;
        char word[MAX_STRING];
        size_t sentence_counter = 0;
        size_t key;
        char *end;
        auto& params = global_cache.cache().params();
        // read words
        for(;;) {
            if (feof(file)) break;
            if(sentence_counter >= num_sents_to_cache) break;
            if(word_count > word_task) break;
            key = ReadWordKey(file);
            if(key == -1) break;
            if(key == -2) {
                sentence_counter ++;
                continue;
            }
            word_count ++;
            if(params.count(key) > 0) local_keys.insert(key);
        }
        // resume position
        fseek(file, cur_pos, SEEK_SET);
        RAW_DLOG(INFO, "collect %d keys", local_keys.size());
    }

    void pull() {
        RAW_DLOG(INFO, "minibatch pull %lu keys", local_keys.size());
        CHECK(! local_keys.empty());
        pull_access.pull_with_barrier(local_keys, global_cache.cache());
    }

    void push() {
        RAW_DLOG(INFO, "minibatch push %lu keys", local_keys.size());
        if(local_keys.empty()) {
            RAW_DLOG(INFO, "local_keys empty, do not push");
            return;
        }
        push_access.push_with_barrier(local_keys, global_cache.cache());
    }

    void clear() {
        local_keys.clear();
    }

    std::unordered_set<key_t> local_keys;

private:
    pull_access_t &pull_access;
    push_access_t &push_access;
    GlobalCache &global_cache;
    size_t num_sents_to_cache = 0;
};




class Word2Vec {

public:
    typedef index_t          key_t;
    typedef Word2VecParam val_t;
    typedef Word2VecGrad grad_t;

	Word2Vec(const string& data_path) :
        global_param(vocab),
        data_path(data_path)
    {
		min_len = global_config().get_config("min_len").to_int32();
		//local_train = global_config().get_config("local_train").to_int32() > 0;
		len_vec = global_config().get_config("len_vec").to_int32();
		num_threads =
		        global_config().get_config("async_channel_thread_num").to_int32();
		window = global_config().get_config("window").to_int32();
		negative = global_config().get_config("negative").to_int32();
		cbow = global_config().get_config("cbow").to_int32() > 0;
        num_sents_to_cache = global_config().get_config("num_sents_to_cache").to_int32();
		CHECK_GT(min_len, 0);
		CHECK_GT(num_threads, 0);
	}

    ~Word2Vec() {
        free(expTable);
    }

    void operator() () {
        vocab(data_path);
        //init_net();
        init_exp_table();
        //init_unigram_table();
        global_param.init();
    }

    void train() {
        Error global_error;
        DLOG(INFO) << "param size:\t" << global_param.cache().size();
        for(int iter = 0; iter < global_config().get_config("num_iters").to_int32(); iter ++) {
            LOG(INFO) << iter << " th train!";
            global_param.pull();
            vector<thread> threads;
            for( int i = 0; i < num_threads; i++) {
                DLOG(INFO) << "start train thread " << i;
                thread t([this, &global_error, i] {
                    TrainModelThread(data_path, i, global_error);
                });
                threads.push_back(std::move(t));
            }

            for(auto &t : threads) {
                t.join();
            }
            
            RAW_LOG(INFO, "error:\t%f", global_error.norm());
            word_count_actual = 0;
            //output(out_path);
            global_param.push();
        }
    }


	void TrainModelThread(const string &train_file, int id, Error& global_error) {
        //LocalParamCache local_param;
        Random random((unsigned long long)id);
		long long a, b, d, word, last_word, sentence_length = 0, sentence_position = 0;
		long long word_count = 0, last_word_count = 0, sen[MAX_SENTENCE_LENGTH + 1];
		long long l1, l2, c, target, label;
        long sentence_counter = 0;
        long last_sentence_counter = sentence_counter;
		//unsigned long long next_random = (long long) id;
		real_t f, g;
		clock_t now;
        MiniBatch minibatch(global_param);
		//real_t *neu1 = (real_t *) calloc(len_vec, sizeof(real_t));
		//real_t *neu1e = (real_t *) calloc(len_vec, sizeof(real_t));
        auto& params = global_param.cache().params();
        Vec neu1(len_vec);
        Vec neu1e(len_vec);
		FILE *fi = fopen(train_file.c_str(), "rb");
		if (fi == NULL) {
			fprintf(stderr, "no such file or directory: %s", train_file.c_str());
			exit(1);
		}
		fseek(fi, file_size / (long long) num_threads * (long long) id, SEEK_SET);
		while (true) {
			if (word_count - last_word_count > 10000000) {
				word_count_actual += word_count - last_word_count;
				last_word_count = word_count;
				if ((debug_mode)) {
					now = clock();
					printf(
					        "%cAlpha: %f  Progress: %.2f%%  Words/thread/sec: %.2fk  ",
					        13, alpha,
					        word_count_actual / (real_t) (train_words + 1) * 100,
					        word_count_actual
					                / ((real_t) (now - start + 1)
					                        / (real_t) CLOCKS_PER_SEC * 1000));
					fflush(stdout);
				}
				alpha = starting_alpha
				        * (1 - word_count_actual / (real_t) (train_words + 1));
                // TODO alpha should be inited again in the second iteration
				if (alpha < starting_alpha * 0.0001)
					alpha = starting_alpha * 0.0001;
			}
            // update paramters
            if(sentence_counter > 0 && sentence_counter % num_sents_to_cache == 0 && sentence_counter != last_sentence_counter) {
                // push last minibatch
                RAW_DLOG(INFO, "sentence_counter:\t%d", sentence_counter);
                last_sentence_counter = sentence_counter;
                minibatch.push();
                minibatch.clear();
                minibatch.collect_keys(fi, word_count, train_words / num_threads);
                minibatch.pull();
            }
            // read a sentence
			if (sentence_length == 0) {
				while (1) {
					word = ReadWordKey(fi);
					if (feof(fi))
						break;
					if (word == -1)
						continue;
					word_count++;
					if (word == -2) // get </s> or \n
                    {
                        sentence_counter ++;
						break;
                    }
					// The subsampling randomly discards frequent words while keeping the ranking same
					if (sample > 0) {
						real_t ran = (sqrt(vocab[word].count / (sample * train_words))
						        + 1) * (sample * train_words) / vocab[word].count;
						//next_random = next_random * (unsigned long long) 25214903917
						        //+ 11;
						if (ran < (random() & 0xFFFF) / (real_t) 65536)
							continue;
					}
					sen[sentence_length] = word;
					sentence_length++;
					if (sentence_length >= MAX_SENTENCE_LENGTH)
						break;
				}
				sentence_position = 0;
			}
			if (feof(fi))
				break;
			if (word_count > train_words / num_threads)
				break;
			word = sen[sentence_position];
			if (word == -1) continue;
			for (c = 0; c < len_vec; c++) neu1[c] = 0;
			for (c = 0; c < len_vec; c++) neu1e[c] = 0;
			//next_random = next_random * (unsigned long long) 25214903917 + 11;
			b = random() % window;
			if (cbow) {  //train the cbow architecture
				// in -> hidden
				for (a = b; a < window * 2 + 1 - b; a++)
					if (a != window) {
						c = sentence_position - window + a;
						if (c < 0)
							continue;
						if (c >= sentence_length)
							continue;
						last_word = sen[c];
						if (last_word == -1)
							continue;
                        if(params.count(last_word) == 0) continue;
                        CHECK(vocab.vocab.count(last_word) > 0) << last_word;
                        CHECK(params.count(last_word) > 0) << last_word;
                        Vec& syn0_lastword = params[last_word].v();
						for (c = 0; c < len_vec; c++) {
                            neu1[c] += syn0_lastword[c];
                        }
							//neu1[c] += syn0[c + last_word * len_vec];
					}

				// NEGATIVE SAMPLING
				if (negative > 0)
					for (d = 0; d < negative + 1; d++) {
						if (d == 0) {
							target = word;
							label = 1;
						} else {
							//next_random = next_random
							        //* (unsigned long long) 25214903917 + 11;
							target = vocab.table[(random() >> 16) % table_size];
							if (target == 0)
                                target = vocab.table[(random() >> 16) % table_size];
								//target = random() % (vocab.size() - 1) + 1;
							if (target == word)
								continue;

							label = 0;
						}
                        if (params.count(target) == 0) continue;
                        //RAW_LOG(INFO, "target:\t%d\tlabel:%d", target, label);
						//l2 = target * len_vec;
                        CHECK(vocab.vocab.count(target) > 0) << target;
                        CHECK(params.count(target) > 0) << target;
                        Vec& syn1neg_target = params[target].h();
						f = 0;
						for (c = 0; c < len_vec; c++)
                            f += neu1[c] * syn1neg_target[c];
							//f += neu1[c] * syn1neg[c + l2];
						if (f > MAX_EXP)
							g = (label - 1) * alpha;
						else if (f < -MAX_EXP)
							g = (label - 0) * alpha;
						else
							g = (label
							        - expTable[(int) ((f + MAX_EXP)
							                * (EXP_TABLE_SIZE / MAX_EXP / 2))])
							        * alpha;
                        // accumerate error
                        global_error.accu(10000* g * g);
                        
						for (c = 0; c < len_vec; c++)
                            neu1e[c] += g * syn1neg_target[c];
							//neu1e[c] += g * syn1neg[c + l2];
                        /*
						for (c = 0; c < len_vec; c++)
                            syn1neg_target[c] += g * neu1[c];
                        */

                        global_param.cache().grads()[target].accu_h(g * neu1);// / alpha);
							//syn1neg[c + l2] += g * neu1[c];
                        //RAW_LOG(INFO, "g:%f", g);
					}
				// hidden -> in
				for (a = b; a < window * 2 + 1 - b; a++)
					if (a != window) {
						c = sentence_position - window + a;
						if (c < 0)
							continue;
						if (c >= sentence_length)
							continue;
						last_word = sen[c];
						if (last_word == -1)
							continue;
                        Vec &syn0_lastword = params[last_word].v();
                        /*
						for (c = 0; c < len_vec; c++)
                            syn0_lastword[c] += neu1e[c];
                        */
							//syn0[c + last_word * len_vec] += neu1e[c];
                        global_param.cache().grads()[last_word].accu_v(neu1e);// / alpha);
					}
			}
			sentence_position++;
			if (sentence_position >= sentence_length) {
				sentence_length = 0;
				continue;
			}
		}
        minibatch.push();
		fclose(fi);
		//free(neu1);
		//free(neu1e);
		pthread_exit(NULL);
	}
    void init_exp_table() {
        expTable = (real_t *) malloc((EXP_TABLE_SIZE + 1) * sizeof(real_t));
        if (expTable == NULL) {
            fprintf(stderr, "out of memory\n");
            exit(1);
        }
        for (int i = 0; i < EXP_TABLE_SIZE; i++) {
            expTable[i] = exp((i / (real_t) EXP_TABLE_SIZE * 2 - 1) * MAX_EXP); // Precompute the exp() table
            expTable[i] = expTable[i] / (expTable[i] + 1); // Precompute f(x) = x / (x + 1)
	    }
    }


    GlobalVocab& get_vocab() {
		return vocab;
	}

    void output(const string& path) {
        ofstream file(path);
        CHECK(file.is_open())<< "output path is valid!";
        for( auto & item : vocab.vocab) {
            file << item.first << "\t";
            file << "h:\t";
            for(int i = 0; i < len_vec; i++) {
                file << syn1neg[ item.second.id * len_vec + i];
                if( i != len_vec - 1) {
                    file << " ";
                } else file << "\t";
            }
            file << "v:\t";
            for(int i = 0; i < len_vec; i++) {
                file << syn0[ item.second.id * len_vec + i];
                if( i != len_vec - 1) {
                    file << " ";
                } else file << "\n";
            }
        }
        LOG(WARNING)  << "output model to " << path;
    }

private:
	std::unique_ptr<real_t[]> syn1neg;
	std::unique_ptr<real_t[]> syn0;
	GlobalVocab vocab;
	int min_len = 0;
	int len_vec = 0;
	//bool local_train = false;
	int num_threads = 0;
	int window = 0;
	std::random_device rd;
	int negative;
    int num_sents_to_cache = 0;
	bool cbow = false;
	real_t alpha = 0.025;
    real_t sample = 1e-3;
    bool debug_mode = true;
    real_t starting_alpha = 0.025;
    real_t *expTable;
    GlobalCache global_param;
    string data_path;
};
