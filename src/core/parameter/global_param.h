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
template<typename GradMethod>
class GlobalParamCache {
public:
    typedef GradMethod              grad_method_t;
    typedef typename grad_method_t::grad_t grad_t;
    typedef typename grad_method_t::key_t  key_t;
    typedef typename grad_method_t::val_t  val_t;

    explicit GlobalParamCache() {
    }
    // thread-safe
    // should run first
    void init_keys(std::set<key_t> &keys) {
        rwlock_write_guard lk(_rwlock);

        for(auto& key : keys) {
            _data[key] = val_t();
        }
    }

    size_t size() const {
        return _data.size();
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

private:
    std::map<key_t, val_t> _params;
    std::map<key_t, grad_t> _grads;
    // number of iterations
    std::atomic<int> num_iters;
    RWLock _rwlock;
};


template<GradMethod> 
GlobalParamCache<GradMethod>& global_param_cache() {
    static GlobalParamCache<GradMethod> cache;
    return cache;
}


};
