#pragma once
//
//  SparseAccessMethod.h
//
//  Created by Chunwei on 3/21/15.
//  Copyright (c) 2015 Chunwei. All rights reserved.
//
namespace swift_snails {

template <typename Table, typename PullVal, typename PullParam>
class PullAccessMethod {
public:
  typedef Table table_t;
  typedef typename Table::key_t key_t;
  typedef typename Table::value_t value_t;
  // client side
  typedef PullVal pull_val_t;
  // server side
  typedef PullParam pull_param_t;

  virtual void init_param(const key_t &key, pull_param_t &param) = 0;
  virtual void get_pull_value(const key_t &key, const pull_param_t &param,
                              pull_val_t &val) = 0;
  // virtual void apply_pull_value(const key_t &key, pull_param_t &param, const
  // pull_val_t& val) = 0;
  // virtual void get_default_param(const key_t &key, pull_param_t &val) = 0;

}; // class PullAccessMethod

template <typename Table, typename PushVal, typename PushParam>
class PushAccessMethod {
public:
  typedef Table table_t;
  typedef typename Table::key_t key_t;
  typedef typename Table::value_t value_t;
  typedef PushVal push_val_t;
  typedef PushParam push_param_t;

  virtual void merge_push_value(const key_t &key, push_val_t &push_val,
                                const push_val_t &other_push_val) = 0;
  /*
   * @push_param : ref to param value of sparsetable
   * @push_val:    the grad pushed by worker
   */
  virtual void apply_push_value(const key_t &key, push_param_t &push_param,
                                const push_val_t &push_val) = 0;

}; // class PushAccessMethod

}; // end namespace swift_snails
