#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "../../utils/all.h"

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

struct Vocab {
	typedef size_t id_t;

	Vocab() {
		vocab.set_empty_key(std::numeric_limits<id_t>::max());
		len_vec = global_config().get_config("len_vec").to_int32();
		min_reduce = global_config().get_config("min_reduce").to_int32();
		min_len = global_config().get_config("min_len").to_int32();
		CHECK_GT(len_vec, 0);
		CHECK_GT(min_reduce, 0);
		CHECK_GT(min_len, 0);
	}
	// init from a file
	void init(const std::string& path) {
		LineFileReader linereader;
		FILE* file = fopen(path.c_str(), "r");
		CHECK(file != NULL) << "file can not be open\t" << path;
		string line;
		while (true) {
			char* res = linereader.getline(file);
			if (res == NULL)
				break;
			line = linereader.get();
			auto rcd = parse_record(line, min_len);
			for (const auto& key : rcd) {
				inc_word(key);
				train_words++;
				if (train_words % 100000 == 0) {
					printf("%lldK%c", train_words / 1000, 13);
					fflush(stdout);
				}
			}
		}
		file_size = ftell(file);
		worditems.reset(new std::pair<key_t, WordItem>[vocab.size()]);
		for (const auto& item : vocab) {
			worditems[item.second.id] = item;
		}
		LOG(INFO)<< "vocabulary load\t" << vocab.size() << "\t words";
		LOG(INFO)<< "dataset has\t" << train_words << "\twords";
		fclose(file);
	}

	struct WordItem {
		WordItem() {
		}
		int count = 0;
		id_t id = std::numeric_limits<id_t>::max();
	};  // end struct WordItem

	void inc_word(id_t id) {
		if (vocab.count(id) == 0) {
			vocab[id] = WordItem();
			vocab[id].id = word_counter++;
		}
		vocab[id].count++;
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

	size_t size() const {
		return vocab.size();
	}

	dense_hash_map<key_t, WordItem> vocab;
	std::unique_ptr<std::pair<key_t, WordItem>[]> worditems;
	//std::unique_ptr<real[]> feas;
	id_t word_counter = 0;
	int len_vec = 0;
	int min_reduce = 0;
	int min_len = 0;
	const id_t maxid = std::numeric_limits<id_t>::max();
};
// end struct Vocab

//
int ReadWordIndex(FILE *fin, Vocab &vocab) {
	char word[MAX_STRING];
	ReadWord(word, fin);
	if (feof(fin))
		return -1;
    char *end;
    size_t key  = strtol(word, &end, 10);
    return vocab.vocab[key].id;
}


class Word2Vec {

public:

	Word2Vec() {
		min_len = global_config().get_config("min_len").to_int32();
		local_train = global_config().get_config("local_train").to_int32() > 0;
		len_vec = global_config().get_config("len_vec").to_int32();
		num_threads =
		        global_config().get_config("async_channel_thread_num").to_int32();
		window = global_config().get_config("window").to_int32();
		negative = global_config().get_config("negative").to_int32();
		cbow = global_config().get_config("cbow").to_int32() > 0;
		CHECK_GT(min_len, 0);
		CHECK_GT(num_threads, 0);
	}

    void operator() (const string &data_path) {
        vocab.init(data_path);
        init_net();
        init_exp_table();
        init_unigram_table();
    }

	void init_net() {
		syn1neg.reset(new real_t[len_vec * vocab.size()]);
		syn0.reset(new real_t[len_vec * vocab.size()]);
		// random init
		if (local_train) {
			for (size_t b = 0; b < len_vec; b++)
				for (size_t a = 0; a < vocab.size(); a++)
					syn0[a * len_vec + b] = (rand() / (real_t) RAND_MAX - 0.5)
					        / len_vec;
			for (size_t b = 0; b < len_vec; b++)
				for (size_t a = 0; a < vocab.size(); a++)
					syn1neg[a * len_vec + b] =
					        (rand() / (real_t) RAND_MAX - 0.5) / len_vec;
		}
		for (int i = 0; i < vocab.size(); i++) {
			cout << syn0[i] << " ";
		}
		cout << endl;
	}

	void init_unigram_table() {
		LOG(INFO)<< "... init_unigram_table";
		CHECK_GT(vocab.size(), 0) << "word_freq should be inited before";
		int a, i;
		double train_words_pow = 0;
		double d1, power = 0.75;
		table.reset( new int[table_size]);

		i = 0;
		for(const auto& item : vocab.vocab) {
			train_words_pow += std::pow(item.second.count, power);
		}
		i = 0;
		d1 = pow(vocab.worditems[i].second.count, power) / (double)train_words_pow;
		for(a = 0; a < table_size; a++) {
			table[a] = i;
			if(a / (double)table_size > d1) {
				i++;
				d1 += pow(vocab.worditems[i].second.count, power) / (double)train_words_pow;
			}
			if(i >= vocab.size()) i = vocab.size() - 1;
		}
	}


    void train(const string& data_path) {
        Error global_error;
        for(int iter = 0; iter < 10; iter ++) {
            vector<thread> threads;
            for( int i = 0; i < num_threads; i++) {
                thread t([this, &data_path, &global_error, i]{
                    TrainModelThread(data_path, i, global_error);
                });
                threads.push_back(std::move(t));
            }

            for(auto &t : threads) {
                t.join();
            }
            
            RAW_LOG(INFO, "error:\t%f", global_error.norm());
        }
    }


	void TrainModelThread(const string &train_file, int id, Error& global_error) {
		long long a, b, d, word, last_word, sentence_length = 0, sentence_position =
		        0;
		long long word_count = 0, last_word_count = 0, sen[MAX_SENTENCE_LENGTH + 1];
		long long l1, l2, c, target, label;
		unsigned long long next_random = (long long) id;
		real_t f, g;
		clock_t now;
		real_t *neu1 = (real_t *) calloc(len_vec, sizeof(real_t));
		real_t *neu1e = (real_t *) calloc(len_vec, sizeof(real_t));
		FILE *fi = fopen(train_file.c_str(), "rb");
		if (fi == NULL) {
			fprintf(stderr, "no such file or directory: %s", train_file.c_str());
			exit(1);
		}
		fseek(fi, file_size / (long long) num_threads * (long long) id, SEEK_SET);
		while (1) {
			if (word_count - last_word_count > 10000) {
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
				if (alpha < starting_alpha * 0.0001)
					alpha = starting_alpha * 0.0001;
			}
			if (sentence_length == 0) {
				while (1) {
					word = ReadWordIndex(fi, vocab);
					if (feof(fi))
						break;
					if (word == -1)
						continue;
					word_count++;
					if (word == 0)
						break;
					// The subsampling randomly discards frequent words while keeping the ranking same
					if (sample > 0) {
						real_t ran = (sqrt(vocab.worditems[word].second.count / (sample * train_words))
						        + 1) * (sample * train_words) / vocab.worditems[word].second.count;
						next_random = next_random * (unsigned long long) 25214903917
						        + 11;
						if (ran < (next_random & 0xFFFF) / (real_t) 65536)
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
			if (word == -1)
				continue;
			for (c = 0; c < len_vec; c++)
				neu1[c] = 0;
			for (c = 0; c < len_vec; c++)
				neu1e[c] = 0;
			next_random = next_random * (unsigned long long) 25214903917 + 11;
			b = next_random % window;
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
						for (c = 0; c < len_vec; c++)
							neu1[c] += syn0[c + last_word * len_vec];
					}

				// NEGATIVE SAMPLING
				if (negative > 0)
					for (d = 0; d < negative + 1; d++) {
						if (d == 0) {
							target = word;
							label = 1;
						} else {
							next_random = next_random
							        * (unsigned long long) 25214903917 + 11;
							target = table[(next_random >> 16) % table_size];
							if (target == 0)
								target = next_random % (vocab.size() - 1) + 1;
							if (target == word)
								continue;
							label = 0;
						}
						l2 = target * len_vec;
						f = 0;
						for (c = 0; c < len_vec; c++)
							f += neu1[c] * syn1neg[c + l2];
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
                        global_error.accu(g * g);
						for (c = 0; c < len_vec; c++)
							neu1e[c] += g * syn1neg[c + l2];
						for (c = 0; c < len_vec; c++)
							syn1neg[c + l2] += g * neu1[c];
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
						for (c = 0; c < len_vec; c++)
							syn0[c + last_word * len_vec] += neu1e[c];
					}
			}
			sentence_position++;
			if (sentence_position >= sentence_length) {
				sentence_length = 0;
				continue;
			}
		}
		fclose(fi);
		free(neu1);
		free(neu1e);
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


	Vocab& get_vocab() {
		return vocab;
	}

private:
	std::unique_ptr<int[]> table;
	std::unique_ptr<real_t[]> syn1neg;
	std::unique_ptr<real_t[]> syn0;
	Vocab vocab;
	int min_len = 0;
	int len_vec = 0;
	bool local_train = false;
	int num_threads = 0;
	int window = 0;
	std::random_device rd;
	int negative;
	bool cbow = false;
	real_t alpha = 0.025;
    real_t sample = 1e-3;
    bool debug_mode = true;
    real_t starting_alpha = 0.025;
    real_t *expTable;
}
;
