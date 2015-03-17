#include <climits>
#include "../../utils/string.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

TEST(_string, trim) {
    string a = "hello world\t \t\r";
    a = trim(a);
    ASSERT_EQ(a, string("hello world"));
}

TEST(_string, split) {
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
}

TEST(_string, format_string) {
    string s = "hello %s, your age is %d";
    string a;

    format_string(a, s.c_str(), "Superjom", 25);

    ASSERT_EQ(a, string("hello Superjom, your age is 25"));

}
