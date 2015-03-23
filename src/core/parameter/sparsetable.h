#pragma once
#include "../../utils/all.h"
namespace mio_table {

template<typename Key, typename Value> 
struct alignas(64) SparseTableShard : public VirtualObject {
public:
    typedef Key     key_t;
    typedef Value   value_t;

    // not thread safe!
    SparseHashMap<key_t, value_t>& data() {
        return _data;
    }

    bool find(const key_t& key, value_t* val) {
        auto it = _data.find(key);
        if (it == _data.end()) return false;
        val = it;
        return true;
    }
    bool find(const key_t& key, value_t &val) {
        auto it = _data.find(key);
        if (it == _data.end()) return false;
        val = *it;
        return true;
    }

    index_t size() const {
        rwlock_read_guard lock(_rwlock);
        return _data.size();
    }
    void set_shard_id( int x) {
        CHECK_GE(x, 0);
        _shard_id =  x;
    }
    int shard_id() const {
        return _shard_id;
    }

private:
    SparseHashMap<key_t, value_t> _data;
    int _shard_id = -1;
    RWLock _rwlock;
    //mutable std::mutex _mutex;
};  // struct SparseTableShard


template<typename Key, typename Value>
class SparseTable : public VirtualObject {
public:
    typedef Key     key_t;
    typedef Value   value_t;

    SparseTable() {
        _shard_num = global_config().get_config("shard_num").to_int32();
        _shards.reset(new SparseTableShard[shard_num()]);
    }

    SparseTableShard &shard(int shard_id) {
        return _shards[shard_id];
    }

    bool find(const key_t &key, value_t *val) {
        int shard_id = to_shard_id(key);
        return shard(shard_id).find(key, val);
    }

    bool find(const key_t& key, value_t &val) {
        int shard_id = to_shard_id(key);
        return shard(shard_id).find(key, val);
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
    void init(table_t& table, const AccessMethod& method)
    {
        _table = &table;
        _access_method = &method;
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
    // param -> value
    void get_pull_value(const key_t& key, pull_val &val) {
        pull_param_t param;
        if (! _table.find(key, param)) {
            _access_method->init_param(key, param);
            hashmap[key] = param;
        }
        _access_method->get_pull_value(key, param, val);
    }
    // client side
    // set local parameters with the value from remote nodes
    void apply_pull_value(const key_t &key, pull_param_t &param, const pull_val_t& val) {
        _access_method->apply_pull_value(key, param, val);
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
    void init(table_t& table, const access_method_t& access_method) {
        _table = &table;
        _access_method = &access_method;
    }

    explicit PushAccessAgent(table_t& table, const access_method_t& access_method) :
        _table(&table), 
        _access_method(access_method)
    { }

    void merge_push_value(const key_t &key, push_val_t &push_val, const push_val_t &other_push_val) {
        _access_method.merge_push_value(key, push_val, other_push_val);
    }
    // update parameters with the value from remote worker nodes
    void apply_push_value(const key_t& key, const push_val_t& push_val)
    {
        push_param_t *param;
        // TODO improve this in fix mode?
        CHECK( find(key, param) ) << "new key should be inited before";
        _access_method.apply_push_value(key, *param, push_val);
    }

private:
    table_t         *_table = nullptr;
    access_method_t _access_method;

};  // class PushAccessAgent



};  // end namespace swift_snails
