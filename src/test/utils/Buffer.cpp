#include <iostream>
#include "../../utils/Buffer.h"
using namespace swift_snails;
using namespace std;

void test_BasicBuffer() {
    BasicBuffer bb;
}

void test_BinaryBuffer() {
    BinaryBuffer bb;
    bb << (int32_t) 12;
    bb << (double) 3.1415;
    bb << (uint64_t) 11111111;
    bb << (uint16_t) 6;
    cout << "size:\t" << bb.size() * 8 << endl;
    int32_t c;
    bb >> c;
    double d;
    bb >> d;
    uint16_t e;
    cout << "c:\t" << c << endl;
    cout << "d:\t" << d << endl;
    cout << "11111111111 uint64_t:\t" << bb.get<uint64_t>() << endl;
    bb >> e;
    cout << "6 uint16:\t" << e << endl;
    cout << "size:\t" << bb.size() << endl;
    for(int i=0; i < 1000; i++) {
        bb << (int32_t)112;
    }
    cout << "size:\t" << bb.size() << endl;
}


int main()
{
    test_BasicBuffer();
    test_BinaryBuffer();
    return 0;
}
