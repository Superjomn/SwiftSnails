//
//  BasicServer.h
//  SwiftSnails
//
//  Created by Chunwei on 12/23/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//

#ifndef SwiftSnails_SwiftSnails_server_BasicServer_h_
#define SwiftSnails_SwiftSnails_server_BasicServer_h_
#include <map>
#include "zmq.h"
#include "../../utils/common.h"
#include "common.h"
#include "../../utils/SpinLock.h"

namespace swift_snails {
/*
 * basic server deal with the network connections
 * it has following functions:
 *
 * 1. listen to an ip address
 * 2. connect to multiple ip addresses
 */
class BasicServer : public VirtualObject {  // one process with only one server instance
public:
    typedef uint32_t index_t;
    BasicServer() {
        _zmq_ctx = zmq_ctx_new();
        PCHECK(_receiver = zmq_socket(_zmq_ctx, ZMQ_PULL));
    }

    ~BasicServer() {
        LOG(WARNING) << "server out";
        PCHECK(0 == zmq_close(_receiver));
        for( auto& sender : _senders) {
            PCHECK(0 == zmq_close(sender.second));
        }
        PCHECK(0 == zmq_ctx_destroy(_zmq_ctx));
    }

    // every node in the cluster should have a unique id
    void add_sender(index_t id, const std::string& addr) {
        CHECK(_senders.count(id) == 0); // a sender can be registered only once
        void *sender = zmq_socket(_zmq_ctx, ZMQ_PUSH);
        _senders[id] = sender;
        _send_addrs[id] = addr;
    }

    int listen(const std::string &addr) {
        _recv_addr = addr;
        zmq_bind_random_port(_receiver, _recv_addr, _recv_port);
        return _recv_port;
    }
    // connect sender to all addresses
    // if a new sender should be connected
    // then `add_sender` and  `connect(id)` should be called
    void connect() {
        CHECK(_send_addrs.size() == _senders.size());
        for(auto sender : _senders) {
            connect(sender.first);
        }
    }

    void* receiver() {
        return _receiver;
    }
    std::map<index_t, void*>& senders() {
        return _senders;
    }
    std::map<index_t, std::string>& send_addrs() {
        return _send_addrs;
    }
    const std::string &send_addr(index_t id) const {
        return _send_addrs[id];
    }

protected:
    void connect(index_t id) {
        PCHECK(0 == ignore_signal_call(zmq_connect, _senders[id], _send_addrs[id].c_str()));
    }

private:
    void*       _zmq_ctx = NULL;
    void*       _receiver = NULL;
    std::string _recv_addr;
    int         _recv_port;
    std::map<index_t, void*>        _senders;
    std::map<index_t, std::string>  _send_addrs;   // with port
};




}; // end namespace swift_snails

#endif
