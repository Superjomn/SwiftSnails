#include <climits>
#include "../../../core/parameter/hashfrag.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

TEST( BasicHashFrag, init) {
    BasicHashFrag<index_t> hashfrag;

    global_config().clear();
    global_config().register_config("frag_num", "100");

    hashfrag.set_num_nodes(7);
    hashfrag.init();

    LOG(INFO) << hashfrag;
}

TEST( BasicHashFrag, to_node_id) {
    BasicHashFrag<index_t> hashfrag;

    global_config().clear();
    global_config().register_config("frag_num", "100");

    hashfrag.set_num_nodes(7);
    hashfrag.init();

    auto show_node_id = [&hashfrag](const index_t &key) {
        LOG(INFO) << "key\t" << key 
            << "\tnode_id\t" 
            << hashfrag.to_node_id(key);
    };

    show_node_id(2323);
    show_node_id(323);
    show_node_id(2323323);
    show_node_id(323323);
}
