cc_library(
  name = "common",
  deps = [
    "//thirdparty/glog:glog",
    "//thirdparty/gflags:gflags",
  ],
  linkopts = ["-pthread"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "test",
  deps = [
    "//thirdparty/gtest:gtest",
    "//thirdparty/gtest:gtest_main",
  ],
  linkopts = ["-pthread"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "mock",
  deps = [
    "//thirdparty/gmock:gmock",
    "//thirdparty/gmock:gmock_main",
  ],
  linkopts = ["-pthread"],
  visibility = ["//visibility:public"],
)


cc_import(
    name = "grpc",
    static_library = "lib64/libgrpc.a",
    visibility = ["//visibility:public"],
)

cc_import(
    name = "grpc++",
    static_library = "lib64/libgrpc++.a",
    visibility = ["//visibility:public"],
)