//
//  BasicChannel.h
//  SwiftSnails
//
//  Created by Chunwei on 12/8/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//
#ifndef SwiftSnails_SwiftSnails_BasicChannel_h_
#define SwiftSnails_SwiftSnails_BasicChannel_h_
#include "../utils/common.h"

namespace swift_snails {

/*
 * Channel for thread pool
 * a channel acts like a queue and save tasks
 * if call `close()`, the channel will tell all the
 * waiting threads to exit
 */
template<typename T>
class BasicChannel : public threadsafe_queue<T> {
public:
    explicit BasicChannel() {}
    /* 
     * return true if channel is closed
     * the thread pool will exit if channel is closed
    */
    bool pop(T& value) {
        if(_closed) return false;
        std::lock_guard<std::mutex> lk(mut);
        data_cond.wait(
                lk, [this] { return !data_queue.empty() || _closed});
        if(_closed) return false;
        value = std::move(*data_queue.front());
        data_queue.pop();
        return true;
    }
    /*
     * return null if closed
     * the waiting threads will exit
     */
    std::shared_ptr<T> pop() {
        if(_closed) return std::shared_ptr<T>();
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(
                lk, [this] { return !data_queue.empty() || _closed});
        if(_closed) return std::shared_ptr<T>();
        std::shared_ptr<T> res = data_queue.front();
        data_queue.pop();
        return res;
    }
    /*
     * close the channel and tell all the 
     * waiting threads to exit
     */
    void close() {
        std::lock_guard<std::mutex> lk(mut);
        _closed = true;
        data_cond.notify_all(); // notify all waiting threads to exit
    }
private:
    bool _closed = false;
};

};  // end namespace swift_snails
#endif
