#pragma once
#include "../../utils/all.h"
#include "hashfrag.h"
namespace swift_snails {

template<typename Key, typename Val, typename Grad>
class GradPramProcMethod {
public:
    typedef Grad grad_t;
    typedef Key  key_t;
    typedef Val  val_t;
    // update local Grad to remote
    virtual update_param(const key_t& key, val_t& val, const grad_t& grad) = 0;
    virtual merge_grad(const key_t& key, grad_t& grad, const grad_t& other) = 0;
    // after PULL , rewrite local parameter cache
    virtual rewrite_param(const key_t& key, val_t &val, const val_t &remote_val) = 0;
};
/*
 * Key: key of parameter
 * Val: value of parameter
 * Grad: type of Grad 
 *
 * run init_keys first
 * write single record will not use lock(+, -)
 */
template<typename Key, typename Val, typename Grad>
class GlobalParamCache {
public:
    typedef Key key_t;
    typedef Val val_t;
    typedef Grad grad_t;

    explicit GlobalParamCache() {
    }
    // thread-safe
    // should run first
    // init paramters and grads
    void init_keys(std::set<key_t> &keys) {
        rwlock_write_guard lk(_rwlock);

        for(auto& key : keys) {
            _params[key] = val_t();
            _grads[key] = grad_t();
        }
    }

    size_t size() const {
        return _params.size();
    }
    // not thread-safe
    // should use rwlock first
    std::map<key_t, val_t>& params() {
        return _params;
    }

    std::map<key_t, val_t>& grads() {
        return _grads;
    }

    RWLock& rwlock() {
        return _rwlock;
    }
    
    std::atomic<int>& num_iters() {
        return _num_iters;
    }

private:
    std::map<key_t, val_t> _params;
    std::map<key_t, grad_t> _grads;
    // number of iterations
    std::atomic<int> _num_iters = 0;
    RWLock _rwlock;
};


template<class Key, class Val, class Grad> 
GlobalParamCache<Key, Val, Grad>& global_param_cache() {
    static GlobalParamCache<Key, Val, Grad> cache;
    return cache;
}


};
