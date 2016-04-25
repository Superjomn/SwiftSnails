//
//  Timer.h
//  SwiftSnails
//
//  Created by Chunwei on 3/17/15.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//
#ifndef SwiftSnals_utils_Timer_h_
#define SwiftSnals_utils_Timer_h_
#include <chrono>
#include <ostream>
namespace swift_snails {

class Timer {
  typedef std::chrono::high_resolution_clock high_resolution_clock;
  // typedef std::chrono::milliseconds milliseconds;
  typedef std::chrono::seconds seconds;

public:
  explicit Timer() {}

  void reset() { _start = high_resolution_clock::now(); }

  void start() { reset(); }

  void set_time_span(int span) { _time_span = span; }

  bool timeout() const { return elapsed().count() > _time_span; }

  seconds elapsed() const {
    return std::chrono::duration_cast<seconds>(high_resolution_clock::now() -
                                               _start);
  }
  int time_span() const { return _time_span; }
  template <typename T, typename Traits>
  friend std::basic_ostream<T, Traits> &
  operator<<(std::basic_ostream<T, Traits> &out, const Timer &timer) {
    return out << timer.elapsed().count();
  }

private:
  high_resolution_clock::time_point _start;
  int _time_span;
}; // end class Timer

}; // end namespace swift_snails
#endif
