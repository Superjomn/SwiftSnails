#pragma once
#include "../../../utils/all.h"

namespace swift_snails {

void master_init_configs() {
    std::string configs = 
        " \
        expected_node_num \
        master_time_out \
        listen_addr \
        async_exec_num \
        listen_thread_num \
        ";

    init_configs(configs);
}


};  // end namespace swift_snails
