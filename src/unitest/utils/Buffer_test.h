#include <climits>
#include "../../utils/Buffer.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

TEST(_BinaryBuffer, status_test) {
    BinaryBuffer bb;
    bb << (int) 12;
    bb << (float) 1.27;
    bb << (double) 2.11;
    // status test
    ASSERT_EQ   ( bb.size(), sizeof(int) + sizeof(float) + sizeof(double));
    ASSERT_TRUE ( bb.capacity() > bb.size());
    ASSERT_TRUE ( bb.cursor() == bb.buffer());
    ASSERT_TRUE ( bb.cursor() != bb.end());
    ASSERT_TRUE ( bb.end() - bb.buffer() == bb.size());
}

TEST(_BinaryBuffer, read_test) {
    BinaryBuffer _b;
    int a = 12;
    int b = 13;
    double c = 3.14159;
    float d = -1.23;
    // put all the numbers
    _b << a;
    _b << b;
    _b << c;
    _b << d;

    int aa, bb;
    double cc;
    float dd;
    // read and check
    _b >> aa;
    _b >> bb;
    _b >> cc;
    _b >> dd;

    ASSERT_TRUE( a == aa);
    ASSERT_TRUE( b == bb);
    ASSERT_TRUE( c == cc);
    ASSERT_TRUE( d == dd);
}

struct Apple {
    float weight;
    int number;

    friend BinaryBuffer& operator<< (BinaryBuffer &bb, const Apple &app) {
        bb << (float)app.weight;
        bb << (int)app.number;
        return bb;
    }

    friend BinaryBuffer& operator>> (BinaryBuffer &bb, Apple &app) {
        bb >> app.weight;
        bb >> app.number;
        return bb;
    }
};

// test complex structure support
TEST(_BinaryBuffer, struct_read_write_test) {

    BinaryBuffer bb;

    int other = 12;

    Apple app;
    app.weight = 2.14;
    app.number = 100;

    bb << other;
    bb << app;

    Apple bpp;

    int other1;
    bb >> other1;
    bb >> bpp;

    ASSERT_EQ( app.weight, bpp.weight);
    ASSERT_EQ( app.number, bpp.number);
}

TEST(_BinaryBuffer, large_data_support) {
    BinaryBuffer bb;

    int a = 1111;
    for(int i = 0; i < 1024; i++) {
        bb << a;
    }

    ASSERT_TRUE ( bb.size() == sizeof(int) * 1024);

    int b;
    for(int i = 0; i < 1024; i++) {
        bb >> b;
        ASSERT_TRUE ( b == a);
    }
    ASSERT_TRUE( bb.read_finished());

    LOG(INFO) << "after clean bb.size:" << bb.size() << "\t"<< bb.buffer() <<  "\t" << bb.cursor();
    ASSERT_TRUE ( bb.cursor()  == bb.end());

    for(int i = 0; i < 2244; i++) {
        bb << a;
    }

    ASSERT_TRUE ( bb.size() == sizeof(int) * (2244 + 1024));
}

TEST(_BinaryBuffer, rewrite_after_clear) {
    BinaryBuffer bb;
    int a = 100;
    for (int i = 0; i < 1000; i ++) {
        bb << a;
    }
    bb.clear();
    ASSERT_TRUE(bb.size() == 0);
    ASSERT_TRUE(bb.capacity() >= 1000 * sizeof(int));
    for (int i = 0; i < 1000; i ++) {
        bb << a;
    }
    ASSERT_TRUE(bb.capacity() >= 1000 * sizeof(int));
    ASSERT_TRUE(bb.size() == 1000 * sizeof(int));
}

TEST(_BinaryBuffer, move_assign) {
    BinaryBuffer aa;

    int a = 1;
    float b = 1.34;

    aa << a;
    aa << b;

    BinaryBuffer bb = std::move(aa);

    int _a; float _b;
    bb >> _a;
    bb >> _b;

    ASSERT_TRUE(a == _a);
    ASSERT_TRUE(b == _b);
}

TEST(_BinaryBuffer, construct_assign) {
    BinaryBuffer aa;

    int a = 1;
    float b = 1.34;

    aa << a;
    aa << b;

    BinaryBuffer bb(std::move(aa));

    int _a; float _b;
    bb >> _a;
    bb >> _b;

    ASSERT_TRUE(a == _a);
    ASSERT_TRUE(b == _b);
}
