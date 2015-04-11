//
//  SwiftServer.h
//  core
//
//  Created by Chunwei on 4/4/15.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//
#pragma once
#include "../../utils/all.h"
#include "../system/node_init.h"
#include "../system/server/init.h"
//#include "../system/server/init_config.h"
#include "../system/server/terminate.h"

namespace swift_snails {

template<typename Key, typename Val, typename Grad, typename PullMethod, typename PushMethod>
class SwiftServer {
public:
    typedef Key     key_t;
    typedef Val     val_t;
    typedef Grad    grad_t;
    //typedef SparseTable<key_t, val_t> table_t;
    typedef PullMethod pull_method_t;
    typedef PushMethod push_method_t ;

    explicit SwiftServer(const std::string &config_path)  {
        // init configs
        /*
        server_init_configs();
        global_config().load_conf(config_path);
        global_config().parse();
        */
    }

    void operator() () {
        // register to master as server node
        node_transfer_init(true);   
        node_hashfrag_init();
        init_pull_method();
        init_push_method();
        // terminate local service after the task finished
        terminate();
    }
        
private: 
    NodeTransferInit node_transfer_init;
    NodeHashfragInit node_hashfrag_init;
    ServerInitPullMethod<key_t, val_t, grad_t, pull_method_t> init_pull_method;
    ServerInitPushMethod<key_t, val_t, grad_t, push_method_t> init_push_method;
    ServerTerminate<key_t, val_t> terminate;
};  // class SwiftServer

};  // end namespace swift_snails
