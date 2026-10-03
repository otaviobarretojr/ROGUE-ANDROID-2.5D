#include "global.h"
#include "scanline_effect.h"
#include "task.h"
#include <string.h>

u16 ALIGNED(4) gScanlineEffectRegBuffers[2][0x3C0] = {0};
struct ScanlineEffect gScanlineEffect = {0};

void ScanlineEffect_Stop(void)
{
    memset(&gScanlineEffect, 0, sizeof(gScanlineEffect));
}

void ScanlineEffect_Clear(void)
{
    memset(gScanlineEffectRegBuffers, 0, sizeof(gScanlineEffectRegBuffers));
    memset(&gScanlineEffect, 0, sizeof(gScanlineEffect));
}

void ScanlineEffect_SetParams(struct ScanlineEffectParams params)
{
    gScanlineEffect.dmaDest = params.dmaDest;
    gScanlineEffect.dmaControl = params.dmaControl;
    gScanlineEffect.state = params.initState;
    gScanlineEffect.unused16 = params.unused9;
    gScanlineEffect.unused17 = params.unused9;
}

void ScanlineEffect_InitHBlankDmaTransfer(void)
{
    /* Presentation-only on Android Stage 2. */
}

u8 ScanlineEffect_InitWave(
    u8 startLine,
    u8 endLine,
    u8 frequency,
    u8 amplitude,
    u8 delayInterval,
    u8 regOffset,
    bool8 applyBattleBgOffsets)
{
    (void)startLine;
    (void)endLine;
    (void)frequency;
    (void)amplitude;
    (void)delayInterval;
    (void)regOffset;
    (void)applyBattleBgOffsets;
    return TASK_NONE;
}
