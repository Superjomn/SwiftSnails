#include <climits>
#include "../../utils/Buffer.h"
#include "../../core/Message.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;


TEST(_Message, create_and_destroy) {
    Message message;
}

TEST(_Message, init_from_BinaryBuffer) {
    BinaryBuffer bb;
    int a = 123;
    float b = 1.23;
    bb << a;
    bb << b;

    // create Message
    Message message(*bb.buffer(), bb.size());

    // decode data in message
    BinaryBuffer cc;
    cc.set(message.buffer(), message.size());

    int _a; float _b;
    cc >> _a;
    cc >> _b;

    ASSERT_EQ(a, _a);
    ASSERT_EQ(b, _b);
}

TEST(_Message, construct_from_BasicBuffer) {
    BinaryBuffer bb;
    int a = 123;
    float b = 1.23;
    bb << a;
    bb << b;

    // create Message
    Message message(bb);
}

TEST(_Message, construct_move_BasicBuffer) {
    BinaryBuffer bb;
    int a = 123;
    float b = 1.23;
    bb << a;
    bb << b;

    // create Message
    Message message(std::move(bb));

    // decode data in message
    BinaryBuffer cc;
    cc.set(message.buffer(), message.size());

    int _a; float _b;
    cc >> _a;
    cc >> _b;

    ASSERT_EQ(a, _a);
    ASSERT_EQ(b, _b);
}


