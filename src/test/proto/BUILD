load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")
load("@rules_proto//proto:defs.bzl", "proto_library")

proto_library(
  name = "helloworld",
  srcs = ["helloworld.proto"],
)

cc_proto_library(
	name = 'helloworld_proto',
	deps = [":helloworld"],
)

cc_grpc_library(
  name = "helloworld_grpc",
  srcs = [":helloworld"],
  deps = [":helloworld_proto"],
  grpc_only = True,
)

cc_library(
	name = "test_proto",
	deps = [
		":helloworld_grpc",
		":helloworld_proto"
	],
	visibility = ["//visibility:public"],
)