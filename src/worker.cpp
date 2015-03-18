#include <iostream>
#include "utils/CMDLine.h"
#include "core/transfer_framework/worker/init.h"
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

    //global_config().register_config("listen_addr", "");
    global_config().register_config("master_addr");
    global_config().load_conf(config_path);
    global_config().parse();

    NodeTransferInit init;
    init();
    
    return 0;
};
