//
//  ThreadPool.h
//  SwiftSnails
//
//  Created by Chunwei on 12/24/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//
#ifndef SwiftSnails_SwiftSnails_ThreadPool_h_
#define SwiftSnails_SwiftSnails_ThreadPool_h_
#include "BasicChannel.h"

namespace swift_snails {

template<typename T>
class ThreadGroup : public VirtualObject {
public:
    typedef BasicChannel<T> channel_t;

    explicit ThreadPool() { }
    explicit ThreadPool(int num_thread) :
        _thread_num(num_thread)
    { }
    /*
     * @func: threads belong to the threadpool will run the same func
     * @return: pointer to the channel
     *          user can modify the channel by pop()/push() or close()
     * 
     * pop data from channel, if threadpool.close() or channel.close() 
     * then, threads in the threadpool will exit
     */
    shared_ptr<channel_t> start(Func_t &&func) {  // TODO queue to exit
        CHECK(thread_num() > 0);
        for(int i = 0 ; i < thread_num(); i++) {
            std::thread t([this, func]() {
                while(true) {
                    // close threadpool?
                    if(_closed) return; 
                    T data;
                    bool valid = _queue.pop(data);
                    if (!valid) {
                        LOG(INFO) << "channel is closed, one ThreadPool thread exit!";
                        break;
                    }  
                    func(data);
                }
            });
            _threads.emplace_back(std::move(t));
        }

        return { &channel,
            [this](channel_t*) {
                channel.close();
                for (auto& t : threads) {
                    t.join();
                } 
            }};
    }

    void close() {
        _closed = true;
        channel.close();
    }
    int thread_num() const {
        return _thread_num;
    }
    void closed() {
        return _closed;
    } 

private:
    int _thread_num = 0;
    std::vector<thread_guard> _threads;
    BasicChannel<T> channel;
    bool _closed = false;
};


};  // end namespace swift_snails


#endif
