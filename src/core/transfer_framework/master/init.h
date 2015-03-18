//
//  master/init.h
//  SwiftSnails
//
//  Created by Chunwei on 3/17/15.
//  Copyright (c) 2015 Chunwei. All rights reserved.
//
#ifndef SwiftSnails_core_transfer_framework_master_init_h_
#define SwiftSnails_core_transfer_framework_master_init_h_
#include "../../../utils/Barrier.h"
#include "../../../utils/Timer.h"
#include "../message_classes.h"
#include "../ServerWorkerRoute.h"
#include "../../transfer/transfer.h"
namespace swift_snails {

/*
 * when timeout or all the nodes has been registered
 */
typedef std::function<void()> void_lamb;


class MasterTransferInit {
	typedef Transfer<ServerWorkerRoute> transfer_t;
public:
    explicit MasterTransferInit() {
        LOG(INFO) << "init Master ...";
        // TODO should read from config file
        expected_node_num = global_config().get_config("expected_node_num").to_int32();     // nodes include server and worker
        int time_span = global_config().get_config("master_time_out").to_int32();
        LOG(INFO) << "get time_span:\t" << time_span;
        _timer.set_time_span( time_span); 

        LOG(WARNING) << "local register server ...";
        std::string addr = gtransfer.recv_addr();
        gtransfer.route().register_node_(true, std::move(addr));

        _timer.start();
    }

    void operator() () {
        register_init_message_class();
        CHECK(!gtransfer.async_channel()->closed()) << "channel should not been closed before transfer is deconstructed";
        wait_for_workers_register_route();
        // sent route as response to clients
        send_route_to_workers();
    }

    void register_init_message_class() {
        LOG(WARNING) << "register message class ...";
    	auto handler = node_init_address;
        gtransfer.message_class().add(NODE_INIT_ADDRESS, std::move(handler));
        //LOG(WARNING) << "end register message class";
    }

    void wait_for_workers_register_route() {
        std::function<bool()> cond_func = [this]() {
            return ( _timer.timeout() 
                || registered_node_num == expected_node_num);
        };

        void_lamb set_flag = [] { };
        // set a thread to try unblock current thread 
        // when timeout 
        void_lamb timeout_try_unblock = [this] {
            //LOG(WARNING) << "try unblock is called ...";
            LOG(WARNING) << "master will wait for " << _timer.time_span() << " s";
            std::this_thread::sleep_for( std::chrono::milliseconds(1000 + 1000 * _timer.time_span() ));
            void_lamb handle = []{};
            _wait_init_barrier.unblock(handle);
        };
        gtransfer.async_channel()->push(std::move(timeout_try_unblock));
        _wait_init_barrier.block(set_flag, cond_func); 
    }

    void send_route_to_workers() {

        for(auto& r : gtransfer.route().send_addrs()) {
            // TODO make response copy ? 
            // change to reduce ? 
            int id = r.first;
            const std::string &addr = r.second;
            Request response;
            response.cont << gtransfer.route();
            response.meta.message_class = -1;
            response.meta.message_id = init_msg_ids[id];
            LOG(WARNING) << "send route to worker:\t" << addr;
            // skip master
            if(id == 0) continue;
            gtransfer.send_response(std::move(response), id);
        }
    }

protected:
    // message-class handlers   ---------------------------------
    // node register address to master

    transfer_t::msgcls_handler_t node_init_address = [this](std::shared_ptr<Request> request, Request& response) {
        LOG(INFO) << "get node register";
        IP ip;
        request->cont >> ip;
        std::string addr = "tcp://" + ip.to_string();
        LOG(INFO) << "node's addr:\t" << addr;
        auto& gtransfer = global_transfer<ServerWorkerRoute>();
        // tell server/worker by clent_id
        // -1 or 0
        LOG(INFO) << "request.client_id:\t" << request->meta.client_id;
        CHECK(request->meta.client_id <= 0);
        int id = gtransfer.route().register_node_( request->meta.client_id == 0, std::move(addr));
        // cache message_ids
        init_msg_ids[id] = request->meta.message_id;

        registered_node_num ++;

        CHECK(id >= 0);
        // check status 
        // and notify the main thread to check the cond_variable 
        if(registered_node_num >= expected_node_num - 2) {
            void_lamb handler = []{};
            _wait_init_barrier.unblock(handler);
        }
    };

private:
    Transfer<ServerWorkerRoute>& gtransfer = global_transfer<ServerWorkerRoute>();
    // cache message id
    std::map<index_t, index_t> init_msg_ids;

    int registered_node_num = 0;
    // TODO should read from config file
    int expected_node_num = 10; // TODO read from config file

    Timer _timer;
    CompBarrier _wait_init_barrier;

};


}; // end namespace swift_snails
#endif
