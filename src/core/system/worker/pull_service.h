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
        RAW_LOG(WARNING, ".. start pull deamon service");
        RAW_LOG(INFO, ">  pull service period:\t%d", _period);

        auto service_with_wait = [this] {
            //std::unique_lock<std::mutex> lk(param_cache.iter_mutex());
            std::unique_lock<std::mutex> lk(mut);
            while(! param_cache.terminate_flag()) {
                //DLOG(INFO) << ">  pull service deamon waiting";
                param_cache.iter_pull_cond().wait(
                    lk, 
                    [this] {
                        int num_iters = param_cache.num_iters();
                        return param_cache.terminate_flag() || 
                            (num_iters > 0 && last_pulled_iter != num_iters && num_iters % _period == 0);
                    });
                if(param_cache.terminate_flag()) return;
                last_pulled_iter = param_cache.num_iters();
                RAW_DLOG(INFO, ">  %d iter pull-service deamon to pull ...", last_pulled_iter);
                pull_access.pull();
            }
        };
        std::thread t(std::move(service_with_wait));
        t.detach();
        //start(param_cache.terminate_flag(), service_with_wait);
    }

private:

    typedef GlobalParamCache<key_t, val_t, grad_t> param_cache_t;
    typedef GlobalPullAccess<key_t, val_t, grad_t> pull_access_t;
    param_cache_t& param_cache;
    pull_access_t& pull_access; 

    std::mutex mut;

    int _period = 0;
    // record last iter to avoid pulling continuously
    int last_pulled_iter{0};

};  // class PushService



};  // end namespace swift_snails
