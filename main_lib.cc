#include "main_lib.h"

#include "absl/debugging/failure_signal_handler.h"
#include "absl/debugging/symbolize.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/log/check.h"
#include "absl/log/flags.h"
#include "absl/log/initialize.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_split.h"
#include "benchmark/benchmark.h"
#include "gtest/gtest.h"

ABSL_FLAG(std::string, benchmark_flags, "", "...");
ABSL_FLAG(std::string, gtest_flags, "", "...");

struct SubArgv {
  SubArgv() = default;

  // Neither copyable nor movable (pointers into structures).
  // Which, of course, means this isn't POD and shouldn't be a struct;
  SubArgv(const SubArgv&) = delete;
  SubArgv& operator=(const SubArgv&) = delete;

  std::vector<std::string> args;
  std::vector<char*> arg_cstr;
  int argc = 0;
  char** argv = nullptr;
};

void ParseSubArgv(std::string sub_argv, absl::string_view argv0, SubArgv* ret) {
  ret->args = sub_argv.empty() ? std::vector<std::string>{}
                               : absl::StrSplit(sub_argv, ",");

  ret->argc = ret->args.size() + 1;
  ret->arg_cstr = {const_cast<char*>(argv0.data())};
  for (absl::string_view arg : ret->args) {
    ret->arg_cstr.push_back(const_cast<char*>(arg.data()));
  }
  ret->argv = const_cast<char**>(ret->arg_cstr.data());
}

std::vector<char*> InitMain(int argc, char** argv) {
  absl::InitializeSymbolizer(argv[0]);
  absl::InstallFailureSignalHandler(/*options=*/{});
  absl::InitializeLog();

  std::vector<char*> args = absl::ParseCommandLine(argc, argv);

  {
    SubArgv benchmark_argv;
    ParseSubArgv(absl::GetFlag(FLAGS_benchmark_flags), argv[0],
                 &benchmark_argv);
    benchmark::Initialize(&benchmark_argv.argc, benchmark_argv.argv);
    CHECK_EQ(benchmark_argv.argc, 1);
  }

  {
    SubArgv gtest_argv;
    ParseSubArgv(absl::GetFlag(FLAGS_gtest_flags), argv[0], &gtest_argv);
    testing::InitGoogleTest(&gtest_argv.argc, gtest_argv.argv);
    CHECK_EQ(gtest_argv.argc, 1);
  }

  return args;
}
