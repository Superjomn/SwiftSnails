#include <climits>
#include "../../../core/transfer/Sender.h"
#include "../../../core/transfer/ListenService.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

TEST (Sender, construct) {
    class Route : public BaseRoute {
        void update() {
        }
    };

    Route route;

    Sender sender(route);
}


TEST (Sender, client) {
    class Route : public BaseRoute {
        void update() {
        }
    };

    string ip = get_local_ip();
    LOG(INFO) << "get local ip:\t" << ip;

    Route route;

    Sender sender(route);
    sender.set_recv_ip(ip);
    sender.listen();
}

TEST (Sender, ListenService) {
    class Route : public BaseRoute {
        void update() {
        }
    };

    string ip = get_local_ip();
    LOG(INFO) << "get local ip:\t" << ip;

    Route route;

    auto sender = std::make_shared<Sender>(route);
    sender->set_recv_ip(ip);
    sender->listen();

    ListenService listen_service(sender, 4);
    LOG(WARNING) << "sender start service";
    listen_service.start();

    LOG(WARNING) << "sender end service";
    listen_service.end();
}
