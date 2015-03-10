#include <climits>
#include "../../../core/transfer/Sender.h"
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
