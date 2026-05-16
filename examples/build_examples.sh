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

# Homebrew's sfml@2 is keg-only; expose its pkg-config files if present.
if [ -d "/opt/homebrew/opt/sfml@2/lib/pkgconfig" ]; then
    export PKG_CONFIG_PATH="/opt/homebrew/opt/sfml@2/lib/pkgconfig:$PKG_CONFIG_PATH"
fi

cmake ..
make -j"$JOBS"

echo "All examples built successfully!"
