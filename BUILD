cc_library(
    name = "main_lib",
    hdrs = ["main_lib.h"],
    srcs = ["main_lib.cc"],
    deps = [
	"@com_google_absl//absl/debugging:failure_signal_handler",
	"@com_google_absl//absl/debugging:symbolize",
	"@com_google_absl//absl/flags:flag",
	"@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/strings",
        "@com_google_benchmark//:benchmark",
	"@com_google_glog//:glog",
        "@com_google_googletest//:gtest",
    ],
    visibility = [
        "//visibility:public",
    ]
)

cc_library(
    name = "test_main",
    srcs = ["test_main.cc"],
    testonly = 1,
    deps = [
        ":main_lib",
	"@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/strings",
        "@com_google_googletest//:gtest",
    ],
    visibility = [
        "//visibility:public",
    ]
)

