#include "../../core/ThreadPool.h"
using namespace swift_snails;
using namespace std;

typedef std::function<void(int)> func;

void test_ThreadPool() {
    ThreadPool<int, func> tp(2);

    func task = [](int a) {
        cout << "parse " << a << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    };

    auto channel = tp.start(std::move(task));

    for (int i = 0; i < 100; i++) {
        channel->push(i);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    channel->close();
}

int main()
{
    test_ThreadPool();
    return 0;
}


