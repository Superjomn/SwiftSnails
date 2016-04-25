#include <climits>
#include "../../utils/hashmap.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

TEST(hashmap, SparseHashMap_string) {
  SparseHashMap<const char *, int> sparsemap;
  auto &map = sparsemap.get_map();
  map["a"] = 1;
  map["b"] = 2;

  ASSERT_EQ(map["a"], 1);
  ASSERT_EQ(map["b"], 2);
}

TEST(hashmap, SparseHashMap_int) {
  SparseHashMap<int, int> sparsemap;
  auto &map = sparsemap.get_map();

  map[1] = 1000;
  map[2] = 2000;
  ASSERT_EQ(map[1], 1000);
  ASSERT_EQ(map[2], 2000);
}

TEST(hashmap, DenseHashMap_string) {
  DenseHashMap<const char *, int> densemap;
  auto &map = densemap.get_map();

  map["hello world"] = 1;
  map["get"] = 1233;

  ASSERT_EQ(map["hello world"], 1);
  ASSERT_EQ(map["get"], 1233);
}

TEST(hashmap, DenseHashMap_int) {
  DenseHashMap<int, int> sparsemap;
  auto &map = sparsemap.get_map();

  map[1] = 1000;
  map[2] = 2000;
  ASSERT_EQ(map[1], 1000);
  ASSERT_EQ(map[2], 2000);
}
