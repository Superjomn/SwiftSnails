//
//  one.h
//  SwiftSnails
//
//  Created by Chunwei on 3/11/15.
//  Copyright (c) 2015 Chunwei. All rights reserved.
//
#ifndef SwiftSnails_transfer_transfer_h_
#define SwiftSnails_transfer_transfer_h_
#include "../../utils/all.h"
#include "../Message.h"
#include "../AsynExec.h"
#include "Listener.h"
namespace swift_snails {

template<typename Handler>
class MessageClass :public VirtualObject {
public:
    // TODO change spinlock to read-write-lock ? 
    // do not change now , because message_class will only be changed 
    // between read
    void add (index_t id, Handler&& handler) {
        LOG(INFO) << "register message class: " << id;
        std::lock_guard<SpinLock> lock(_spinlock);
        is_writing = true;
        CHECK(_message_classes.count(id) == 0) <<
                "callback should be registerd only once";
        _message_classes.emplace(id, std::move(handler));
        is_writing = false;
    }

    void remove (index_t id) {
        std::lock_guard<SpinLock> lock(_spinlock);
        is_writing = true;
        auto pos = _message_classes.find(id);
        CHECK(pos != _message_classes.end()) <<
                "no message_class:" << id << " found!";
        _message_classes.erase(pos);
        is_writing = false;
    }

    Handler& get (index_t id) {
        auto it = _message_classes.find(id); 
        CHECK(it != _message_classes.end());
        return it->second;
    }

private:
    SpinLock _spinlock;
    std::map<index_t, Handler> _message_classes;
    bool is_writing = false;
    //std::condition_variable _cond;
    //std::mutex _mut;
};  // end MessageClass

template<typename Route>
class Transfer : public Listener {
public:

    // message class handler
    typedef std::function<void(std::shared_ptr<Request>, Request&)> msgcls_handler_t;
    // message respons callback handler
    typedef Request::response_call_back_t   msgrsp_handler_t;


    explicit Transfer() {
        set_zmq_ctx(_route.zmq_ctx());
    }
    // init later
    void init_async_channel(int thread_num) {
        CHECK(!_async_channel) << "async channel has been created";
        AsynExec as(thread_num);
        _async_channel = as.open();
    }
    void set_client_id(int client_id) {
        _client_id = client_id;
    }
    /*
     * @request:    request
     * @to_id:  id of the node where the message is sent to
     */
    void send(Request &&request, int to_id) {
        index_t msg_id = _msg_id_counter++;
        request.set_msg_id(msg_id);
        CHECK(_client_id != -2) << "shoud set client_id first";
        request.meta.client_id = _client_id;
        // convert Request to underlying Package
        Package package(request);
        LOG(INFO) << "send package";
        // cache the recall_back
        // when the sent message's reply is received 
        // the call_back handler will be called
        { std::lock_guard<SpinLock> lock(_send_mut);
            CHECK(_msg_handlers.emplace(msg_id, std::move(request.call_back_handler)).second);
            LOG(INFO) << "call_back_handler is registered";
        }

        // send the package
        Route& route = _route;

        {
            LOG(INFO) << "to lock send_mutex";
            std::lock_guard<std::mutex> lock(
                * route.send_mutex(to_id)
            );
            LOG(INFO) << "zmq to send message";
            PCHECK(ignore_signal_call(zmq_msg_send, &package.meta.zmg(), route.sender(to_id), ZMQ_SNDMORE) >= 0);
            PCHECK(ignore_signal_call(zmq_msg_send, &package.cont.zmg(), route.sender(to_id), 0) >= 0);
        }
    }
    /*
     * should run as a thread
     * receive request  and run corresponding message-class-handler
     * receive reply message, read the reply message and run the correspondding handler
     */
    void main_loop() {
        Package package;
        Request::response_call_back_t handler;
        for(;;) {

            { std::lock_guard<std::mutex> lock(receiver_mutex() );
                PCHECK(ignore_signal_call(zmq_msg_recv, &package.meta.zmg(), receiver(), 0) >= 0);
                if(package.meta.size() == 0) break;
                CHECK(zmq_msg_more(&package.meta.zmg()));
                PCHECK(ignore_signal_call(zmq_msg_recv, &package.cont.zmg(), receiver(), 0) >= 0);
                CHECK(!zmq_msg_more(&package.cont.zmg()));
            }

            std::shared_ptr<Request> request = std::make_shared<Request>(std::move(package));

            if(request->is_response()) {
                LOG(INFO) << "receive a response, message_id: " << request->meta.message_id;
                handle_response(request);
            } else {
                LOG(INFO) << "receive a request, message_class: " << request->meta.message_class;
                handle_request(request);
            }
        }
        LOG(WARNING) << "sender terminated!";
    }

    // handle the request from other node
    // and run message_class-handler
    void handle_request(std::shared_ptr<Request> request) {

        msgcls_handler_t handler = _message_class.get( request->meta.message_class);
        CHECK(!_async_channel->closed());
        LOG(INFO) << "push task to channel";
        _async_channel->push(
            [this, handler, request] {
                Request response;
                handler(request, response);
                // set response meta
                response.meta.message_id = request->meta.message_id;
                response.meta.client_id = request->meta.client_id;
                //response.meta.message_class = request->meta.message_class;
                // response flag
                response.meta.message_class = -1;

                LOG(INFO) << "send response to client " << request->meta.client_id;
                // only send response with content
                // empty response will not be sent, and master should
                // send a response with content later
                if(response.cont.size() > 0) {
                    send_response(std::move(response),  request->meta.client_id);
                } else {
                    LOG(INFO) << "empty response, not send";
                }
            }
        );
    }
    
    // handle the response from other node
    // and run response-callback handler
    void handle_response(std::shared_ptr<Request> &response) {
        Request::response_call_back_t handler;
        // NOTE: allow client_id == 0 , when the cluster's route has not been created
        CHECK((_client_id >=-1 && _client_id <= 0) || response->meta.client_id == client_id()) 
                << "get client_id\t" << response->meta.client_id
                << "\tlocal_id\t" << _client_id;

        //LOG(INFO) << ".. call response handler";
        // call the callback handler
        { std::lock_guard<SpinLock> lock(_msg_handlers_mut);
            auto it = _msg_handlers.find(response->message_id());
            CHECK(it != _msg_handlers.end());
            handler = std::move(it->second);
            _msg_handlers.erase(it);
        }

        //LOG(INFO) << ".. push response handler to channel";

        // execute the response_recallback handler
        _async_channel->push(
            // TODO refrence handler?
            [handler, this, response]() {
                handler(response);
            }
        );
    }

    void send_response(Request &&request, int to_id) {
        request.meta.client_id = to_id;
        Package package(request);
        Route& route = _route;
        {
            // TODO will the mutex share between sender and receiver 
            // effect performance?
            std::lock_guard<std::mutex> lock(
                * route.send_mutex(to_id)
            );
            PCHECK(ignore_signal_call(zmq_msg_send, &package.meta.zmg(), route.sender(to_id), ZMQ_SNDMORE) >= 0);
            PCHECK(ignore_signal_call(zmq_msg_send, &package.cont.zmg(), route.sender(to_id), 0) >= 0);

            LOG(INFO) << "response has been sent";
        }
    }

    int client_id() const {
        return _client_id;
    }
    Route& route() {
        return _route;
    }
    std::shared_ptr<AsynExec::channel_t>& async_channel() {
        return _async_channel;
    }
    // determine whether all sended message get a 
    // reply
    // Attention: not thread safe!
    bool service_complete() {
        return _msg_handlers.empty();
    }
    /*
     * work as an API
     *
     * control Receiver service by adding handler to message_class
     */
    MessageClass<msgcls_handler_t>& message_class() {
        return _message_class;
    }

    ~Transfer() {
        CHECK(service_complete());

        service_end();

        LOG(WARNING) << "transfer listener exit!";
        PCHECK(0 == zmq_close(_receiver));
        _receiver = NULL;

        //_async_channel->close();
    }

private:
    
    Route _route;

    index_t _msg_id_counter = 0;
    std::shared_ptr<AsynExec::channel_t> _async_channel;
    std::map<index_t, Request::response_call_back_t> _msg_handlers;

    SpinLock    _send_mut;
    SpinLock    _msg_handlers_mut;
    MessageClass<msgcls_handler_t> _message_class;

    int _client_id = -2;

};  // end class Transfer



template<typename Route>
Transfer<Route> &global_transfer() {
	static Transfer<Route> transfer;
	static std::once_flag flag;
	std::call_once(flag,
		[]{
            LOG(WARNING) << "init transfer ...";
            std::string listen_addr = global_config().get_config("listen_addr").to_string();
            // TODO read from config file
            int async_thread_num = global_config().get_config("async_exec_num").to_int32();
            int service_thread_num = global_config().get_config("listen_thread_num").to_int32();

            if(!listen_addr.empty()) {
                transfer.listen(listen_addr);
            } else {
                transfer.listen();
            }
            // TODO read from config
            // register master server
            //LOG(WARNING) << "local register server ...";
            //std::string addr = global_config().get_config("listen_addr").to_string();
            //transfer.route().register_node_(true, std::move(addr));
            transfer.init_async_channel(async_thread_num);
            transfer.set_thread_num(service_thread_num);
            transfer.service_start();
		});
    return transfer;
}


};  // end namespace swift_snails
#endif
