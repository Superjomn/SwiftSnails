#pragma once
#include "../../../utils/all.h"

namespace swift_snails {

void worker_init_configs() {
    std::string configs = 
        " \
        listen_addr \
        async_exec_num \
        listen_thread_num \
        master_addr \
        init_timeout \
        ";
    init_configs(configs);
}



};  // end namespace swift_snails
