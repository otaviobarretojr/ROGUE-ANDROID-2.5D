#include "global.h"
#include "bg.h"
#include "dma3.h"
#include "malloc.h"
#include "menu.h"
#include <string.h>

static bool8 sScheduledBgCopiesToVram[4] = { FALSE };
static u16 sTempTileDataBufferIdx = 0;
static void *sTempTileDataBuffer[0x20] = { NULL };

void ClearScheduledBgCopiesToVram(void)
{
    memset(sScheduledBgCopiesToVram, 0, sizeof(sScheduledBgCopiesToVram));
}

void ScheduleBgCopyTilemapToVram(u8 bgId)
{
    if (bgId < ARRAY_COUNT(sScheduledBgCopiesToVram))
        sScheduledBgCopiesToVram[bgId] = TRUE;
}

void DoScheduledBgTilemapCopiesToVram(void)
{
    for (u8 bgId = 0; bgId < ARRAY_COUNT(sScheduledBgCopiesToVram); ++bgId)
    {
        if (sScheduledBgCopiesToVram[bgId])
        {
            CopyBgTilemapBufferToVram(bgId);
            sScheduledBgCopiesToVram[bgId] = FALSE;
        }
    }
}

void ResetTempTileDataBuffers(void)
{
    memset(sTempTileDataBuffer, 0, sizeof(sTempTileDataBuffer));
    sTempTileDataBufferIdx = 0;
}

bool8 FreeTempTileDataBuffersIfPossible(void)
{
    if (IsDma3ManagerBusyWithBgCopy())
        return TRUE;

    for (u16 i = 0; i < sTempTileDataBufferIdx; ++i)
    {
        if (sTempTileDataBuffer[i] != NULL)
        {
            Free(sTempTileDataBuffer[i]);
            sTempTileDataBuffer[i] = NULL;
        }
    }

    sTempTileDataBufferIdx = 0;
    return FALSE;
}

static void *MallocAndDecompress(const void *src, u32 *size)
{
    const u8 *srcBytes = src;
    void *ptr;

    if (src == NULL || size == NULL)
        return NULL;

    *size = (u32)srcBytes[1] | ((u32)srcBytes[2] << 8) | ((u32)srcBytes[3] << 16);
    ptr = Alloc(*size);

    if (ptr != NULL)
        LZ77UnCompWram(src, ptr);

    return ptr;
}

static u16 CopyDecompressedTileDataToVram(u8 bgId, const void *src, u16 size, u16 offset, u8 mode)
{
    switch (mode)
    {
    case 0:
        return LoadBgTiles(bgId, src, size, offset);
    case 1:
        return LoadBgTilemap(bgId, src, size, offset);
    default:
        return (u16)-1;
    }
}

void *DecompressAndCopyTileDataToVram(u8 bgId, const void *src, u32 size, u16 offset, u8 mode)
{
    u32 sizeOut;

    if (sTempTileDataBufferIdx >= ARRAY_COUNT(sTempTileDataBuffer))
        return NULL;

    void *ptr = MallocAndDecompress(src, &sizeOut);
    if (ptr == NULL)
        return NULL;

    if (size == 0)
        size = sizeOut;

    CopyDecompressedTileDataToVram(bgId, ptr, (u16)size, offset, mode);
    sTempTileDataBuffer[sTempTileDataBufferIdx++] = ptr;
    return ptr;
}
