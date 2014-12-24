//
//  ThreadPool.h
//  SwiftSnails
//
//  Created by Chunwei on 12/24/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//
#ifndef SwiftSnails_SwiftSnails_ThreadPool_h_
#define SwiftSnails_SwiftSnails_ThreadPool_h_
#include "../utils/queue.h"

namespace swift_snails {

// ensure thread to exit nomally
class thread_guard {
    std::thread& _t;
public:
    explicit thread_guard(std::thread& t) :
        _t(t)
    { }
    thread_guard(thread_guard const&) = delete;
    thread_guard& operator=(thread_guard const&) = delete;
    ~thread_guard() {
        if(_t.joinable()) _t.join();
    }
};

template<typename Func_t>
class ThreadPool {
public:
    /*
     * @nthreads: number of threads
     * @queue:    threadsafe task queue 
     */
    explicit ThreadPool(int nthreads, thread_guard<Func_t>& queue) :
        _nthreads(nthreads), 
        _queue(queue)
    { }

    void start() {  // TODO queue to exit
        CHECK(nthreads > 0);
        for( int i = 0 ; i < nthreads; i++) {
            std::thread t([this]() {
                while(true) {
                    if(_closed) break;
                    Func_t func;   
                    _queue.wait_and_pop(func);
                    func();
                }
            });
            _threads.push_back(thread_guard(t));
        }
    }

    void close() {
        _close = true;
    } 

private:
    int _nthreads;
    std::vector<thread_guard> _threads;
    threadsafe_queue<Func_t> &_queue;
    bool _closed = false;
};


};  // end namespace swift_snails


#endif
