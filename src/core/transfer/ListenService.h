//
//  ListenService.h
//  SwiftSnails
//
//  Created by Chunwei on 3/10/15.
//  Copyright (c) 2015 Chunwei. All rights reserved.
//
#ifndef Swift_transfer_ListenService_h_
#define Swift_transfer_ListenService_h_
#include "../../utils/common.h"
#include "../AsynExec.h"
#include "../../utils/SpinLock.h"
#include "Listener.h"
namespace swift_snails {

class ListenService {
public:
    explicit ListenService(std::shared_ptr<Listener> sender) :
        _sender(sender)
    { }

    explicit ListenService(std::shared_ptr<Listener> sender, int thread_num) : 
        _sender(sender),
        _thread_num(thread_num)
    { }

    void set_thread_num(int thread_num) {
        _thread_num = thread_num;
    }

    void start() {
        LOG(WARNING) << "SenderService start " << thread_num() << " threads";
        CHECK(_thread_num > 0);
        _threads.resize(thread_num());
        for(int i = 0; i < thread_num(); i++) {
            _threads[i] = std::thread(
                [this]() {
                    _sender->main_loop();
                }
            );
        }
    }

    void end() {
        LOG(WARNING) << "SenderService service threads exit!";
        CHECK(!_threads.empty());

        { std::lock_guard<SpinLock> lock(_spinlock);
            CHECK(_sender->service_complete());
        }
        // tell all service threads to exit
        for(int i = 0; i < thread_num(); i++) {
            zmq_send_push_once(_sender->zmq_ctx(), &Message().zmg(), _sender->recv_addr() );
        }

        for (int i = 0; i < thread_num(); i++) {
            _threads[i].join();
        }
        _threads.clear();
    }

    int thread_num() {
        return _thread_num;
    }

private:
    std::shared_ptr<Listener> _sender;
    int _thread_num = -1;
    std::vector<std::thread> _threads;
    SpinLock _spinlock;
};

}; // namespace swift_snails
#endif
