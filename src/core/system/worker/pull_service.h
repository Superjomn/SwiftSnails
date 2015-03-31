#pragma once
#include "../../../utils/all.h"
#include "../../parameter/hashfrag.h"
#include "../../parameter/global_pull_access.h"

namespace swift_snails {

/*
 * Periodically pull remote parameters to local cache
 */
template<typename Key, typename Val, typename Grad>
class PullService : public DaemonThread {
public:
    typedef Key key_t;
    typedef Val val_t;
    typedef Grad grad_t;

    explicit PullService() : \
        param_cache(global_param_cache<key_t, val_t, grad_t>()),
        pull_access(global_pull_access<key_t, val_t, grad_t>())
    {
        _period = global_config().get_config("pull_period").to_int32();
        CHECK(_period > 0);
    }

    void start_service() {
        start(param_cache.terminate_flag());
    }

protected:
    void service_with_wait() {
        param_cache.iter_cond().wait(
            param_cache.iter_mutex(),
            [this] {
                int num_iters = param_cache.num_iters();
                return num_iters > 0 && num_iters % _period == 0;
            });

        pull_access.pull();
    }

private:

    typedef GlobalParamCache<key_t, val_t, grad_t> param_cache_t;
    typedef GlobalPullAccess<key_t, val_t, grad_t> pull_access_t;
    param_cache_t& param_cache;
    pull_access_t& pull_access; 

    int _period = 0;

};  // class PushService



};  // end namespace swift_snails
