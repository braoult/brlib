/* bits.c - bitops testing.
 *
 * Copyright (C) 2024 Bruno Raoult ("br")
 * Licensed under the GNU General Public License v3.0 or later.
 * Some rights reserved. See COPYING.
 *
 * You should have received a copy of the GNU General Public License along with this
 * program. If not, see <https://www.gnu.org/licenses/gpl-3.0-standalone.html>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later <https://spdx.org/licenses/GPL-3.0-or-later.html>
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "br.h"
#include "bitops.h"
// #include "cutest/CuTest.h"

static void test_popcount()
{
    u32 t32[] = { 0x0,   0x88000101,           0xffffffff           };
    u64 t64[] = { 0x0ll, 0x8880000000000101LL, 0xffffffffffffffffll };

    for (uint i = 0; i < ARRAY_SIZE(t32); ++i) {
        printf("popcount32 (%#x): ", t32[i]);
#       ifdef ___popcount32_native
        printf("native:%d ", __popcount32_native(t32[i]));
#       else
        printf("native:XXX ");
#       endif
        printf("emulated:%d ", __popcount_emulated(t32[i]));
        printf("\n");
    }
        printf("\n");
    for (uint i = 0; i < ARRAY_SIZE(t64); ++i) {
        printf("popcount64 (%#lx): ", t64[i]);
#       ifdef ___popcount64_native
        printf("native:%d ", __popcount64_native(t64[i]));
#       else
        printf("native:XXX ");
#       endif
        printf("emulated:%d ", __popcount_emulated(t64[i]));
        printf("\n");
    }
    printf("\n");
}

static void test_ctz()
{
    u32 t32[] = {
        0x88800101,
        0xffffffff,
        0x800,
        0x80000000,
        0x00800000
    };
    u64 t64[] = {
        0x8880000000000101LL,
        0xffffffffffffffffll,
        0x800ll,
        0x8000000000000000LL,
        0x0080000000000000LL};

    for (uint i = 0; i < ARRAY_SIZE(t32); ++i) {
        printf("ctz32 (%#x): ", t32[i]);
#       ifdef __ctz32_native
        printf("native:%d ", __ctz32_native(t32[i]));
#       else
        printf("native:XXX ");
#       endif
        printf("emulated1:%d ", __ctz32_emulated(t32[i]));
        printf("emulated2:%d ", __ctz32_emulated2(t32[i]));
        //printf("emulated3:%d ", __ctz32_emulated3(t32[i]));
        //printf("emulated4:%d ", __ctz32_emulated4(t32[i]));
        printf("\n");
    }
    printf("\n");
    for (uint i = 0; i < ARRAY_SIZE(t64); ++i) {
        printf("ctz64 (%#lx): ", t64[i]);
#       ifdef __ctz64_native
        printf("native:%d ", __ctz64_native(t64[i]));
#       else
        printf("native:XXX ");
#       endif
        printf("emulated1:%d ", __ctz64_emulated(t64[i]));
        printf("emulated2:%d ", __ctz64_emulated2(t64[i]));
        //printf("emulated3:%d ", __ctz64_emulated3(t64[i]));
        //printf("emulated4:%d ", __ctz64_emulated4(t64[i]));
        printf("\n");
    }
    printf("\n");
}

static void test_clz()
{
    u32 t32[] = {
        0x88800101,
        0xffffffff,
        0x800,
        0x80000000,
        0x00800000
    };
    u64 t64[] = {
        0x8880000000000101LL,
        0xffffffffffffffffll,
        0x800ll,
        0x8000000000000000LL,
        0x0080000000000000LL};

    for (uint i = 0; i < ARRAY_SIZE(t32); ++i) {
        printf("clz32 (%#x): ", t32[i]);
#       ifdef __clz32_native
        printf("native:%d ", __clz32_native(t32[i]));
#       else
        printf("native:XXX ");
#       endif
        printf("emulated1:%d ", __clz32_emulated(t32[i]));
        //printf("emulated2:%d ", __clz32_emulated2(t32[i]));
        //printf("emulated3:%d ", __ctz32_emulated3(t32[i]));
        //printf("emulated4:%d ", __ctz32_emulated4(t32[i]));
        printf("\n");
    }
    printf("\n");
    for (uint i = 0; i < ARRAY_SIZE(t64); ++i) {
        printf("clz64 (%#lx): ", t64[i]);
#       ifdef __clz64_native
        printf("native:%d ", __clz64_native(t64[i]));
#       else
        printf("native:XXX ");
#       endif
        printf("emulated1:%d ", __clz64_emulated(t64[i]));
        //printf("emulated2:%d ", __ctz64_emulated2(t64[i]));
        //printf("emulated3:%d ", __ctz64_emulated3(t64[i]));
        //printf("emulated4:%d ", __ctz64_emulated4(t64[i]));
        printf("\n");
    }
    printf("\n");
}

static void test_ffs()
{
    u32 t32[] = {
        0x88800101,
        0xffffffff,
        0x800,
        0x80000000,
        0x00800000
    };
    u64 t64[] = {
        0x8880000000000101LL,
        0xffffffffffffffffll,
        0x800ll,
        0x8000000000000000LL,
        0x0080000000000000LL};

    for (uint i = 0; i < ARRAY_SIZE(t32); ++i) {
        printf("ffs32 (%#x): ", t32[i]);
#       ifdef __ffs32_native
        printf("native:%d ", __ffs32_native(t32[i]));
#       else
        printf("native:XXX ");
#       endif
        printf("popcount:%d ", __ffs32_popcount(t32[i]));
        printf("ctz:%d ", __ffs32_ctz(t32[i]));
        printf("emulated:%d ", __ffs32_emulated(t32[i]));
        //printf("emulated4:%d ", __ctz32_emulated4(t32[i]));
        printf("\n");
    }
    printf("\n");
    for (uint i = 0; i < ARRAY_SIZE(t64); ++i) {
        printf("ffs64 (%#lx): ", t64[i]);
#       ifdef __ffs64_native
        printf("native:%d ", __ffs64_native(t64[i]));
#       else
        printf("native:XXX ");
#       endif
        printf("popcount:%d ", __ffs64_popcount(t64[i]));
        printf("ctz:%d ", __ffs64_ctz(t64[i]));
        printf("emulated:%d ", __ffs64_emulated(t64[i]));
        //printf("emulated4:%d ", __ctz64_emulated4(t64[i]));
        printf("\n");
    }
    printf("\n");
    printf("\n");
}

int main()
{
    test_popcount();
    test_ctz();
    test_clz();
    test_ffs();
    exit(0);
}
