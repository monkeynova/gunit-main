#include "main_lib.h"

#include "absl/debugging/failure_signal_handler.h"
#include "absl/debugging/symbolize.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/flags/usage_config.h"
#include "absl/log/check.h"
#include "absl/log/flags.h"
#include "absl/log/initialize.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_split.h"
#include "benchmark/benchmark.h"
#include "gtest/gtest.h"
#include "vlog.h"

namespace {

class SubArgv {
 public:
  SubArgv() = default;

  // Neither copyable nor movable (pointers into structures).
  SubArgv(const SubArgv&) = delete;
  SubArgv& operator=(const SubArgv&) = delete;

  void SetArgv0(absl::string_view argv0);
  void AddFlag(std::string sub_argv);

  int argc() { return argc_; }
  int* argc_ptr() { return &argc_; }
  char** argv() const { return argv_; }

 private:
  std::vector<std::string> args_;
  std::vector<char*> arg_cstr_;
  int argc_ = 0;
  char** argv_ = nullptr;
};

void SubArgv::SetArgv0(absl::string_view argv0) {
  argc_ = args_.size() + 1;
  arg_cstr_ = {const_cast<char*>(argv0.data())};
  for (absl::string_view arg : args_) {
    arg_cstr_.push_back(const_cast<char*>(arg.data()));
  }
  arg_cstr_.push_back(nullptr);
  argv_ = const_cast<char**>(arg_cstr_.data());
}

void SubArgv::AddFlag(std::string sub_argv) {
  if (sub_argv.empty()) return;

  std::vector<std::string> split = absl::StrSplit(sub_argv, ",");
  args_.insert(args_.end(), split.begin(), split.end());
}

SubArgv benchmark_argv;
SubArgv gtest_argv;

}

ABSL_FLAG(std::string, benchmark_flags, "", 
          "Flags to pass to the google benchmark library. Multiple values may "
          "be specified in a single entry with comma separation, or by "
          "through multiple values on the command line.").OnUpdate([] {
  benchmark_argv.AddFlag(absl::GetFlag(FLAGS_benchmark_flags));
});

ABSL_FLAG(std::string, gtest_flags, "",
          "Flags to pass to the google test library. Multiple values may be "
          "specified in a single entry with comma separation, or by through "
          "multiple values on the command line.").OnUpdate([] {
  gtest_argv.AddFlag(absl::GetFlag(FLAGS_gtest_flags));
});

std::vector<char*> InitMain(int argc, char** argv, std::string_view usage) {
  absl::SetProgramUsageMessage(usage);
  absl::SetFlagsUsageConfig({
    .contains_help_flags = [](std::string_view pkg) {
      if (absl::StartsWith(pkg, "external/")) {
        // By default don't show flags from external dependencies.
        if (absl::StartsWith(pkg, "external/gunit-main")) {
          // But do show them from this module.
          return true;
        }
        return false;
      }
      return true;
    }
  });
  absl::InitializeSymbolizer(argv[0]);
  absl::InstallFailureSignalHandler(/*options=*/{});
  absl::InitializeLog();

  std::vector<char*> args = absl::ParseCommandLine(argc, argv);

  VLOG(3) << "Init Main";

  {
    benchmark_argv.SetArgv0(argv[0]);
    benchmark::Initialize(benchmark_argv.argc_ptr(), benchmark_argv.argv());
    CHECK_EQ(benchmark_argv.argc(), 1) << benchmark_argv.argv()[1];
  }

  {
    gtest_argv.SetArgv0(argv[0]);
    testing::InitGoogleTest(gtest_argv.argc_ptr(), gtest_argv.argv());
    CHECK_GT(gtest_argv.argc(), 0);
    CHECK_LT(gtest_argv.argc(), 3);
    if (gtest_argv.argc() == 2) {
      // The --help flag is (very explicitly and specially) not consumed by
      // googletest during flag parsing
      CHECK_EQ(gtest_argv.argv()[1], std::string_view("--help"));
    }
  }

  return args;
}
