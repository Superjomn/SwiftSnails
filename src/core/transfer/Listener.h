//
//  Listener.h
//  SwiftSnails
//
//  Created by Chunwei on 3/09/15.
//  Copyright (c) 2015 Chunwei. All rights reserved.
//
#ifndef Swift_transfer_Listener_h_
#define Swift_transfer_Listener_h_
#include "../../utils/common.h"
#include "../AsynExec.h"
#include "../../utils/SpinLock.h"
namespace swift_snails {

class Listener : VirtualObject {
public:
    explicit Listener(void *zmq_ctx) : 
        _zmq_ctx(zmq_ctx) 
    {
        PCHECK(_receiver = zmq_socket(_zmq_ctx, ZMQ_PULL));
    }

    virtual ~Listener() {

        LOG(WARNING) << "listener exit!";
        PCHECK(0 == zmq_close(_receiver));
    }

    virtual void main_loop() = 0;
    virtual bool service_complete() = 0;
    virtual void* zmq_ctx() = 0;

    // set properties
    void set_recv_ip(const std::string &ip) {
        CHECK(!ip.empty());
        _recv_ip = ip;
    }

    int listen() {
        zmq_bind_random_port(_recv_ip, _receiver,  _recv_addr, _recv_port);
        LOG(INFO) << "client listen to address:\t" << _recv_addr;
        return _recv_port;
    }

    // get attributes
    void* receiver() {
        return _receiver;
    }
    std::mutex& receiver_mutex() {
        return _receiver_mutex;
    }
    const std::string& recv_addr() const {
        return _recv_addr;
    }
    const std::string& recv_ip() const {
        return _recv_ip;
    }
    int recv_port() const {
        return _recv_port;
    }

private:
    void*       _zmq_ctx = NULL;
    void*       _receiver = NULL;
    std::mutex  _receiver_mutex;
    std::string _recv_addr;
    std::string _recv_ip;
    int         _recv_port = -1;
    
};


}; // namespace swift_snails
#endif
