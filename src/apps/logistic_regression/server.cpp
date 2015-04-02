#include <iostream>
#include "../../utils/all.h"
#include "../../core/system/node_init.h"
#include "../../core/system/server/init.h"
#include "../../core/system/server/init_config.h"
#include "../../core/system/server/terminate.h"
#include "./access_method.h"
using namespace swift_snails;
using namespace logistic_regression;
using namespace std;
using namespace fms;

int main(int argc, char* argv[]) {
    typedef index_t key_t;
    typedef float val_t;
    typedef float grad_t;
    // init config
    CMDLine cmdline(argc, argv);
    string param_config_path = cmdline.registerParameter("config", "path of config file");
    string param_help = cmdline.registerParameter("help", "this screen");
    // parse parameters
    if(cmdline.hasParameter(param_help) || argc == 1) {
        cout << endl;
        cout << "===================================================================" << endl;
        cout << "server main program" << endl;
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

    server_init_configs();

    global_config().load_conf(config_path);
    global_config().parse();

    NodeTransferInit node_transfer_init;
    NodeHashfragInit node_hashfrag_init;

    node_transfer_init(true);
    node_hashfrag_init();

    ServerInitPullMethod<key_t, val_t, grad_t, LogisticPullMethod> init_pull_method;
    ServerInitPushMethod<key_t, val_t, grad_t, LogisticPushMethod> init_push_method;

    init_pull_method();
    init_push_method();

    ServerTerminate<key_t, val_t> terminate;
    terminate();
    
    return 0;
};
