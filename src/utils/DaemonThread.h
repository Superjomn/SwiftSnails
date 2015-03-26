#pragma once
#include "common.h"

class DaemonThread : public VirtualObject {
protected:
    void start(const bool &terminate_flag) {
        std::thread t(
        [] {
            while(true) {
                if(terminate_flag) break;
                service_with_wait();
            }
            LOG(INFO) << "DaemonThread exit!";
        });
        t.detach();
    }

    void service_with_wait() = 0;

};  // class DaemonThread

