#include "../../SwiftSnails/AsynExec.h"
using namespace swift_snails;

void test_AsynExec() {
    AsynExec as(2);

    const int wait_time = 200;

    std::shared_ptr<AsynExec::channel_t> channel = as.open();

    channel->push(
        [wait_time]() {
            LOG(INFO) << "hello";
            std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
        });
    channel->push(
        [wait_time]() {
            LOG(INFO) << "hello1";
            std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
        });
    channel->push(
        [wait_time]() {
            LOG(INFO) << "hello2";
            std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
        });
    channel->push(
        [wait_time]() {
            LOG(INFO) << "hello3";
            std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
        });
    channel->push(
        [wait_time]() {
            LOG(INFO) << "hello4";
            std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
        });
    channel->push(
        [wait_time]() {
            LOG(INFO) << "hello5";
            std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
        });



    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    channel->close();

    //std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    LOG(INFO) << "here I am running";
}


int main()
{
    test_AsynExec();
    return 0;
}
