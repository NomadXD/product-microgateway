package(default_visibility = ["//visibility:public"])

licenses(["notice"])  # Apache 2

load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
    "envoy_cc_extension"
)


envoy_cc_library(
    name = "lib",
    srcs = ["mgw_websocket.cc"],
    hdrs = ["mgw_websocket.h"],
    repository = "@envoy",
    deps = [
        ":ratelimit_lib",
        "//mgw-api/extensions/filters/http/mgw_websocket/v3:pkg_cc_proto",
        "@envoy//source/common/http:codes_lib",
        
    ]
)

envoy_cc_library(
    name = "config",
    srcs = ["config.cc"],
    hdrs = ["config.h"],
    repository = "@envoy",
    deps = [
        ":lib",
        "@envoy//source/extensions/filters/http/common:factory_base_lib",
    ]
)

envoy_cc_library(
    name = "ratelimit_lib",
    srcs = ["ratelimit_impl.cc"],
    hdrs = ["ratelimit_impl.h"],
    repository = "@envoy",
    deps = [
        ":ratelimit_client_interface",
        "//mgw-api/extensions/filters/http/mgw_websocket/v3:pkg_cc_proto",
        #"@envoy//include/envoy/grpc:async_client_interface",
        "@envoy//source/common/grpc:typed_async_client_lib",

    ]

)

envoy_cc_library(
    name = "ratelimit_client_interface",
    hdrs = ["ratelimit.h"],
    repository = "@envoy",
    deps = [
        "//mgw-api/extensions/filters/http/mgw_websocket/v3:pkg_cc_proto",
        #"@envoy//include/envoy/tracing:http_tracer_interface",
    ]
)