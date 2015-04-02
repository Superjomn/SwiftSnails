//
//  Barrier.h
//  SwiftSnails
//
//  Created by Chunwei on 3/17/15.
//  Copyright (c) 2015 Chunwei. All rights reserved.
//
#pragma once
#include "common.h"
// a wrapper of condition_variable to block current thread
// and wait for future flag
namespace swift_snails {

class BaseBarrier {
public:
	virtual void block() = 0;
    virtual void unblock() = 0;
    virtual ~BaseBarrier() {}
};

class Barrier : public BaseBarrier {
public:
    explicit Barrier() {
    }
    virtual ~Barrier() {
    }
    // block current thread
    void block() {
        if(!is_block()) {
            std::lock_guard<std::mutex> lk(_flag_mut);
            _is_block = true;
        } 
        std::unique_lock<std::mutex> lk(_flag_mut);
        _block_cond.wait(lk, [this]{
                return ! is_block();
            });
    }
    // tell all the waiting thead to continue running
    void unblock() {
        // change block flag
        std::lock_guard<std::mutex> lk(_flag_mut);
        _is_block = false;
        _block_cond.notify_all();
    }

    bool is_block() const {
        return _is_block;
    }

private:
    // block the current thread
    mutable std::mutex _flag_mut;
    mutable std::mutex _block_mut;
    bool _is_block = false;
    std::condition_variable _block_cond;
};  // end class Barrier


// a more flexible Barrier
class CompBarrier : public BaseBarrier {

    typedef std::function<void()> void_lamb;

public:
    void block(void_lamb &set_flag, std::function<bool()> &cond_func) {
        set_flag();
        std::unique_lock<std::mutex> lk(_flag_mut);
        _block_cond.wait(lk, cond_func);
    }

    void unblock(void_lamb &func) {
        func(); // reset flag
        _block_cond.notify_all();
    }

    virtual void block() {
        throw std::bad_function_call();
    }
    virtual void unblock() {
        throw std::bad_function_call();
    }

    virtual ~CompBarrier() {
    }

private:
    mutable std::mutex _flag_mut;
    std::condition_variable _block_cond;
};  // end class FinishInitBarrier


class StateBarrier {
public:
    explicit StateBarrier() 
    { }

    void block() {
        std::unique_lock<std::mutex> lk(mut);
        cond.wait(lk, [this]{ return unblock_state == true; });
    }
    /*
     * when time out , this handler will be called;
     */
    void time_limit(int milliseconds, std::function<void()> &&out_time_handler)
    {
        std::function<void()> handler = [this, milliseconds, out_time_handler]{
            std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
            if(unblock_state) {
                try_unblock();
            } else {
                out_time_handler();
            }
        };
        std::thread t(std::move(handler));
        t.detach();
    }

    void try_unblock() {
        cond.notify_one();
    }

    void set_state_valid() {
        unblock_state = true;
    }

    bool valid_state() {
        return unblock_state;
    }

private:
    std::atomic<bool> unblock_state{false};
    std::condition_variable cond;
    std::mutex mut;
};

/*
 * the count should be the main thread + child threads
 */
class CounterBarrier
{
private:
    std::mutex _mutex;
    std::condition_variable _cv;
    std::size_t _count;
public:
    explicit CounterBarrier(std::size_t count) : _count{count} { }

    void wait()
    {
        std::unique_lock<std::mutex> lock{_mutex};
        if (--_count == 0) {
            _cv.notify_all();
        } else {
            _cv.wait(lock, [this] { return _count == 0; });
        }
    }
};



};  // end namespace swift_snails
