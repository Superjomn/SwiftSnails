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

    for (int i = 0; i < 100; i ++) {
        channel->push(task);
    }

    while( ! channel->empty() ) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    channel->close();
}

