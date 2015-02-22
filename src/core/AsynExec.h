//
//  AsynExec.h
//  core
//
//  Created by Chunwei on 12/24/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//
#ifndef SwiftSnails_core_AsynExec_h_
#define SwiftSnails_core_AsynExec_h_
#include "../utils/common.h"
#include "BasicChannel.h"
#include "common.h"

namespace swift_snails {

class AsynExec : public VirtualObject {
public:
    typedef std::function<void()> task_t;
    typedef BasicChannel<task_t> channel_t;

    explicit AsynExec() {}

    explicit AsynExec(int thread_num) : 
        _thread_num(thread_num)
    { 
        _channel = open();
    }

    std::shared_ptr<channel_t> open() {

        std::shared_ptr<MultiWorker> worker = std::make_shared<MultiWorker>();

        for(int i = 0; i < thread_num(); i++) {

            worker->threads.emplace_back(
                [worker]() {
                    worker->start();
                }
            );
        } // for

        return { &worker->channel, 
            [worker](channel_t*) {
                worker->channel.close();
                LOG(INFO) << "channel to destroy, worker threads join";
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
    std::shared_ptr<channel_t> channel() {
        return _channel;
    }
private:
    int _thread_num = 0;
    std::vector<std::thread> _threads;
    std::shared_ptr<channel_t> _channel;

    struct MultiWorker {
        std::vector<std::thread> threads;
        channel_t channel;

        void start() {
            task_t func;
            bool valid;

            LOG(INFO) << "thread " << std::this_thread::get_id() << " started";
            while ((valid = channel.pop(func))) {
                //LOG(INFO) << std::this_thread::get_id() << " job's valid: " << valid;
                func();
            }
            LOG(INFO) << "thread " << std::this_thread::get_id() << " exit";
        }
        ~MultiWorker() {
            for( auto &t : threads) {
                if(t.joinable()) t.join();
            }
        }
    }; // end struct MultiWorker
}; // class AsynExec


/*
 * a global async channel with fixed number of threads
 */
// TODO should be changed in future
const int num_async_threads = 4;

std::shared_ptr<AsynExec::channel_t> global_async_channel() {
    static AsynExec global_async(num_async_threads);
    return global_async.channel();
}

}; // end namespace swift_snails

#endif
