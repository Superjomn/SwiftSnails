#include <iostream>
#include "../../utils/all.h"
#include "../../core/system/master/init.h"
#include "../../core/system/master/init_config.h"
#include "../../core/system/master/terminate.h"
using namespace swift_snails;
using namespace std;
using namespace fms;

int main(int argc, char* argv[]) {
    // init config
    CMDLine cmdline(argc, argv);
    string param_config_path = cmdline.registerParameter("config", "path of config file");
    string param_help = cmdline.registerParameter("help", "this screen");
    // parse parameters
    if(cmdline.hasParameter(param_help) || argc == 1) {
        cout << endl;
        cout << "===================================================================" << endl;
        cout << "master server main program";
        cout << "===================================================================" << endl;
        cmdline.print_help();
        cout << endl;
        cout << endl;
        return 0;
    }
    if(!cmdline.hasParameter(param_config_path)) {
        LOG(ERROR) << "missing parameter: config";
        return 0;
    }
    std::string config_path = cmdline.getValue(param_config_path);

    master_init_configs();

    global_config().load_conf(config_path);

    global_config().parse();

    MasterTransferInit master_transfer_init;

    master_transfer_init();

    MasterTerminate terminate;
    terminate.set_worker_num(master_transfer_init.worker_num());
    terminate.set_server_num(master_transfer_init.server_num());
    terminate();


    return 0;
}
