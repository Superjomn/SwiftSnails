/*
 * common.cpp
 *
 *  Created on: Jan 5, 2015
 *      Author: Chunwei
 */

#include "../../SwiftSnails/common.h"
using namespace swift_snails;
using namespace std;

void test_IP() 
{
    IP ip("127.0.0.1:8080");
    cout << ip.to_string() << endl;
    BinaryBuffer bb;
    bb << ip;
    cout << "buffer:\t" << bb.buffer() << endl;
    cout << "from buffer" << endl;
    cout << bb.status();
    IP newip;
    bb >> newip;
    cout << "newip:\t" << newip.to_string() << endl;
}


int main()
{
    test_IP();
    return 0;
}



