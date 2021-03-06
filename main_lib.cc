#include "absl/flags/parse.h"
#include "absl/strings/str_join.h"
#include "absl/strings/strip.h"
#include "benchmark/benchmark.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "gtest/gtest.h"
#include "gtest/internal/gtest-port.h"

// Terrible hack to try make gflags and absl sort of play nice.
// TODO(@monkeynova): Rip this out when absl::log obviates the need for
// gflags.
static void StripGflags(int* argc, char*** argv) {
  for (int i = 1; i < *argc; ++i) {
    absl::string_view this_arg = (*argv)[i];
    int argv_to_remove = 0;
    gflags::CommandLineFlagInfo flag_info;
    if (absl::ConsumePrefix(&this_arg, "--")) {
      std::string this_flag(this_arg.data(), this_arg.length());
      size_t flag_end = this_arg.find("=");
      if (flag_end != this_arg.npos) {
        this_flag.resize(flag_end);
        // Single argv flag+value.
        if (gflags::GetCommandLineFlagInfo(this_flag.c_str(), &flag_info)) {
          argv_to_remove = 1;
        }
      } else {
        // Maybe pair flag+value.
        if (gflags::GetCommandLineFlagInfo(this_flag.c_str(), &flag_info)) {
          argv_to_remove = flag_info.type == "bool" ? 1 : 2;
        }
      }
    }
    if (argv_to_remove > 0) {
      for (int j = i; j < *argc - argv_to_remove; ++j) {
        (*argv)[j] = (*argv)[j + argv_to_remove];
      }
      *argc -= argv_to_remove;
    }
  }
}

void InitializeAbslFlagsFromGtest() {
  std::vector<std::string> string_argvs = testing::internal::GetArgvs();
  std::vector<const char*> raw_argvs(string_argvs.size());
  for (int i = 0; i < string_argvs.size(); ++i) {
    raw_argvs[i] = string_argvs[i].c_str();
  }
  int argc = raw_argvs.size();
  char** argv = const_cast<char**>(raw_argvs.data());
  gflags::AllowCommandLineReparsing();
  gflags::ParseCommandLineFlags(&argc, &argv, /*remove_flags=*/false);
  StripGflags(&argc, &argv);
  // benchmark and gflags both want --v=. Give priority to gflags since that's
  // the one in glog and we want VLOG(#) to work in puzzle code.
  benchmark::Initialize(&argc, argv);
  absl::ParseCommandLine(argc, argv);
}

std::vector<char*> InitMain(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  testing::InitGoogleTest(&argc, argv);
  gflags::AllowCommandLineReparsing();
  gflags::ParseCommandLineFlags(&argc, &argv, /*remove_flags=*/false);
  StripGflags(&argc, &argv);
  // benchmark and gflags both want --v=. Give priority to gflags since that's
  // the one in glog and we want VLOG(#) to work in puzzle code.
  benchmark::Initialize(&argc, argv);
  return absl::ParseCommandLine(argc, argv);
}
