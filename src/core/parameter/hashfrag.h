#pragma once
#include "../../utils/all.h"
namespace swift_snails {

// get hash code
uint64_t hash_fn(uint64_t x) {
    return get_hash_code(x);
}

/*
 * Basic Hash Fragment
 * without Replication, Fault Tolerance and Repair
 */
template<typename Key>
class BasicHashFrag : public VirtualObject {
public:
    typedef Key key_t;
    // num_nodes: initial number of nodes
    explicit BasicHashFrag() 
    { 
    }
    /*
     * number of nodes should be setted by master
     */
    void set_num_nodes(int x) {
        CHECK(x > 0);
        _num_nodes = x;
    }
    // init nodes in the hash ring
    // can be called only once
    // and should be called by *Master Server*
    // the worker nodes should not call it
    void init() {
        CHECK(num_nodes() > 0);
        //num_nodes = global_config().register_config("init_node_num").to_int32();
        _num_frags = global_config().get_config("frag_num").to_int32();
        _map_table.reset(new index_t[num_frags()]);
        // divide the fragments
        int num_frag_each_node = int(num_frags() / num_nodes() );
        for(int i = 0; i < num_frags(); i ++) {
            _map_table[i] = int(i / num_frag_each_node);
        }
    }

    int to_node_id(const key_t &key) {
        CHECK(_map_table) << "map_table has not been inited";
        int frag_id = hash_fn(key) % num_frags();
        int node_id = _map_table[frag_id];
        return node_id;
    }

    void serialize(BinaryBuffer &bb) const {
        bb << num_nodes();
        bb << num_frags();

        for(int i = 0; i < num_frags(); i ++) {
            bb << _map_table[i];
        }
    }

    void deserialize(BinaryBuffer &bb) {
        int num_nodes_, num_frags_;
        bb >> num_nodes_;    
        bb >> num_frags_;
        CHECK( 
            (_map_table && (num_frags_ == num_frags())) ||
            (!_map_table)
        );
        _num_nodes = num_nodes_;
        _num_frags = num_frags_;
        // memory should not been inited
        // the size of the map table will not be changed
        for(int i = 0; i < num_frags(); i ++) {
            bb << _map_table[i];
        }
    }

    int num_nodes() const {
        return _num_nodes;
    }
    int num_frags() const {
        return _num_frags;
    }

    friend std::ostream& operator<< (std::ostream &os, BasicHashFrag<key_t> &frag) {
        os << "hash frag" << std::endl;
        for(int i = 0; i < frag.num_frags(); i ++) {
            os << frag._map_table[i] << " ";
        }
        os << std::endl;
        return os;
    }


private:
    int _num_nodes = 0;
    int _num_frags = 0;
    // record visit frequency
    //std::map<int, NodeVisitFreq> visit_freqs;
    // register config 
    std::unique_ptr<index_t[]> _map_table;

};  // class HashFrag



};  // end namespace swift_snails
