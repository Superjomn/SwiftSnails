//
//  SpinLock.h
//  SwiftSnails
//
//  Created by Chunwei on 12/2/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//

#ifndef SwiftSnails_utils_SpinLock_h
#define SwiftSnails_utils_SpinLock_h
#include "pthread.h"

namespace swift_snails {

// 封装pthread_spinlock
class SpinLock : public VirtualObject {
public:
    SpinLock() {
        PCHECK( 0 == pthread_spin_init(&_spin, 0));    
    }
    ~SpinLock() {
        PCHECK( 0 == pthread_spin_destroy(&_spin));
    }
    lock() {
        PCHECK( 0 == pthread_spin_lock(&_spin));
    }
    unlock() {
        PCHECK( 0 == pthread_spin_unlock(&_spin));
    }

private:
    pthread_spinlock_t _spin;
};

}; // end namespace swift_snails

#endif
