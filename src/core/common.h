//
//  common.h
//  core
//
//  Created by Chunwei on 12/9/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//

#ifndef SwiftSnails_core_common_h_
#define SwiftSnails_core_common_h_
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* for strncpy */
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include "../utils/common.h"
#include "../utils/string.h"
#include "../utils/Buffer.h"
//#include "Message.h"
namespace swift_snails {

template<class FUNC, class... ARGS>
auto ignore_signal_call(FUNC func, ARGS&&... args) 
    -> typename std::result_of<FUNC(ARGS...)>::type {
    for (;;) {
        auto err = func(args...);
        if (err < 0 && errno == EINTR) {
            LOG(INFO) << "Signal is caught. Ignored.";
            continue;
        }
        return err;
    }
}

inline void zmq_bind_random_port(const std::string& ip, void* socket, std::string& addr, int& port) {
    for(;;) {
        addr = "";
        port = 1024 + rand() % (65536 - 1024);
        format_string(addr, "tcp://%s:%d", ip.c_str(), port);
        // ATTENTION: fix the wied memory leak
        // add the LOG valhind detect no memory leak, else ...
        LOG(WARNING) << "try addr: " << addr;
        int res = 0;
        PCHECK((res = zmq_bind(socket, addr.c_str()), 
                res == 0 || errno == EADDRINUSE));  // port is already in use
        if(res == 0) break;
    }
}

inline void zmq_send_push_once(void* zmq_ctx, zmq_msg_t* zmg, const std::string& addr) {
    void* sender = nullptr;
    PCHECK(sender = zmq_socket(zmq_ctx, ZMQ_PUSH));
    PCHECK(0 == ignore_signal_call(zmq_connect, sender, addr.c_str()));
    PCHECK(ignore_signal_call(zmq_msg_send, zmg, sender, 0) >= 0);
    PCHECK(0 == zmq_close(sender));
}

// ensure thread to exit normally
class thread_guard {
    std::thread& _t;
public:
    explicit thread_guard(std::thread& t) :
        _t(t)
    { }
    explicit thread_guard(std::thread&& t) : 
        _t(t)
    { }
    explicit thread_guard(thread_guard&& other) :
        _t(other._t)
    { }
    thread_guard(thread_guard const&) = delete;
    thread_guard& operator=(thread_guard const&) = delete;

    void join() {
        CHECK(_t.joinable());
        _t.join();
    }
    ~thread_guard() {
        if(_t.joinable()) _t.join();
    }
};

std::string get_local_ip() {
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;
    std::string ip;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            if(strcmp(addressBuffer, "127.0.0.1") != 0) {
                ip = addressBuffer;
            }
            //printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer); 
        } 
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
    return std::move(ip);
}

struct Addr {
    uint16_t addr[4] = {0};
    uint16_t port = 0;

    Addr() {
    }

    Addr(const Addr& other) {
        port = other.port;
        for(int i = 0; i < 4; i ++) {
            addr[i] = other.addr[i];
        }
    }

    Addr(const std::string &ip) {
        from_string(ip);
    }
    void from_string(const std::string &_addr) {
        std::string ip;
        // TODO fix this trick
        // heads with tcp://?
        if(_addr[0] == 't') {
            ip = _addr.substr(6);
        } else {
            ip = _addr;
        }
        const char* begin = &ip[0];
        char* end; 
        for(int i = 0; i < 4; i++) {
            //LOG(INFO) << i << "begin:" << begin;
            addr[i] = (uint16_t) std::strtoul(begin, &end, 10);
            begin = end+1;
        }
        port = (uint16_t) std::strtoul(begin, &end, 10);
    }
    std::string to_string() const {
        std::stringstream ss;
        ss << addr[0];
        for(int i = 1; i < 4; i++) {
            ss << "." << addr[i];
        }
        ss << ":" << port;
        return std::move(ss.str());
    }
    friend BinaryBuffer& operator<<(BinaryBuffer& bb, const Addr& ip) {
        for(int i = 0; i < 4; i++) {
            bb << ip.addr[i];
        }
        bb << ip.port;
        return bb;
    }
    friend BinaryBuffer& operator>>(BinaryBuffer& bb, Addr& ip) {
        for(int i = 0; i < 4; i++) {
            bb >> ip.addr[i];
        }
        bb >> ip.port;
        return bb;
    }
}; // struct Addr

};  // end namespace swift_snails
#endif
