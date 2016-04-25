#pragma once

#include "common.h"
#include "string.h"
namespace swift_snails {

class GlobalShell : public VirtualObject {
public:
  bool verbose() { return _verbose; }
  void set_verbose(bool x) { _verbose = x; }
  shared_ptr<FILE> make_pipe(const char *cmd, const char *mode) {
    if (_verbose) {
      LOG(INFO) << "Making pipe [ " << cmd << " ] with mode [ " << mode << " ]";
    }
    shared_ptr<ManagedPipe> pipe;
    pipe = std::make_shared<ManagedPipe>();
    pipe->cmd = cmd;
    pipe->verbose = _verbose;
    PCHECK(pipe->fp = guarded_popen(
               format_string("set -o pipefail; %s", cmd).c_str(), mode))
        << "command=[ " << cmd << " ] mode=[ " << mode << " ]";
    return shared_ptr<FILE>(pipe, pipe->fp);
  }
  void execute(const char *cmd) { make_pipe(cmd, "w"); }
  std::string get_command_output(const char *cmd) {
    shared_ptr<FILE> pipe = make_pipe(cmd, "r");
    LineFileReader reader;
    if (reader.getdelim(&*pipe, 0)) {
      return reader.get();
    }
    return "";
  }

private:
  struct ManagedPipe {
    FILE *fp = NULL;
    std::string cmd;
    bool verbose = false;
    ~ManagedPipe() {
      if (fp) {
        if (verbose) {
          LOG(INFO) << "Closing pipe [ " << cmd << " ]";
        }
        int err = guarded_pclose(fp);
        ;
        PCHECK(err == 0 || err == (128 + SIGPIPE) * 256 ||
               err == -1 && errno == ECHILD)
            << "err=" << err << " command=[ " << cmd << " ]";
        if (errno == ECHILD) {
          LOG(WARNING) << "errno is ECHILD";
        }
      }
    }
  };

  bool _verbose = false;
};

inline GlobalShell &global_shell() {
  static GlobalShell shell;
  return shell;
}

}; // end namespace swift_snails
