/* bitops.h - bits functions.
 *
 * Copyright (C) 2021-2024 Bruno Raoult ("br")
 * Licensed under the GNU General Public License v3.0 or later.
 * Some rights reserved. See COPYING.
 *
 * You should have received a copy of the GNU General Public License along with this
 * program. If not, see <https://www.gnu.org/licenses/gpl-3.0-standalone.html>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later <https://spdx.org/licenses/GPL-3.0-or-later.html>
 *
 */
#ifndef _BITS_H
#define _BITS_H

#include "br.h"
#include "bitops-emulated/generic-ctz.h"
#include "bitops-emulated/generic-clz.h"

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

/* determine which native builtins are available
 */
#if __has_builtin(__builtin_popcount)
#   define HAS_POPCOUNT
#endif
#if __has_builtin(__builtin_ctz)
#   define HAS_CTZ
#endif
#if __has_builtin(__builtin_clz)
#   define HAS_CLZ
#endif
#if __has_builtin(__builtin_ffs)
#   define HAS_FFS
#endif


/**
 * print_bitops_impl() - print bitops implementation.
 *
 * For basic bitops (popcount, ctz, etc...), print the implementation
 * (builtin, emulated, ...).
 */
void print_bitops_impl(void);

/* no plan to support 32bits for now...
 * #if __WORDSIZE != 64
 * #error "Only 64 bits word size supported."
 * #endif
 */

/**
 * lsb, msb: 0-indexed least/most significant bit: 10101000
 *                                         msb = 7 ^   ^ lsb = 3
 *
 */
#define lsb64(x) (ctz64(x))
#define lsb32(x) (ctz32(x))
#define msb64(x) (63 ^ clz64(x))
#define msb32(x) (31 ^ clz32(x))

/**
 * popcount32, popcout64 - count set bits:  10101000 -> 3
 * @n: unsigned 32 or 64 bits integer.
 *
 */
#if defined(HAS_POPCOUNT)
#   define __popcount32_native(n) __builtin_popcount(n)
#   define __popcount64_native(n) __builtin_popcountll(n)

#   define popcount64(n) __popcount64_native(n)
#   define popcount32(n) __popcount32_native(n)

/* see ctz section below */
#   define __ctz32_popcount(n) (popcount(n & -n) - 1)
#   define __ctz64_popcount(n) (popcountll(n & -n) - 1)

/* see ffs section below */
#   define __ffs32_popcount(n) (__builtin_popcount((n) ^ ~-(n)))
#   define __ffs64_popcount(n) (__builtin_popcountll((n) ^ ~-(n)))

#endif
/* Brian Kernighan's algorithm - pretty efficient for likely sparse values
 */
#define __popcount_emulated(n)  ({      \
    int ___count = 0;                   \
    while (n) {                         \
        ___count++;                     \
        n &= (n - 1);                   \
    }                                   \
    ___count; })
#if !defined(popcount32)
#   define popcount32(n) __popcount_emulated(n)
#endif
#if !defined(popcount64)
#   define popcount64(n) __popcount_emulated(n)
#endif

/**
 * ctz32, ctz64 - count trailing zeros: 00101000 -> 3
 * @n: unsigned 32 or 64 bits integer.
 *
 * Not defined if no bit set, so check for non-zero before calling this.
 * This is similat the FFS (First Find Set), which has FFS(0) = 0.
 */
#if defined(HAS_CTZ)
#   define __ctz32_native(n)  __builtin_ctz(n)
#   define __ctz64_native(n)  __builtin_ctzll(n)
#   define ctz32(n) __ctz32_native(n)
#   define ctz64(n) __ctz64_native(n)

/* see ffs section below */
#   define __ffs32_ctz(n) ({ n? __builtin_ctz(n) + 1: 0; })
#   define __ffs64_ctz(n) ({ n? __builtin_ctzll(n) + 1: 0; })
#endif
#if !defined(ctz32) && defined(__ctz32_popcount)
#   define ctz32(n) __ctz32_popcount(n)
#   define ctz64(n) __ctz64_popcount(n)
#endif
#if !defined(ctz32)
#   define ctz32(n) __ctz32_emulated(n)
#   define ctz64(n) __ctz64_emulated(n)
#endif

/**
 * clz32, clz64 - count leading zeros: 00101000 -> 2
 *
 * @n: unsigned 32 or 64 bits integer.
 *
 * Not defined if no bit set, so check for non-zero before calling this.
 */
#if defined (HAS_CLZ)
#   define __clz32_native(n)  __builtin_clz(n)
#   define __clz64_native(n)  __builtin_clzll(n)
#   define clz32(n) __clz32_native(n)
#   define clz64(n) __clz64_native(n)
#endif
#if !defined(clz32)
#   define clz32(n) __clz32_emulated(n)
#   define clz64(n) __clz64_emulated(n)
#endif

/**
 * ffs32, ffs64 - find first bit set, indexed from 0: 00101000 -> 4
 * ffz32, ffz64 - find first bit unset, indexed from 0: 00101010 -> 2
 * @n: unsigned 32 or 64 bits integer.
 *
 * ffs(n) is similar to ctz(n) + 1, but returns 0 if n == 0 (except
 * for ctz version, where ffs(0) is undefined).
 * ffz(n) is ffz(~n), with undefine value if n = 0.
 */
#if defined(HAS_FFS)
#   define __ffs32_native(n)  __builtin_ffs(n)
#   define __ffs64_native(n)  __builtin_ffsll(n)
#   define ffs32(n) __ffs32_native(n)
#   define ffs64(n) __ffs64_native(n)
#endif
#define __ffs32_emulated(n) (popcount32((n) ^ ~-(n)))
#define __ffs64_emulated(n) (popcount64((n) ^ ~-(n)))
#if !defined(ffs32) && defined(__ffs32_popcount)
#   define ffs32(n) __ffs32_popcount(n)
#   define ffs64(n) __ffs64_popcount(n)
#endif
#if !defined(ffs32) && defined(__ffs32_ctz)
#   define ffs32(n) __ffs32_ctz(n)
#   define ffs64(n) __ffs64_ctzll(n)
#endif
#if !defined(ffs32)
#   define ffs32(n) __ffs32_emulated(n)
#   define ffs64(n) __ffs64_emulated(n)
#endif
#define ffz32(n)  ffs32(~(n))
#define ffz64(n)  ffs64(~(n))

/**
 * fls32, fls64 - return one plus MSB index: 00101000 -> 6
 * @n: unsigned 32 or 64 bits integer.
 *
 * Similar to nbits(n) - clz(n), but returns 0 if n == 0;
 */
#define fls32(n) ((n)? 32 - clz32(n): 0)
#define fls64(n) ((n)? 64 - clz64(n): 0)

/*
 * rol32, rot64 - rotate left
 * @num: unsigned 8, 16, 32 or 64 bits integer
 * @n: bits to roll
 * See: https://stackoverflow.com/a/31488147/3079831
 */
#define rol8(num, n)  ((num << (n &  7)) | (num >> ((-n) &  7)))
#define rol16(num, n) ((num << (n & 15)) | (num >> ((-n) & 15)))
#define rol32(num, n) ((num << (n & 31)) | (num >> ((-n) & 31)))
#define rol64(num, n) ((num << (n & 63)) | (num >> ((-n) & 63)))

/**
 * ror8, ror16, ror32, ror64 - rotate right
 * @num: unsigned 8, 16, 32 or 64 bits integer
 * @n: bits to roll
 */
#define ror8(num, n)  ((num >> (n &  7)) | (num << ((-n) &  7)))
#define ror16(num, n) ((num >> (n & 15)) | (num << ((-n) & 15)))
#define ror32(num, n) ((num >> (n & 31)) | (num << ((-n) & 31)))
#define ror64(num, n) ((num >> (n & 63)) | (num << ((-n) & 63)))

/**
 * ilog2_32, ilog2_64 - log base 2
 * @n: unsigned 32 or 64 bits integer.
 *
 * Undefine value if n = 0.
 */
#define ilog2_32(n) (msb32(n))
#define ilog2_64(n) (msb64(n))

/**
 * is_pow2() - check if number is a power of two
 * @n: the value to check
 *
 * Zero is *not* considered a power of two.
 */
#define is_pow2(n) (n != 0 && (((n) & ((n) - 1)) == 0))

/**
 * bit_for_eachXX - iterate over an integer bits (0-indexed)
 * bit_for_eachXX_ffs - iterate over an integer bits (1-indexed)
 * @pos:  int used as current bit
 * @tmp:  temp u64/u32 used as temporary storage
 * @u:    u32/u64 to loop over
 *
 * Bits are 0-indexed from 0 with bit_for_each, and 1-indexed with
 * bits_for_each_ffs.
 *
 * Example:
 * u64 u=139, _t;           // u=b10001011
 * int cur;
 * bit_for_each64(cur, _t, u) {
 *     printf("%d\n", cur);
 * }
 * This will display the position of each bit set in ul: 0, 1, 3, 7
 *
 */
#define bit_for_each32(pos, tmp, u)                  \
    for (tmp = u, pos = ctz32(tmp);                  \
         tmp;                                        \
         tmp ^= 1U << pos, pos = ctz32(tmp))
#define bit_for_each64(pos, tmp, ul)                 \
    for (tmp = ul, pos = ctz64(tmp);                 \
         tmp;                                        \
         tmp ^= 1UL << pos, pos = ctz64(tmp))
#define bit_for_each64_ffs(pos, tmp, u)              \
    for (tmp = u, pos = ffs64(tmp);                  \
         tmp;                                        \
         tmp &= (tmp - 1),  pos = ffs64(tmp))
#define bit_for_each32_ffs(pos, tmp, ul)             \
    for (tmp = ul, pos = ffs32(tmp);                 \
         tmp;                                        \
         tmp &= (tmp - 1),  pos = ffs32(tmp))

#endif  /* _BITS_H */
