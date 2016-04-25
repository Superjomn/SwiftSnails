#pragma once
#include "common.h"

namespace swift_snails {

class DaemonThread : public VirtualObject {
protected:
  void start(const bool &terminate_flag,
             std::function<void()> service_with_wait) {
    std::thread t([this, &terminate_flag, service_with_wait] {
      while (!terminate_flag) {
        // if(terminate_flag) break;
        service_with_wait();
      }
      LOG(INFO) << "DaemonThread exit!";
    });
    t.detach();
  }

}; // class DaemonThread

}; // end namespace swift_snails
