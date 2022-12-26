alias(
    name = "cleanup",
    actual = "@com_monkeynova_base_workspace//:cleanup",
)

alias(
    name = "update_workspace",
    actual = "@com_monkeynova_base_workspace//:update_workspace",
)

sh_test(
    name = "diff_bazelrc_test",
    srcs = ["diff_bazelrc_test.sh"],
    data = [
        "base-bazelrc",
        "@com_monkeynova_base_workspace//:base-bazelrc-file",
    ],
)

cc_library(
    name = "main_lib",
    srcs = ["main_lib.cc"],
    hdrs = ["main_lib.h"],
    visibility = [
        "//visibility:public",
    ],
    deps = [
        "@com_github_google_benchmark//:benchmark",
        "@com_google_absl//absl/debugging:failure_signal_handler",
        "@com_google_absl//absl/debugging:symbolize",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/log",
        "@com_google_absl//absl/log:check",
        "@com_google_absl//absl/log:flags",
        "@com_google_absl//absl/log:initialize",
        "@com_google_absl//absl/strings",
        "@com_google_googletest//:gtest",
    ],
)

cc_library(
    name = "test_main",
    testonly = 1,
    srcs = ["test_main.cc"],
    visibility = [
        "//visibility:public",
    ],
    deps = [
        ":main_lib",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/log:check",
        "@com_google_absl//absl/strings",
        "@com_google_googletest//:gtest",
    ],
)
