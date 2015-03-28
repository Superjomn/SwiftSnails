#pragma once
#include "../../utils/all.h"
#include "../system/message_classes.h"
#include "hashfrag.h"
#include "global_param_cache.h"
namespace swift_snails {
/*
 *  key-value
 *  key-grad
 */
template<class Key, class Val, class Grad>
class GlobalPullAccess : public VirtualObject {

public:
    typedef Key key_t;
    typedef Val val_t;
    typedef Grad grad_t;
    typedef std::pair<key_t, val_t> pull_val_t;

    void pull() {
        // node_id : vals
        std::map<int, std::vector<pull_val_t> > node_reqs;
        arrange_local_vals(node_reqs);
        // send message to each nodes
        send(node_reqs);
    }

protected:
    void arrange_local_vals(std::map<int, std::vector<pull_val_t> > &node_reqs) {
        auto &vals = param_cache.params();
        for( auto& item : vals) {
            auto& key = item.first;
            auto& val = item.second;

            int node_id = global_hashfrag().to_node_id(item.key);
            if(node_reqs.count(node_id) == 0) {
                node_reqs[node_id] = std::move(std::vector<pull_val_t>());
            }
            node_reqs[node_id].push_back(item);
        }
    }

    void send(
        //std::shared_ptr<RecvParcel> &recv_parcel, 
        std::map<int, std::vector<pull_val_t> &items
        ) 
    {
        for( auto& item : items) {
            int &node_id = item.first;
            auto &values = item.second;

            Request req;
            req.message_class = MSG_CLS.WORKER_PULL_REQUEST;
            for(auto& value : values) {
                req.cont << value.first;
                req.cont << value.second;
            }
            // get remote parameters
            // rewrite to local cache
            req.call_back_handler = [](std::shard_ptr<Request> rsp) {
                key_t key;
                val_t val;
                // write local cache 
                auto& params = global_param_cache().params();
                // TODO put rwlock inside? 
                rwlock_write_guard lk (global_param_cache.rwlock());
                while(! rsp.cont.read_finished()) {
                    rsp.cont >> key;
                    rsp.cont >> val;
                    params[key] = std::move(val);
                }
                //recv_parcel->try_wake_up();
            };

            global_transfer().send(node_id, std::move(req));
            //recv_parcel->send(node_id);
        }
    }

private:
    auto &param_cache = global_param_cache<key_t, val_t, grad_t>();
};  // class GlobalPullAccess


template <class Key, class Val, class Grad>
GlobalPullAccess<Key, Val, Grad>& global_pull_access() {
    static GlobalPullAccess<Key, Val, Grad> access;
    return access;
}


};  // end namespace swift_snails


