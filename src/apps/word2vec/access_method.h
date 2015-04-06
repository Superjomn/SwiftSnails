#pragma once
#include "../../utils/all.h"
#include "../../core/parameter/sparsetable.h"
#include "../../core/parameter/sparse_access_method.h"
#include "common.h"
#include "param.h"

using namespace std;
using namespace swift_snails;

typedef uint32_t          access_key_t;
typedef Word2VecParam   access_val_t;
typedef Word2VecGrad    access_grad_t;

class Word2VecPullMethod : public PullAccessMethod<SparseTable<access_key_t, access_val_t>, access_val_t, access_val_t>
{
public:
    explicit Word2VecPullMethod(int len_vec) : \
        len_vec(len_vec)
    { }

    explicit Word2VecPullMethod() {
        len_vec = global_config().get_config("len_vec").to_int32();
    }

    virtual void init_param(const key_t& key, pull_param_t &param) {
        CHECK(len_vec > 0);
        param.init(true);
    }

    virtual void get_pull_value(const key_t &key, const pull_param_t &param, pull_val_t &val) {
        val.copy_value(param);
    }

private:
    int len_vec{0};
    std::random_device rd;

};  // class Word2VecPullMethod


class Word2VecPushMethod : public PushAccessMethod<SparseTable<access_key_t, access_val_t>, access_grad_t, access_val_t> {
    float learning_rate = 0.01;
public:
    Word2VecPushMethod() {
        learning_rate = global_config().get_config("learning_rate").to_float();
    }
    virtual void merge_push_value(const key_t& key, push_val_t &push_val, const push_val_t &other_push_val) 
    {
        push_val.merge_with(other_push_val);
    }
    virtual void apply_push_value(const key_t& key, push_param_t &param, const push_val_t &grad)  
    {
        float fudge_factor = 1e-6;
        //grad.norm();
        CHECK(grad.n_h() == 0);
        CHECK(grad.n_v() == 0);
        param.h2sum() += (grad.h_grad() * grad.h_grad());
        param.v2sum() += (grad.v_grad() * grad.v_grad());
        param.h() += learning_rate * 
                grad.h_grad() / (sqrt(param.h2sum() + fudge_factor ));

        param.v() += learning_rate * 
                grad.v_grad() / (sqrt(param.v2sum() + fudge_factor ));

        /*
        RAW_LOG(INFO, "key:\t%u", key);
        RAW_LOG(INFO, "h2sum:\t%s", param.h2sum().to_str().c_str());
        RAW_LOG(INFO, "v2sum:\t%s", param.v2sum().to_str().c_str());
        RAW_LOG(INFO, "h_grad:\t%s", grad.h_grad().to_str().c_str());
        RAW_LOG(INFO, "v_grad:\t%s", grad.v_grad().to_str().c_str());
        RAW_LOG(INFO, "h:\t%s", param.h().to_str().c_str());
        RAW_LOG(INFO, "v:\t%s", param.v().to_str().c_str());
        */
    }
    /*
    virtual void apply_push_value(const key_t& key, push_param_t &param, const push_val_t &grad)  
    {
        //grad.norm();
        CHECK(grad.n_h() == 0);
        CHECK(grad.n_v() == 0);
        //param.h2sum() += (grad.h_grad() * grad.h_grad());
        //param.v2sum() += (grad.v_grad() * grad.v_grad());
        //RAW_LOG(INFO, "h2sum:\t%s", param.h2sum().to_str().c_str());
        //RAW_LOG(INFO, "v2sum:\t%s", param.v2sum().to_str().c_str());
        param.h() += learning_rate * 
                grad.h_grad() ;// / sqrt(param.h2sum());

        param.v() += learning_rate * 
                grad.v_grad() ; // / sqrt(param.v2sum());

        RAW_LOG(INFO, "h:\t%s", param.h().to_str().c_str());
        RAW_LOG(INFO, "v:\t%s", param.v().to_str().c_str());
    }
    */
};  // class Word2VecPullMethod



BinaryBuffer& operator<< (BinaryBuffer &bb, Word2VecGrad& grad) {
    grad.norm();
    bb << grad.len_vec();
    // input h v
    for(int i = 0; i < grad.len_vec(); i++) {
        bb << grad.h_grad()[i];
        bb << grad.v_grad()[i];
    }
    return bb;
}


BinaryBuffer& operator>> (BinaryBuffer &bb, Word2VecGrad& grad) {
    int len_vec;
    bb >> len_vec;
    CHECK(grad.len_vec() > 0);
    for(int i = 0; i < len_vec; i++) {
        bb >> grad.h_grad()[i];
        bb >> grad.v_grad()[i];
    }
    return bb;
}

BinaryBuffer& operator<< (BinaryBuffer &bb, const Word2VecParam &param) {
    bb << param.len_vec();
    for(int i = 0; i < param.len_vec(); i ++) {
        bb << param.h()[i];
        bb << param.v()[i];
    }
    return bb;
}

BinaryBuffer& operator>> (BinaryBuffer &bb, Word2VecParam & param) {
    int len_vec{0};
    bb >> len_vec;
    param.init(false);

    for(int i = 0; i < len_vec; i++) {
        bb >> param.h()[i];
        bb >> param.v()[i];
    }
    return bb;
}
/*
// output parameter
std::ostream& operator<< (std::ostream &os, const Word2VecParam& param) {
    const auto& v = param.v();
    for(int i = 0; i < param.len_vec(); i++) {
        os << v[i] << " ";
    }
    return os;
}
*/
