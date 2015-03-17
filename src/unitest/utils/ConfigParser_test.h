#include <climits>
#include "../../utils/ConfigParser.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

TEST(ConfigParser, init) {
    ConfigParser config;
}

TEST(ConfigParser, parsefile) {
    ConfigParser config("1.conf");
    config.register_config("ip");
    config.register_config("thread_num");
    config.parse();

    ASSERT_EQ(config.get_config("ip").to_string(), "tcp://127.0.0.1:8080");
    ASSERT_EQ(config.get_config("thread_num").to_int32(), 12);
}
