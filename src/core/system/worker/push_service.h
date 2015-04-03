#pragma once
#include "../../../utils/all.h"
#include "../../parameter/hashfrag.h"
#include "../../parameter/global_push_access.h"

namespace swift_snails {

/*
 * Periodically upload local grads to remote parameter servers
 */

template<typename Key, typename Val, typename Grad>
class PushService : public DaemonThread {
public:
    typedef Key key_t;
    typedef Val val_t;
    typedef Grad grad_t;

    explicit PushService() : \
        param_cache(global_param_cache<key_t, val_t, grad_t>()),
        push_access( global_push_access<key_t, val_t, grad_t>())
    {
        _period = global_config().get_config("push_period").to_int32();
        _num_iters = global_config().get_config("num_iters").to_int32();
        CHECK(_period > 0);
    }

    void start_service() {
        RAW_LOG(WARNING, ".. start push deamon service");
        RAW_LOG(INFO, ">  push service period:\t%d",  _period);

        auto service_with_wait = [this] {
            std::unique_lock<std::mutex> lk(mut);
            while(! param_cache.terminate_flag()) {
                param_cache.iter_push_cond().wait(
                    lk,
                    [this] {
                        int num_iters = param_cache.num_iters();
                        return param_cache.terminate_flag() || 
                               num_iters == _num_iters ||
                            (num_iters > 0 && last_push_iter != num_iters && num_iters % _period == 0);
                    });

                if(param_cache.terminate_flag() || _num_iters == param_cache.num_iters() ) {
                    RAW_LOG(WARNING, ".. push deamon thread terminate!");
                    return;
                }
                last_push_iter = param_cache.num_iters();
                RAW_LOG_INFO(">  %d iter push-service deamon to push ...", last_push_iter);
                push_access.push();
            }
        };

        std::thread t(std::move(service_with_wait));
        t.detach();

        //start(param_cache.terminate_flag(), service_with_wait);
    }


private:

    typedef GlobalParamCache<key_t, val_t, grad_t> param_cache_t;
    typedef GlobalPushAccess<key_t, val_t, grad_t> push_access_t;

    std::mutex mut;

    param_cache_t& param_cache; 
    push_access_t& push_access;

    int _period = 0;
    int _num_iters = 0;
    int last_push_iter = 0;

};  // class PushService



};  // end namespace swift_snails
