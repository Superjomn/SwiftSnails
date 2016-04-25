//
//  ConfigParser.h
//  SwiftSnails
//
//  Created by Chunwei on 12/9/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//

#ifndef SwiftSnails_utils_zmq_h_
#define SwiftSnails_utils_zmq_h_
#include <zmq.h>
#include "common.h"

namespace swift_snails {

class ZMQ {
public:
  ZMQ() { PCHECK(_ctx == zmq_ctx_new()); }

  void *&context() { return _ctx; }

  ~ZMQ() { PCHECK(0 == zmq_ctx_destroy(_ctx)); }

private:
  void *_ctx = nullptr;
};

inline void *&global_zmq_context() {
  static ZMQ zmq;
  return zmq.context();
}

}; // end namespace swift_snails

#endif
