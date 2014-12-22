#include <iostream>
#include "../../utils/mpi.h"
using namespace swift_snails;
using namespace std;

int main()
{
    string ip = global_mpi().ip();
    cout << "rank:\t" << global_mpi().rank() << endl;
    cout << "size:\t" << global_mpi().size() << endl;
    for(int i = 0; i < global_mpi().size(); ++i) 
        cout << "ip:\t" << global_mpi().ip(i) << endl;

    cout << "local ip:\t" << ip << endl;
    return 0;
}
