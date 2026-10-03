#include "global.h"
#include "gba/syscall.h"
#include <stdint.h>
#include <string.h>

static void CopyCpuUnits(const void *src, void *dest, u32 count, bool32 word32, bool32 fixed)
{
    if (word32)
    {
        const u32 *s = (const u32 *)src;
        u32 *d = (u32 *)dest;
        if (fixed)
        {
            u32 value = *s;
            for (u32 i = 0; i < count; ++i)
                d[i] = value;
        }
        else
        {
            memcpy(d, s, count * sizeof(u32));
        }
    }
    else
    {
        const u16 *s = (const u16 *)src;
        u16 *d = (u16 *)dest;
        if (fixed)
        {
            u16 value = *s;
            for (u32 i = 0; i < count; ++i)
                d[i] = value;
        }
        else
        {
            memcpy(d, s, count * sizeof(u16));
        }
    }
}

void CpuSet(const void *src, void *dest, u32 control)
{
    u32 count = control & 0x001FFFFF;
    bool32 fixed = (control & CPU_SET_SRC_FIXED) != 0;
    bool32 word32 = (control & CPU_SET_32BIT) != 0;
    if (src == NULL || dest == NULL || count == 0)
        return;
    CopyCpuUnits(src, dest, count, word32, fixed);
}

void CpuFastSet(const void *src, void *dest, u32 control)
{
    u32 count = control & 0x001FFFFF;
    bool32 fixed = (control & CPU_FAST_SET_SRC_FIXED) != 0;
    if (src == NULL || dest == NULL || count == 0)
        return;
    CopyCpuUnits(src, dest, count, TRUE, fixed);
}

static void Lz77Decompress(const u8 *src, u8 *dest)
{
    if (src == NULL || dest == NULL || src[0] != 0x10)
        return;

    u32 size = (u32)src[1] | ((u32)src[2] << 8) | ((u32)src[3] << 16);
    src += 4;

    u32 out = 0;
    while (out < size)
    {
        u8 flags = *src++;
        for (u32 bit = 0; bit < 8 && out < size; ++bit)
        {
            if ((flags & (0x80u >> bit)) == 0)
            {
                dest[out++] = *src++;
            }
            else
            {
                u8 a = *src++;
                u8 b = *src++;
                u32 length = (u32)(a >> 4) + 3;
                u32 displacement = (((u32)a & 0x0Fu) << 8) | b;
                displacement += 1;

                if (displacement > out)
                    return;

                for (u32 i = 0; i < length && out < size; ++i)
                {
                    dest[out] = dest[out - displacement];
                    ++out;
                }
            }
        }
    }
}

void LZ77UnCompWram(const u32 *src, void *dest)
{
    Lz77Decompress((const u8 *)src, (u8 *)dest);
}

void LZ77UnCompVram(const u32 *src, void *dest)
{
    /*
     * The portable core models VRAM as ordinary host memory. Producing the
     * same byte stream is therefore sufficient; the GBA BIOS halfword write
     * restriction does not apply to the Android backing buffer.
     */
    Lz77Decompress((const u8 *)src, (u8 *)dest);
}
