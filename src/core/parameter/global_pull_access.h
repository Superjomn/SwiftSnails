#pragma once
#include "../../utils/all.h"
#include "../system/message_classes.h"
#include "hashfrag.h"
#include "global_param.h"
namespace swift_snails {

/*

// bundle of requests
template<class Key, class Val>
class RecvParcel : public VirtualObject {
public:
    typedef Key key_t;
    typedef Val val_t; 

    void sent(int node_id) {
        //std::lock_guard lock(mut);
        //_rsp_rcv_flags[id] = false;
        _num_reqs ++;
    }
    // return true if all responses have been received
    bool recv(int node_id) {
        std::lock_guard lock(mut); 
        //_rsp_rcv_flags[id] = true;
        _num_reqs --;
        return _num_reqs == 0;
    }

    std::map<key_t, val_t>& data() {
        return _data;
    }

    void try_wake_up() {
        if(_num_reqs == 0) {
            wait_cond.notify_one();
        }
    }
    // wait for all responses
    void wait_for_rsps() {
        std::unique_lock<std::mutex> lk(cond_mut);
        wait_cond.wait(lk, [this] { return _num_reqs == 0;});
    }

private:
    // node_id recevs
    //std::map<int, bool> _rsp_rcv_flags;
    //std::map<key_t, val_t> _data;
    //std::mutex mut;

    std::mutex cond_mut;
    std::condition_variable wait_cond;
    std::atomic<int> _num_reqs = 0;
};  // end struct Parcel

*/

template<class Key, class Val, class GradPramProcMethod>
class GlobalPullAccess {

GradPramProcMethod _cache_access;

public:
    typedef Key key_t;
    typedef Val val_t;
    typedef std::pair<key_t, val_t> pull_val_t;

    void pull() {
        std::map<key_t, val_t> &vals = global_param_cache().params();
        // node_id : vals
        std::map<int, std::vector<pull_val_t> > node_reqs;
        // split to diffent nodes
        std::shard_ptr<RecvParcel> recv_parcel(new RecvParcel);
        recv_parcel->data() = std::move(vals);

        for( auto& item : vals) {
            auto& key = item.first;
            auto& val = item.second;

            int node_id = global_hashfrag().to_node_id(item.key);
            node_reqs[node_id].push_back(item);
        }
        // send message to each nodes
        send(recv_parcel, node_reqs);
    }

protected:

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
    // determine whether the access is blocked by former push
    //is_blocked = true;  
};  // class GlobalPullAccess


};  // end namespace swift_snails


