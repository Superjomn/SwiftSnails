//
//  node_init.h
//  SwiftSnails
//
//  Created by Chunwei on 3/15/15.
//  Copyright (c) 2015 Chunwei. All rights reserved.
//
#pragma once
#include "../../utils/all.h"
#include "../transfer/transfer.h"
#include "../parameter/hashfrag.h"
#include "ServerWorkerRoute.h"
#include "message_classes.h"
namespace swift_snails {


class NodeTransferInit {
public:
    explicit NodeTransferInit()
    { }

    void operator() (bool is_server=false) {
        _node_is_server = is_server;
        register_master();
        register_local_node_to_master();
        wait_for_route_init();
    }


protected:
    void register_master() {
        std::string addr = global_config().get_config("master_addr").to_string();
        LOG(WARNING) << "register master:\t" << addr;
        gtransfer.route().register_node(0, std::move(addr));
    }
    
    // register local address to remote master server
    void register_local_node_to_master() {

        LOG(INFO) << "local listen addr:\t" << gtransfer.recv_addr();
        Addr ip(gtransfer.recv_addr());
        //LOG(INFO) << "after init ip.to_string():\t" << ip.to_string();
        //ip.port = gtransfer.recv_port();
        // TODO read from config
        gtransfer.set_client_id(-1);
        // create message
        Request request;
        request.meta.message_class = MSG_CLS::NODE_INIT_ADDRESS;
        //request.meta.client_id = -1;
        request.meta.addr = ip;
        request.cont << ip;
        if(_node_is_server) 
            request.set_server();
        else 
            request.set_worker();
        // response-callback
        request.call_back_handler = [this](std::shared_ptr<Request> response) {
            LOG(WARNING) << "get response from master and init route, set client_id to\t" << response->meta.client_id;
            // init route
            response->cont >> gtransfer.route();
            gtransfer.set_client_id(response->meta.client_id);
            // timeout's unit is seconds
            route_init_barrier.set_state_valid();
            route_init_barrier.try_unblock();
        };
        // send request to the master server
        LOG(WARNING) << "send route to master ...";
        gtransfer.send(std::move(request), 0);

        LOG(WARNING) << "route is sent";
    }

    void wait_for_route_init() {
        LOG(WARNING) << "to block current thread";

        int timeout = global_config().get_config("init_timeout").to_int32();
        LOG(WARNING) << "[worker] init will wait for\t" << timeout <<" s";
        // barrier with timeout
        route_init_barrier.time_limit(1000 * timeout, []{
            //LOG(ERROR) << "[worker] init timeout!";
            CHECK(1 == 2) << "[worker] init timeout!";
        });
        route_init_barrier.block();
    }

private:
    bool _node_is_server = false;

    std::string _master_addr;
    //Barrier _init_barrier;
    Transfer<ServerWorkerRoute>& gtransfer = global_transfer<ServerWorkerRoute>();
    // route init barrier
    StateBarrier route_init_barrier;
};  // end class WorkerTransferInit

/*
 * init hashfrag
 */
class NodeHashfragInit {
public:
    explicit NodeHashfragInit() 
    { }

    void operator() () {
        askfor_hashfrag();
        wait_to_get_hashfrag();
    }

protected:

    // ask master for hashfrag
    void askfor_hashfrag() {
        LOG(WARNING) << "[worker] ask master for hashfrag init ...";
        Request req;
        // just put some useless content
        req.cont << 123;
        req.meta.message_class = NODE_ASKFOR_HASHFRAG;
        req.call_back_handler = [this](std::shared_ptr<Request> rsp) {
            LOG(WARNING) << "get hashfrag from master";
            LOG(INFO) << "hashfrag rsp size:\t" << rsp->cont.size();
            hashfrag.deserialize(rsp->cont);
            // unblock hashfrag_barrier
            hashfrag_init_barrier.set_state_valid();
            hashfrag_init_barrier.try_unblock();
        };
        gtransfer.send(std::move(req), 0);
    }

    void wait_to_get_hashfrag() {
        int timeout = global_config().get_config("init_timeout").to_int32();
        LOG(WARNING) << "[worker] ask for hashfrag will wait for\t" << timeout <<" s";
        // barrier with timeout
        hashfrag_init_barrier.time_limit(1000 * timeout, []{
            //LOG(ERROR) << "[worker] init timeout!";
            CHECK(1 == 2) << "[worker] askfor hashfrag timeout!";
        });
        hashfrag_init_barrier.block();
    }


private:

    BasicHashFrag<index_t> &hashfrag = global_hashfrag<index_t>();
    Transfer<ServerWorkerRoute>& gtransfer = global_transfer<ServerWorkerRoute>();
    StateBarrier hashfrag_init_barrier;


};  // class NodeHashfragInit





}; // end namespace swift_snails
