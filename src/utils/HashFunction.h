//
//  HashFunction.h
//  SwiftSnails
//
//  Created by Chunwei on 12/7/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//

#ifndef SwiftSnails_utils_HashFunction_h_
#define SwiftSnails_utils_HashFunction_h_

#include "common.h"

namespace swift_snails {

inline uint64 get_hash_code(uint64 x) {
   // copy from MurmurHash3
   x ^= x >> 33;
   x *= 0xff51afd7ed558ccd;
   x ^= x >> 33;
   x *= 0xc4ceb9fe1a85ec53;
   x ^= x >> 33;
   return x;
}

}; // end namespace swift_snails

#endif
