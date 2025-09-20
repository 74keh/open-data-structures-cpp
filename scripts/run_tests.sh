#!/usr/bin/env bash
set -euo pipefail

CXX=${CXX:-g++}
CXXFLAGS=${CXXFLAGS:--std=c++17 -Wall -Wextra -Iincludes}
BUILD_DIR=${BUILD_DIR:-build/tests}

if ! command -v "$CXX" >/dev/null 2>&1; then
    echo "Compiler \"$CXX\" not found" >&2
    exit 1
fi

mkdir -p "$BUILD_DIR"

status=0
shopt -s nullglob
for test_src in tests/*.cpp; do
    test_name=$(basename "${test_src%.cpp}")
    output_path="$BUILD_DIR/$test_name"

    echo "Compiling $test_src"
    if ! "$CXX" $CXXFLAGS "$test_src" -o "$output_path"; then
        status=1
        continue
    fi

    echo "Running $output_path"
    if ! "$output_path"; then
        status=1
    fi
    echo

done
shopt -u nullglob

exit $status
