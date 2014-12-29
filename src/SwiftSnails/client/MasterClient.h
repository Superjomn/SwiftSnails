//
//  MasterClient.h
//  SwiftSnails
//
//  Created by Chunwei on 12/26/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//
#ifndef SwiftSnails_SwiftSnails_client_MasterClient_h_
#define SwiftSnails_SwiftSnails_client_MasterClient_h_
#include "../../utils/common.h"
#include "../common.h"
#include "FunctionBasicClient.h"
namespace swift_snails {

typedef std::function<void()> ResponseCallback;

class MasterClient : public FunctionBasicClient<ResponseCallback> {
public:
    explicit MasterClient(const std::string &serv_addr) {
        _local_ip = get_local_ip();
        _serv_addr = serv_addr;
        set_recv_addr(local_ip());
        listen();
        // register Master server as node 0
        register_node(serv_id, serv_addr());
        connect(serv_id);
    }

    const std::string& local_ip() const {
        CHECK(!_local_ip.empty());
        return _local_ip;
    }
    const std::string& serv_addr() const {
        return _serv_addr;   
    }

private:
    std::string _local_ip();
    std::string _serv_addr();   // TODO should be static?
    static const serv_id;
}; // class MasterClient
const MasterClient::serv_id = 0;    // id of Master server is set to 0

MasterClient& global_master_client() {
    static MasterClient mc("http://127.0.0.1:8081");
    return mc;
}

// MasterClient functions

/*
 * send local_ip to master server
 * and register current node to master
 *
 * the master should send node id back to this client
 */

void register_client_to_master() {
    // register server-side function
    if(global_master_server()) {
        global_master_server().register_message_class(1, 
            [](BinaryBuffer& meta_msg_, BinaryBuffer& cont_msg, BinaryBuffer& obb)
            {
                static index_t node_counter = 0; 
                MetaMessage *meta_msg = (MetaMessage*)meta_msg_.zmg();   
                IP client_ip;
                cont_msg >> ip;
                // register client's ip
                global_master_server().add_sender(++node_counter, ip.to_string());
                const int client_id = node_counter;
                // wait for 5000s to ensure all the clients
                // have registered their ip
                // and send back the full ip table
                std::this_thread::sleep_for(std::chrono::milliseconds(5000));

                MetaMessage meta;
                meta.message_class = meta_msg->message_class;
                meta.message_id = meta_msg->message_id;
                meta.client_id = client_id;

                BinaryBuffer obb;
                // content: <id ip> <id ip>
                for(auto &ip : send_addrs) {
                    obb << (index_t) ip.first;
                    obb << IP(ip.second);
                }
                // send back response
                Message meta_message = meta;
                Message cont_message = obb;
                _sender_mutexs[client_id].lock();
                PCHECK(ignore_signal_call(zmq_msg_send, meta_message.zmg(), _senders[client_id], ZMQ_SNDMORE) >= 0);
                PCHECK(ignore_signal_call(zmq_msg_send, cont_message.zmg(), _senders[client_id], 0) >= 0);
                _sender_mutexs[client_id].unlock();
            });

    }
}







}; // end namespace swift_snails
#endif
