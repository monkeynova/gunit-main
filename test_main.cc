#include "absl/flags/flag.h"
#include "absl/log/check.h"
#include "absl/strings/str_join.h"
#include "benchmark/benchmark.h"
#include "fuzztest/init_fuzztest.h"
#include "gtest/gtest.h"
#include "main_lib.h"

ABSL_FLAG(bool, benchmark, false,
          "If true, runs benchmarks rather than gunit test suite.");

ABSL_FLAG(bool, run_fuzz_tests, true,
          "If true (the default), includes any fuzz tests in the gunit test "
          "suite.");

int main(int argc, char** argv) {
  std::vector<char*> args = InitMain(
    argc, argv,
    absl::StrCat("This program runs the collection of gunit tests (default) "
                 "or benchmarks linked in (if the --benchmark flag is set). "
                 "No arguments are accepted. Usage:\n", argv[0],
                 " [--benchmark]"));
  CHECK_EQ(args.size(), 1) << absl::StrJoin(args, ",");

  if (absl::GetFlag(FLAGS_benchmark)) {
    benchmark::RunSpecifiedBenchmarks();
    return 0;
  }
  if (absl::GetFlag(FLAGS_run_fuzz_tests)) {
    fuzztest::InitFuzzTest(&argc, &argv);
  }

  return RUN_ALL_TESTS();
}
