//
//  Sender.h
//  SwiftSnails
//
//  Created by Chunwei on 3/09/15.
//  Copyright (c) 2015 Chunwei. All rights reserved.
//
#ifndef Swift_transfer_Transfer_h_
#define Swift_transfer_Transfer_h_
#include "../../utils/common.h"
#include "../AsynExec.h"
#include "../../utils/SpinLock.h"
#include "Sender.h"
#include "Receiver.h"
namespace swift_snails {

template<typename Route>
class Transfer : public VirtualObject {

public:
    // message class handler
    typedef Receiver::handler_t             msgcls_handler_t;
    // message respons callback handler
    typedef Request::response_call_back_t   msgrsp_handler_t;

    explicit Transfer() {
        // init listen services
        _sender_listen_service = std::make_shared<ListenService>(_sender);
        _receiver_listen_service = std::make_shared<ListenService>(_receiver);
    }
    void init_async_channel(int thread_num) {
        CHECK(!_channel) << "async channel has been created";
        AsynExec as(thread_num);
        _channel = as.open();
    }

    explicit Transfer(int channel_thread_num) {
        // init listen services
        _sender_listen_service = std::make_shared<ListenService>(_sender);
        _receiver_listen_service = std::make_shared<ListenService>(_receiver);
        // set async channel
        AsynExec as(channel_thread_num);
        _channel = as.open();
    }
    ~Transfer() {
        LOG(WARNING) << "Transfer desconstruct!";
        _sender_listen_service->end();
        _receiver_listen_service->end();
        // close the channel
        CHECK(_channel) << "async channel is not inited or has been closed";
        _channel->close();
    }

    void set_listen_service_num(int num) {
        CHECK(num > 0);
        _listen_thread_num = num;
        _sender_listen_service->set_thread_num(num);
        _receiver_listen_service->set_thread_num(num);
    }
    /*
    void set_async_channel(std::shared_ptr<AsynExec::channel_t> channel) {
        CHECK(channel);
        _channel = channel;
    }
    */
    void set_client_id(int id) {
        _sender->set_client_id(id);
    }
    void send(Request &&request, int to_id) {
        _sender->send(std::move(request), to_id);
    }
    MessageClass<Receiver::handler_t>& 
    message_class() {
        return _receiver->message_class();

    }

    // listen
    void start_sender_service(const std::string &addr = "") {
        if(!addr.empty()) {
            _sender->listen(addr);
        } else {
            _sender->listen();
        }
        CHECK(_channel);
        _sender->set_async_channel(_channel);
        _sender_listen_service->start();
    }

    // listen
    void start_receiver_service(const std::string &addr = "") {
        if(!addr.empty()) {
            _receiver->listen(addr);
        } else {
            _receiver->listen();
        }
        CHECK(_channel);
        _receiver->set_async_channel(_channel);
        _receiver_listen_service->start();
    }

    Route& route() {
        return _route;
    }
    // receiver listen address
    const std::string &receiver_addr() const {
        return _receiver->recv_addr();
    }
    // sender listen address
    const std::string &sender_addr() const {
        return _sender->recv_addr();
    }

private:
    Route _route;
    std::shared_ptr<Sender> _sender = std::make_shared<Sender>(_route);
    std::shared_ptr<Receiver> _receiver = std::make_shared<Receiver>(_route);
    // need init
    std::shared_ptr<ListenService> _sender_listen_service;
    std::shared_ptr<ListenService> _receiver_listen_service;
    int _listen_thread_num = 2;
    // channel
    //int _channel_thread_num = 2;
    std::shared_ptr<AsynExec::channel_t> _channel;
};  // end class Transfer


template<typename Route>
inline Transfer<Route>& global_transfer() {
    static Transfer<Route> transfer;
    static std::once_flag flag;

    std::call_once( flag,
        []() {
            // TODO load config from file
            int listen_thread_num = 2;
            int channel_thread_num = 2;
            transfer.init_async_channel(channel_thread_num);
            transfer.set_listen_service_num(listen_thread_num);
            transfer.start_sender_service();
            transfer.start_receiver_service();
        });

    return transfer;
}


};  // end namespace swift_snails
#endif

