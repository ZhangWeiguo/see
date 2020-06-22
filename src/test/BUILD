cc_binary(
  name = "test_bazel",
  srcs = ["test_bazel.cc"],
)

cc_binary(
  name = "test",
  srcs = ["test.cc"],
  deps = [
      "//thirdparty:common",
    ]
)

cc_test(
  name = "test_test1",
  srcs = ["test_test1.cc"],
  deps = [
      "@gtest//:gtest_main",
      "@gtest//:gtest",
    ]
)

cc_test(
  name = "test_test2",
  srcs = ["test_test2.cc"],
  deps = [
      "@gtest//:gtest_main",
      "@gtest//:gtest",
    ]
)

cc_binary(
  name = "test_proto",
  srcs = ["test_proto.cc"],
  deps = [
      "//test/proto:test_proto",
      "@com_github_grpc_grpc//:grpc++",
    ]
)