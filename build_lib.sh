#!/bin/bash
set -e

# Portable core count (macOS has no nproc)
if command -v nproc >/dev/null 2>&1; then
    JOBS=$(nproc)
else
    JOBS=$(sysctl -n hw.ncpu)
fi

mkdir -p build
cd build
cmake ..
make -j"$JOBS"

echo "Physix2D library built successfully!"
