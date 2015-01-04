/*
 * hash_map
 *
 *  Created on: Jan 4, 2015
 *      Author: Chunwei
 */
#include "../../utils/common.h"
#include "../../utils/hash_map.h"
using namespace swift_snails;
using namespace std;

int main()
{

	SparseHashMap<string, string> map;
	map["hello"] = "world";

    cout << "map" << map["hello"] << endl;
	return 0;
}


