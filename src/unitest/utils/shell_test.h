#include <climits>
#include "../../utils/shell.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;


TEST(shell, init) {
    GlobalShell shell;
    std::string output = shell.get_command_output("ls");
    LOG(INFO) << "CMD: ls\t" << output;
}

TEST(shell, call_python) {
    GlobalShell shell;
    string output = shell.get_command_output("python -c 'print \"hello world\"'");
    LOG(INFO) << "python -c 'print \"hello world\"" ;
    LOG(INFO) << output;
}


