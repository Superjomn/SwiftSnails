#include <climits>
#include "../../../core/transfer/Receiver.h"
#include "../../../core/transfer/Sender.h"
#include "../../../core/transfer/ListenService.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

namespace _test_transfer {

const int wait_time = 2000;

class Route : public BaseRoute {
    void update() {
    }
};

std::shared_ptr<AsynExec::channel_t> global_channel() {
    int thread_num = 4;
    static AsynExec as(thread_num);
    static std::shared_ptr<AsynExec::channel_t> global_channel = as.open();
    return global_channel;
}

void run_receiver() {

    // get address 
    int port = 80805;
    std::string addr;
    std::string ip = get_local_ip();
    format_string(addr, "tcp://%s:%d",  ip.c_str(), port);

    Route route;

    string sender_addr;
    format_string(sender_addr, "tcp://%s:%d", ip.c_str(), port+1);
    route.register_node(1, std::move(sender_addr));

    auto receiver = std::make_shared<Receiver>(route);
    // add message classes
    Receiver::handler_t handler1 = [](std::shared_ptr<Request> request, Request &response) {
        LOG(INFO) << "message-class-1 handler";
        LOG(INFO) << ".. message_id\t" << request->meta.message_id;
        LOG(INFO) << ".. message_class\t" << request->meta.message_class;
        LOG(INFO) << ".. client_id\t" << request->meta.client_id;
        int a;
        request->cont >> a;
        ASSERT_EQ(a, 12);
        LOG(INFO) << ".. cont\t" << a;
        response.cont << 13;
    };

    Receiver::handler_t handler2= [](std::shared_ptr<Request> request, Request &response) {
        LOG(INFO) << "message-class-2 handler";
    };
    receiver->message_class().add(1, std::move(handler1));
    receiver->message_class().add(2, std::move(handler2));

    // set async channel
    receiver->set_async_channel(global_channel());

    receiver->listen(addr);
    ListenService listen_service(receiver, 4);
    LOG(WARNING) << "receiver start service";
    listen_service.start();

    LOG(INFO) << "receiver main process sleep for time: " << wait_time;
    // sleep for some time
    std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));

    LOG(WARNING) << "receiver end service";
    listen_service.end();
}

void run_sender() {
    // get address 
    int port = 80806;
    std::string addr;
    std::string ip = get_local_ip();
    format_string(addr, "tcp://%s:%d",  ip.c_str(), port);
    Route route;

    string receiver_addr;
    format_string(receiver_addr, "tcp://%s:%d", ip.c_str(), port-1);
    route.register_node(0, std::move(receiver_addr));

    // register server's address

    auto sender = std::make_shared<Sender>(route);
    sender->set_recv_ip(ip);
    sender->set_client_id(1);
    sender->listen(addr);

    sender->set_async_channel(global_channel());

    ListenService listen_service(sender, 4);
    LOG(WARNING) << "sender start service";
    listen_service.start();
    
    // try to send a message
    Request request;
    request.meta.message_class = 1;
    request.meta.client_id = 1;
    request.cont << 12;

    request.call_back_handler = [] (std::shared_ptr<Request> response) {
        LOG(WARNING) << "sender get a response";
        int a;
        response->cont >> a;
        ASSERT_EQ(a, 13);
    };

    LOG(INFO) << ".. send a message";
    sender->send(std::move(request), 0);

    std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
    LOG(WARNING) << "sender end service";
    listen_service.end();
}

};  // end namespace _test_transfer

TEST(transfer, send) {
    using namespace _test_transfer;
    
    LOG(WARNING) << "start receiver and sender threads";
    std::thread receiver(run_receiver);
    std::thread sender(run_sender);

    receiver.join();
    sender.join();
}
    
