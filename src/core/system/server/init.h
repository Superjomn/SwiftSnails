//
//  server/init.h
//  SwiftSnails
//
//  Created by Chunwei on 3/27/15.
//  Copyright (c) 2015 Chunwei. All rights reserved.
//
#pragma once
#include "../../../utils/all.h"
#include "../../transfer/transfer.h"
#include "../../parameter/hashfrag.h"
#include "../../parameter/sparsetable.h"
#include "../message_classes.h"
#include "../ServerWorkerRoute.h"

namespace swift_snails {

/*
 * register message_class: Pull
 *
 * parameters:  key-value
 * gradient:    key-grad
 *
 * types of key, value and grad should be consistent
 * with workers 
 */
template<typename Key, typename Val, typename Grad, typename AccessMethod>
class ServerInitPullMethod {
public:
    typedef Key key_t;
    typedef Val val_t;
    typedef Grad grad_t;
    typedef SparseTable<key_t, val_t> table_t;
    typedef std::pair<key_t, val_t> pull_val_t;
	typedef Transfer<ServerWorkerRoute> transfer_t;

    explicit ServerInitPullMethod() : \
        gtransfer(global_transfer<ServerWorkerRoute>()) ,
        sparse_table(global_sparse_table<key_t, val_t>()),
        pull_access(make_pull_access<table_t, AccessMethod>(sparse_table))
    { }

    void operator() () {
        LOG(WARNING) << "register pull message class";
        register_pull_message_class();
    }

protected:
    void register_pull_message_class() {
        transfer_t::msgcls_handler_t handler = pull_handler;

        gtransfer.message_class().add(
            WORKER_PULL_REQUEST, 
            std::move(handler)
        );
    }

private:
    transfer_t::msgcls_handler_t pull_handler = \
        [this] (std::shared_ptr<Request> req,  std::shared_ptr<Request>& rsp)
        {
            // read request
            std::vector<pull_val_t> req_items;
            while(! req->cont.read_finished()) {
                key_t key;
                val_t val;
                req->cont >> key;
                req->cont >> val;
                req_items.emplace_back(std::move(key), std::move(val));
            }
            // query parameters
            for( auto& item : req_items) {
                key_t& key = item.first;
                val_t& val = item.second;
                pull_access->get_pull_value(key, val);
                // put response
                rsp->cont << key;
                rsp->cont << val;
            }
        };


private:
    Transfer<ServerWorkerRoute>& gtransfer;

    SparseTable<key_t, val_t>& sparse_table; 

    std::unique_ptr<PullAccessAgent<table_t, AccessMethod> >& pull_access; 

};  // class ServerInitPullMethod

/*
 * register message_class: Push
 *
 * parameters: key-value
 * gradient: key-grad
 */
template<typename Key, typename Val, typename Grad, typename AccessMethod>
class ServerInitPushMethod {
public:
    typedef Key key_t;
    typedef Val val_t;
    typedef Grad grad_t;
    typedef SparseTable<key_t, val_t> table_t;
    typedef std::pair<key_t, grad_t> push_val_t;
	typedef Transfer<ServerWorkerRoute> transfer_t;

    explicit ServerInitPushMethod() : \
        gtransfer(global_transfer<ServerWorkerRoute>()),
        sparse_table(global_sparse_table<key_t, val_t>()),
        push_access(make_push_access<table_t, AccessMethod>(sparse_table))
    { }

    void operator() () {
        LOG(WARNING) << "register push message class";
        register_push_message_class();
    }

protected:
    void register_push_message_class() {
        transfer_t::msgcls_handler_t handler = push_handler;

        gtransfer.message_class().add(
            WORKER_PUSH_REQUEST, 
            std::move(handler)
        );
    }
   
private:

    transfer_t::msgcls_handler_t push_handler = \
        [this] (std::shared_ptr<Request> req,  std::shared_ptr<Request>& rsp)
        {
            //std::vector<push_val_t> req_items;
            while(! req->cont.read_finished()) {
                key_t key;
                grad_t grad;
                req->cont >> key;
                req->cont >> grad;
                //req_items.emplace_back(std::move(key), std::move(grad));
                push_access->apply_pull_value(key, grad);
            }
        };
    
private:

    Transfer<ServerWorkerRoute>& gtransfer; 

    SparseTable<key_t, val_t>& sparse_table;

    std::unique_ptr<PushAccessAgent<table_t, AccessMethod> >& push_access;


};  // class ServerInitPushMethod




};  // end namespace swift_snails
