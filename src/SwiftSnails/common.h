//
//  common.h
//  SwiftSnails
//
//  Created by Chunwei on 12/9/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//

#ifndef SwiftSnails_SwiftSnails_common_h_
#define SwiftSnails_SwiftSnails_common_h_
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* for strncpy */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include "../utils/common.h"
#include "../utils/string.h"
#include "../utils/buffer.h"
namespace swift_snails {

inline void zmq_bind_random_port(const std::string& ip, void* socket, std::string& addr, int& port) {
    const int nturns2try = 10000;
    for(int i = 0; i < nturns2try; i++) {
        port = 1024 + rand() % (65536 - 1024);
        format_string(addr, "tcp://%s:%d", ip.c_str(), port);
        int res;
        PCHECK((res = zmq_bind(socket, addr.c_str()), 
                res == 0 || errno == EADDRINUSE));  // port is already in use
        if(res == 0) break;
        CHECK(i < nturns2try-1) << "can not bind port";
    }
}

template<class FUNC, class... ARGS>
auto ignore_signal_call(FUNC func, ARGS&&... args) 
    -> typename std::result_of<FUNC(ARGS...)>::type 
{
    while (true) {
        auto err = func(args...);
        if (err < 0 && errno == EINTR) {
            LOG(INFO) << "Stop signal is caught. Ignored";
            continue;
        }
        return err;
    }
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
    int sockfd;
    char buf[512];
    struct ifconf ifconf;
    struct ifreq* ifreq;

    ifconf.ifc_len = 512;
    ifconf.ifc_buf = buf;
    PCHECK((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0);
    PCHECK(ioctl(sockfd, SIOCGIFCONF, &ifconf) >= 0);
    PCHECK(0 == close(sockfd));

    ifreq = (struct ifreq*)buf;
    for (int i = 0; i < int(ifconf.ifc_len / sizeof(struct ifreq)); i++) {
        std::string ip;
        ip = inet_ntoa(((struct sockaddr_in*)&ifreq->ifr_addr)->sin_addr);
        if (ip != "127.0.0.1") {
            return ip;
        }
        ifreq++;
    }
    LOG(FATAL) << "IP not found";
    return "";
}

struct IP {
    uint16_t addr[4] = {0};
    uint16_t port = 0;

    IP() {
    }

    IP(const std::string &ip) {
        from_string(ip);
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
    void from_string(const std::string &ip) {
        const char* begin = &ip[0];
        char* end; 
        for(int i = 0; i < 4; i++) {
            //LOG(INFO) << i << "begin:" << begin;
            addr[i] = (uint16_t) std::strtoul(begin, &end, 10);
            begin = end+1;
        }
        port = (uint16_t) std::strtoul(begin, &end, 10);
    }
    friend BinaryBuffer& operator<<(BinaryBuffer& bb, const IP& ip) {
        for(int i = 0; i < 4; i++) {
            bb << ip.addr[i];
        }
        bb << ip.port;
        return bb;
    }
    friend BinaryBuffer& operator>>(BinaryBuffer& bb, IP& ip) {
        for(int i = 0; i < 4; i++) {
            bb >> ip.addr[i];
        }
        bb >> ip.port;
        return bb;
    }
}; // struct IP

};  // end namespace swift_snails
#endif
