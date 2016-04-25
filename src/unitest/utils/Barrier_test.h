#include <climits>
#include "../../utils/Barrier.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

TEST(Barrier, init) { Barrier barrier; }

TEST(Barrier, block) {
  Barrier barrier;

  std::function<void()> handler1 = [&barrier] {
    LOG(INFO) << "handler1 running";
    LOG(INFO) << "handler1 block";
    barrier.block();
    LOG(INFO) << "handler1 complete";
  };
  std::function<void()> handler2 = [&barrier] {
    LOG(INFO) << "handler2 running";
    LOG(INFO) << "handler2 block";
    barrier.block();
    LOG(INFO) << "handler2 complete";
  };
  std::function<void()> handler3 = [&barrier] {
    LOG(INFO) << "handler3 running";
    LOG(INFO) << "handler3 sleep for 0.5 s";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    LOG(INFO) << "handler3 unblock the barrier";
    barrier.unblock();
  };

  std::thread t1(handler1);
  std::thread t2(handler2);
  std::thread t3(handler3);

  t1.join();
  t2.join();
  t3.join();
}
