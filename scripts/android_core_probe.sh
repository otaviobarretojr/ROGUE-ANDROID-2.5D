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

echo "== Generate Rogue compile-time headers =="
mkdir -p "$CORE/include/generated"
"$CORE/tools/Pokabbie/Build/CustomJson/customjson" \
  quest_consts_h \
  "$CORE/src/data/rogue/quests.json" \
  "$CORE/include/generated/quest_consts.h"
mkdir -p "$CORE/include/constants/generated"
"$CORE/tools/Pokabbie/Build/CustomJson/customjson" \
  quests_h \
  "$CORE/src/data/rogue/quests.json" \
  "$CORE/include/constants/generated/quests.h"
test -s "$CORE/include/generated/quest_consts.h"
test -s "$CORE/include/constants/generated/quests.h"
"$CORE/tools/Pokabbie/Build/CustomJson/customjson" \
  decoration_h \
  "$CORE/src/data/rogue/decorations.json" \
  "$CORE/include/constants/generated/decorations.h"
test -s "$CORE/include/constants/generated/decorations.h"
"$CORE/tools/Pokabbie/Build/CustomJson/customjson" \
  custom_mons_h \
  "$CORE/src/data/rogue/custom_mons.json" \
  "$CORE/include/constants/generated/custom_mons.h"
test -s "$CORE/include/constants/generated/custom_mons.h"

echo "== Generate map/layout assembly inputs =="
make -C "$CORE" -j2 \
  data/layouts/layouts.inc \
  data/layouts/layouts_table.inc \
  data/maps/headers.inc \
  data/maps/groups.inc \
  data/maps/connections.inc

echo "== Probe Android/arm64 C compatibility =="
CLANG="$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android26-clang"
OUT="$ROOT/build/core-probe"
mkdir -p "$OUT"

COMMON=(
  -std=gnu17 -O1 -funsigned-char -fno-strict-aliasing -fwrapv -fcommon
  -DPORTABLE=1 -DROGUE_EXPANSION=1 -DROGUE_DEBUG=1 -DROGUE_BAKING=1
  -I"$CORE/include" -I"$CORE/gflib" -I"$CORE/tools/agbcc/include"
  -Wno-incompatible-pointer-types -Wno-int-conversion
  -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
  -include alloca.h
)

# Start with the platform-neutral pieces that define the simulated GBA memory
# and the real game frame entry point. More files are added as blockers fall.
"$CLANG" "${COMMON[@]}" -c "$CORE/src/platform/system.c" -o "$OUT/system.o"
"$CLANG" "${COMMON[@]}" -c "$CORE/src/main.c" -o "$OUT/main.o"
"$CLANG" "${COMMON[@]}" -c "$ROOT/core/android_flash.c" -o "$OUT/android_flash.o"
echo "== Generate native-pointer Rogue Hub data =="
python3 "$ROOT/scripts/generate_arm64_hub_data.py" --core "$CORE" --out "$OUT/hub_native.c"
"$CLANG" "${COMMON[@]}" -c "$OUT/hub_native.c" -o "$OUT/hub_native.o"

echo "== Map/layout data ABI note =="
echo "Generated map data is validated but not linked yet: upstream portable maps use 32-bit pointer tables (-m32)."
echo "ARM64 requires a native-pointer map data adapter before gMapGroups/gMapLayouts can be linked safely."

# Second wave: exercise gameplay state, RNG and save/load translation units.
# This is compile-only on purpose; unresolved game symbols are expected until
# the complete relocatable core target is assembled.
for src in random.c event_data.c load_save.c save.c \
  script.c fieldmap.c field_control_avatar.c field_player_avatar.c \
  overworld.c event_object_movement.c task.c util.c \
  field_camera.c bike.c decompress.c metatile_behavior.c palette.c field_door.c field_screen_effect.c field_message_box.c trainer_see.c rogue_hub.c; do
  echo "Probing src/$src"
  EXTRA=()
  if [[ "$src" == "event_object_movement.c" ]]; then
    EXTRA=(-DMODERN=1)
  fi
  "$CLANG" "${COMMON[@]}" "${EXTRA[@]}" -c "$CORE/src/$src" -o "$OUT/${src%.c}.o"
done

for src in malloc.c sprite.c dma3_manager.c string_util.c bg.c gpu_regs.c blit.c text.c window.c; do
  echo "Probing gflib/$src"
  "$CLANG" "${COMMON[@]}" -c "$CORE/gflib/$src" -o "$OUT/gflib_${src%.c}.o"
done

echo "== Partial relocatable Rogue core link =="
LD="$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin/ld.lld"
OBJECTS=(
  "$OUT/system.o" "$OUT/main.o" "$OUT/android_flash.o"
  "$OUT/random.o" "$OUT/event_data.o" "$OUT/load_save.o" "$OUT/save.o"
  "$OUT/script.o" "$OUT/fieldmap.o" "$OUT/field_control_avatar.o"
  "$OUT/field_player_avatar.o" "$OUT/overworld.o" "$OUT/event_object_movement.o"
  "$OUT/task.o" "$OUT/util.o" "$OUT/field_camera.o" "$OUT/bike.o" "$OUT/decompress.o" "$OUT/metatile_behavior.o" "$OUT/palette.o"
  "$OUT/field_door.o" "$OUT/field_screen_effect.o" "$OUT/field_message_box.o" "$OUT/trainer_see.o" "$OUT/rogue_hub.o"
  "$OUT/gflib_malloc.o" "$OUT/gflib_sprite.o" "$OUT/gflib_dma3_manager.o"
  "$OUT/gflib_string_util.o" "$OUT/gflib_bg.o" "$OUT/gflib_gpu_regs.o" "$OUT/gflib_blit.o"
  "$OUT/gflib_text.o" "$OUT/gflib_window.o"
)
"$LD" -r "${OBJECTS[@]}" -o "$OUT/rogue_core_stage1.o"

echo "== Stage 1 unresolved symbol inventory =="
NM="$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-nm"
"$NM" -u "$OUT/rogue_core_stage1.o" | sort -u > "$OUT/unresolved-stage1.txt"
COUNT="$(wc -l < "$OUT/unresolved-stage1.txt")"
echo "$COUNT unresolved symbols (baseline: 575; infra checkpoint: 548)"
wc -l "$OUT/unresolved-stage1.txt"
head -n 80 "$OUT/unresolved-stage1.txt"

echo "== Candidate providers for unresolved symbols =="
python3 "$ROOT/scripts/map_unresolved_providers.py" \
  "$OUT/unresolved-stage1.txt" "$CORE" \
  --details "$OUT/unresolved-providers.txt" \
  --summary "$OUT/provider-summary.txt"
head -n 120 "$OUT/provider-summary.txt"

echo "ANDROID_CORE_PROBE_OK"
file "$OUT"/*.o
