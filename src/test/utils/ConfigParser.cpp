#include <iostream>
#include "../../utils/ConfigParser.h"
using namespace std;
using namespace swift_snails;

string conf_path {"1.conf"};

void test_conf() {
    ConfigParser cp(conf_path);
    cp.register_config("ROOT");
    cp.register_config("name");
    cp.register_config("age");
    cp.parse();

    cout << cp << endl;
    string name = std::move(cp.get_config("name").to_string());
    int age = std::move(cp.get_config("age").to_int32());
    CHECK(name == "superjom");
    CHECK(age == 25);
}

int main()
{
    test_conf();
    return 0;
}
