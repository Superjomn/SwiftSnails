#pragma once
#include "common.h"

namespace swift_snails {

class DaemonThread : public VirtualObject {
protected:
    void start(const bool &terminate_flag) {
        std::thread t(
        [this, &terminate_flag] {
            while(true) {
                if(terminate_flag) break;
                service_with_wait();
            }
            LOG(INFO) << "DaemonThread exit!";
        });
        t.detach();
    }

    virtual void service_with_wait() = 0;

};  // class DaemonThread


};  // end namespace swift_snails
