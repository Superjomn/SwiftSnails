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

std::shared_ptr<AsynExec::channel_t> global_channel() {
    int thread_num = 4;
    static AsynExec as(thread_num);
    static std::shared_ptr<AsynExec::channel_t> global_channel = as.open();
    return global_channel;
}

class TransferTest : public testing::Test {
protected:
    virtual void SetUp() {
        transfer.set_listen_service_num(3);
        transfer.set_async_channel(global_channel());
        transfer.start_sender_service();
        transfer.start_receiver_service();
    }

    virtual void TearDown() {
    }

protected:
    Transfer<Route> transfer;

};  // end class TransferTest


TEST_F (TransferTest, init) {
}

};  // end namespace _test_one_transfer

