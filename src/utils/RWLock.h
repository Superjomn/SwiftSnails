//
//  RWLock.h
//  SwiftSnails
//
//  Created by Chunwei on 3/10/15.
//  Copyright (c) 2015 Chunwei. All rights reserved.
//
#ifndef SwiftSnails_utils_RWLock_h_
#define SwiftSnails_utils_RWLock_h_
#include "common.h"
#include "pthread.h"
namespace swift_snails {

class RWLock {
public:
    RWLock() {
        PCHECK((pthread_rwlock_init(&_lock, NULL) == 0));
    }
    ~RWLock() {
        PCHECK((pthread_rwlock_destroy(&_lock) == 0));
    }

    void rdlock() {
        PCHECK((pthread_rwlock_rdlock(&_lock) == 0));
    }

    void wrlock() {
        PCHECK((pthread_rwlock_wrlock(&_lock) == 0));
    }

    void unlock() {
        PCHECK((pthread_rwlock_unlock(&_lock) == 0));
    }

private:

    pthread_rwlock_t _lock;
    
};



};  // end namespace swift_snails
#endif

