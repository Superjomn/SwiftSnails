#include "../../utils/all.h"
#include "../system/master/init.h"
#include "../system/master/init_config.h"
#include "../system/master/terminate.h"

namespace swift_snails {

class SwiftMaster {
public:
    explicit SwiftMaster(const std::string& config_path) {
        // init configs
        master_init_configs();
        global_config().load_conf(config_path);
        global_config().parse();
    }

    void operator() () {
        master_transfer_init();
        terminate.set_worker_num(master_transfer_init.worker_num());
        terminate.set_server_num(master_transfer_init.server_num());
        terminate();
    }

private:
    MasterTransferInit master_transfer_init;
    MasterTerminate terminate;
};

};  // end namespace swift_snails

