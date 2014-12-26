//
//  common.h
//  SwiftSnails
//
//  Created by Chunwei on 12/9/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//

#ifndef SwiftSnails_SwiftSnails_common_h_
#define SwiftSnails_SwiftSnails_common_h_
#include "../utils/common.h"
#include "../utils/string.h"
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

// ensure thread to exit nomally
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



};  // end namespace swift_snails
#endif
