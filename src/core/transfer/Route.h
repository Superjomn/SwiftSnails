//
//  Route.h
//  SwiftSnails
//
//  Created by Chunwei on 3/09/15.
//  Copyright (c) 2015 Chunwei. All rights reserved.
//
#ifndef Swift_transfer_Route_h_
#define Swift_transfer_Route_h_
#include "../../utils/common.h"
#include "../AsynExec.h"
#include "../../utils/SpinLock.h"
//#include "../../utils/hashmap.h"
#include "../../utils/RWLock.h"
namespace swift_snails {

/*
 * a controller of routes
 */
class BaseRoute : public VirtualObject {

public:
  explicit BaseRoute() { _zmq_ctx = zmq_ctx_new(); }

  // update the route
  virtual void update() = 0;

  void *zmq_ctx() { return _zmq_ctx; }
  // TODO change node at once
  // Attention: not thread-safe
  void register_node(int id, std::string &&addr) {
    // std::lock_guard<std::mutex> lock(_write_mut);
    CHECK(_send_addrs.count(id) == 0) << "id exists!";
    _send_addrs.emplace(id, std::move(addr));
    _send_mutexes.emplace(id, std::unique_ptr<std::mutex>(new std::mutex));
    void *sender = zmq_socket(_zmq_ctx, ZMQ_PUSH);
    _senders.emplace(id, sender);
    connect(id);
  }

  // Attention: not thread-safe
  // should use rwlock(Read-Write lock)
  void delete_node(int id) {
    // std::lock_guard<std::mutex> lock(_write_mut);
    LOG(WARNING) << "delete node " << id << " " << _send_addrs[id];
    // write lock
    {
      auto it = _send_addrs.find(id);
      CHECK(it != _send_addrs.end());
      _send_addrs.erase(it);
    }
    {
      auto it = _send_mutexes.find(id);
      CHECK(it != _send_mutexes.end());
      _send_mutexes.erase(it);
    }
    // drop the connection
    {
      auto it = _senders.find(id);
      CHECK(it != _senders.end());
      PCHECK(0 == zmq_close(it->second));
      _senders.erase(it);
    }
  }

  void *sender(int id) {
    rwlock_read_guard lock(_read_write_lock);
    return _senders[id];
  }

  const std::string &sender_addr(int id) {
    rwlock_read_guard lock(_read_write_lock);
    return _send_addrs[id];
  }

  std::unique_ptr<std::mutex> &send_mutex(int id) {
    rwlock_read_guard lock(_read_write_lock);
    return _send_mutexes[id];
  }

  virtual ~BaseRoute() {
    LOG(WARNING) << "Route deconstruct";
    for (auto &sender : _senders) {
      PCHECK(0 == zmq_close(sender.second));
    }
    PCHECK(0 == zmq_ctx_destroy(_zmq_ctx));
  }

  std::map<int, std::string> &send_addrs() { return _send_addrs; }

protected:
  void connect(index_t id) {
    LOG(INFO) << "client connect " << _send_addrs[id];
    PCHECK(0 == ignore_signal_call(zmq_connect, _senders[id],
                                   _send_addrs[id].c_str()));
  }

protected:
  void *_zmq_ctx = NULL;
  std::map<int, void *> _senders;
  std::map<int, std::string> _send_addrs;
  std::map<int, std::unique_ptr<std::mutex>> _send_mutexes;
  // version of the route
  // if _version is out of date, route will
  // be updated
  // index_t _version = -1;
  // protect only one writer
  RWLock _read_write_lock;
  // std::mutex _write_mut;
  // determine whether version is valid
  // bool _version_valid = true;
};

}; // end namespace swift_snails
#endif
