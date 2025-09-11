#!/bin/bash
set -e

mkdir -p build
cd build
cmake ..
make -j$(nproc)

echo "✅ Physix2D library built successfully!"
