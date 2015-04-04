#pragma once
#include "../../utils/all.h"
#include "../system/node_init.h"
#include "../system/worker/init_config.h"
#include "../system/worker/terminate.h"
#include "../parameter/global_pull_access.h"
#include "../parameter/global_push_access.h"
#include "../system/worker/pull_service.h"
#include "../system/worker/push_service.h"
#include "access_method.h"

namespace swift_snails {

template<typename Key, typename Val, typename Grad>
class BaseAlgorithm : public VirtualObject {
public:
    typedef Key     key_t;
    typedef Val     val_t;
    typedef Grad    grad_t;

    explicit BaseAlgorithm() { }

    virtual void train() = 0;

    void set_data_path(const std::string& path) {
        _data_path = path;
    }

    const std::string& data_path() {
        CHECK(!data_path.empty()) << "should set_data_path first";
        return _data_path;
    }

private:
    std::string _data_path;

};  // class BaseAlgorithm
/*
 *
 */
template<typename Algorithm>
class SwiftWorker {

typedef typename Algorithm::key_t   key_t;
typedef typename Algorithm::val_t   val_t;
typedef typename Algorithm::grad_t  grad_t;
using pull_access_t = GlobalPullAccess<key_t, val_t, grad_t> ;
using push_access_t = GlobalPushAccess<key_t, val_t, grad_t> ;
using param_cache_t = GlobalParamCache<key_t, val_t, grad_t> ;

public:
    explicit SwiftWorker() : \
        param_cache(global_param_cache<key_t, val_t, grad_t>()),
        pull_access(global_pull_access<key_t, val_t, grad_t>()),
        push_access(global_push_access<key_t, val_t, grad_t>())
    {
        // init configs
        _num_iters = global_config().get_config("num_iters").to_int32();
        CHECK_GT(_num_iters, 0);
        _learning_rate = global_config().get_config("learning_rate").to_float();
        CHECK_GT(_learning_rate, 0);
        _async_channel_thread_num = global_config().get_config("async_channel_thread_num").to_int32();
        CHECK_GT(_async_channel_thread_num, 0);

        create_async_channel();
    }

    void operator() {
        init_local_param_keys(_async_channel_thread_num);
        first_pull_to_init_local_param();
        start_deamon_service();
        // API
        alg.train();
        final_push();
    }

protected:
    void create_async_channel() {
        AsynExec as(_async_channel_thread_num);
        _async_channel = as.open();
    }

    // start pull and push service
    // these threads will run background
    void start_deamon_service() {
        pull_service.start_service();
        push_service.start_service();
    }
    // should be called before train 
    // just initial local parameter cache
    void init_local_param_keys(int thread_num) {
        LOG(WARNING) << "init local parameter cache";
        DLOG(INFO) << "start " << thread_num << " threads to gather keys";
        CHECK_GT(thread_num, 0);
        // make sure the following task wait for the init period
        FILE* file = std::fopen(data_path().c_str(), "r");
        CHECK(file) << "file: open " << data_path() << " failed!";
        std::mutex file_mut;

        std::set<key_t> keys;
        std::mutex keys_mut;

        std::function<void(const std::string& line)> handle_line \
            = [this, &keys, &keys_mut] (const std::string& line) {
                auto rcd = parse_record(line);
                std::lock_guard<std::mutex> lk(keys_mut);
                for(auto &item : rcd.feas) {
                    keys.emplace(item.first);
                }
            };

        AsynExec::task_t task = [file, &file_mut, handle_line] {
            auto _handle_line = handle_line;
            scan_file_by_line(file, file_mut, std::move(_handle_line) );
        };

        async_exec(thread_num, std::move(task), async_channel());
        std::fclose(file);

        RAW_LOG(INFO, "to get number of features");
        // get num of features
        for(auto& key : keys) {
            if(key > num_feas) num_feas = key;
        }
        param_cache.init_keys(keys);
        num_feas ++;
        RAW_LOG(INFO, "finish init_local_param_keys");
    }

    // pull parameters from remote server
    // should init local parameter cache's keys
    void first_pull_to_init_local_param() {
        RAW_LOG(WARNING, "... first_pull_to_init_local_param");
        pull_access.pull_with_barrier();
        RAW_LOG(WARNING, "... finish first_pull_to_init_local_param");
    }
    // local task complete and push local grad 
    void final_push() {
        RAW_LOG(WARNING, "... try to push");
        push_access.push_with_barrier();
        RAW_LOG(WARNING, "... finish try push");
    }

private:
    int _num_iters{0};
    float _learning_rate{0.01};
    int _async_channel_thread_num{1};
    Algorithm alg;

};  // class Framework

};  // end namespace swift_snails
