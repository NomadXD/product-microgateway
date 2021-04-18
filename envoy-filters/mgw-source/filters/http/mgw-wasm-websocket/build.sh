#!/bin/bash

# Setting up parent path to make the script executable from anywhere.
echo "Setting up parent_path..."
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
echo "Parent path configured : ${parent_path}"
cd "$parent_path"

# Defining mount directories between host and container
SOURCE_DIR="${PWD}"
SOURCE_DIR_MOUNT_DEST=/source/mgw-source/filters/http/mgw-wasm-websocket

SOURCE_DIR_API="${PWD}"/../../../../mgw-api/extensions/filters/http/mgw-wasm-websocket/v3/
SOURCE_DIR_MOUNT_API=/source/mgw-api/extensions/filters/http/mgw-wasm-websocket/v3/

BUILD_DIR="${PWD}"/../../../../../router/target/mgw-wasm/
BUILD_DIR_MOUNT_DEST=/build

ENVOY_BUILD_IMAGE=envoyproxy/envoy-build-ubuntu:e33c93e6d79804bf95ff80426d10bdcc9096c785

# START_COMMAND=("/bin/sh" -c 'cd source/mgw-source/filters/http/mgw-wasm-websocket \
#                             && bazel build //:mgw-websocket.wasm \
#                             && tar czf mgw-websocket.tar.gz -C /build bazel-bin/mgw-websocket.wasm \
#                             && cp -a bazel-bin/mgw-websocket.wasm /build')
 

# START_COMMAND=("/bin/sh" -c 'cd source/mgw-source/filters/http/mgw-wasm-websocket \
#                             && bazel build //:mgw-websocket.wasm \
#                             && cp -a bazel-bin/mgw-websocket.wasm /build \
#                             && tar cfz mgw.tar.gz bazel-bin/mgw-websocket.wasm \
#                             && cp -a mgw.tar.gz /build')

START_COMMAND=("/bin/bash" "-lc" "groupadd --gid $(id -g) -f envoygroup \
    && useradd -o --uid $(id -u) --gid $(id -g) --no-create-home --home-dir /build envoybuild \
    && usermod -a -G pcap envoybuild \
    && sudo -EHs -u envoybuild bash -c '\
        cd source/mgw-source/filters/http/mgw-wasm-websocket \
        && bazel build -c opt //:mgw-websocket.wasm \
        && cp -a bazel-bin/mgw-websocket.wasm /build'")

ENVOY_DOCKER_OPTIONS+=(-u root:root)
ENVOY_DOCKER_OPTIONS+=(-v /var/run/docker.sock:/var/run/docker.sock)
ENVOY_DOCKER_OPTIONS+=(--cap-add SYS_PTRACE --cap-add NET_RAW --cap-add NET_ADMIN)

mkdir -p "${BUILD_DIR}"

docker run --rm \
    "${ENVOY_DOCKER_OPTIONS[@]}" \
    -v "${BUILD_DIR}":"${BUILD_DIR_MOUNT_DEST}" \
    -v "${SOURCE_DIR}":"${SOURCE_DIR_MOUNT_DEST}" \
    -v "${SOURCE_DIR_API}":"${SOURCE_DIR_MOUNT_API}" \
    "${ENVOY_BUILD_IMAGE}" \
    "${START_COMMAND[@]}"


chmod a+rwx "${PWD}"/../../../../../router/target/mgw-wasm/mgw-websocket.wasm