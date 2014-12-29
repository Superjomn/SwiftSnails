//
//  BasicClient.h
//  SwiftSnails
//
//  Created by Chunwei on 12/26/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//
#ifndef SwiftSnails_SwiftSnails_client_FunctionBasicClient_h_
#define SwiftSnails_SwiftSnails_client_FunctionBasicClient_h_
#include "BasicClient.h"
namespace swift_snails {

template<typename ResponseCallback>
class FunctionBasicClient : public BasicClient {
public:
    explicit FunctionBasicClient() { }
    /*
     * @client_id: id of this client
     *             each node will have a unique client
     *             client's id = node's id
     */
    explicit FunctionBasicClient(int client_id) :
        _client_id(client_id)
    { }
    /*
     * @dest_id: destnation node's id
     * @msg_class: class id of message
     *             specify a server-side handler to process the message
     * @obb: content of the message to send
     * @handler: response callback handler
     *             when a response from server is received
     *             the cached response callback will be used to process
     *             the response
     */
    void send_request(int dest_id, int64_t msg_class, BinaryBuffer& obb, ResponseCallback handler) {
        CHECK(obb.cursor() == obb.buffer());
        _spinlock.lock();
        int64_t msg_id = _msg_id_counter++;
        // cache the recall_back
        CHECK(_msg_handlers.emplace(msg_id, std::move(handler)).second);
        // meta message
        MetaMessage meta;
        meta.message_class = msg_class;
        meta.message_id = msg_id;
        meta.client_id = _client_id;
        Message meta_msg;
        meta_msg.assign((char*)&head, sizeof(MetaMessage));
        // message content
        Message msg = obb;
        // send message to dest
        _sender_mutexs[dest_id].lock();
        PCHECK(ignore_signal_call(zmq_msg_send, meta.zmg(), _senders[dest_id], ZMQ_SNDMORE) >= 0);
        PCHECK(ignore_signal_call(zmq_msg_send, msg.zmg(), _senders[dest_id], 0) >= 0);
        _sender_mutexs[dest_id].unlock();
    }

    void main_loop() {
        Message meta_msg;
        Message msg;
        MetaMessage meta;
        ResponseCallback handler;
        BinaryBuffer iar;

        while(true) {
            _receiver_mutex.lock();
            PCHECK(ignore_signal_call(zmq_msg_recv, meta_msg.zmg(), _receiver, 0) >= 0);
            if (meta_msg.length() == 0) {
                LOG(INFO) << "receive empty meta message, client exit!";
                _receiver_mutex.unlock();
                return;
            }
            CHECK(meta_msg.length() == sizeof(MetaMessage));
            CHECK(zmq_msg_more(meta_msg.zmg()));
            PCHECK(ignore_signal_call(zmq_msg_recv, msg.zmg(), _receiver, 0) >= 0);
            CHECK(!zmq_msg_more(msg.zmg()));
            _receiver_mutex.unlock();

            memcpy(&meta, meta_msg.buffer(), sizeof(MetaMessage));
            CHECK(meta.client_id == _client_id);
            
            _spinlock.lock();
            auto it = _msg_handlers.find(meta.message_id);
            CHECK(it != _msg_handlers.end());
            handler = std::move(it->second);
            _msg_handlers.erase(it);
            _spinlock.unlock();
            // TODO wait to complete
            iar = msg;
            CHECK(iar.length = msg.length());
        }

    }

    int client_id() {
        return _client_id;
    }
    void set_client_id(int client_id) {
        CHECK(client_id > 0);
        _client_id = client_id;
    }

protected:
    int _msg_id_counter = 0;
    std::unordered_map<int64_t, ResponseCallback> _msg_handlers;
    int _client_id;

}; // class FunctionBasicClient

}; // namespace swift_snails
#endif
