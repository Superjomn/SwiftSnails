#include <climits>
#include "../../core/BasicChannel.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;


TEST(_BasicChannel, push_pop) {
    BasicChannel<int> channel;

    std::function<void()> handler = [&channel] {
        for(;;) {
            int task;
            bool res = channel.pop(task);
            if(!res) {
                LOG(INFO) << std::this_thread::get_id() << " exit!";
                return;
            }
            LOG(INFO) << std::this_thread::get_id() << " get task:\t" << task;
            for(int i = 0; i < 10000; i++) {
                task *= 2;
            }
        }
    };

    std::function<void()> produce = [&channel]() {
        for(int i = 0; i < 1000; i ++) {
            channel.push(i);
            //LOG(INFO) << std::this_thread::get_id() << " put task:\t" << i;
        }

        LOG(INFO) << "channel to close";
        channel.close();


        LOG(INFO) << "producer " << std::this_thread::get_id() << " exit!";
    };

    thread producer (produce);

    vector<thread> handlers;
    for(int i = 0; i < 10; i ++) {
        handlers.emplace_back(handler);
    }


    for(thread& t : handlers) {
        t.join();
    }

    producer.join();

}


