#!/usr/bin/env bash

set -e

HERE="$(cd "$(dirname "$0")" && pwd)"
OUT="${1:-$HERE/../bin}"

PRO="$(find "$HERE" -maxdepth 1 -name '*.pro' -print -quit)"

if [[ -z "$PRO" ]]; then
    echo "No .pro found"
    exit 1
fi

mkdir -p "$OUT"

cd "$HERE"

~/Qt68/6.8.3/gcc_64/bin/qmake "$PRO" \
    CONFIG+=release \
    DESTDIR="$OUT"

make -j"$(nproc)"

echo "Ready: $OUT"
