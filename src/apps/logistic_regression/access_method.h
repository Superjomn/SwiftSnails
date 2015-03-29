#pragma once
#include "../../utils/all.h"
#include "../../core/parameter/sparsetable.h"
#include "../../core/parameter/sparse_access_method.h"

namespace logistic_regression {

using namespace swift_snails;

typedef index_t key_t;
typedef float value_t;
typedef float grad_t;

class LogisticPullMethod : public PullAccessMethod< SparseTable<key_t, value_t>, value_t, value_t> 
{
public:
    virtual void init_param(const key_t& key, pull_param_t &param) {

        std::mt19937 gen(rd());
        std::normal_distribution<> d(0,1);
        param = d(rd);
    }

    virtual void get_pull_value(const key_t &key, const pull_param_t &param, pull_val_t &val) {
        val = param;
    }

private:
    std::random_device rd;

};  // class LogisticPullMethod




class LogisticPushMethod : public PushAccessMethod< SparseTable<key_t, value_t>, value_t, value_t> 
{
public:

    virtual void merge_push_value(const key_t& key, push_val_t &push_val, const push_val_t &other_push_val) 
    {
        push_val += other_push_val;
    }

    virtual void apply_pull_value(const key_t& key, push_param_t &push_param, const push_val_t &push_val)
    {
        push_param += push_val;
    }

};  // end class LogisticPushMethod








};  // end namespace logistic_regression

