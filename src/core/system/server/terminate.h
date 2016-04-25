//
//  server/terminate.h
//  SwiftSnails
//
//  Created by Chunwei on 3/29/15.
//  Copyright (c) 2015 Chunwei. All rights reserved.
//
#pragma once
#include "../../../utils/all.h"
#include "../../transfer/transfer.h"
#include "../message_classes.h"
#include "../ServerWorkerRoute.h"

namespace swift_snails {

template <class Key, class Value> class ServerTerminate {
public:
  typedef Transfer<ServerWorkerRoute> transfer_t;
  typedef Key key_t;
  typedef Value val_t;

  explicit ServerTerminate()
      : gtransfer(global_transfer<ServerWorkerRoute>()),
        sparse_table(global_sparse_table<key_t, val_t>()) {}

  void operator()() {
    wait_for_master_terminate_message();
    LOG(WARNING) << "Server terminated normally";
  }

protected:
  void wait_for_master_terminate_message() {
    transfer_t::msgcls_handler_t handler = [this](
        std::shared_ptr<Request> request, Request &response) {
      // export local parameter to pipe
      sparse_table.output();

      _wait_master_terminate_barrier.set_state_valid();
      _wait_master_terminate_barrier.try_unblock();
      response.cont << 1234;
    };

    gtransfer.message_class().add(SERVER_TOLD_TO_TERMINATE, std::move(handler));
    _wait_master_terminate_barrier.block();
  }

private:
  Transfer<ServerWorkerRoute> &gtransfer;

  SparseTable<key_t, val_t> &sparse_table;

  StateBarrier _wait_master_terminate_barrier;

}; // class ServerTerminate

}; // end namespace swift_snails
