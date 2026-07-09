#!/bin/bash
# Capture all demos to 10s mp4s via the engine's record mode.
set -e
cd "$(dirname "$0")/examples"

DEMODIR=build
OUT=/tmp/physix_videos
FRAMES=300      # 300 frames @ 30fps = 10s
SUBSTEPS=2      # 2 physics steps per captured frame (600 steps = 10s sim @ 60Hz)
mkdir -p "$OUT"

DEMOS=$(ls -1 "$DEMODIR" | grep -vE '\.|CMakeFiles|physix2d_build|Makefile' | sort)

for demo in $DEMOS; do
    echo "=== capturing $demo ==="
    TMP=$(mktemp -d)
    PHYSIX_RECORD=1 PHYSIX_OUTDIR="$TMP" PHYSIX_FRAMES=$FRAMES PHYSIX_SUBSTEPS=$SUBSTEPS \
        "$DEMODIR/$demo" >/dev/null 2>&1 || true
    n=$(ls "$TMP" | wc -l | tr -d ' ')
    if [ "$n" -lt 10 ]; then
        echo "  !! only $n frames for $demo, skipping"
        rm -rf "$TMP"; continue
    fi
    ffmpeg -y -framerate 30 -i "$TMP/frame_%05d.png" \
        -c:v libx264 -pix_fmt yuv420p -movflags +faststart \
        -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" \
        "$OUT/$demo.mp4" >/dev/null 2>&1
    echo "  -> $OUT/$demo.mp4 ($(du -h "$OUT/$demo.mp4" | cut -f1), $n frames)"
    rm -rf "$TMP"
done

echo "=== DONE. Videos in $OUT ==="
ls -la "$OUT"
