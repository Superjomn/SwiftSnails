//
//  common.h
//  SwiftSnails
//
//  Created by Chunwei on 12/2/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//
#ifndef SwiftSnails_utils_common_h
#define SwiftSnails_utils_common_h

#include "VirtualObject.h"
#include "glog/logging.h"
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <fstream>

namespace swift_snails {
// common types
typedef unsigned char   byte_t;
typedef short           int16_t;
typedef unsigned short  uint16_t;
typedef int             int32_t;
typedef unsigned int    uint32_t;
typedef long long       int64_t;
typedef unsigned long long   uint64_t;

typedef uint32_t        index_t;

// for repeat patterns
#define SS_REPEAT1(X) SS_REPEAT_PATTERN(X)
#define SS_REPEAT2(X, args...) SS_REPEAT_PATTERN(X) SS_REPEAT1(args)
#define SS_REPEAT3(X, args...) SS_REPEAT_PATTERN(X) SS_REPEAT2(args)
#define SS_REPEAT4(X, args...) SS_REPEAT_PATTERN(X) SS_REPEAT3(args)
#define SS_REPEAT5(X, args...) SS_REPEAT_PATTERN(X) SS_REPEAT4(args)
#define SS_REPEAT6(X, args...) SS_REPEAT_PATTERN(X) SS_REPEAT5(args)

};

#endif

