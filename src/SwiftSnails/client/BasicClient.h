//
//  BasicClient.h
//  SwiftSnails
//
//  Created by Chunwei on 9/01/15.
//  Copyright (c) 2015 Chunwei. All rights reserved.
//

#ifndef SwiftSnails_SwiftSnails_BasicClient_h_
#define SwiftSnails_SwiftSnails_BasicClient_h_
#include "../../utils/common.h"
#include "../common.h"
#include "../AsynExec.h"
#include "../../utils/SpinLock.h"
namespace swift_snails {

class BasicClient : public VirtualObject {
public:
    explicit BasicClient() {
        _zmq_ctx = zmq_ctx_new();
        PCHECK(_receiver = zmq_socket(_zmq_ctx, ZMQ_PULL));
    }

    ~BasicClient() {
        LOG(WARNING) << "client out";
        PCHECK(0 == zmq_close(_receiver));
        for( auto& sender : _senders) {
            PCHECK(0 == zmq_close(sender.second));
        }
        PCHECK(0 == zmq_ctx_destroy(_zmq_ctx));
    }
    /*
     * listen to a random port
     */
    int listen() {
        //LOG(INFO) << "client try to listen to address:\t" << _recv_addr;
        zmq_bind_random_port(_recv_ip, _receiver,  _recv_addr, _recv_port);
        LOG(INFO) << "client listen to address:\t" << _recv_addr;
        return _recv_port;
    }
    /*
     * add address to client
     *
     * @id:      id of the node
     * @addr:    address of the node
     */
    void register_node(int id, const std::string &addr) {
        std::lock_guard<std::mutex> lock(_mut);
        _send_addrs.emplace(id, addr);
        _send_mutexes.emplace(id, std::unique_ptr<std::mutex>(new std::mutex));
        void *sender = zmq_socket(_zmq_ctx, ZMQ_PULL);
        _senders.emplace(id, sender);
    }
    /*
     * connect to all nodes
     */
    void connect_all() {
        for(auto& sender : _senders) {
            connect(sender.first);
        }
    }
    /*
     * @request:    request
     * @to_id:  id of the node where the message is sent to
     */
    void send(Request &request, int to_id) {
        // build package to send
    	index_t msg_id = _msg_id_counter++;
    	request.meta.message_id = msg_id;
        Package pkg(request);
        // cache the recall_back
        // when the sent message's reply is received by this client
        // the call_back handler should be called
    	_spinlock.lock();
        CHECK(_msg_handlers.emplace(msg_id, std::move(request.call_back_handler)).second);
    	_spinlock.unlock();
        // send package
        _send_mutexes[to_id]->lock();
        PCHECK(ignore_signal_call(zmq_msg_send, &pkg.meta.zmg(), _senders[to_id], ZMQ_SNDMORE) >= 0);
        PCHECK(ignore_signal_call(zmq_msg_send, &pkg.cont.zmg(), _senders[to_id], 0) >= 0);
        _send_mutexes[to_id]->unlock();
    }
    /*
     * should run as a thread
     * receive reply message, read the reply message and run the correspond handler
     */
    void main_loop() {
        Package pkg;
        Request::ResponseCallBack handler;
        for(;;) {
            // only one thread can receive message
            // receive message
            { 
                std::lock_guard<std::mutex> lock(_receiver_mutex);
                PCHECK(ignore_signal_call(zmq_msg_recv, &pkg.meta.zmg(), _receiver, 0) >= 0);
                if(pkg.meta.size() == 0) break;
                CHECK(zmq_msg_more(&pkg.meta.zmg()));
                PCHECK(ignore_signal_call(zmq_msg_recv, &pkg.cont.zmg(), _receiver, 0) >= 0);
                CHECK(!zmq_msg_more(&pkg.cont.zmg()));
            } // unlock mutex

            Response response(std::move(pkg));
            CHECK(response.meta.client_id == client_id());
            // call the callback handler
            { 
                std::lock_guard<SpinLock> lock(_spinlock);
                auto it = _msg_handlers.find(response.meta.message_id);
                CHECK(it != _msg_handlers.end());
                handler = std::move(it->second);
                _msg_handlers.erase(it);
            }
            _async_channel->push(
                [&handler, &response] () {
                    //handler(response);
                    handler(response);
                });
        }
        LOG(WARNING) << "thread terminate!";
    }
    // set properties
    void set_recv_addr(const std::string &addr) {
        CHECK(!addr.empty());
        _recv_addr = addr;
    }
    void set_recv_ip(const std::string &ip) {
        CHECK(!ip.empty());
        _recv_ip = ip;
    }
    void set_client_id(index_t client_id) {
        CHECK(client_id > 0);
        _client_id = client_id;
    }
    void set_async_channel(std::shared_ptr<AsynExec::channel_t> channel) {
        _async_channel = channel;
    }
    void set_listen_thread_num(int x) {
        CHECK(x > 0);
        _thread_num = x;
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
        { std::lock_guard<SpinLock> lock(_spinlock);
            // assure all messages get a response
            CHECK(_msg_handlers.empty());
        }
        // terminate all threads
        for (int i = 0; i < thread_num(); i++) {
            zmq_send_push_once(_zmq_ctx, &Message().zmg(), _recv_addr);
        }
        for (int i = 0; i < thread_num(); i++) {
            _threads[i].join();
        }
        _threads.clear();
    }
    // get properties
    const std::string& recv_addr() const {
        return _recv_addr;
    }
    const std::string& recv_ip() const {
        return _recv_ip;
    }
    int recv_port() const {
        return _recv_port;
    }
    int thread_num() const {
        return _thread_num;
    }   
    index_t client_id() const {
        return _client_id;
    }
protected:
    /*
     * connect to idth-node
     *
     * @id: id of the node
     */
    void connect(index_t id) {
        PCHECK(0 == ignore_signal_call(zmq_connect, _senders[id], _send_addrs[id].c_str()));
    }

private:
    void* _zmq_ctx = NULL;
    int _thread_num = 0;
    // client
    index_t _client_id = 0;
    // receiver
    std::string _recv_ip;
    std::string _recv_addr;
    void* _receiver = NULL;
    int _recv_port = 0;
    // senders
    std::map<index_t, void*> _senders;
    std::map<index_t, std::string> _send_addrs;
    std::map<index_t, std::unique_ptr<std::mutex>> _send_mutexes;
    // message
    index_t _msg_id_counter = 0;
    // unordered_map
    std::map<index_t, Request::ResponseCallBack> _msg_handlers;
    // locks
    SpinLock _spinlock;
    std::mutex _mut;    // protect data of this client
    std::mutex _receiver_mutex;
    // async channel
    std::shared_ptr<AsynExec::channel_t> _async_channel;
    std::vector<std::thread> _threads;
}; // class BasicClient

}; // end namespace swift_snails
#endif
