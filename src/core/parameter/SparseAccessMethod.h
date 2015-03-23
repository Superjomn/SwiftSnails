//
//  SparseAccessMethod.h
//
//  Created by Chunwei on 3/21/15.
//  Copyright (c) 2015 Chunwei. All rights reserved.
//
#ifndef SwiftSnails_core_sparsetable_SparseAccessMethod_h_
#define SwiftSnails_core_sparsetable_SparseAccessMethod_h_
namespace mio_table {

template<typename Table, typename PullVal, typename PullParam>
class PullAccessMethod {
public:
    typedef typename Table::key_t   key_t;
    typedef typename Table::value_t value_t;
    typedef typename Table      table_t;
    // client side 
    typedef typename PullVal    pull_val_t;
    // server side
    typedef typename PullParam    pull_param_t;

    virtual void get_pull_value(const key_t &key, const pull_param_t &param, pull_val_t &val) = 0;
    //virtual void get_default_param(const key_t &key, pull_param_t &val) = 0;
    virtual void init_param(const key_t& key, pull_param_t &param) = 0;

};  // class PullAccessMethod


template<typename Table, typename PushVal, typename PushParam>
class PushAccessMethod {
public:
    typedef typename Table::key_t   key_t;
    typedef typename Table::value_t value_t;
    typedef typename Table      table_t;
    typedef typename PushVal    push_val_t;
    typedef typename PushParam  push_param_t;

    virtual void merge_push_value(const key_t& key, push_val_t &push_val, const push_val_t &other_push_val) = 0;

    virutal apply_push_value(const key_t& 

};  // class PushAccessMethod


};  // end namespace swift_snails
#endif
