//
//  BasicClient.h
//  SwiftSnails
//
//  Created by Chunwei on 12/26/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//

#ifndef SwiftSnails_SwiftSnails_client_BasicClient_h_
#define SwiftSnails_SwiftSnails_client_BasicClient_h_
#include "../../utils/common.h"
#include "../common.h"
namespace swift_snails {

class BasicClient : public VirtualObject {
public:
    explicit BasicClient() {
        _zmq_ctx = zmq_ctx_new();
        PCHECK(_receiver = zmq_socket(_zmq_ctx, ZMQ_PULL));
    }

    int listen() {
        zmq_bind_random_port(_receiver, _recv_addr, _recv_port);
        return _recv_port;
    }

    void connect() {
        for( auto& s : _senders) {
            connect(s.first);
        }
    }
    
    void register_node(int id, const std::string addr) {
        _send_addrs.emplace(id, addr);
        _send_mutexes.emplace(id, std::mutex());
        _senders.emplace(zmq_socket(_zmq_ctx, ZMQ_PULL));
    }
    void set_recv_addr(const std::string addr) {
        _recv_addr = addr;
    }
    void set_thread_num(int x) {
        CHECK(x > 0);
        _thread_num = x;
    }
    int thread_num() {
        return _thread_num;
    }
protected:
    // senders should be registered before
    void connect(index_t id) {
        PCHECK(0 == ignore_signal_call(zmq_connect, _senders[id], _send_addrs[id]));
    }

protected:
    void* _zmq_ctx = NULL;
    int _thread_num = 1;
    std::string _recv_addr;
    int _recv_port;
    void* _receiver = NULL;
    std::map<index_t, void*> _senders;
    std::map<index_t, std::string> _send_addrs;
    std::map<index_t, std::mutex> _send_mutexes;
}; // class BasicClient




}; // end namespace swift_snails
#endif

