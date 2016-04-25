#include <climits>
#include "../../utils/RWLock.h"
#include "../../core/BasicChannel.h"
#include "../../core/AsynExec.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

TEST(RWLock, init) { RWLock lock; }

TEST(RWLock, lock_unlock) {

  RWLock lock;

  AsynExec::task_t writer = [&lock]() {
    {
      lock.wrlock();
      LOG(INFO) << std::this_thread::get_id() << " writting ...";
      // std::this_thread::sleep_for(std::chrono::milliseconds(300));
      lock.unlock();
    }
  };

  AsynExec::task_t reader = [&lock]() {
    {
      lock.rdlock();
      LOG(INFO) << std::this_thread::get_id() << " reading ...";
      // std::this_thread::sleep_for(std::chrono::milliseconds(300));
      lock.unlock();
    }
  };

  AsynExec async(4);
  auto channel = async.channel();

  for (int i = 0; i < 20; i++) {
    channel->push(reader);
    channel->push(writer);
  }

  while (!channel->empty()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  channel->close();
}
