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
#include <iostream>
#include <sparsehash/sparse_hash_map>
#include <sparsehash/dense_hash_map>
#include "common.h"
// using std::hash;
using google::sparse_hash_map;
using google::dense_hash_map;

namespace swift_snails {

struct _eqstr {
  bool operator()(const char *s1, const char *s2) const {
    return (s1 == s2) || (s1 && s2 && strcmp(s1, s2) == 0);
  }
};

/*
 * a wrapper for google sparse_hash_map
 *
 * example:
 *  SparseHashMap<int, int> sparsehash;
 *
 *  // get the ownership of sparse_hash_map
 *  auto& map = sparsehash.get_map();
 *
 *  map[1] = 1000;
 *  map[2] = 2000;
 */
template <typename K, typename V> class SparseHashMap {
public:
  using map_t = sparse_hash_map<K, V, std::hash<K>>;
  map_t &get_map() { return map; }

private:
  sparse_hash_map<K, V, std::hash<K>> map;
};
/*
 * a specification wrapper for SparseHashMap<const char*, V>
 *
 * call `get_map()` to get the sparse_hash_map
 */
template <typename V> class SparseHashMap<const char *, V> {

public:
  using map_t =
      sparse_hash_map<const char *, V, std::hash<const char *>, _eqstr>;

  map_t &get_map() { return map; }

private:
  map_t map;
};

template <typename K, typename V> class DenseHashMap {
public:
  using map_t = dense_hash_map<K, V, std::hash<K>>;

  explicit DenseHashMap() { map.set_empty_key(0); }

  map_t &get_map() { return map; }

private:
  map_t map;
};

template <typename V> class DenseHashMap<const char *, V> {
public:
  using map_t =
      dense_hash_map<const char *, V, std::hash<const char *>, _eqstr>;

  explicit DenseHashMap() { map.set_empty_key(NULL); }

  map_t &get_map() { return map; }

private:
  map_t map;
};

}; // end namespace swift_snails
#endif
