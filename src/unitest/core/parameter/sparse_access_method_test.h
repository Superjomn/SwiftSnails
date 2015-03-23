#include <climits>
#include "gtest/gtest.h"
#include "../../../core/parameter/sparsetable.h"
#include "../../../core/parameter/sparse_access_method.h"
using namespace std;
using namespace swift_snails;

namespace sparse_table_test {

// 参数格式
struct Param {
    int a;
    float b;
};

typedef float Val;

typedef SparseTable<index_t, Param> table_t;

class PullMethod : public PullAccessMethod<table_t, Val, Param> {

public:
    virtual void init_param(const key_t& key, pull_param_t &param)
    {
        param.a = 0;
        param.b = 0.5;
    }

    virtual void get_pull_value(const key_t &key, const pull_param_t &param, pull_val_t &val) {
        val = param.a + param.b;
    }

    virtual void apply_pull_value(const key_t &key, pull_param_t &param, const pull_val_t& val) {
        param.a += val;
        param.b += val;
    }

};  // class PullMethod

};  // end namespace sparse_table_test


TEST(SparseTable, access_method) {
    using namespace sparse_table_test;
    table_t table;
    auto access = make_pull_access<table_t, PullMethod>(table);
}
