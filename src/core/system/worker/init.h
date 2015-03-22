//
//  worker/init.h
//  SwiftSnails
//
//  Created by Chunwei on 3/15/15.
//  Copyright (c) 2015 Chunwei. All rights reserved.
//
#ifndef SwiftSnails_core_transfer_framework_worker_init_h_
#define SwiftSnails_core_transfer_framework_worker_init_h_
#include "../../../utils/Barrier.h"
#include "../../../utils/Timer.h"
#include "../ServerWorkerRoute.h"
#include "../message_classes.h"
#include "../../transfer/transfer.h"
#include "../../system/message_classes.h"
namespace swift_snails {
/*
 * init management of Worker Node
 *
 * register message_classes  and response_handlers
 */
//template<typename Route>
class NodeTransferInit;

class InitWaitBarrier {
public:

    explicit InitWaitBarrier() {
    }
    void set_timespan(int seconds) {
        _timer.set_time_span(seconds);
    }

    void start_timeout_thread() {
        std::function<void()> timeout_try_unblock = [this] {
            LOG(WARNING) << "master will wait for " << _timer.time_span() << " s";
            std::this_thread::sleep_for( std::chrono::milliseconds(1000 + 1000 * _timer.time_span() ));
            if(! response_arrived) {
                LOG(ERROR) << "master wait *timeout* and didn't get a response";
                CHECK(1 == 2) << "set ERROR";
            }
            std::function<void()> handle = []{};
            barrier.unblock(handle);
        };

        std::thread t(timeout_try_unblock);
        t.detach();
    }

    void block() {
        std::function<void()> set_flag = [] { };
        std::function<bool()> cond_func = [this]() {
            return response_arrived;
        };
        barrier.block(set_flag, cond_func);
    }

    void try_unblock() {
        std::function<void()> handler = []{};
        barrier.unblock(handler);
    }

    void set_response_arrive() {
        response_arrived = true;
    }

private:
    bool response_arrived = false;
    CompBarrier barrier;
    Timer _timer;
};

class NodeTransferInit {
public:
    explicit NodeTransferInit()
    {
        //global_config().register_config("master_addr", "");
        // TODO load config 
        int timeout = global_config().get_config("init_timeout").to_int32();
        _init_barrier.set_timespan(timeout);
    }

    void operator() () {
        register_master();
        register_local_node_to_master();
    }
    void register_master() {
        std::string addr = global_config().get_config("master_addr").to_string();
        LOG(WARNING) << "register master:\t" << addr;
        gtransfer.route().register_node(0, std::move(addr));
        //LOG(WARNING) << "local register server ...";
        //std::string addr = gtransfer.recv_addr();
        //gtransfer.route().register_node_(true, std::move(addr));
    }
    
    // register local address to remote master server
    void register_local_node_to_master() {
        //auto& gtransfer = global_transfer<ServerWorkerRoute>();
        LOG(INFO) << "local listen addr:\t" << gtransfer.recv_addr();
        IP ip(gtransfer.recv_addr());
        LOG(INFO) << "after init ip.to_string():\t" << ip.to_string();
        //ip.port = gtransfer.recv_port();
        // TODO read from config
        gtransfer.set_client_id(-1);
        // create message
        Request request;
        request.meta.message_class = MSG_CLS::NODE_INIT_ADDRESS;
        request.meta.client_id = -1;
        request.cont << ip;
        // response-callback
        request.call_back_handler = [this](std::shared_ptr<Request> response) {
            LOG(WARNING) << "get response from master and init route";
            // init route
            response->cont >> gtransfer.route();
            gtransfer.set_client_id(response->meta.client_id);

            _init_barrier.set_response_arrive();
            _init_barrier.try_unblock();    // continue running
        };
        // send request to the master server
        LOG(WARNING) << "send route to master ...";
        gtransfer.send(std::move(request), 0);
        LOG(WARNING) << "route is sent";
        LOG(WARNING) << "to block current thread";
        _init_barrier.start_timeout_thread();
        _init_barrier.block();
    }

private:
    std::string _master_addr;
    //Barrier _init_barrier;
    InitWaitBarrier _init_barrier;
    Transfer<ServerWorkerRoute>& gtransfer = global_transfer<ServerWorkerRoute>();

};  // end class WorkerTransferInit


}; // end namespace swift_snails
#endif
