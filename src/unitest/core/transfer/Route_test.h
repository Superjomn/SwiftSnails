#include <climits>
#include "../../../core/transfer/Route.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

class Route : public BaseRoute {
    void update() {
    }
};

TEST (Route, call) {
    Route route;
}

