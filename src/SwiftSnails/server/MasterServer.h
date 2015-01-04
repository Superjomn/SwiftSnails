//
//  MasterServer.h
//  SwiftSnails
//
//  Created by Chunwei on 12/23/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//

#ifndef SwiftSnails_SwiftSnails_server_MasterServer_h_
#define SwiftSnails_SwiftSnails_server_MasterServer_h_
#include "../Message.h"
#include "../../utils/common.h"

namespace swift_snails {

/*
 * meta_msg: Meta Message (binary type)
 *          contain some meta infomation
 * cont_msg: Content Message (text type, control message) , 
 *          contain the content(such as ip list), the control message 
 *          will be text
 * obb: output text buffer(the response)
 */
typedef std::function<void(BinaryBuffer& meta_msg, TextBuffer& cont_msg, TextBuffer& obb)> MasterCallBack;

/*
 * basic function:
 * receive Message and push message into a queue
 * messages in the queue will be processed by a thread-pool
 * the result will be sent to sender
 */
class MasterServer : public FunctionBasicServer<MetaMessage, char, MasterCallBack> {
public:
    /*
     * local_addr: ip address of current node (meta server node)
     * // both metanode's ip and port should be specified manually
     */
    typedef MetaMessage MetaMsg_t;
    typedef char Msg_t;

    MasterServer(const std::string &local_addr, int port) {
        listen(local_addr);
    }

    void process_message(MetaMsg_t *meta_msg, Msg_t *cont_msg) {
        index_t message_id = meta_msg->message_id;
        (MasterCallBack) (message_class[message_id]) (meta_msg, cont_msg);
    }

    std::map<index_t, IP> &cluster_ip_table() {
        return _cluster_ip_table;
    }

private: 
    std::map<index_t, IP> _cluster_ip_table;

};  // end class MetaServer


/*
 * only the real Master server will return Master
 */
MasterServer& global_master_server() {
    static MasterServer ms;
    return ms;
}

}; // end namespace swift_snails

#endif
