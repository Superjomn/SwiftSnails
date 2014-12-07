#include <iostream>
#include <vector>
#include "../../utils/HashFunction.h"
using namespace swift_snails;
using namespace std;

void test_hash() {
    for(uint64 a = 0; a < 1000; a++) {
        cout << a << "\t" << get_hash_code(a) << endl;
    }
}


int main()
{
    test_hash(); 

    return 0;
}

