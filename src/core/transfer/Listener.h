//
//  Listener.h
//  SwiftSnails
//
//  Created by Chunwei on 3/09/15.
//  Copyright (c) 2015 Chunwei. All rights reserved.
//
#ifndef Swift_transfer_Listener_h_
#define Swift_transfer_Listener_h_
#include "../../utils/common.h"
#include "../AsynExec.h"
#include "../../utils/SpinLock.h"
namespace swift_snails {

class Listener : VirtualObject {
public:
  explicit Listener() {}
  void set_zmq_ctx(void *zmq) {
    _zmq_ctx = zmq;
    PCHECK(_receiver = zmq_socket(_zmq_ctx, ZMQ_PULL));
  }

  explicit Listener(void *zmq_ctx) : _zmq_ctx(zmq_ctx) {
    PCHECK(_receiver = zmq_socket(_zmq_ctx, ZMQ_PULL));
  }

  ~Listener() {
    LOG(WARNING) << "listener exit!";
    if (_receiver) {
      PCHECK(0 == zmq_close(_receiver));
      _receiver = NULL;
    }
  }

  virtual void main_loop() = 0;
  virtual bool service_complete() = 0;

  void set_thread_num(int num) {
    CHECK(num > 0);
    _thread_num = num;
  }
  int thread_num() const { return _thread_num; }

  void service_start() {
    LOG(WARNING) << "ListenService start " << thread_num() << " threads";
    CHECK(_thread_num > 0);
    _threads.resize(thread_num());
    for (int i = 0; i < thread_num(); i++) {
      _threads[i] = std::thread([this]() { main_loop(); });
    }
  }

  void service_end() {
    LOG(WARNING) << "SenderService service threads exit!";
    CHECK(!_threads.empty());

    {
      std::lock_guard<SpinLock> lock(_spinlock);
      CHECK(service_complete());
    }
    // tell all service threads to exit
    for (int i = 0; i < thread_num(); i++) {
      zmq_send_push_once(zmq_ctx(), &Message().zmg(), recv_addr());
    }

    for (int i = 0; i < thread_num(); i++) {
      _threads[i].join();
    }
    _threads.clear();
  }
  // set properties
  void set_recv_ip(const std::string &ip) {
    CHECK(!ip.empty());
    _recv_ip = ip;
  }

  /*
   * listen to a random port
   */
  int listen() {
    if (_recv_ip.empty()) {
      _recv_ip = get_local_ip();
    }
    CHECK(_recv_port == -1) << "local receiver can only listen once";
    zmq_bind_random_port(_recv_ip, _receiver, _recv_addr, _recv_port);
    LOG(INFO) << "client listen to address:\t" << _recv_addr;
    return _recv_port;
  }
  /*
   * listen to a specified address
   */
  void listen(const std::string &addr) {
    LOG(INFO) << "server listen to " << addr;
    _recv_addr = addr;
    int res;
    PCHECK((res = zmq_bind(_receiver, addr.c_str()), res == 0));
  }

  void *zmq_ctx() { return _zmq_ctx; }
  // get attributes
  void *receiver() { return _receiver; }
  std::mutex &receiver_mutex() { return _receiver_mutex; }
  const std::string &recv_addr() const { return _recv_addr; }
  const std::string &recv_ip() const { return _recv_ip; }
  int recv_port() const { return _recv_port; }

protected:
  void *_zmq_ctx = NULL;
  void *_receiver = NULL;
  std::mutex _receiver_mutex;
  std::string _recv_addr;
  std::string _recv_ip;
  int _recv_port = -1;
  int _thread_num = -1;
  // listen service
  std::vector<std::thread> _threads;
  SpinLock _spinlock;

}; // end Listener

}; // namespace swift_snails
#endif
