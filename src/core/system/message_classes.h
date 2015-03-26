//
//  message_classes.h
//  SwiftSnails
//
//  Created by Chunwei on 3/15/15.
//  Copyright (c) 2015 Chunwei. All rights reserved.
//
#ifndef SwiftSnails_core_transfer_framework_message_classes_h_
#define SwiftSnails_core_transfer_framework_message_classes_h_
#include "../common.h"
namespace swift_snails {

// registed message class ids
enum MSG_CLS {
    /*
     * transfer framework init 
     * all the workers and servers register themselves to master
     */
	NODE_INIT_ADDRESS,  
    /*
     * node ask for hashfrag
     */
    NODE_ASKFOR_HASHFRAG,
    /*
     * worker PULL parameter from server
     */
    WORKER_PULL_REQUEST,
    /*
     * worker push local grad to server
     */
    WORKER_PUSH_REQUEST
};  // end enum MSG_CLS



}; // end namespace swift_snails
#endif
