#include <climits>
#include "../../core/common.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

TEST(core_common, zmq_bind_random_port) {

    void* _zmq_ctx = zmq_ctx_new();
    void*       _receiver = NULL;
    PCHECK(_receiver = zmq_socket(_zmq_ctx, ZMQ_PULL));
    string ip = get_local_ip();
    string addr;
    int port;
    zmq_bind_random_port(ip, _receiver, addr, port);
    PCHECK(0 == zmq_close(_receiver));
    PCHECK(0 == zmq_ctx_destroy(_zmq_ctx));
}

TEST(core_common, IP) {
    IP ip;

    ip.from_string("127.0.0.1");
    ip.port = 8080;

    ASSERT_EQ(ip.to_string(), "127.0.0.1:8080");

    ip.from_string("127.0.0.1:8080");
    ASSERT_EQ(ip.to_string(), "127.0.0.1:8080");
}
