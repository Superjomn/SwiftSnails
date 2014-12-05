#include <iostream>
#include <vector>
#include "../../utils/string.h"
using namespace swift_snails;
using namespace std;

void test_trim()
{
    LOG(INFO) << "<test_trim>";
    string a = "\thello world \n";  
    trim(a);
    CHECK(a == "hello world");
    LOG(INFO) << "a now is \t'" << a << "'";
}

void test_split()
{
    LOG(INFO) << "<test_split>";
    string a = "key1:value1 key2:value2 key3:value3";
    vector<string> vs = split(a, " ");
    CHECK(vs.size() == 3);
    cout << "origin text:\t" << a << endl;
    for(auto part : vs) {
        LOG(INFO) << "part:\t'" << part<<"'";
    }
    CHECK(vs[0] == "key1:value1");
    CHECK(vs[1] == "key2:value2");
    CHECK(vs[2] == "key3:value3");
}

void test_key_value_split()
{
    LOG(INFO) << "<test_key_value_split>";
    string a = "key1:value1";
    pair<string, string> kv = std::move(key_value_split(a, ":"));
    LOG(INFO) << "origin text:\t" << a;
    LOG(INFO) << "key:\t'" << kv.first << "'\tvalue:\t'" << kv.second << "'";
}

void test_headswith()
{
    LOG(INFO) << "<test_headswith>";
    string a = "# comment";
    trim(a);
    CHECK(headswith(a, "#") == true);
    CHECK(headswith(a, "df") == false);
    string b = " import ../../path";
    trim(b);
    CHECK(headswith(b, "import"));
    pair<string, string> kv = std::move(key_value_split(b, " "));
    CHECK(kv.second == "../../path");
}

int main()
{
    test_trim();
    test_split();
    test_key_value_split();
    test_headswith();

    return 0;
}
