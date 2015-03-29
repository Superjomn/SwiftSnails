//
//  master/terminate.h
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

class MasterTerminate {
public:
	typedef Transfer<ServerWorkerRoute> transfer_t;

    explicit MasterTerminate () {
    }

    void set_worker_num(int x) {
        CHECK_GT(x, 0);
        _worker_num = x;
    }
    void set_server_num(int x) {
        CHECK_GT(x, 0);
        _server_num = x;
    }

    void operator() () {
        CHECK_GT(_worker_num, 0) << "_worker_num should be inited first";
        CHECK_GT(_server_num, 0) << "_server_num should be inited first";

        wait_for_all_workers_finish();
        // wait for all workers to finish their work
        // then the framework will terminate
        tell_all_servers_to_terminate();
        // all servers have terminated
        // the master terminate
        LOG(WARNING) << "Master terminated normally!";
    }

protected:

    void wait_for_all_workers_finish() {
        // worker finish work and the master get the info
        transfer_t::msgcls_handler_t get_worker_finish_work = \
        [this](std::shared_ptr<Request> request, Request& response) {
            int node_id = request->meta.client_id;
            LOG(WARNING) << "node " << node_id << " finish work !";
            _num_finished_workers ++;
            // unblock the barrier
            if(_num_finished_workers == _worker_num) {
                _wait_workers_to_finish_barrier.set_state_valid();
                _wait_workers_to_finish_barrier.try_unblock();
            }
        };
        gtransfer.message_class().add(WORKER_FINISH_WORK, std::move(get_worker_finish_work));
        _wait_workers_to_finish_barrier.block();
    }
    /*
     * all the workers will terminate theirself when they finish their work
     * the servers should be told to terminate
     */
    void tell_all_servers_to_terminate() {
        for(int id : gtransfer.route().server_ids()) {
            Request req;
            req.meta.message_class = SERVER_TOLD_TO_TERMINATE;
            req.cont << 1234;
            req.call_back_handler = [this](std::shared_ptr<Request> response) {
                _num_finished_servers++;
                // all servers have terminated?
                if(_num_finished_servers == _server_num) {
                    _wait_servers_to_finish_barrier.set_state_valid();
                    _wait_workers_to_finish_barrier.try_unblock();
                }
            };
            gtransfer.send(std::move(req), id);
        }
        _wait_servers_to_finish_barrier.block();
    }

    void master_terminate() {
        LOG(WARNING) << "**********************************";
        LOG(WARNING) << "Master terminated normally!";
        LOG(WARNING) << "**********************************";
    }

private:

    int _worker_num = 0;
    int _server_num = 0;
    transfer_t& gtransfer = global_transfer<ServerWorkerRoute>();
    std::atomic<int> _num_finished_workers{0};
    std::atomic<int> _num_finished_servers{0};
    StateBarrier _wait_workers_to_finish_barrier;
    StateBarrier _wait_servers_to_finish_barrier;

};  // class MasterTerminate

};  // end namespace swift_snails
