//
//  shell.h
//  SwiftSnails
//
//  Created by Chunwei on 12/25/14.
//  Copyright (c) 2014 Chunwei. All rights reserved.
//
#ifndef SwiftSnails_utils_shell_h_
#define SwiftSnails_utils_shell_h_
#include "common.h"
#include "string.h"
namespace swift_snails {

class GlobalShell : public VirtualObject {
public:
    shared_ptr<FILE> make_pipe(const char* cmd, const char* mode) {
        if(_verbose) 
            LOG(INFO) << "Making pipe [ " << cmd << " ] with mode [ " << mode << " ]";
        shared_ptr<ManagedPipe> pipe = std::make_shared<ManagedPipe>();
        pipe->cmd = cmd;
        pipe->verbose = _verbose;
        PCHECK(pipe->fp = 
            guarded_popen(
                format_string("set -o pipefail; %s", cmd).c_str(), mode))
            << "command=[ " << cmd << " ] mode=[ " << mode << " ]";
        return shared_ptr<FILE>(pipe, pipe->fp);
    }

    void execute(const char* cmd) {
        make_pipe(cmd, "w");
    }

    std::string get_command_output(const char* cmd) {
        shared_ptr<FILE> pipe = make_pipe(cmd, "r");
        fseek(pipe, 0, SEEK_END); 
        int len = ftell(pipe);  
        rewind(pipe);
        shared_ptr<char> content = new char[len+1];
        len = fread(content, sizeof(char), len, pipe);
        content[len] = '\0'; 
        return std::move(std::string(content));
    }

private:
    struct ManagedPipe {
        FILE* fp = NULL;
        std::string cmd;
        bool verbose = false;
        ~ManagedPipe() {
            if (fp) {
                if (verbose) LOG(INFO) << "Closing pipe [ " << cmd << " ]";
                int err = guarded_pclose(fp);
                PCHECK(err == 0 || err == (128 + SIGPIPE) * 256 || 
                        err == -1 && errno == ECHILD)
                    << "err=" << err << " command=[ " << cmd << " ]";
                if(errno = ECHILD) LOG(WARNING) << "errno is ECHILD";
            }
        }
    };


}; // class GlobalShell


}; // end namespace swift_snails
#endif


