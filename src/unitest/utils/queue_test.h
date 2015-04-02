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


TEST(queue_with_capacity, init) {
    queue_with_capacity<int> queue;
    int num_workers = 1;

    queue.set_capacity(2);
    auto producer = [&queue] {
        for(int i = 0; i < 1000; i ++) {
            LOG(INFO) << "put " << i;
            queue.push(std::move(i));
        }
        queue.end_input(1, -1);
    };

    auto summer = [&queue] {
        for(;;) {
            int j;
            queue.wait_and_pop(j);
            LOG(INFO) << "get " << j;
            if(j == -1) {
                LOG(INFO) << "summer exit!";
                break;
            }
        }
    };

    std::thread p1(producer);
    std::thread p2(producer);

    std::thread s1(summer);
    std::thread s2(summer);

    p1.join();
    p2.join();
    s1.join();
    s2.join();
};
