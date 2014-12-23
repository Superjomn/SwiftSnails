//
//  common.h
//  SwiftSnails
//
//  Created by Chunwei on 12/9/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//

#ifndef SwiftSnails_SwiftSnails_common_h_
#define SwiftSnails_SwiftSnails_common_h_
namespace swift_snails {

inline void zmq_bind_random_port(const std::string& ip, void* socket, std::string& addr, int& port) {
    const int nturns2try = 10000;
    for(int i = 0; i < nturns2try; i++) {
        port = 1024 + rand() % (65536 - 1024);
        format_string(addr, "tcp://%s:%d", ip, port);
        int res;
        PCHECK((res = zmq_bind(socket, addr.c_str()), 
                res == 0 || errno == EADDRINUSE));  // port is already in use
        if(res == 0) break;
        CHECK(i < nturns2try-1) << "can not bind port";
    }
}

};  // end namespace swift_snails
#endif
