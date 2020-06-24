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
      "//thirdparty:test",
    ]
)

cc_test(
  name = "test_test2",
  srcs = ["test_test2.cc"],
  deps = [
      "//thirdparty:test",
    ]
)

cc_binary(
  name = "test_grpc_server",
  srcs = ["test_grpc_server.cc"],
  deps = [
      "//test/proto:test_proto",
      "@com_github_grpc_grpc//:grpc++",
    ]
)

cc_binary(
  name = "test_grpc_client",
  srcs = ["test_grpc_client.cc"],
  deps = [
      "//test/proto:test_proto",
      "@com_github_grpc_grpc//:grpc++",
    ]
)