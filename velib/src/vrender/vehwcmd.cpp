/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
#include "vehwcmd.hpp"
#ifdef LINUX
#ifndef __APPLE__
#include <malloc.h>
#endif
#endif

#if 0

#define ARCH_X86 1

#if ARCH_X86

static int has_sse2     = -1;
static int has_sse3     = -1;
static int has_sse4     = -1;

#if defined _WIN32 && !defined __MINGW32__
//  Windows
#define cpuid    __cpuid

#else
inline void cpuid(int CPUInfo[4],int InfoType)
{
    __asm__ __volatile__ (
    // pic requires to save ebx/rbx
#if ARCH_X86_32
    "push       %%ebx\n"
#endif
    "cpuid\n"
    "movl %%ebx ,%[ebx]\n"
#if ARCH_X86_32
    "pop        %%ebx\n"
#endif
    :"=a" (CPUInfo[0]),
    [ebx] "=r"(CPUInfo[1]),
    "=c" (CPUInfo[2]),
    "=d" (CPUInfo[3])
    :"a" (InfoType)
    );
}
#endif

static inline bool sse2_check()
{
    if (has_sse2 != -1)
    {
        return has_sse2;
    }

    int info[4];
    cpuid(info, 0);
    int nIds = info[0];

    //  Detect Features
    if (nIds >= 0x00000001)
    {
        cpuid(info,0x00000001);
        has_sse2  = (info[3] & ((int)1 << 26)) != 0;
        has_sse3  = (info[2] & ((int)1 <<  0)) != 0;
        has_sse4  = (info[2] & ((int)1 << 19)) != 0;
    }
    else
    {
        has_sse2  = 0;
        has_sse3  = 0;
        has_sse4  = 0;
    }
    return has_sse2;
}

static inline bool sse3_check()
{
    if (has_sse3 != -1)
    {
        return has_sse3;
    }

    sse2_check();

    return has_sse3;
}

static inline bool sse4_check()
{
    if (has_sse4 != -1)
    {
        return has_sse4;
    }

    sse2_check();

    return has_sse4;
}

static inline void SSE_splitplanes(uint8_t* dstu, int dstu_pitch,
                                   uint8_t* dstv, int dstv_pitch,
                                   const uint8_t* src, int src_pitch,
                                   int width, int height)
{
    const uint8_t shuffle[] = { 0, 2, 4, 6, 8, 10, 12, 14,
                                1, 3, 5, 7, 9, 11, 13, 15 };
    const uint8_t mask[] = { 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
                             0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00 };
    const bool sse3 = sse3_check();

	_mm_mfence();

#define LOAD64A \
    "movdqa  0(%[src]), %%xmm0\n" \
    "movdqa 16(%[src]), %%xmm1\n" \
    "movdqa 32(%[src]), %%xmm2\n" \
    "movdqa 48(%[src]), %%xmm3\n"

#define LOAD64U \
    "movdqu  0(%[src]), %%xmm0\n" \
    "movdqu 16(%[src]), %%xmm1\n" \
    "movdqu 32(%[src]), %%xmm2\n" \
    "movdqu 48(%[src]), %%xmm3\n"

#define STORE2X32 \
    "movq   %%xmm0,   0(%[dst1])\n" \
    "movq   %%xmm1,   8(%[dst1])\n" \
    "movhpd %%xmm0,   0(%[dst2])\n" \
    "movhpd %%xmm1,   8(%[dst2])\n" \
    "movq   %%xmm2,  16(%[dst1])\n" \
    "movq   %%xmm3,  24(%[dst1])\n" \
    "movhpd %%xmm2,  16(%[dst2])\n" \
    "movhpd %%xmm3,  24(%[dst2])\n"

    for (int y = 0; y < height; y++)
    {
        int x = 0;

        if (((uintptr_t)src & 0xf) == 0)
        {
            if (sse3)
            {
                for (; x < (width & ~31); x += 32)
                {
                    __asm  (
                        "movdqu (%[shuffle]), %%xmm7\n"
                        LOAD64A
                        "pshufb  %%xmm7, %%xmm0\n"
                        "pshufb  %%xmm7, %%xmm1\n"
                        "pshufb  %%xmm7, %%xmm2\n"
                        "pshufb  %%xmm7, %%xmm3\n"
                        STORE2X32
                        : : [dst1]"r"(&dstu[x]), [dst2]"r"(&dstv[x]), [src]"r"(&src[2*x]), [shuffle]"r"(shuffle) : "memory", "xmm0", "xmm1", "xmm2", "xmm3", "xmm7");
                }
            }
            else
            {
                for (; x < (width & ~31); x += 32)
                {
					__asm volatile (
                        "movdqu (%[mask]), %%xmm7\n"
                        LOAD64A
                        "movdqa   %%xmm0, %%xmm4\n"
                        "movdqa   %%xmm1, %%xmm5\n"
                        "movdqa   %%xmm2, %%xmm6\n"
                        "psrlw    $8,     %%xmm0\n"
                        "psrlw    $8,     %%xmm1\n"
                        "pand     %%xmm7, %%xmm4\n"
                        "pand     %%xmm7, %%xmm5\n"
                        "pand     %%xmm7, %%xmm6\n"
                        "packuswb %%xmm4, %%xmm0\n"
                        "packuswb %%xmm5, %%xmm1\n"
                        "pand     %%xmm3, %%xmm7\n"
                        "psrlw    $8,     %%xmm2\n"
                        "psrlw    $8,     %%xmm3\n"
                        "packuswb %%xmm6, %%xmm2\n"
                        "packuswb %%xmm7, %%xmm3\n"
                        STORE2X32
                        : : [dst2]"r"(&dstu[x]), [dst1]"r"(&dstv[x]), [src]"r"(&src[2*x]), [mask]"r"(mask) : "memory", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7");
                }
            }
        }
        else
        {
            if (sse3)
            {
                for (; x < (width & ~31); x += 32)
                {
					__asm volatile (
                        "movdqu (%[shuffle]), %%xmm7\n"
                        LOAD64U
                        "pshufb  %%xmm7, %%xmm0\n"
                        "pshufb  %%xmm7, %%xmm1\n"
                        "pshufb  %%xmm7, %%xmm2\n"
                        "pshufb  %%xmm7, %%xmm3\n"
                        STORE2X32
                        : : [dst1]"r"(&dstu[x]), [dst2]"r"(&dstv[x]), [src]"r"(&src[2*x]), [shuffle]"r"(shuffle) : "memory", "xmm0", "xmm1", "xmm2", "xmm3", "xmm7");
                }
            }
            else
            {
                for (; x < (width & ~31); x += 32)
                {
					__asm volatile (
                        "movdqu (%[mask]), %%xmm7\n"
                        LOAD64U
                        "movdqu   %%xmm0, %%xmm4\n"
                        "movdqu   %%xmm1, %%xmm5\n"
                        "movdqu   %%xmm2, %%xmm6\n"
                        "psrlw    $8,     %%xmm0\n"
                        "psrlw    $8,     %%xmm1\n"
                        "pand     %%xmm7, %%xmm4\n"
                        "pand     %%xmm7, %%xmm5\n"
                        "pand     %%xmm7, %%xmm6\n"
                        "packuswb %%xmm4, %%xmm0\n"
                        "packuswb %%xmm5, %%xmm1\n"
                        "pand     %%xmm3, %%xmm7\n"
                        "psrlw    $8,     %%xmm2\n"
                        "psrlw    $8,     %%xmm3\n"
                        "packuswb %%xmm6, %%xmm2\n"
                        "packuswb %%xmm7, %%xmm3\n"
                        STORE2X32
                        : : [dst2]"r"(&dstu[x]), [dst1]"r"(&dstv[x]), [src]"r"(&src[2*x]), [mask]"r"(mask) : "memory", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7");
                }
            }
        }

        for (; x < width; x++)
        {
            dstu[x] = src[2*x+0];
            dstv[x] = src[2*x+1];
        }
        src  += src_pitch;
        dstu += dstu_pitch;
        dstv += dstv_pitch;
    }
	_mm_mfence();

#undef STORE2X32
#undef LOAD64U
#undef LOAD64A
}
#endif /* ARCH_X86 */



#endif

#ifndef __MAX
#   define __MAX(a, b)   ( ((a) > (b)) ? (a) : (b) )
#endif
#ifndef __MIN
#   define __MIN(a, b)   ( ((a) < (b)) ? (a) : (b) )
#endif
#ifndef LINUX
# define vlc_memalign(align, size) (_aligned_malloc(size, align))
# define vlc_free(base)            (_aligned_free(base))
#else
# define vlc_memalign(align, size) memalign(align, size)
# define vlc_free(base) free(base)
#endif
int CopyInitCache(copy_cache_t *cache, unsigned width)
{
    cache->size = __MAX((width + 0x3f) & ~ 0x3f, 4096);
	cache->buffer = (unsigned char *)vlc_memalign(64, cache->size);

    return 0;
}
