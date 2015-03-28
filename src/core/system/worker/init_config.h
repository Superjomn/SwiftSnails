#pragma once
#include "../../../utils/all.h"

namespace swift_snails {

void worker_init_configs() {
    /*
     * push_period: push every N times
     * pull_period: pull  every N times
     * data_path: path of the dataset
     * async_channel_thread_num: number of threads in the async channel
     */
    std::string configs = 
        " \
        listen_addr \
        async_exec_num \
        listen_thread_num \
        master_addr \
        init_timeout \
        push_period \
        pull_period \
        data_path \
        async_channel_thread_num \
        ";
    init_configs(configs);
}

void server_init_configs() {
    /*
     * frag_num: number of fragments
     */
    std::string configs = 
        " \
        listen_addr \
        async_exec_num \
        listen_thread_num \
        master_addr \
        init_timeout \
        frag_num \
        ";
    init_configs(configs);
}



};  // end namespace swift_snails
