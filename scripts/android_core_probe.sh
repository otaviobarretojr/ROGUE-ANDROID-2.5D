#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
CORE="$ROOT/upstream/pokeemerald-rogue"
: "${ANDROID_NDK_HOME:?ANDROID_NDK_HOME is required}"

if [[ ! -d "$CORE" ]]; then
  "$ROOT/scripts/fetch_rogue_core.sh"
fi

echo "== Rogue upstream =="
git -C "$CORE" rev-parse HEAD

echo "== Generate host-side build tools =="
make -C "$CORE" -f make_tools.mk -j2

echo "== Probe Android/arm64 C compatibility =="
CLANG="$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android26-clang"
OUT="$ROOT/build/core-probe"
mkdir -p "$OUT"

COMMON=(
  -std=gnu17 -O1 -funsigned-char -fno-strict-aliasing -fwrapv -fcommon
  -DPORTABLE=1 -DROGUE_EXPANSION=1 -DROGUE_DEBUG=1
  -I"$CORE/include" -I"$CORE/gflib" -I"$CORE/tools/agbcc/include"
  -Wno-incompatible-pointer-types -Wno-int-conversion
  -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
)

# Start with the platform-neutral pieces that define the simulated GBA memory
# and the real game frame entry point. More files are added as blockers fall.
"$CLANG" "${COMMON[@]}" -c "$CORE/src/platform/system.c" -o "$OUT/system.o"
"$CLANG" "${COMMON[@]}" -c "$CORE/src/main.c" -o "$OUT/main.o"

echo "ANDROID_CORE_PROBE_OK"
file "$OUT/system.o" "$OUT/main.o"
