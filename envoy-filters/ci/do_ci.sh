#!/bin/bash

# Run a CI build/test target, e.g. docs, asan.

set -e

# build_setup_args=""
# if [[ "$1" == "fix_format" || "$1" == "check_format" || "$1" == "check_repositories" || \
#         "$1" == "check_spelling" || "$1" == "fix_spelling" || "$1" == "bazel.clang_tidy" || \
#         "$1" == "check_spelling_pedantic" || "$1" == "fix_spelling_pedantic" ]]; then
#   build_setup_args="-nofetch"
# fi

SRCDIR="${PWD}"
echo "no build ${NO_BUILD_SETUP:-}"
NO_BUILD_SETUP="${NO_BUILD_SETUP:-}"
if [[ -z "$NO_BUILD_SETUP" ]]; then
    # shellcheck source=ci/setup_cache.sh
    #. "$(dirname "$0")"/setup_cache.sh
    # shellcheck source=ci/build_setup.sh
    . "$(dirname "$0")"/build_setup.sh #$build_setup_args
fi
cd "${SRCDIR}"

if [[ "${ENVOY_BUILD_ARCH}" == "x86_64" ]]; then
  BUILD_ARCH_DIR="/linux/amd64"
elif [[ "${ENVOY_BUILD_ARCH}" == "aarch64" ]]; then
  BUILD_ARCH_DIR="/linux/arm64"
else
  # Fall back to use the ENVOY_BUILD_ARCH itself.
  BUILD_ARCH_DIR="/linux/${ENVOY_BUILD_ARCH}"
fi

echo "building using ${NUM_CPUS} CPUs"
echo "building for ${ENVOY_BUILD_ARCH}"

function collect_build_profile() {
  declare -g build_profile_count=${build_profile_count:-1}
  mv -f "$(bazel info output_base)/command.profile.gz" "${ENVOY_BUILD_PROFILE}/${build_profile_count}-$1.profile.gz" || true
  ((build_profile_count++))
}

function bazel_with_collection() {
  local failed_logs
  declare -r BAZEL_OUTPUT="${ENVOY_SRCDIR}"/bazel.output.txt
  bazel "$@" | tee "${BAZEL_OUTPUT}"
  declare BAZEL_STATUS="${PIPESTATUS[0]}"
  if [ "${BAZEL_STATUS}" != "0" ]
  then
    pushd bazel-testlogs
    failed_logs=$(grep "  /build.*test.log" "${BAZEL_OUTPUT}" | sed -e 's/  \/build.*\/testlogs\/\(.*\)/\1/')
    while read -r f; do
      cp --parents -f "$f" "${ENVOY_FAILED_TEST_LOGS}"
    done <<< "$failed_logs"
    popd
    exit "${BAZEL_STATUS}"
  fi
  collect_build_profile "$1"
  run_process_test_result
}

function cp_binary_for_outside_access() {
  DELIVERY_LOCATION="$1"
  cp -f \
    bazel-bin/"${ENVOY_BIN}" \
    "${ENVOY_DELIVERY_DIR}"/"${DELIVERY_LOCATION}"
}

function cp_debug_info_for_outside_access() {
  DELIVERY_LOCATION="$1"
  cp -f \
    bazel-bin/"${ENVOY_BIN}".dwp \
    "${ENVOY_DELIVERY_DIR}"/"${DELIVERY_LOCATION}".dwp
}


function cp_binary_for_image_build() {
  # TODO(mattklein123): Replace this with caching and a different job which creates images.
  local BASE_TARGET_DIR="${ENVOY_SRCDIR}${BUILD_ARCH_DIR}"
  echo "Copying binary for image build..."
  COMPILE_TYPE="$2"
  mkdir -p "${BASE_TARGET_DIR}"/build_"$1"
  cp -f "${ENVOY_DELIVERY_DIR}"/envoy "${BASE_TARGET_DIR}"/build_"$1"
  # if [[ "${COMPILE_TYPE}" == "dbg" || "${COMPILE_TYPE}" == "opt" ]]; then
  #   cp -f "${ENVOY_DELIVERY_DIR}"/envoy.dwp "${BASE_TARGET_DIR}"/build_"$1"
  # fi
  mkdir -p "${BASE_TARGET_DIR}"/build_"$1"_stripped
  strip "${ENVOY_DELIVERY_DIR}"/envoy -o "${BASE_TARGET_DIR}"/build_"$1"_stripped/envoy

  # Copy for azp which doesn't preserve permissions, creating a tar archive
  tar czf "${ENVOY_BUILD_DIR}"/envoy_binary.tar.gz -C "${BASE_TARGET_DIR}" build_"$1" build_"$1"_stripped

  # Remove binaries to save space, only if BUILD_REASON exists (running in AZP)
  [[ -z "${BUILD_REASON}" ]] || \
    rm -rf "${BASE_TARGET_DIR}"/build_"$1" "${BASE_TARGET_DIR}"/build_"$1"_stripped "${ENVOY_DELIVERY_DIR}"/envoy{,.dwp} \
      bazel-bin/"${ENVOY_BIN}"{,.dwp}
}

function bazel_binary_build() {
  BINARY_TYPE="$1"
  if [[ "${BINARY_TYPE}" == "release" ]]; then
    COMPILE_TYPE="opt"
  elif [[ "${BINARY_TYPE}" == "debug" ]]; then
    COMPILE_TYPE="dbg"
  elif [[ "${BINARY_TYPE}" == "sizeopt" ]]; then
    # The COMPILE_TYPE variable is redundant in this case and is only here for
    # readability. It is already set in the .bazelrc config for sizeopt.
    COMPILE_TYPE="opt"
    CONFIG_ARGS="--config=sizeopt"
  elif [[ "${BINARY_TYPE}" == "fastbuild" ]]; then
    COMPILE_TYPE="fastbuild"
  fi

  echo "Building..."
  ENVOY_BIN=$(echo "${ENVOY_BUILD_TARGET}" | sed -e 's#^@\([^/]*\)/#external/\1#;s#^//##;s#:#/#')

  # This is a workaround for https://github.com/bazelbuild/bazel/issues/11834
  [[ -n "${ENVOY_RBE}" ]] && rm -rf bazel-bin/"${ENVOY_BIN}"*

  bazel build "${BAZEL_BUILD_OPTIONS[@]}" -c "${COMPILE_TYPE}" "${ENVOY_BUILD_TARGET}" ${CONFIG_ARGS}
  collect_build_profile "${BINARY_TYPE}"_build

  # Copy the built envoy binary somewhere that we can access outside of the
  # container.
  cp_binary_for_outside_access envoy

  #if [[ "${COMPILE_TYPE}" == "dbg" || "${COMPILE_TYPE}" == "opt" ]]; then
    # Generate dwp file for debugging since we used split DWARF to reduce binary
    # size
    #bazel build "${BAZEL_BUILD_OPTIONS[@]}" -c "${COMPILE_TYPE}" "${ENVOY_BUILD_DEBUG_INFORMATION}" ${CONFIG_ARGS}
    # Copy the debug information
    #cp_debug_info_for_outside_access envoy
  #fi

  cp_binary_for_image_build "${BINARY_TYPE}" "${COMPILE_TYPE}"

}

function run_process_test_result() {
  echo "running flaky test reporting script"
  "${ENVOY_SRCDIR}"/ci/flaky_test/run_process_xml.sh "$CI_TARGET"
}

CI_TARGET=$1
shift
echo "XXXXXx $@"
if [[ $# -ge 1 ]]; then
  COVERAGE_TEST_TARGETS=("$@")
  TEST_TARGETS=("$@")
else
  # Coverage test will add QUICHE tests by itself.
  #COVERAGE_TEST_TARGETS=("//test/...")
  COVERAGE_TEST_TARGETS=("@envoy//test/...")
  TEST_TARGETS=("${COVERAGE_TEST_TARGETS[@]}" "@com_googlesource_quiche//:ci_tests")
fi

if [[ "$CI_TARGET" == "bazel.release" ]]; then
  # When testing memory consumption, we want to test against exact byte-counts
  # where possible. As these differ between platforms and compile options, we
  # define the 'release' builds as canonical and test them only in CI, so the
  # toolchain is kept consistent. This ifdef is checked in
  # test/common/stats/stat_test_utility.cc when computing
  # Stats::TestUtil::MemoryTest::mode().
  [[ "${ENVOY_BUILD_ARCH}" == "x86_64" ]] && BAZEL_BUILD_OPTIONS+=("--test_env=ENVOY_MEMORY_TEST_EXACT=true")

  setup_clang_toolchain
  echo "Testing ${TEST_TARGETS[*]} with options: ${BAZEL_BUILD_OPTIONS[*]}"
  bazel_with_collection test "${BAZEL_BUILD_OPTIONS[@]}" -c opt "${TEST_TARGETS[@]}"

  echo "bazel release build with tests..."
  bazel_binary_build release
  exit 0
elif [[ "$CI_TARGET" == "bazel.release.server_only" ]]; then
  setup_clang_toolchain
  echo "bazel release build..."
  bazel_binary_build release
  exit 0
elif [[ "$CI_TARGET" == "bazel.gcc" ]]; then
  BAZEL_BUILD_OPTIONS+=("--test_env=HEAPCHECK=")
  setup_gcc_toolchain

  echo "Testing ${TEST_TARGETS[*]}"
  bazel_with_collection test "${BAZEL_BUILD_OPTIONS[@]}" -c fastbuild "${TEST_TARGETS[@]}"

  echo "bazel release build with gcc..."
  bazel_binary_build fastbuild
  exit 0
elif [[ "$CI_TARGET" == "bazel.debug" ]]; then
  setup_clang_toolchain
  echo "Testing ${TEST_TARGETS[*]}"
  bazel test "${BAZEL_BUILD_OPTIONS[@]}" -c dbg "${TEST_TARGETS[@]}"

  echo "bazel debug build with tests..."
  bazel_binary_build debug
  exit 0
elif [[ "$CI_TARGET" == "bazel.debug.server_only" ]]; then
  setup_clang_toolchain
  echo "bazel debug build..."
  bazel_binary_build debug
  exit 0
elif [[ "$CI_TARGET" == "bazel.dev" ]]; then
  setup_clang_toolchain
  # This doesn't go into CI but is available for developer convenience.
  echo "bazel fastbuild build with tests..."
  echo "Building..."
  bazel_binary_build fastbuild

  echo "Building and testing ${TEST_TARGETS[*]}"
  bazel_with_collection test "${BAZEL_BUILD_OPTIONS[@]}" -c fastbuild "${TEST_TARGETS[@]}"
  # TODO(foreseeable): consolidate this and the API tool tests in a dedicated target.
  bazel_with_collection //tools/envoy_headersplit:headersplit_test --spawn_strategy=local
  bazel_with_collection //tools/envoy_headersplit:replace_includes_test --spawn_strategy=local
  exit 0
else
  echo "Invalid do_ci.sh target, see ci/README.md for valid targets."
  exit 1
fi
