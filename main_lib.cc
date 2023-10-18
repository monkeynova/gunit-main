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

class SubArgv {
 public:
  SubArgv() = default;

  // Neither copyable nor movable (pointers into structures).
  SubArgv(const SubArgv&) = delete;
  SubArgv& operator=(const SubArgv&) = delete;

  void ParseSubArgv(std::string sub_argv, absl::string_view argv0);

  int argc() { return argc_; }
  int* argc_ptr() { return &argc_; }
  char** argv() const { return argv_; }

 private:
  std::vector<std::string> args_;
  std::vector<char*> arg_cstr_;
  int argc_ = 0;
  char** argv_ = nullptr;
};

void SubArgv::ParseSubArgv(std::string sub_argv, absl::string_view argv0) {
  args_ = sub_argv.empty() ? std::vector<std::string>{}
                           : absl::StrSplit(sub_argv, ",");

  argc_ = args_.size() + 1;
  arg_cstr_ = {const_cast<char*>(argv0.data())};
  for (absl::string_view arg : args_) {
    arg_cstr_.push_back(const_cast<char*>(arg.data()));
  }
  arg_cstr_.push_back(nullptr);
  argv_ = const_cast<char**>(arg_cstr_.data());
}

std::vector<char*> InitMain(int argc, char** argv) {
  absl::InitializeSymbolizer(argv[0]);
  absl::InstallFailureSignalHandler(/*options=*/{});
  absl::InitializeLog();

  std::vector<char*> args = absl::ParseCommandLine(argc, argv);

  {
    SubArgv benchmark_argv;
    benchmark_argv.ParseSubArgv(absl::GetFlag(FLAGS_benchmark_flags), argv[0]);
    benchmark::Initialize(benchmark_argv.argc_ptr(), benchmark_argv.argv());
    CHECK_EQ(benchmark_argv.argc(), 1);
  }

  {
    SubArgv gtest_argv;
    gtest_argv.ParseSubArgv(absl::GetFlag(FLAGS_gtest_flags), argv[0]);
    testing::InitGoogleTest(gtest_argv.argc_ptr(), gtest_argv.argv());
    CHECK_EQ(gtest_argv.argc(), 1);
  }

  return args;
}
