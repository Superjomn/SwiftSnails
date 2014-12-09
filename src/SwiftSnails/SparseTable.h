//
//  SparseTable.h
//  SwiftSnails
//
//  Created by Chunwei on 12/8/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//

#ifndef SwiftSnails_SwiftSnails_SparseTable_h_
#define SwiftSnails_SwiftSnails_SparseTable_h_
#include "../utils/common.h"
#include "../utils/HashFunction.h"
#include "../utils/hash_map.h"

namespace swift_snails {

template<typename KeyType, typename ValueType, typename CmpObj>
class SparseTableShard : public VirtualObject {
public:
    typedef KeyType key_t;
    typedef ValueType value_t;
    typedef DenseHashMap<key_t, value_t, CmpObj> hash_map_t;

    explicit SparseTableShard() 
    { 
        _node_rank = global_mpi().rank();
        CHECK(_node_rank >= 0);
    }

    // should call after construct
    void init(int local_shard_id, int local_shard_num) {
        _local_shard_id = id;
        _local_shard_num = local_shard_id;
    }

    int global_shard_id() const {   // unique id globally
        return _local_shard_num * _node_rank + _local_shard_id;
    }

    int local_shard_id() const {
        return _local_shard_id;
    }

    size_t size() const {
        return _dic.size();
    }

    bool exists(const key_t &key) {
        return _dic.count(key) > 0;
    }

    ValueType& operator[](const key_t &key) {
        return _dic[key];
    }

private:
    hash_map_t _dic;
    int _local_shard_id;
    int _local_shard_num;
    int _node_rank;
};


template<typename KeyType, typename ValueType, typename CmpObj>
class SparseTable : public VirtualObject {
public:
    typedef KeyType key_t;
    typedef ValueType value_t;
    typedef SparseTableShard<key_t, value_t, CmpObj> shard_t;

    explicit SparseTable(int local_shard_num) : \
        _local_shard_num(local_shard_num) 
    {
        CHECK(_local_shard_num > 0);
        _local_shards.reset(new shard_t[_local_shard_num]);
        _shard_num = _local_shard_num * global_mpi().size();
    }

    shard_t& local_shard(int local_shard_id) {
        return _local_shards[local_shard_id];
    }

    size_t local_total_size() {
        size_t size = 0;
        for(int i = 0; i < _shard_num; i++) {
            size += local_shard(i).size();
        }
        return size;
    }

    size_t total_size() {   // global size
        global_mpi().barrier();
        size_t size = local_total_size();
        size = global_mpi().all_reduce(size, MPI_SUM);
        global_mpi().barrier();
        return size;
    }

    int shard_num() {   // global shard num
        return _shard_num;
    }

    int partition_key(const key_t &key)
    {
        return (int)get_hash_code(key) % 1000000009 % shard_num();
    }

private:
    int _local_shard_num;
    int _shard_num;
    std::shared_ptr<shard_t> _local_shards;
}; // end class SparseTable


}; // end namespace swift_snails
#endif
