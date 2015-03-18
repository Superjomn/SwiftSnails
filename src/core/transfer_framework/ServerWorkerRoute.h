//
//  ServerWorkerRoute.h
//  SwiftSnails
//
//  Created by Chunwei on 3/16/15.
//  Copyright (c) 2015 Chunwei. All rights reserved.
//
#ifndef SwiftSnails_transfer_framework_ServerWorkerRoute_h_
#define SwiftSnails_transfer_framework_ServerWorkerRoute_h_
#include "../transfer/Route.h"
namespace swift_snails {

// Route with server and worker's route support
class ServerWorkerRoute : public BaseRoute {
public:
    // thread-safe
    int register_node_(bool is_server, std::string &&addr) {
        //rwlock_write_guard lock(_read_write_lock);
        int id{-1};
        if(is_server) {
            id = _server_num ++;
        	register_node(id, std::move(addr));
        } else {
            id = id_max_range - ++_worker_num;
            register_node(id, std::move(addr));
        }
        CHECK(id >= 0);
        return id;
    }

    virtual void update() {
    }

    friend BinaryBuffer& operator<<(BinaryBuffer& bb, const ServerWorkerRoute& route) {
        // put meta info
        bb << route.server_num();
        bb << route.worker_num();
        CHECK(route._send_addrs.size() == route.server_num() + route.worker_num());
        // put addresses
        for (const auto& rcd : route._send_addrs) {
            bb << rcd.first;    // id
            CHECK(headswith(rcd.second, "tcp://"));  // skip "tcp://"
            std::string ip_no_head = rcd.second.substr(7);
            IP ip(ip_no_head);
            bb << ip;
        }
        return bb;
    }

    // register all nodes
    friend BinaryBuffer& operator>>(BinaryBuffer& bb, ServerWorkerRoute& route) {
        bb >> route._server_num;
        bb >> route._worker_num;

        int id;
        IP ip;

        rwlock_write_guard lock(route._read_write_lock);
        for(int i = 0; i < route._server_num + route._worker_num; i++) {
            bb >> id;
            bb >> ip;
            if(id == 0) continue;
            std::string addr = "tcp://" + ip.to_string();
            route.register_node(id, std::move(addr));   // not thread-safe
        }

        return bb;
    }

    int server_num() const {
        return _server_num;
    }
    int worker_num() const {
        return _worker_num;
    }

private:
    int _server_num = 0;
    int _worker_num = 0;
    const int id_max_range = std::numeric_limits<int>::max();
};



}; // end namespace swift_snails
#endif

