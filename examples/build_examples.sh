#!/bin/bash
set -e

# Portable core count (macOS has no nproc)
if command -v nproc >/dev/null 2>&1; then
    JOBS=$(nproc)
else
    JOBS=$(sysctl -n hw.ncpu)
fi

BUILD_DIR="build"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake ..
make -j"$JOBS"

echo "All examples built successfully!"
