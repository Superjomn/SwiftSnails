#include "Master.h"
using namespace swift_snails;

int main()
{
    const int wait_time = 50000;
    init_server();
    server_register_message_class();
    std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
    local_server().finalize();
    return 0;
}
