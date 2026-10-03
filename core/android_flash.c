#include "global.h"
#include "gba/flash_internal.h"
#include "platform.h"
#include <string.h>

static u16 AndroidProgramFlashByte(u16 sectorNum, u32 offset, u8 data);
static u16 AndroidProgramFlashSector(u16 sectorNum, u8 *src);
static u16 AndroidEraseFlashChip(void);
static u16 AndroidEraseFlashSector(u16 sectorNum);
static u16 AndroidWaitForFlashWrite(u8 phase, u8 *addr, u8 lastData);

static const u16 sAndroidFlashMaxTime[] = {0, 0, 0, 0};
static const struct FlashType sAndroidFlashType =
{
    131072,
    {
        4096,
        12,
        32,
        0
    },
    {3, 1},
    {{0xCC, 0xCC}}
};

u8 gFlashTimeoutFlag;
u8 (*PollFlashStatus)(u8 *);
u16 gFlashNumRemainingBytes;
u16 (*ProgramFlashByte)(u16, u32, u8) = AndroidProgramFlashByte;
u16 (*ProgramFlashSector)(u16, u8 *) = AndroidProgramFlashSector;
u16 (*EraseFlashChip)(void) = AndroidEraseFlashChip;
u16 (*EraseFlashSector)(u16) = AndroidEraseFlashSector;
u16 (*WaitForFlashWrite)(u8, u8 *, u8) = AndroidWaitForFlashWrite;
const u16 *gFlashMaxTime = sAndroidFlashMaxTime;
const struct FlashType *gFlash = &sAndroidFlashType;

u16 IdentifyFlash(void)
{
    ProgramFlashByte = AndroidProgramFlashByte;
    ProgramFlashSector = AndroidProgramFlashSector;
    EraseFlashChip = AndroidEraseFlashChip;
    EraseFlashSector = AndroidEraseFlashSector;
    WaitForFlashWrite = AndroidWaitForFlashWrite;
    gFlashMaxTime = sAndroidFlashMaxTime;
    gFlash = &sAndroidFlashType;
    return 0;
}

u16 ReadFlashId(void)
{
    return 0xCCCC;
}

void ReadFlash(u16 sectorNum, u32 offset, u8 *dest, u32 size)
{
    Platform_ReadFlash(sectorNum, offset, dest, size);
}

static u16 AndroidProgramFlashByte(u16 sectorNum, u32 offset, u8 data)
{
    u32 start;
    if (sectorNum >= sAndroidFlashType.sector.count || offset >= sAndroidFlashType.sector.size)
        return 0x80FF;

    start = ((u32)sectorNum << sAndroidFlashType.sector.shift) + offset;
    FLASH_BASE[start] = data;
    return 0;
}

static u16 AndroidProgramFlashSector(u16 sectorNum, u8 *src)
{
    u32 start;
    if (sectorNum >= sAndroidFlashType.sector.count || src == NULL)
        return 0x80FF;

    start = (u32)sectorNum << sAndroidFlashType.sector.shift;
    memcpy(&FLASH_BASE[start], src, sAndroidFlashType.sector.size);
    return 0;
}

static u16 AndroidEraseFlashChip(void)
{
    memset(FLASH_BASE, 0xFF, sizeof(FLASH_BASE));
    return 0;
}

static u16 AndroidEraseFlashSector(u16 sectorNum)
{
    u32 start;
    if (sectorNum >= sAndroidFlashType.sector.count)
        return 0x80FF;

    start = (u32)sectorNum << sAndroidFlashType.sector.shift;
    memset(&FLASH_BASE[start], 0xFF, sAndroidFlashType.sector.size);
    return 0;
}

static u16 AndroidWaitForFlashWrite(u8 phase, u8 *addr, u8 lastData)
{
    (void)phase;
    (void)addr;
    (void)lastData;
    return 0;
}

u32 VerifyFlashSector(u16 sectorNum, u8 *src)
{
    u32 start;
    if (sectorNum >= sAndroidFlashType.sector.count || src == NULL)
        return 1;

    start = (u32)sectorNum << sAndroidFlashType.sector.shift;
    return memcmp(&FLASH_BASE[start], src, sAndroidFlashType.sector.size) == 0 ? 0 : 1;
}

u32 VerifyFlashSectorNBytes(u16 sectorNum, u8 *src, u32 n)
{
    u32 start;
    if (sectorNum >= sAndroidFlashType.sector.count || src == NULL)
        return 1;

    if (n > sAndroidFlashType.sector.size)
        n = sAndroidFlashType.sector.size;
    start = (u32)sectorNum << sAndroidFlashType.sector.shift;
    return memcmp(&FLASH_BASE[start], src, n) == 0 ? 0 : 1;
}

u32 ProgramFlashSectorAndVerify(u16 sectorNum, u8 *src)
{
    u16 result = AndroidProgramFlashSector(sectorNum, src);
    if (result != 0)
        return result;
    return VerifyFlashSector(sectorNum, src);
}

u32 ProgramFlashSectorAndVerifyNBytes(u16 sectorNum, u8 *src, u32 n)
{
    u32 start;
    if (sectorNum >= sAndroidFlashType.sector.count || src == NULL)
        return 1;

    if (n > sAndroidFlashType.sector.size)
        n = sAndroidFlashType.sector.size;
    start = (u32)sectorNum << sAndroidFlashType.sector.shift;
    memcpy(&FLASH_BASE[start], src, n);
    return VerifyFlashSectorNBytes(sectorNum, src, n);
}
