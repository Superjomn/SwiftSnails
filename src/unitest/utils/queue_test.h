#include <climits>
#include "../../utils/queue.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

TEST(_queue, try_pop) {
    threadsafe_queue<int> queue;
    threadsafe_queue<int> out_queue;
    const int num_thread = 1;

    std::function<void()> handler = [&queue, &out_queue]() {
        for(;;) {
            int task;
            queue.wait_and_pop(task);
            LOG(INFO) << std::this_thread::get_id() << " get task:\t" << task;
            if (task == -1) 
            {
                LOG(INFO) << "handler " << std::this_thread::get_id() << " exit";
                return;
            }
            task *= 2;
            out_queue.push(task);
        }
    };

    std::function<void()> produce = [&queue]() {
        for(int i = 0; i < 1000; i ++) {
            queue.push(i);
            LOG(INFO) << std::this_thread::get_id() << " put task:\t" << i;
        }

        for(int i = 0; i < 2 * num_thread; i++) {
            queue.push(-1);
        }
        LOG(INFO) << "producer " << std::this_thread::get_id() << " exit!";
    };

    std::thread producer(produce);

    std::vector<std::thread> threads;

    for(int i = 0; i < num_thread; i ++) {
        std::thread t(handler);
        threads.push_back(std::move(t));
    }

    for(std::thread &t : threads) {
        t.join();
    }
    producer.join();
}
