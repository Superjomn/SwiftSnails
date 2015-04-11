#pragma once
#include "../../../utils/all.h"

namespace swift_snails {

void server_init_configs() {
    /*
     * frag_num: number of fragments
     */
    std::string configs = 
        " \
        listen_addr \
        async_exec_num \
        shard_num \
        listen_thread_num \
        master_addr \
        init_timeout \
        frag_num \
        param_backup_period \
        param_backup_root \
        ";
    init_configs(configs);
}



};  // end namespace swift_snails
