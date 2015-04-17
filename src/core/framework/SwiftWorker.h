//
//  SwiftWorker.h
//  core
//
//  Created by Chunwei on 4/4/15.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//
#pragma once
#include "../../utils/all.h"
#include "../system/node_init.h"
#include "../system/worker/terminate.h"
#include "../parameter/global_pull_access.h"
#include "../parameter/global_push_access.h"

namespace swift_snails {
/*
 * @ Record: line split to a record
 */
template<typename Key, typename Val, typename Grad, typename Record>
class BaseAlgorithm : public VirtualObject {
public:
    typedef Key     key_t;
    typedef Val     val_t;
    typedef Grad    grad_t;
    typedef Record  rcd_t;

    explicit BaseAlgorithm()
    { }

    virtual void train() = 0;
    virtual rcd_t parse_record(const std::string &line) = 0;

    void set_data_path(const std::string& path) {
        _data_path = path;
    }

    const std::string& data_path() {
        CHECK(!_data_path.empty()) << "should set_data_path first";
        return _data_path;
    }

    std::shared_ptr<AsynExec::channel_t>& async_channel() {
        return _async_channel;
    }

    void create_async_channel() {
        CHECK(!_async_channel) << "can be called only once";
        _async_channel_thread_num = global_config().get_config("async_channel_thread_num").to_int32();
        CHECK_GT(_async_channel_thread_num, 0);
        AsynExec as(_async_channel_thread_num);
        _async_channel = as.open();
    }

protected:
    std::string _data_path;
    int _async_channel_thread_num{0};
    std::shared_ptr<AsynExec::channel_t> _async_channel;

};  // class BaseAlgorithm
/*
 *
 */
template<typename Algorithm>
class SwiftWorker {

public:

    typedef typename Algorithm::key_t   key_t;
    typedef typename Algorithm::val_t   val_t;
    typedef typename Algorithm::grad_t  grad_t;
    using pull_access_t = GlobalPullAccess<key_t, val_t, grad_t> ;
    using push_access_t = GlobalPushAccess<key_t, val_t, grad_t> ;
    using param_cache_t = GlobalParamCache<key_t, val_t, grad_t> ;

    explicit SwiftWorker(std::string& config_path, Algorithm& alg) : \
        alg(alg),
        //param_cache(global_param_cache<key_t, val_t, grad_t>()),
        pull_access(global_pull_access<key_t, val_t, grad_t>()),
        push_access(global_push_access<key_t, val_t, grad_t>())
    {
        CHECK(!config_path.empty());
        // init configs
        _num_iters = global_config().get_config("num_iters").to_int32();
        CHECK_GT(_num_iters, 0);
        _learning_rate = global_config().get_config("learning_rate").to_float();
        _async_channel_thread_num = global_config().get_config("async_channel_thread_num").to_int32();
        _local_train = global_config().get_config("local_train").to_int32() > 0;
        CHECK_GT(_learning_rate, 0);
        //alg.create_async_channel();
    }

    void operator() () {
        if( ! _local_train) {
            // route init
            LOG(WARNING) << ".. node_transfer_init";
            node_transfer_init(false);
            LOG(WARNING) << ".. node_hashfrag_init";
            node_hashfrag_init();
            //LOG(WARNING) << ".. init_local_param_keys";
            //init_local_param_keys(_async_channel_thread_num);
            //LOG(WARNING) << ".. first_pull_to_init_local_param";
            //first_pull_to_init_local_param();
            //LOG(WARNING) << ".. start_deamon_service";
            //start_deamon_service();
            // API
            RAW_LOG(WARNING, "... begin to train alg");
            alg.train();
            // final push local grad to paramter servers
            //LOG(WARNING) << ".. final_push";
            //final_push();
            LOG(WARNING) << ".. terminate";
            // terminate local task
            terminate();

        } else {
            LOG(WARNING) << ".. init_local_param_keys";
            //init_local_param_keys(_async_channel_thread_num);
            // random init
            //for( auto& item : param_cache.params()) {
                //item.second.random_init();
            //}
            alg.train();
            //std::cout << param_cache;
        }
    }

protected:
    // pull parameters from remote server
    // should init local parameter cache's keys
    void first_pull_to_init_local_param() {
        RAW_LOG(WARNING, "... first_pull_to_init_local_param");
        pull_access.pull_with_barrier();
        RAW_LOG(WARNING, "... finish first_pull_to_init_local_param");
    }

    int async_channel_thread_num() {
        CHECK(_async_channel_thread_num > 0);
        return _async_channel_thread_num;
    }

private:
    int _num_iters{0};
    float _learning_rate{0.01};
    int _async_channel_thread_num{0};
    bool _local_train = false;
    // control modules
    Algorithm &alg;
    NodeTransferInit node_transfer_init;
    NodeHashfragInit node_hashfrag_init;
    ClientTerminate<key_t, val_t, grad_t> terminate;
    pull_access_t& pull_access;
    push_access_t& push_access;
    std::string _data_path;
};  // class Framework

};  // end namespace swift_snails
