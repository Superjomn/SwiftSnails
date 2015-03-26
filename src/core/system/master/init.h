//
//  master/init.h
//  SwiftSnails
//
//  Created by Chunwei on 3/17/15.
//  Copyright (c) 2015 Chunwei. All rights reserved.
//
#pragma once
#include "../../../utils/all.h"
#include "../../transfer/transfer.h"
#include "../message_classes.h"
#include "../ServerWorkerRoute.h"
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

        LOG(WARNING) << "local register server ...";
        std::string addr = gtransfer.recv_addr();
        gtransfer.route().register_node_(true, std::move(addr));
    }

    void operator() () {
        register_init_message_class();
        CHECK(!gtransfer.async_channel()->closed()) << "channel should not been closed before transfer is deconstructed";
        wait_for_workers_register_route();
        // sent route as response to clients
        send_route_to_workers();
    }

protected:

    void register_init_message_class() {
        LOG(WARNING) << "register message class ...";
    	auto handler = node_init_address;
        gtransfer.message_class().add(NODE_INIT_ADDRESS, std::move(handler));
        //LOG(WARNING) << "end register message class";
    }

    void wait_for_workers_register_route() {
        int timeout = global_config().get_config("master_time_out").to_int32();
        LOG(WARNING) << "master will wait for " << timeout << " s";

        _route_init_barrier.time_limit( 1000 * timeout, [this] {
            CHECK(1 == 2) << "[master] init route timeout!";
        });
        _route_init_barrier.block();
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
        Addr ip;
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

        CHECK(id > 0);

        registered_node_num ++;

        if(registered_node_num == expected_node_num) {
            _route_init_barrier.set_state_valid();
            _route_init_barrier.try_unblock();
        }
    };

private:
    Transfer<ServerWorkerRoute>& gtransfer = global_transfer<ServerWorkerRoute>();
    // cache message id
    std::map<index_t, index_t> init_msg_ids;

    std::atomic<int> registered_node_num{0};
    // TODO should read from config file
    int expected_node_num = 10; // TODO read from config file

    StateBarrier _route_init_barrier;
};


}; // end namespace swift_snails
