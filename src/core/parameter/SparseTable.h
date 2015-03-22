#ifndef SwiftSnails_core_sparsetable_SparseTable_h_ 
#define SwiftSnails_core_sparsetable_SparseTable_h_ 
#include "../../utils/all.h"
namespace mio_table {

template<typename Key, typename Value> 
struct alignas(64) SparseTableShard : public VirtualObject {
public:
    typedef Key     key_t;
    typedef Value   value_t;

    SparseHashMap<key_t, value_t>& data() {
        return _data;
    }

    std::mutex& mutex() {
        return _mutex;
    }

    index_t size() const {
        rwlock_read_guard lock(_rwlock);
        return _data.size();
    }

private:
    SparseHashMap<key_t, value_t> _data;
    RWLock _rwlock;
    //mutable std::mutex _mutex;
};  // struct SparseTableShard


template<typename Key, typename Value>
class SparseTable : public VirtualObject {
public:
    typedef Key     key_t;
    typedef Value   value_t;

    SparseTable() {
        _shards.reset(new SparseTableShard[shard_num()]);
    }

    SparseTableShard &local_shard(int shard_id) {
        return _shards[shard_id];
    }

    index_t size() const {
        index_t res = 0;
        for(auto& shard: _shards) {
            res += shard.size();
        }
        return res;
    }
    int to_shard_id(const key_t& key) {
        return int(get_hash_code(key) % 1000000009) % shard_num();
    }
    int shard_num() const {
        return _shard_num;
    }

private:
    std::unique_ptr<SparseTable[]> _shards; 
    int _shard_num = 1;
};  // class SparseTable


template<typename Table, typename AccessMethod>
class PullAccessAgent {
public:
    typedef typename Table          table_t;
    typedef typename Table::key_t   key_t;
    typedef typename Talbe::value_t value_t;
    typedef typename AccessMethod   access_method_t;
    typedef typename AccessMethod::pull_val_t pull_val;
    typedef typename AccessMethod::pull_param_t pull_param;

    explicit PullAccessAgent() {
    }

    explicit PullAccessAgent(table_t& table, const AccessMethod& method) :
        _table(&table),
        _access_method(method)
    { }

    int to_shard_id(const key_t& key) {
        return _table->to_shard_id(key);
    }

    // server side
    // query keys
    void get_pull_value(const key_t& key, pull_val &val) {
        int shard_id = to_shard_id(key);
        auto& hashmap = _table->local_shard(shard_id).data();
        auto it = hashmap.find(key);
        // key exists
        if( it != hashmap.end()) { 
            _access_method.get_pull_value(key, it->second, val);
        } else {
            pull_param_t def_param;
            _access_method.init_param(key, def_param);
            hashmap[key] = std::move(def_param);
            _access_method.get_pull_value(key, def_param, val);
        }
    }

private:
    table_t     *_table;
    AccessMethod _access_method;
};  // class AccessAgent


template<typename Table, typename AccessMethod>
class PushAccessAgent {
public:
    typedef typename Table          table_t;
    typedef typename Table::key_t   key_t;
    typedef typename Talbe::value_t value_t;
    typedef typename AccessMethod   access_method_t;
    typedef typename AccessMethod::push_val_t push_val_t;
    typedef typename AccessMethod::push_param_t push_param_t;

    explicit PushAccessAgent() {
    }

    explicit PushAccessAgent(table_t& table, const access_method_t& access_method) :
        _table(&table), 
        _access_method(access_method)
    { }

    int to_shard_id(const key_t& key) {
        return _table->to_shard_id(key);
    }

    void merge_push_value(const key_t &key, push_val_t push_val, const push_val_t &other_push_val) {
        _access_method.merge_push_value(key, push_val, other_push_val);
    }
    // update parameters
    void apply_push_value(const key_t& key, const push_val_t& push_val)
    {
        auto& hashmap = _table->local_shard(shard_id).data();
        auto it = hashmap.find(key);
        // key exists
        if (it != hashmap.end()) {
            _access_method.apply_push_value(key, it->second, push_val);

        } else {    // key not exists

            push_param_t init_param;
            _access_method.init_param(key, init_param);
            hashmap[key] = init_param;
            auto _it = hashmap.find(key);
            _access_method.init_param(key, _it->second, init_param);
        }
    }

private:
    table_t         *_table = nullptr;
    access_method_t _access_method;

};  // class PushAccessAgent



};  // end namespace swift_snails
#endif
