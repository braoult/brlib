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

    for (uint i = 0; i < sizeof(t32); ++i) {
        printf("popcount 32 (%#x): ", t32[i]);
#       ifdef ___popcount32_native
        printf("native:%d ", ___popcount32_native(t32[i]));
#       endif
        printf("emulated:%d\n", ___popcount_emulated(t32[i]));
    }
    for (uint i = 0; i < sizeof(t64); ++i) {
        printf("popcount 64 (%#lx): ", t64[i]);
#       ifdef ___popcount64_native
        printf("native:%d ", ___popcount64_native(t64[i]));
#       endif
        printf("emulated:%d\n", ___popcount_emulated(t64[i]));
    }
}

int main()
{
    test_popcount();
    exit(0);
}
