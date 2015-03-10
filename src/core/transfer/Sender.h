//
//  Sender.h
//  SwiftSnails
//
//  Created by Chunwei on 3/09/15.
//  Copyright (c) 2015 Chunwei. All rights reserved.
//
#ifndef Swift_transfer_Sender_h_
#define Swift_transfer_Sender_h_
#include "../../utils/common.h"
#include "../AsynExec.h"
#include "../../utils/SpinLock.h"
namespace swift_snails {

class Sender {
public:
    Sender(BaseRoute &route) :
        _route_ptr(&route),
        _zmq_ctx(route.zmq_ctx())
    {
        PCHECK(_receiver = zmq_socket(_zmq_ctx, ZMQ_PULL));
    }

    void set_async_channel(std::shared_ptr<AsynExec::channel_t> channel) {
        _async_channel = channel;
    }

    // set properties
    void set_recv_addr(const std::string &addr) {
        CHECK(!addr.empty());
        _recv_addr = addr;
    }

    int listen() {
        zmq_bind_random_port(_recv_ip, _receiver,  _recv_addr, _recv_port);
        LOG(INFO) << "client listen to address:\t" << _recv_addr;
        return _recv_port;
    }

    /*
     * @request:    request
     * @to_id:  id of the node where the message is sent to
     */
    void send(Request &&request, int to_id) {
        index_t msg_id = _msg_id_counter++;
        request.set_msg_id(msg_id);
        // convert Request to underlying Package
        Package package(request);
        // cache the recall_back
        // when the sent message's reply is received 
        // the call_back handler will be called
        { std::lock_guard<SpinLock> lock(_send_mut);
            CHECK(_msg_handlers.emplace(msg_id, std::move(request.call_back_handler)).second);
        }

        // send the package
        BaseRoute& route = *_route_ptr;

        {
            std::lock_guard<std::mutex> lock(
                * route.send_mutexes()[to_id]
            );
            PCHECK(ignore_signal_call(zmq_msg_send, &package.meta.zmg(), route.senders()[to_id], ZMQ_SNDMORE) >= 0);
            PCHECK(ignore_signal_call(zmq_msg_send, &package.cont.zmg(), route.senders()[to_id], 0) >= 0);
        }
    }

    /*
     * should run as a thread
     * receive reply message, read the reply message and run the correspondding handler
     */
    void main_loop() {
        Package package;
        Request::response_call_back_t handler;
        for(;;) {
            {
                std::lock_guard<std::mutex> lock(_receiver_mutex);
                PCHECK(ignore_signal_call(zmq_msg_recv, &package.meta.zmg(), _receiver, 0) >= 0);
                if(package.meta.size() == 0) break;
                CHECK(zmq_msg_more(&package.meta.zmg()));
                PCHECK(ignore_signal_call(zmq_msg_recv, &package.cont.zmg(), _receiver, 0) >= 0);
                CHECK(!zmq_msg_more(&package.cont.zmg()));
            }

            std::shared_ptr<Request> response = std::make_shared<Request>(std::move(package));
            CHECK(_client_id == 0 || response->meta.client_id == client_id());
            // call the callback handler
            { std::lock_guard<SpinLock> lock(_msg_handlers_mut);
                auto it = _msg_handlers.find(response->message_id());
                CHECK(it != _msg_handlers.end());
                handler = std::move(it->second);
                _msg_handlers.erase(it);
            }

            // execute the response_recallback handler
            /*
            index_t message_id = response->message_id();
            { std::lock_guard<SpinLock> lock(_response_cache_mut);
                // cache response
                _responses.emplace(message_id, std::move(response));
            }
            */

            _async_channel->push(
                // TODO refrence handler?
                [&handler, this, response]() {
                    /*
                    //{
                    _response_cache_mut.lock();    
                    // get cached response
                    auto it = _responses.find(message_id);
                    CHECK(it != this->_responses.end());
                    Request rsp(std::move(it->second));
                    // delete cache
                    this->_responses.erase(message_id);
                    _response_cache_mut.unlock();   
                    //}
                    */
                    handler(response);
                }
            );
        }
    }

    int client_id() const {
        return _client_id;
    }

    // determine whether all sended message get a 
    // reply
    // Attention: not thread safe!
    bool finish_response() {
        return _msg_handlers.empty();
    }

    ~Sender() {
        LOG(WARNING) << "sender exit!";
        PCHECK(0 == zmq_close(_receiver));
    }

    const std::string& recv_ip() const {
        return _recv_ip;
    }
    const std::string& recv_addr() const {
        return _recv_addr;    
    }
    void* zmq_ctx() {
        return _zmq_ctx;
    }

private:

    void*       _zmq_ctx;

    BaseRoute*  _route_ptr;
    void*       _receiver;
    std::mutex  _receiver_mutex;
    std::string _recv_ip;
    std::string _recv_addr;
    int         _recv_port = -1;

    index_t _msg_id_counter = 0;
    std::shared_ptr<AsynExec::channel_t> _async_channel;
    std::map<index_t, Request::response_call_back_t> _msg_handlers;

    SpinLock    _send_mut;
    SpinLock    _msg_handlers_mut;
    //SpinLock    _response_cache_mut;

    // cache response until it is handled
    //std::map<index_t, Request> _responses;

    int _client_id = 0;
}; // end class 


class SenderService {
public:
    SenderService(std::shared_ptr<Sender> sender, int thread_num) : 
        _sender(sender),
        _thread_num(thread_num)
    {
    }

    void start() {
        LOG(WARNING) << "SenderService start " << thread_num() << " threads";
        CHECK(_thread_num > 0);
        _threads.resize(thread_num());
        for(int i = 0; i < thread_num(); i++) {
            _threads[i] = std::thread(
                [this]() {
                    _sender->main_loop();
                }
            );
        }
    }

    void end() {
        LOG(WARNING) << "SenderService service threads exit!";
        CHECK(!_threads.empty());

        { std::lock_guard<SpinLock> lock(_spinlock);
            CHECK(_sender->finish_response());
        }
        // tell all service threads to exit
        for(int i = 0; i < thread_num(); i++) {
            zmq_send_push_once(_sender->zmq_ctx(), &Message().zmg(), _sender->recv_addr() );
        }

        for (int i = 0; i < thread_num(); i++) {
            _threads[i].join();
        }
        _threads.clear();
    }

    int thread_num() {
        return _thread_num;
    }

private:
    std::shared_ptr<Sender> _sender;
    int _thread_num = -1;
    std::vector<std::thread> _threads;
    SpinLock _spinlock;

};



}; // end namespace
#endif
