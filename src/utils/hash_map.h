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
#include <iostream>
#include <sparsehash/dense_hash_map>
#include "../utils/common.h"
using std::hash; 
using google::sparse_hash_map;
using google::dense_hash_map;

namespace swift_snails {

template<typename KeyType, typename ValueType>
using SparseHashMap = sparse_hash_map<KeyType, ValueType, hash<KeyType>>;

template<typename KeyType, typename ValueType>
using DenseHashMap = dense_hash_map<KeyType, ValueType, hash<KeyType>>;

}; // end namespace swift_snails
#endif
