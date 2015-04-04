#pragma once
#include "../../utils/all.h"
#include "../../core/parameter/sparsetable.h"
#include "../../core/parameter/sparse_access_method.h"
#include "common.h"
#include "param.h"

using namespace std;
using namespace swift_snails;

typedef size_t          key_t;
typedef Word2VecParam   val_t;
typedef Word2VecGrad    grad_t;

class Word2VecPullMethod : public PullAccessMethod<SparseTable<key_t, val_t>, val_t, val_t>
{
public:
    explicit Word2VecPullMethod(int len_vec) : \
        len_vec(len_vec)
    { }

    explicit Word2VecPullMethod() {
    }
    void init(int len_vec) {
        this->len_vec = len_vec;
    }

    virtual void init_param(const key_t& key, pull_param_t &param) {
        pull_param_t val;
        val.init(len_vec, true);
        param = std::move(val);
    }

    virtual void get_pull_value(const key_t &key, const pull_param_t &param, pull_val_t &val) {
        val.copy_value(param);
    }

private:
    int len_vec{0};
    std::random_device rd;

};  // class Word2VecPullMethod


class Word2VecPullMethod : public PushAccessMethod<SparseTable<key_t, val_t>, grad_t, val_t> {
    virtual void merge_push_value(const key_t& key, push_val_t &push_val, const push_val_t &other_push_val) 
    {
        push_val.merge_with(other_push_val);
    }

    virtual void apply_push_value(const key_t& key, push_param_t &param, const push_val_t &grad)  
    {
        grad.norm();
        param.h2sum += (grad.h_grad() * grad.h_grad());
        param.v2sum += (grad.v_grad() * grad.v_grad());
        param.h += learning_rate * grad.h_grad / sqrt(param.h2sum);
        param.v += learning_rate * grad.v_grad / sqrt(param.v2sum);
    }
};  // class Word2VecPullMethod


BinaryBuffer& operator<< (BinaryBuffer &bb, const Word2VecGrad& grad) {
    bb << grad.len_vec;
    // input h v
    for(int i = 0; i < grad.len_vec(); i++) {
        bb << grad.h()[i];
        bb << grad.v()[i];
    }
    return bb;
}


BinaryBuffer& operator>> (BinaryBuffer &bb, Word2VecGrad& grad) {
    int len_vec;
    bb >> len_vec;
    CHECK(grad.len_vec() == 0) << "grad should be empty to write";
    grad.init(len_vec);
    for(int i = 0; i < len_vec; i++) {
        bb >> grad.h()[i];
        bb >> grad.v()[i];
    }
    return bb;
}

BinaryBuffer&& operator>> (BinaryBuffer &bb, const Word2VecParam &param) {
    bb << param.len_vec();
    for(int i = 0; i < param.len_vec(); i ++) {
        bb << param.h()[i];
        bb << param.v()[i];
    }
}

BinaryBuffer&& operator>> (BinaryBuffer &&bb, Word2VecParam & param) {
    CHECK(param.len_vec() == 0) << "param should be empty to write";
    int len_vec{0};
    bb >> len_vec;
    param.init(len_vec, false);

    for(int i = 0; i < len_vec; i++) {
        bb >> param.h()[i];
        bb >> param.v()[i];
    }
}
