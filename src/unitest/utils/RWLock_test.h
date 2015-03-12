#include <climits>
#include "../../utils/RWLock.h"
#include "../../core/AsynExec.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

TEST(RWLock, init) {
    RWLock lock;
}
