#pragma once
#include "../../utils/all.h"
#include "../system/message_classes.h"
#include "hashfrag.h"
#include "global_param_cache.h"
namespace swift_snails {

/*
 * push local grads to remote parameter servers
 */
template<typename Key, typename Val, typename Grad>
class GlobalPushAccess : public VirtualObject {
public:
    typedef Key key_t;
    typedef Val val_t;
    typedef Grad grad_t;
    typedef std::pair<key_t, grad_t> push_val_t;

    GlobalPushAccess() : \
        param_cache(global_param_cache<key_t, val_t, grad_t>()),
        gtransfer(global_transfer<ServerWorkerRoute>())
    { }

    void push(voidf_t extra_rsp_callback = voidf_t()) {
        // nodeid to reqs
        std::map<int, std::vector<push_val_t> > node_reqs;
        arrange_local_grads(node_reqs);

        send(node_reqs, extra_rsp_callback);

        reset_local_grads();
    }

    size_t push_without_reset(voidf_t extra_rsp_callback = voidf_t()) {
        // nodeid to reqs
        std::map<int, std::vector<push_val_t> > node_reqs;
        arrange_local_grads(node_reqs);

        return send(node_reqs, extra_rsp_callback);

        //reset_local_grads();
    }
    /*
     * set local grads to zero(initial status)
     */
    void reset_local_grads() {
        rwlock_write_guard(param_cache.rwlock());
        for(auto& item : param_cache.params()) {
            //item.second.reset();    // set to 0
            item.second = std::move(val_t());
        }
    }

protected:

    void arrange_local_grads(std::map<int, std::vector<push_val_t>>& node_reqs) {
        auto &grads = param_cache.grads();
        // split grads to different nodes
        for(auto& item : grads) {
            auto& key = item.first;
            //auto& grad = item.second;

            int node_id = global_hashfrag<key_t>().to_node_id(key);
            if(node_reqs.count(node_id) == 0) {
                node_reqs[node_id] = std::move(std::vector<push_val_t>());
            }
            node_reqs[node_id].push_back(item);
        }
    }

    size_t send(
            std::map<int, std::vector<push_val_t>>& items,
            voidf_t extra_rsp_callback
    ) {
        for (auto& item : items) {
            int node_id = item.first;
            auto &grads = item.second;

            Request req;
            req.meta.message_class = WORKER_PUSH_REQUEST;
            for(auto& grad : grads) {
                req.cont << grad.first; // key
                req.cont << grad.second;// grad value
            }
            // nothing to do after grads are pushed
            req.call_back_handler = [extra_rsp_callback](std::shared_ptr<Request> rsp) {
                RAW_DLOG(INFO, "Grads are pushed");
                if(extra_rsp_callback) extra_rsp_callback();
            };
            gtransfer.send(std::move(req), node_id);
        }
        return items.size();
    }

private:
    typedef GlobalParamCache<key_t, val_t, grad_t> param_cache_t;
    param_cache_t &param_cache; 
    Transfer<ServerWorkerRoute>& gtransfer; 

};  // end class GlobalPushAccess


template <class Key, class Val, class Grad>
GlobalPushAccess<Key, Val, Grad>& global_push_access() {
    static GlobalPushAccess<Key, Val, Grad> access;
    return access;
}


};  // end namespace swift_snails
