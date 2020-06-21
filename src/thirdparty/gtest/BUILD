cc_library(
    name = "gtest",
    srcs = ["lib64/libgtest.a", "lib64/libgtest_main.a"],
    hdrs = glob([
            "internal/*.h",
            "*.h"
        ]),
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "gmock",
    srcs = ["lib64/libmock.a", "lib64/libmock_main.a"],
    hdrs = glob([
            "internal/*.h",
            "*.h"
        ]),
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
)
