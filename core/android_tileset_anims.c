#include "global.h"
#include "tileset_anims.h"

/*
 * Stage 2 Android presentation shim.
 *
 * The authoritative Hub logic consumes map/metatile data, collision and events.
 * GBA tileset animation only mutates presentation VRAM. The Android 2.5D renderer
 * will own animated presentation, so these hooks intentionally do no work while
 * the minimal Hub graph is being closed.
 */
void InitTilesetAnimations(void) {}
void InitSecondaryTilesetAnimation(void) {}
void UpdateTilesetAnimations(void) {}
void TransferTilesetAnimsBuffer(void) {}
