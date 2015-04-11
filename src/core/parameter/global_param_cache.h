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
    virtual void update_param(const key_t& key, val_t& val, const grad_t& grad) = 0;
    virtual void merge_grad(const key_t& key, grad_t& grad, const grad_t& other) = 0;
    // after PULL , rewrite local parameter cache
    virtual void rewrite_param(const key_t& key, val_t &val, const val_t &remote_val) = 0;
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
        rwlock_read_guard lk(_rwlock);
        return _params.size();
    }
    // not thread-safe
    // should use rwlock first
    std::map<key_t, val_t>& params() {
        return _params;
    }

    std::map<key_t, grad_t>& grads() {
        return _grads;
    }

    RWLock& rwlock() {
        return _rwlock;
    }

    std::condition_variable& iter_pull_cond() {
        return _iter_pull_cond;
    }
    std::condition_variable& iter_push_cond() {
        return _iter_push_cond;
    }
    std::mutex& iter_mutex() {
        return _iter_mutex;
    }

    friend std::ostream& operator<< (std::ostream& os, GlobalParamCache & cache)
    {
        for(auto& item : cache._params ) {
            os << item.first << "\t";
            os << item.second << std::endl;
        }
        return os;
    }
    
    // should be called after each iteration
    // to support pull and push service
    void inc_num_iters() {
        _num_iters ++;
        _iter_pull_cond.notify_all();    // ? notify_one ? 
        _iter_push_cond.notify_all();    // ? notify_one ? 
    }
    std::atomic<int>& num_iters() {
        return _num_iters;
    }
    bool terminate_flag() {
        return _terminate_flag;
    }
    void terminate_service_deamons() {
        _terminate_flag = true;
        _iter_pull_cond.notify_all();
        _iter_push_cond.notify_all();
    }

    std::set<key_t>& local_keys() {
        return _local_keys;
    }

private:
    RWLock _rwlock;
    std::map<key_t, val_t> _params;
    std::map<key_t, grad_t> _grads;
    std::set<key_t> _local_keys;
    // number of iterations
    std::atomic<int> _num_iters{0};
    mutable std::mutex _iter_mutex;
    std::condition_variable _iter_pull_cond;
    std::condition_variable _iter_push_cond;
    // tell the push and pull service deamons to terminate
    std::atomic<bool> _terminate_flag{false};
};


template<class Key, class Val, class Grad> 
GlobalParamCache<Key, Val, Grad>& global_param_cache() {
    static GlobalParamCache<Key, Val, Grad> cache;
    return cache;
}


};
