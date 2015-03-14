#include <climits>
#include "../../../core/transfer/Transfer.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

namespace _test_one_transfer {

class Route : public BaseRoute {
    void update() {
    }
};


class TransferTest : public testing::Test {
protected:
    virtual void SetUp() {
        // open global channel
        //int thread_num = 2;
        transfer.init_async_channel(2);
        transfer.set_listen_service_num(2);
        //transfer.set_async_channel(global_channel);
        //transfer.start_sender_service("tcp://10.15.229.188:17831");
        //transfer.start_receiver_service("tcp://10.15.229.188:17832");
        transfer.start_sender_service();
        transfer.start_receiver_service();

    }

    virtual void TearDown() {
        //global_channel->close();
    }

protected:
    Transfer<Route> transfer;
    //std::shared_ptr<AsynExec::channel_t> global_channel;
};  // end class TransferTest



// the transfer send message to itself
TEST_F (TransferTest, send_receive_message) {
    LOG(WARNING) << "transfer receiver address:\t" << transfer.receiver_addr();
    LOG(WARNING) << "transfer receiver address:\t" << transfer.sender_addr();

    string receiver_addr = transfer.receiver_addr();
    string sender_addr = transfer.sender_addr();

    transfer.route().register_node(0, std::move(receiver_addr));
    transfer.route().register_node(1, std::move(sender_addr));


    Transfer<Route>::msgcls_handler_t msgcls1 = [](std::shared_ptr<Request> request, Request& response) {
        LOG(INFO) << ".. get a message";
        LOG(INFO) << "message_class\t" << request->meta.message_class;
        LOG(INFO) << "message_id\t" << request->meta.message_id;
        LOG(INFO) << "client_id\t" << request->meta.client_id;

        int a;
        request->cont >> a;

        ASSERT_EQ(a, 2008);

        LOG(INFO) << "cont\t" << a;
        response.cont  << a+1 ;
    };

    Transfer<Route>::msgrsp_handler_t msgrsp1 = [](std::shared_ptr<Request> response) {
        LOG(INFO) << ".. get a response";
        LOG(INFO) << "message_id\t" << response->meta.message_id;
        int b;
        response->cont >> b;
        LOG(INFO) << "cont\t" << b;
        ASSERT_EQ(b, 2009);
    };


    transfer.message_class().add(1, std::move(msgcls1));

    Request request;
    request.meta.message_class = 1;
    request.call_back_handler = std::move(msgrsp1);
    request.cont << 2008;

    transfer.set_client_id(1);
    transfer.send(std::move(request), 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

}

};  // end namespace _test_one_transfer

