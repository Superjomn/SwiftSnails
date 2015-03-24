#include <climits>
#include "../../utils/string.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

TEST(string, trim) {
    string a = "hello world\t \t\r";
    a = trim(a);
    ASSERT_EQ(a, string("hello world"));
}

TEST(string, split) {
    string a = "hello world";
    vector<string> cols = split(a, " ");
    ASSERT_EQ(cols.size(), 2);
    ASSERT_EQ(cols[0], "hello");
    ASSERT_EQ(cols[1], "world");

    // complex split
    string b = "hello world@bb";
    vector<string> cols1 = split(b, "@ ");

    LOG(INFO) << "b cols:";
    for(auto &c : cols1) {
        LOG(INFO) << "'" << c <<"'";
    }
    ASSERT_EQ(cols1.size(), 3);
    ASSERT_EQ(cols1[0], "hello");
    ASSERT_EQ(cols1[1], "world");
    ASSERT_EQ(cols1[2], "bb");

    string c = "hello \
    world \
    yes go";

    vector<string> cols2 = split(c, "\n ");

    ASSERT_EQ(cols2[0], "hello");
    ASSERT_EQ(cols2[1], "world");
    ASSERT_EQ(cols2[2], "yes");
    ASSERT_EQ(cols2[3], "go");
}

TEST(string, format_string) {
    string s = "hello %s, your age is %d";
    string a;

    format_string(a, s.c_str(), "Superjom", 25);

    ASSERT_EQ(a, string("hello Superjom, your age is 25"));

}

TEST(string, headswith) {
    ASSERT_TRUE( headswith("tcp://127.0.0.1:8080", "tcp://"));
}

TEST(string, LineFileReader_get_lines) {
    FILE* f = fopen("1.txt", "r");
    LineFileReader file; 

    while(file.getline(f)) {
        LOG(INFO) << "line:\t" << file.get();
    }
}
