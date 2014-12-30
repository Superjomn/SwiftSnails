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

template<typename T, typename Func>
class ThreadPool : public VirtualObject {
public:
    typedef BasicChannel<T> channel_t;

    explicit ThreadPool() { }
    explicit ThreadPool(int num_thread) :
        _thread_num(num_thread)
    { }
    ~ThreadPool() {
        for (auto &t : _threads) {
            if(t.joinable()) t.join();
        }
    }
    /*
     * @func: threads belong to the threadpool will run the same func
     * @return: pointer to the channel
     *          user can modify the channel by pop()/push() or close()
     * 
     * pop data from channel, if threadpool.close() or channel.close() 
     * then, threads in the threadpool will exit
     */
    std::shared_ptr<channel_t> start(Func &&func) {  // TODO queue to exit
        CHECK(thread_num() > 0);
        for(int i = 0 ; i < thread_num(); i++) {
            _threads.emplace_back([this, func]() {
                bool valid;
                T data;
                LOG(INFO) << "thread " << std::this_thread::get_id() << " started";
                while((valid = _channel.pop(data))) {
                    // close threadpool?
                    if(_closed) break; 
                    func(data);
                }
                LOG(INFO) << "thread " << std::this_thread::get_id() << " exit";
            });
        }

        return { &_channel,
            [this](channel_t*) {
                LOG(INFO) << "ThreadPool channel close, threads to exit";
                for (auto& t : _threads) {
                    t.join();
                } 
            }};
    }

    void close() {
        _closed = true;
        _channel.close();
    }
    int thread_num() const {
        return _thread_num;
    }
    bool closed() {
        return _closed;
    } 

private:
    int _thread_num = 0;
    std::vector<std::thread> _threads;
    BasicChannel<T> _channel;
    bool _closed = false;
};


};  // end namespace swift_snails


#endif
