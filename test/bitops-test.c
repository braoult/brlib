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

#include "brlib.h"
#include "bitops.h"
#include "cutest/CuTest.h"

static const struct test32_1 {
    u32 t32;                                      /* input */
    u32 bswap;
    int popc;
    int ctz;
    int clz;
    int ffs;
    int ffz;
    int fls;
    int ilog2;
    struct {
        int size;
        uchar bfe[32];
    };
} test32_1[] = {
    { 0x00000000, 0x00000000, 0, 32, 32,  0, 1,  0,  0, /* sometimes undefined */
      { 0, { 0 } } },
    { 0xffffffff, 0xffffffff, 32,   0,  0,  1, 0, 32, 31,
      { 32, { 0,  1,  2,  3,  4,  5,  6,  7, 8,  9, 10, 11, 12, 13, 14, 15,
            16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 } } },
    { 0x00000001, 0x01000000,  1,   0, 31,  1, 2,  1,  0,
      { 1, { 0 } } },
    { 0x80000000, 0x00000080,  1,  31,  0, 32, 1, 32, 31,
      { 1, { 31 } } },
    { 0x71800718, 0x18078071, 10,   3,  1,  4, 1, 31, 30,
      /* 0111 0001 1000 0000 0000 0111 0001 1000 */
      { 10, { 3, 4, 8, 9, 10, 23, 24, 28, 29, 30 } } },
    { 0x07eeeef7, 0xf7eeee07, 22,   0,  5,  1, 4, 27, 26,
      /* 0000 0111 1110 1110 1110 1110 0111 1111*/
      { 22, { 0, 1, 2, 4, 5, 6, 7, 9, 10, 11, 13,
            14, 15, 17, 18, 19, 21, 22, 23, 24, 25, 26 } } },
};

static const struct test64_1 {
    u64 t64;                                      /* input */
    u64 bswap;
    int popc;                                     /* some values may be undefined */
    int ctz;
    int clz;
    int ffs;
    int ffz;
    int fls;
    int ilog2;
    struct {
        int size;
        uchar bfe[64];
    };
} test64_1[] = {
    { 0x0000000000000000, 0x0000000000000000,  0, 64, 64,  0, 1,  0,  0,
      { 0, { 0 } } },
    { 0xffffffffffffffff, 0xffffffffffffffff, 64,  0,  0,  1, 0, 64, 63,
      { 64, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
            32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
            48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63 } } },
    { 0x0000000100000001, 0x0100000001000000,  2,  0, 31,  1, 2, 33, 32,
      { 2, { 0, 32 } } },
    { 0x8000000000000000, 0x0000000000000080,  1, 63,  0, 64, 1, 64, 63,
      { 1, { 63 } } },
    { 0x7180071871800718, 0x1807807118078071, 20,  3,  1,  4, 1, 63, 62,
      /* 2 x 0111 0001 1000 0000 0000 0111 0001 1000 */
      { 20, { 3, 4, 8, 9, 10, 23, 24, 28, 29, 30,
            35, 36, 40, 41, 42, 55, 56, 60, 61, 62 } } },
    { 0x07eeeef707eeeef7, 0xf7eeee07f7eeee07, 44,  0,  5,  1, 4, 59, 58 ,
      /* 2 x 0000 0111 1110 1110 1110 1110 0111 1111*/
      { 44, { 0, 1, 2, 4, 5, 6, 7, 9, 10, 11, 13,
            14, 15, 17, 18, 19, 21, 22, 23, 24, 25, 26,
            32, 33, 34, 36, 37, 38, 39, 41, 42, 43, 45,
            46, 47, 49, 50, 51, 53, 54, 55, 56, 57, 58 } } },
};

static void cutest_popcount(CuTest *tc)
{
    for (uint i = 0; i < ARRAY_SIZE(test32_1); ++i) {
        int res = popcount32(test32_1[i].t32);
        CuAssertIntEquals(tc, test32_1[i].popc, res);
    }
    for (uint i = 0; i < ARRAY_SIZE(test64_1); ++i) {
        int res = popcount64(test64_1[i].t64);
        CuAssertIntEquals(tc, test64_1[i].popc, res);
    }
}

static void cutest_ctz(CuTest *tc)
{
    for (uint i = 0; i < ARRAY_SIZE(test32_1); ++i) {
        int res = ctz32(test32_1[i].t32);
        CuAssertIntEquals(tc, test32_1[i].ctz, res);
    }
    for (uint i = 0; i < ARRAY_SIZE(test64_1); ++i) {
        int res = ctz64(test64_1[i].t64);
        //printf("t=%#llx r=%d e=%d\n", test64_1[i].t64, res, test64_1[i].ctz);
        CuAssertIntEquals(tc, test64_1[i].ctz, res);
    }
}

static void cutest_clz(CuTest *tc)
{
    for (uint i = 0; i < ARRAY_SIZE(test32_1); ++i) {
        int res = clz32(test32_1[i].t32);
        //printf("clz t=%#x r=%d e=%d\n", test32_1[i].t32, res, test32_1[i].clz);
        CuAssertIntEquals(tc, test32_1[i].clz, res);
    }
    for (uint i = 0; i < ARRAY_SIZE(test64_1); ++i) {
        int res = clz64(test64_1[i].t64);
        //printf("clz t=%#llx r=%d e=%d\n", test64_1[i].t64, res, test64_1[i].clz);
        CuAssertIntEquals(tc, test64_1[i].clz, res);
    }
}

static void cutest_ffs(CuTest *tc)
{
    for (uint i = 0; i < ARRAY_SIZE(test32_1); ++i) {
        int res = ffs32(test32_1[i].t32);
        CuAssertIntEquals(tc, test32_1[i].ffs, res);
    }
    for (uint i = 0; i < ARRAY_SIZE(test64_1); ++i) {
        int res = ffs64(test64_1[i].t64);
        //printf("ffs64 t=%#llx r=%d e=%d\n", test64_1[i].t64, res, test64_1[i].ffs);
        CuAssertIntEquals(tc, test64_1[i].ffs, res);
    }
}

static void cutest_ffz(CuTest *tc)
{
    for (uint i = 0; i < ARRAY_SIZE(test32_1); ++i) {
        int res = ffz32(test32_1[i].t32);
        //printf("ffz32 t=%#x r=%d e=%d\n", test32_1[i].t32, res, test32_1[i].ffz);
        CuAssertIntEquals(tc, test32_1[i].ffz, res);
    }
    for (uint i = 0; i < ARRAY_SIZE(test64_1); ++i) {
        int res = ffz64(test64_1[i].t64);
        //printf("ffz64 t=%#llx r=%d e=%d\n", test64_1[i].t64, res, test64_1[i].ffz);
        CuAssertIntEquals(tc, test64_1[i].ffz, res);
    }
}

static void cutest_fls(CuTest *tc)
{
    for (uint i = 0; i < ARRAY_SIZE(test32_1); ++i) {
        int res = fls32(test32_1[i].t32);
        //printf("fls32 t=%#x r=%d e=%d\n", test32_1[i].t32, res, test32_1[i].fls);
        CuAssertIntEquals(tc, test32_1[i].fls, res);
    }
    for (uint i = 0; i < ARRAY_SIZE(test64_1); ++i) {
        int res = fls64(test64_1[i].t64);
        //printf("fls64 t=%#llx r=%d e=%d\n", test64_1[i].t64, res, test64_1[i].fls);
        CuAssertIntEquals(tc, test64_1[i].fls, res);
    }
}

static void cutest_bswap(CuTest *tc)
{
    for (uint i = 0; i < ARRAY_SIZE(test32_1); ++i) {
        int res = bswap32(test32_1[i].t32);
        //printf("fls32 t=%#x r=%d e=%d\n", test32_1[i].t32, res, test32_1[i].fls);
        CuAssertIntEquals(tc, test32_1[i].bswap, res);
    }
    for (uint i = 0; i < ARRAY_SIZE(test64_1); ++i) {
        int res = bswap64(test64_1[i].t64);
        //printf("fls64 t=%#llx r=%d e=%d\n", test64_1[i].t64, res, test64_1[i].fls);
        CuAssertIntEquals(tc, test64_1[i].bswap, res);
    }
}

static void cutest_ilog(CuTest *tc)
{
    for (uint i = 0; i < ARRAY_SIZE(test32_1); ++i) {
        if (!test32_1[i].t32) {
            //printf("ilog2_32 t=%#x skipped\n", test32_1[i].t32);
            continue;
        }
        int res = ilog2_32(test32_1[i].t32);
        //printf("ilog2_32_n t=%#x r=%d e=%d\n", test32_1[i].t32,
        //       res, test32_1[i].ilog2);
        CuAssertIntEquals(tc, test32_1[i].ilog2, res);
    }
    for (uint i = 0; i < ARRAY_SIZE(test64_1); ++i) {
        if (!test64_1[i].t64) {
            //printf("ilog2_32 t=%#llx skipped\n", test64_1[i].t64);
            continue;
        }
        int res = ilog2_64(test64_1[i].t64);
        //printf("ilog2_64_n t=%#llx r=%d e=%d\n", test64_1[i].t64,
        //       res, test64_1[i].ilog2);
        CuAssertIntEquals(tc, test64_1[i].ilog2, res);
    }
}

static void cutest_bfe(CuTest *tc)
{
    char s[64];

    for (uint i = 0; i < ARRAY_SIZE(test32_1); ++i) {
        u32 tmp;
        int cur, nb = 0;
        char bfe_ffs[32];

        /* create bit_for_each32_ffs values */
        for (int j = 0; j < test32_1[i].size; ++j)
            bfe_ffs[j] = test32_1[i].bfe[j] + 1;

        bit_for_each32(cur, tmp, test32_1[i].t32) {
            s[nb++] = cur;
        }
        CuAssertIntEquals(tc, test32_1[i].size, nb);
        CuAssertMemEquals(tc, test32_1[i].bfe, s, test32_1[i].size);
        nb = 0;
        bit_for_each32_ffs(cur, tmp, test32_1[i].t32) {
            s[nb++] = cur;
        }
        CuAssertMemEquals(tc, bfe_ffs, s, test32_1[i].size);
    }

    for (uint i = 0; i < ARRAY_SIZE(test64_1); ++i) {
        u64 tmp;
        int cur, nb = 0;
        char bfe_ffs[64];

        /* create bit_for_each64_ffs values */
        for (int j = 0; j < test64_1[i].size; ++j)
            bfe_ffs[j] = test64_1[i].bfe[j] + 1;

        bit_for_each64(cur, tmp, test64_1[i].t64) {
            s[nb++] = cur;
        }
        //printf("\n64 nb=%d size=%d\n", nb, test64_1[i].size);
        CuAssertIntEquals(tc, test64_1[i].size, nb);
        CuAssertMemEquals(tc, test64_1[i].bfe, s, test64_1[i].size);
        nb = 0;
        bit_for_each64_ffs(cur, tmp, test64_1[i].t64) {
            s[nb++] = cur;
        }
        CuAssertMemEquals(tc, bfe_ffs, s, test64_1[i].size);
    }
}

struct test32_2 {
    u32 t32;                                      /* input */
    u32 arg;
    u32 rol;
    u32 ror;
    struct {
        int size;
        uchar bfe[32];
    };
} test32_2[] = {
    { 0x00000000, 0,  0, 0,
      {  0, { 0 }  } },
    { 0x00000000, 1,  0, 0,
      {  0, { 0 }  } },
    { 0x10000001, 2,  0x40000004, 0x44000000,
      {  2, { 0, 28 } } },
    { 0x80000008, 3,  0x00000044, 0x10000001,
      {  2, { 3, 31 } } },
    { 0x71800718, 8,  0x80071871, 0x18718007,
      { 10, { 3, 4, 8, 9, 10, 23, 24, 28, 29, 30 } } },
    { 0x07eeeef7, 4,  0x7eeeef70, 0x707eeeef,
      { 22, { 0, 1, 2, 4, 5, 6, 7, 9, 10,
            11, 13, 14, 15, 17, 18, 19,
            21, 22, 23, 24, 25, 26 } } },
};

struct test64_2 {
    u64 t64;                                      /* input */
    u32 arg;
    u64 rol;
    u64 ror;
} test64_2[] = {
    { 0x0000000000000000, 0,  0, 0 },
    { 0x0000000000000000, 1,  0, 0 },
    { 0x1000000110000001, 2,  0x4000000440000004, 0x4400000044000000 },
    { 0x8000000880000008, 3,  0x0000004400000044, 0x1000000110000001 },
    { 0x7180071871800718, 8,  0x8007187180071871, 0x1871800718718007 },
    { 0x07eeeef707eeeef7, 4,  0x7eeeef707eeeef70, 0x707eeeef707eeeef }
};

static void cutest_rol(CuTest *tc)
{
    for (uint i = 0; i < ARRAY_SIZE(test32_2); ++i) {
        u32 res = rol32(test32_2[i].t32, test32_2[i].arg);
        //printf("rol32 t=%#x a=%u r=%#08x e=%#08x\n", test32_2[i].t32,
        //       test32_2[i].arg, res, test32_2[i].rol);
        CuAssertU32Equals(tc, test32_2[i].rol, res);
    }
    for (uint i = 0; i < ARRAY_SIZE(test64_2); ++i) {
        u64 res = rol64(test64_2[i].t64, test64_2[i].arg);
        //printf("rol64 t=%#llx a=%u r=%#08llx e=%#08llx\n", test64_2[i].t64,
        //       test64_2[i].arg, res, test64_2[i].rol);
        CuAssertU64Equals(tc, test64_2[i].rol, res);
    }
}

static void cutest_ror(CuTest *tc)
{
    for (uint i = 0; i < ARRAY_SIZE(test32_2); ++i) {
        u32 res = ror32(test32_2[i].t32, test32_2[i].arg);
        //printf("ror32 t=%#x a=%u r=%#08x e=%#08x\n", test32_2[i].t32,
        //       test32_2[i].arg, res, test32_2[i].ror);
        CuAssertU32Equals(tc, test32_2[i].ror, res);
    }
    for (uint i = 0; i < ARRAY_SIZE(test64_2); ++i) {
        u64 res = ror64(test64_2[i].t64, test64_2[i].arg);
        //printf("ror64 t=%#0llx a=%u r=%#08llx e=%#08llx\n", test64_2[i].t64,
        //       test64_2[i].arg, res, test64_2[i].ror);
        CuAssertU64Equals(tc, test64_2[i].ror, res);
    }
}

static CuSuite *bitops_GetSuite()
{
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, cutest_popcount);
    SUITE_ADD_TEST(suite, cutest_ctz);
    SUITE_ADD_TEST(suite, cutest_clz);
    SUITE_ADD_TEST(suite, cutest_ffs);
    SUITE_ADD_TEST(suite, cutest_ffz);
    SUITE_ADD_TEST(suite, cutest_fls);
    SUITE_ADD_TEST(suite, cutest_ilog);
    SUITE_ADD_TEST(suite, cutest_bfe);
    SUITE_ADD_TEST(suite, cutest_bswap);

    SUITE_ADD_TEST(suite, cutest_rol);
    SUITE_ADD_TEST(suite, cutest_ror);
    return suite;
}

static void RunAllTests(void)
{
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();
    CuSuiteAddSuite(suite, bitops_GetSuite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
}

int main()
{
    RunAllTests();
    exit(0);
}
