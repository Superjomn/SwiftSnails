#pragma once
#include "../../../utils/all.h"

namespace swift_snails {

void master_init_configs() {
    /*
     * master_max_alive_duration:
     *  when the cluster finish work then
     *  master will quit
     *  else master will stay alive during the longest duration
     */
    std::string configs = 
        " \
        expected_node_num \
        master_time_out \
        listen_addr \
        async_exec_num \
        listen_thread_num \
        master_longest_alive_duration \
        frag_num \
        ";

    init_configs(configs);
}


};  // end namespace swift_snails
