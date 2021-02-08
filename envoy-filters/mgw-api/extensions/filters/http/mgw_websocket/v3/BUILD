package(default_visibility = ["//visibility:public"])

load("@envoy_api//bazel:api_build_system.bzl", "api_proto_package")
load("@io_bazel_rules_go//proto:def.bzl", "go_proto_library")

api_proto_package(
    name = "proto_mgw",
    srcs = ["mgw_websocket_rls.proto", "mgw_websocket.proto"],
    deps = [
        "@envoy_api//envoy/config/ratelimit/v3:pkg",
        "@com_github_cncf_udpa//udpa/annotations:pkg",
        "@envoy_api//envoy/config/core/v3:pkg",
    ],
)


go_proto_library(
    name = "proto_mgw_go",
    compilers = ["@io_bazel_rules_go//proto:go_proto", "@envoy_api//bazel:pgv_plugin_go"],
    proto = ":pkg",
    visibility = ["//visibility:public"],
    importpath = "github.com/envoyproxy/go-control-plane/envoy/extensions/filters/http/mgw_websocket/v3",
    deps = [
        "@envoy_api//envoy/config/ratelimit/v3:pkg_go_proto",
        "@com_github_cncf_udpa//udpa/annotations:pkg_go_proto",
        "@envoy_api//envoy/config/core/v3:pkg_go_proto",
        "@com_envoyproxy_protoc_gen_validate//validate:go_default_library",
        "@com_github_golang_protobuf//ptypes:go_default_library"
    ],
)

