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
#include "../AsynExec.h"
#include "../../utils/common.h"
#include "../common.h"
#include "../../utils/SpinLock.h"

namespace swift_snails {

class BasicServer : public VirtualObject {
public:

    typedef std::function<void(Request&)> Handler;

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

    void register_node(index_t id, const std::string& addr) {
        CHECK(_senders.count(id) == 0); // a sender can be registered only once
        void *sender = zmq_socket(_zmq_ctx, ZMQ_PUSH);
        _senders.emplace(id, sender);
        _send_addrs.emplace(id, addr);
    }
    /*
     * listen to addr
     * the address should be determined first
     *
     * @addr:   like: tcp://127.0.0.1:80801
     */
    void listen(const std::string &addr) {
        LOG(INFO) << "server listen to " << addr;
        _recv_addr = addr;
        int res;
        PCHECK((res = zmq_bind(_receiver, addr.c_str()), res == 0));
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

    void register_message_class(index_t id, Handler&& handler) {
        LOG(INFO) << "register message class: " << id;
        std::lock_guard<SpinLock> lock(_spinlock);
        CHECK(_message_classes.count(id) == 0) <<
                "callback should be registerd only once";
        _message_classes.emplace(id, std::move(handler));
    }

    void remove_message_class(index_t id) {
        std::lock_guard<SpinLock> lock(_spinlock);
        auto pos = _message_classes.find(id);
        CHECK(pos != _message_classes.end()) <<
                "no message_class:" << id << " found!";
        _message_classes.erase(pos);
    }

    void set_async_channel(std::shared_ptr<AsynExec::channel_t> channel) {
        _async_channel = channel;
    }

    void main_loop() {
        Package package;
        for(;;) {
            { std::lock_guard<std::mutex> lock(_receiver_mutex);
                // receive meta-message first
                PCHECK(ignore_signal_call(zmq_msg_recv, &package.meta.zmg(), _receiver, 0) >= 0);
                if(package.meta.size() == 0) break; // exit server
                CHECK(package.meta.size() == sizeof(MetaMessage));
                CHECK(zmq_msg_more(&package.meta.zmg()));
                // receive content-message later
                PCHECK(ignore_signal_call(zmq_msg_recv, &package.cont.zmg(), _receiver, 0) >= 0);
                CHECK(!zmq_msg_more(&package.cont.zmg()));
            }
            Request request(std::move(package));
            // read package
            auto it = _message_classes.find(request.meta.message_id);
            CHECK(it != _message_classes.end());
            // push the handler to async channel
            Handler handler = it->second;
            _async_channel->push(
                [this, &handler, &request]{
                    handler(request);
                });
        }
        LOG(WARNING) << "server terminated!";
    }

    /* 
     * multi listen threads
     */ 
    void set_listen_thread_num(int x) {
        CHECK(x > 0);
        _thread_num = x;
    }
    int thread_num() const {
        return _thread_num;
    }   
    // init threads
    void initialize() {
        LOG(INFO) << "initialize " << thread_num() << " threads";
        CHECK(_thread_num > 0);
        _threads.resize(thread_num());
        for(int i = 0; i < thread_num(); i++) {
            _threads[i] = std::thread(
                [this]() {
                    main_loop();
                });
            /*
            _threads.emplace_back(
                [this] () {
                    main_loop()
                });
            */
        }
    }
    // terminate all threads
    void finalize() {
        LOG(WARNING) << "client exit!";
        CHECK(!_threads.empty());
        // terminate all threads
        for (int i = 0; i < thread_num(); i++) {
            zmq_send_push_once(_zmq_ctx, &Message().zmg(), _recv_addr);
        }
        for (int i = 0; i < thread_num(); i++) {
            _threads[i].join();
        }
        _threads.clear();
    }

protected:
    void connect(index_t id) {
        PCHECK(0 == ignore_signal_call(zmq_connect, _senders[id], _send_addrs[id].c_str()));
    }

private:
    void*       _zmq_ctx = NULL;
    void*       _receiver = NULL;
    std::mutex  _receiver_mutex;
    std::string _recv_addr;
    //std::string _recv_ip;
    int _thread_num;
    std::vector<std::thread> _threads;
    //int         _recv_port = -1;
    std::map<index_t, void*>        _senders;
    std::map<index_t, std::string>  _send_addrs;   // with port
    std::map<index_t, Handler> _message_classes;
    SpinLock _spinlock;
    std::shared_ptr<AsynExec::channel_t> _async_channel;
}; // class BasicServer


}; // namespace swift_snails



#endif
