//
//  AsynExec.h
//  SwiftSnails
//
//  Created by Chunwei on 12/24/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//
#ifndef SwiftSnails_SwiftSnails_AsynExec_h_
#define SwiftSnails_SwiftSnails_AsynExec_h_
#include "../utils/common.h"
#include "BasicChannel.h"
#include "common.h"

namespace swift_snails {

class AsynExec : public VirtualObject {
public:
    typedef std::function<void()> task_t;
    typedef BasicChannel<task_t> channel_t;

    explicit AsynExec() {}

    shared_ptr<channel_t> open() {
        for(int i = 0; i < thread_num(); i++) {
            shared_ptr<MultiWorker> workder = make_shared<MultiWorker>();
            for(int i = 0; i < thread_num(); i++) {
                workder->threads.emplace_back(
                    std::thread([worker]() {
                        worker->run();
                    }))
            } // for
        }
        return {&worker->channel, [worker](channel_t*) {
            worker->channel.close();
            for (auto& t : worker->threads) {
                t.join();
            }
        }};
    }

    void set_thread_num(int x) {
        CHECK(x > 0);
        _thread_num = x;
    }
    int thread_num() const {
        return _thread_num;
    }
private:
    int _thread_num = 0;
    std::vector<thread_guard> _threads;

    struct MultiWorker {
        std::vector<thread_guard> threads;
        channel_t channel;
        void start() {
            task_t func;
            while (channel.pop(func)) {
                func();
            }
        }
    }; // end struct MultiWorker
}; // class AsynExec

}; // end namespace swift_snails

#endif
