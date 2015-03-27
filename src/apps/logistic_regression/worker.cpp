#include <iostream>
#include "../../utils/CMDLine.h"
#include "../../core/system/node_init.h"
#include "../../core/system/worker/init_config.h"
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

    worker_init_configs();

    global_config().load_conf(config_path);
    global_config().parse();

    NodeTransferInit node_transfer_init;
    NodeHashfragInit node_hashfrag_init;

    node_transfer_init(false);
    node_hashfrag_init();
    
    return 0;
};
