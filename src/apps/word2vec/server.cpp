#include "../../core/framework/SwiftServer.h"
#include "access_method.h"
using namespace swift_snails;
using namespace std;
using namespace fms;


int main(int argc, char* argv[]) {
    typedef uint32_t key_t;
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

    /*
    server_init_configs();
    global_config().register_config("len_vec");
    global_config().register_config("learning_rate");
    global_config().register_config("window");
    global_config().register_config("negative");
    */

    global_config().load_conf(config_path);
    global_config().parse();

    SwiftServer<key_t, Word2VecParam, Word2VecGrad, Word2VecPullMethod, Word2VecPushMethod> server(config_path);

    server();

    return 0;
}
