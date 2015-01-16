#include "Master.h"
using namespace swift_snails;

int main()
{
    local_client();
    init_client();
    LOG(INFO) << "waiting ...";
    const int wait_time = 5000;
    std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
    client_send_IP();
    std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
    local_client().finalize();
    return 0;
}


