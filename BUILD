load("@com_monkeynova_base_workspace//:default_rules.bzl", "default_rules")

default_rules(workspace_dep = "update_workspace.date")

cc_library(
    name = "main_lib",
    srcs = ["main_lib.cc"],
    hdrs = ["main_lib.h"],
    visibility = [
        "//visibility:public",
    ],
    deps = [
        ":vlog",
        "@google_benchmark//:benchmark",
        "@abseil-cpp//absl/debugging:failure_signal_handler",
        "@abseil-cpp//absl/debugging:symbolize",
        "@abseil-cpp//absl/flags:flag",
        "@abseil-cpp//absl/flags:parse",
        "@abseil-cpp//absl/log",
        "@abseil-cpp//absl/log:check",
        "@abseil-cpp//absl/log:flags",
        "@abseil-cpp//absl/log:initialize",
        "@abseil-cpp//absl/strings",
        "@googletest//:gtest",
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
        "@abseil-cpp//absl/flags:flag",
        "@abseil-cpp//absl/log:check",
        "@abseil-cpp//absl/strings",
        "@fuzztest//fuzztest:init_fuzztest",
        "@googletest//:gtest",
    ],
)

cc_test(
    name = "meta_test",
    srcs = ["meta_test.cc"],
    deps = [
        ":test_main",
        "@fuzztest//fuzztest",
        "@googletest//:gtest",
    ],
)

cc_library(
    name = "vlog",
    hdrs = ["vlog.h"],
    visibility = ["//visibility:public"],
    deps = [
        "@abseil-cpp//absl/flags:flag",
        "@abseil-cpp//absl/log",
    ],
    alwayslink = 1,
)
