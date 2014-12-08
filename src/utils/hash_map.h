//
//  hash_map.h
//  SwiftSnails
//
//  Created by Chunwei on 12/8/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//
//
#ifndef SwiftSnails_utils_hash_map_h_
#define SwiftSnails_utils_hash_map_h_
#include "common.h"
#include <iostream>
#include <sparsehash/sparse_hash_map>
#include "../utils/common.h"
using google::sparse_hash_map;
using google::dense_hash_map;

namespace swift_snails {

// memory-saving version
template<typename KeyType, typename ValueType, typename CmpObj>
class SparseHashMap : VirtualObject {
public:
    explicit SparseHashMap() { }

private:
    sparse_hash_map<KeyType, ValueType, hash<KeyType>, CmpObj> _dic;
}; // end class SparseTable


// time-saving version
template<typename KeyType, typename ValueType, typename CmpObj>
class DenseHashMap : VirtualObject {
public:
    explicit DenseHashMap () { }

private:
    dense_hash_map<KeyType, ValueType, hash<KeyType>, CmpObj> dic;
}; // end class DenseHashMap

}; // end namespace swift_snails
#endif
