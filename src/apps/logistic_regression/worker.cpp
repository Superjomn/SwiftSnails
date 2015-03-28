#include <iostream>
#include "../../utils/all.h"
#include "../../core/system/node_init.h"
#include "../../core/system/worker/init_config.h"
#include "access_method.h"
using namespace swift_snails;
using namespace std;
using namespace fms;
using namespace logistic_regression;


class Algorithm {

public:

    typedef index_t key_t;
    typedef float val_t;
    typedef pair<key_t, val_t> rcd_t;

    explicit Algorithm() {
        _path = global_config().get_config("data_path");
        _num_iters = global_config().get_config("num_iters");
        CHECK_NE(_path.empty) << "data_path config need to be inited";
        // init async channel
        int _async_channel_thread_num = global_config().get_config("async_channel_thread_num").to_int32();
        CHECK_GT(async_channel_thread_num, 0);
        AsynExec as(async_channel_thread_num);
        _async_channel = as.open();
    }

    void calculate() {
        init_local_param_cache(_async_channel_thread_num);
    }

    void init_local_param_cache(int thread_num) {
        LOG(WARNING) << "init local parameter cache";
        GLOG(INFO) << "start " << thread_num << " threads to gather keys";
        CHECK_GT(thread_num, 0);
        // make sure the following task wait for the init period
        FILE* file = fopen(_path.c_str());
        LineFileReader line_reader;
        std::mutex file_mut;

        std::set<key_t> keys;

        std::function<void(const std::string& line)> handle_line \
            = [&keys] (const std::string& line) {
                auto rcds = parse_record(line);
                for(auto &item : rcds) {
                    keys.add(item.first);
                }
            };

        AsynExec::task_t task = [file, &file_mut, &handle_line] {
            auto _handle_line = handle_line;
            scan_file_by_line(file, file_mut, std::move(_handle_line) );
        };

        async_exec(thread_num, std::move(task), async_channel());
        fclose(file);
    }

    // batch train
    void train() {
        for(int i = 0; i < _num_iters; i ++) {
            LOG(WARNING) << i << " th iteration";
            train_iter(_async_channel_thread_num);
        }
    }

protected:
    /*
     * learn the records in one line
     */
    void learn_one_record(std::vector<rcd_t> &&rcds) {

    }
    // one iteration
    void train_iter(int thread_num) {

        LOG(INFO) << "train file with " << thread_num << " threads";
        FILE* file = fopen(_path.c_str());
        LineFileReader line_reader;
        std::mutex file_mut;

        std::function<void(const std::string& line)> handle_line \
            = [this, &keys] (const std::string& line) {
                auto rcds = parse_record(line);
                learn_one_record(std::move(rcds));
            };

        AsynExec::task_t task = [file, &file_mut, &handle_line] {
            auto _handle_line = handle_line;
            scan_file_by_line(file, file_mut, std::move(_handle_line) );
        };

        async_exec(thread_num, std::move(task), async_channel());
        fclose(file);
    }

    std::vector<rcd_t> parse_record(const string &line) {
        std::vector<rcd_t> rcds;
        for(const auto& item : split(line, " ")) {
            std::pair<string, string> kv = key_value_split(item, ":");
            key_t key = stoi(kv.first);
            val_t val = stof(kv.second);
            
            rcds.emplace_back(key, val);
        }
        return std::move(rcds);
    }


protected:
    std::shared_ptr<AsynExec::channel_t>& async_channel() {
        return _async_channel;
    }

private:
    string _path;
    std::shared_ptr<AsynExec::channel_t> _async_channel;
    int _async_channel_thread_num = 0;
};

int main(int argc, char* argv[]) {
    // init config
    CMDLine cmdline(argc, argv);
    string param_config_path = cmdline.registerParameter("config", "path of config file");
    string param_help = cmdline.registerParameter("help", "this screen");
    // parse parameters
    if(cmdline.hasParameter(param_help) || argc == 1) {
        cout << endl;
        cout << "===================================================================" << endl;
        cout << "master server main program";
        cout << "===================================================================" << endl;
        cmdline.print_help();
        cout << endl;
        cout << endl;
        return 0;
    }
    if(!cmdline.hasParameter(param_config_path)) {
        LOG(ERROR) << "missing parameter: config";
        return 0;
    }
    std::string config_path = cmdline.getValue(param_config_path);

    worker_init_configs();

    global_config().load_conf(config_path);
    global_config().parse();

    NodeTransferInit node_transfer_init;
    NodeHashfragInit node_hashfrag_init;

    node_transfer_init(false);
    node_hashfrag_init();
    
    return 0;
};
