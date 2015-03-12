#include <climits>
#include "../../../core/transfer/Route.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

class Route : public BaseRoute {
    void update() {
    }
};


class RouteTest : public testing::Test {
protected:
    // add local address
    virtual void SetUp() {
        int port = 80806;
        std::string addr;
        std::string ip = get_local_ip();
        format_string(addr, "tcp://%s:%d",  ip.c_str(), port);

        route.register_node(1, std::move(addr));
    }

    virtual void TearDown() {
    }

protected:
    Route route;
};


TEST_F (RouteTest, register_node) {
    route.register_node(2, "tcp://127.0.0.1:80881");
}

TEST_F (RouteTest, delete_node) {
    route.delete_node(1);
}

