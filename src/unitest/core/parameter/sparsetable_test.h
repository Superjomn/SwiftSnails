#include <climits>
#include "../../../core/parameter/sparsetable.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

TEST(SparseTableShard, init) {
    SparseTableShard<int, int> table;
}

TEST(SparseTableShard, assign) {
    SparseTableShard<int, int> table;
    auto map = table.data();
    map[127] = 127;
    map[128] = 128;

    ASSERT_EQ(map[127], 127);
    ASSERT_EQ(map[128], 128);
}

TEST(SparseTable, init) {
    global_config().register_config("shard_num", "5");
    SparseTable<int, int> table;
    ASSERT_EQ( table.shard_num(), 5);

    auto show_shard_id = [&table](int key) {
        LOG(INFO) << "key:\t" << key << "\tshard_id\t" << table.to_shard_id(key)
            <<"\thash_code\t" << get_hash_code(key);
    };
    show_shard_id(23232);
    show_shard_id(312);
    show_shard_id(9102);
    show_shard_id(91102);
    show_shard_id(31102);
    show_shard_id(11102);
}

TEST(SparseTable, find) {
    //global_config().register_config("shard_num", "5");
    SparseTable<int, int> table;

    table.assign(23232, 1);
    table.assign(3232, 2);

    ASSERT_EQ(table.size(), 2);

    int val;
    ASSERT_TRUE( table.find(23232, val));
    ASSERT_EQ(val, 1);
}

