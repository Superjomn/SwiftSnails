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

struct MetaMessage : public MessageMetaBase {
    index_t message_id;     // used for client's response
    index_t client_id;      // to send response
    index_t shard_id;
};

/*
 * meta_msg: Meta Message (binary type)
 *          contain some meta infomation
 * cont_msg: Content Message (text type, control message) , 
 *          contain the content(such as ip list), the control message 
 *          will be text
 * obb: output text buffer(the response)
 */
typedef std::function<void(BinaryBuffer& meta_msg, TextBuffer& cont_msg, TextBuffer& obb)> MetaCallBack;

/*
 * basic function:
 * receive Message and push message into a queue
 * messages in the queue will be processed by a thread-pool
 * the result will be sent to sender
 */
class BasicMasterServer : public FunctionBasicServer<MetaMessage, char, MetaCallBack> {
public:
    /*
     * local_addr: ip address of current node (meta server node)
     * // both metanode's ip and port should be specified manually
     */
    BasicMasterServer(const std::string &local_addr, int port) {
        listen(local_addr);
    }
private:

};  // end class MetaServer

}; // end namespace swift_snails

#endif
