#include <climits>
#include "../../utils/vec1.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

TEST(Vec1, init) { Vec vec(10); }

TEST(Vec1, copy) {
  Vec vec;
  vec.init(10, true);
  Vec b(vec);
}

TEST(Vec1, exists_copy) {
  Vec vec;
  vec.init(10, true);
  Vec b;
  b.init(15, true);
  b = vec;
}

TEST(Vec1, get_elem) {
  Vec vec(10);
  vec[0] = 1;
  ASSERT_EQ(vec[0], 1);
}

TEST(Vec1, outer) {
  Vec a, b;
  a.init(10, true);
  b.init(20, true);
  LOG(INFO) << "a:\t" << a;
  LOG(INFO) << "b:\t" << b;
  auto res = outer(a, b);
}
