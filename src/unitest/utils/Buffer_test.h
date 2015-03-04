#include <climits>
#include "../../utils/Buffer.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

TEST(_BinaryBuffer, get_test) {
    BinaryBuffer bb;
    bb << (int) 12;
    bb << (float) 1.27;
    // status test
    ASSERT_EQ( bb.size(), sizeof(int) + sizeof(float));
}

