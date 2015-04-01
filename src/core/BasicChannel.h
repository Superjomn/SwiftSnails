//
//  BasicChannel.h
//  core
//
//  Created by Chunwei on 12/8/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//
#ifndef SwiftSnails_core_BasicChannel_h_
#define SwiftSnails_core_BasicChannel_h_
#include "../utils/common.h"
#include "../utils/queue.h"

namespace swift_snails {

/*
 * Channel for thread pool
 *
 * a channel acts like a task queue
 * with full control of multi-thread 
 * when call `close()`, the channel will tell all the
 * threads(waiting or working) to exit
 * the threads can pop the data and check the return flag 
 * to tell whether stop work
 *
 * Example: 
 *  
 *  BasicChannel<int> channel;
 *
 *  std::function<void()> handle = [&channel] {
 *      int task;
 *      bool toexit = channel.pop(task);
 *      if(toexit) return;
 *      // deal with the task ...
 *      // ...
 *  };
 */
template<typename T>
class BasicChannel : public threadsafe_queue<T> {
public:
    explicit BasicChannel() {}
    /* 
     * return false if channel is closed
     * the pop thread can use this flag to tell whether exit
    */
    bool pop(T& value) {
        //LOG(INFO) << std::this_thread::get_id() << " pop job " << _closed;
        if(_closed) return false;
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(
                lk, [this] { return !data_queue.empty() || _closed; });
        if(_closed) return false;
        value = std::move(*data_queue.front());
        data_queue.pop();
        return true;
    }
    /*
     * return null if the channel is closed
     * the waiting threads can use this flag to tell whether exit
     */
    std::shared_ptr<T> pop() {
        //LOG(INFO) << std::this_thread::get_id() << " pop job";
        if(_closed) return std::shared_ptr<T>();
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(
                lk, [this] { return !data_queue.empty() || _closed; });
        if(_closed) return std::shared_ptr<T>();
        std::shared_ptr<T> res = data_queue.front();
        data_queue.pop();
        return res;
    }
    void push(T new_value) {
        std::shared_ptr<T> data (
            std::make_shared<T>(std::move(new_value)));
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(data);
        data_cond.notify_one();
    }
    /*
     * close the channel and tell all the 
     * waiting threads to exit
     */
    void close() {
        LOG(INFO) << "!!channel is closed";
        std::lock_guard<std::mutex> lk(mut);
        _closed = true;
        data_cond.notify_all(); // notify all waiting threads to exit
        LOG(INFO) << "notify all threads to exit";
    }

    bool closed() const {
        return _closed;
    }

    void size() const {
        return data_queue.size();
    }

    bool empty() const {
        return data_queue.empty();
    }
private:
    bool _closed = false;
    mutable std::mutex mut;
    std::queue<std::shared_ptr<T>> data_queue;
    std::condition_variable data_cond;
};

};  // end namespace swift_snails
#endif
