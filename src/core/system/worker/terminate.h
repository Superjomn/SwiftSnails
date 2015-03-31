//
//  worker/terminate.h
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

class ClientTerminate {
public:
	typedef Transfer<ServerWorkerRoute> transfer_t;

    explicit ClientTerminate() : \
        gtransfer(global_transfer<ServerWorkerRoute>())
    {
    }

    void operator() () {
        worker_send_finish_message(); 
        LOG(WARNING) << "Worker terminate normally";
    }

protected:

    // send finish message to master
    void worker_send_finish_message() {
        Request req;
        req.meta.message_class = WORKER_FINISH_WORK;
        req.call_back_handler = [this] (std::shared_ptr<Request> response) { 
            _wait_rsp_barrier.set_state_valid();
            _wait_rsp_barrier.try_unblock();
        };
        req.cont << 1234;
        gtransfer.send(std::move(req), 0);
        _wait_rsp_barrier.block();

        LOG(WARNING) << "worker terminate!";
    }
    
private:
    transfer_t& gtransfer; 
    StateBarrier _wait_rsp_barrier;

};  // class ClientTerminate


};  // end namespace swift_snails
