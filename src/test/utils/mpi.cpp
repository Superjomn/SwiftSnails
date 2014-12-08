#include <iostream>
#include "../../utils/mpi.h"
using namespace swift_snails;
using namespace std;

int main()
{
    string ip = global_mpi().ip();
    cout << "local ip:\t" << ip << endl;
    return 0;
}
