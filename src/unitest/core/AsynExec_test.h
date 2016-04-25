#include <climits>
#include "../../core/AsynExec.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

TEST(AsynExec, open) {
  AsynExec::task_t task = []() {
    int a = 1024;
    a *= 2;
    LOG(INFO) << std::this_thread::get_id() << " task run!";
  };

  AsynExec async(4);
  auto channel = async.channel();

  for (int i = 0; i < 100; i++) {
    channel->push(task);
  }

  while (!channel->empty()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  channel->close();
}

TEST(AsynExec, async_exec) {
  std::atomic<int> count{0};

  AsynExec::task_t task = [&count] {
    for (int i = 0; i < 10; i++) {
      LOG(INFO) << std::this_thread::get_id() << " run\t" << i;
      count++;
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  };

  AsynExec async(4);
  auto channel = async.channel();

  async_exec(4, std::move(task), channel);

  ASSERT_EQ(count, 40);
}
