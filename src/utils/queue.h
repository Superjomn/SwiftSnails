//
//  queue.h
//  SwiftSnails
//
//  Created by Chunwei on 12/8/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//

#ifndef SwiftSnails_utils_queue_h_
#define SwiftSnails_utils_queue_h_
#include "common.h"

namespace swift_snails {

template <typename T> class threadsafe_queue {
private:
  mutable std::mutex mut;
  std::queue<std::shared_ptr<T>> data_queue;
  std::condition_variable data_cond;

public:
  explicit threadsafe_queue() {}

  void wait_and_pop(T &value) {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, [this] { return !data_queue.empty(); });
    value = std::move(*data_queue.front());
    data_queue.pop();
  }

  bool try_pop(T &value) {
    std::lock_guard<std::mutex> lk(mut);
    if (data_queue.empty())
      return false;
    value = std::move(*data_queue.front());
    data_queue.pop();
    return true;
  }

  std::shared_ptr<T> wait_and_pop() {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, [this] { return !data_queue.empty(); });
    std::shared_ptr<T> res = data_queue.front();
    data_queue.pop();
    return res;
  }

  std::shared_ptr<T> try_pop() {
    std::lock_guard<std::mutex> lk(mut);
    if (data_queue.empty())
      return std::shared_ptr<T>();
    std::shared_ptr<T> res = data_queue.front();
    data_queue.pop();
    return res;
  }

  void push(T new_value) {
    std::shared_ptr<T> data(std::make_shared<T>(std::move(new_value)));
    std::lock_guard<std::mutex> lk(mut);
    data_queue.push(std::move(data));
    data_cond.notify_one();
  }

  bool empty() const {
    std::lock_guard<std::mutex> lk(mut);
    return data_queue.empty();
  }

  index_t size() const {
    std::lock_guard<std::mutex> lk(mut);
    return data_queue.size();
  };

}; // end class threadsafe_queue

template <class T> class queue_with_capacity {
private:
  threadsafe_queue<T> queue;
  size_t capacity{0};
  std::condition_variable push_cond;
  std::mutex mut;

public:
  explicit queue_with_capacity(size_t capacity) : capacity(capacity) {}

  explicit queue_with_capacity() {}
  void set_capacity(size_t capacity) { this->capacity = capacity; }

  void push(T &&v) {
    std::unique_lock<std::mutex> lk(mut);
    push_cond.wait(lk, [this] { return queue.size() <= capacity; });
    queue.push(v);
  }
  /*
   * if the workers get empty value
   * then terminate
   */
  void wait_and_pop(T &value) {
    queue.wait_and_pop(value);
    std::lock_guard<std::mutex> lk(mut);
    push_cond.notify_one();
  }
  /*
   * input empty values to tell working threads to exit
   */
  void end_input(int x, T empty_v) {
    for (int i = 0; i < x; i++) {
      queue.push(empty_v);
    }
  }
};

}; // end namespace swift_snails

#endif
