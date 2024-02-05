/* generic-ctz.h - generic ctz implementations.
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
#ifndef _GENERIC_CTZ_H_
#define _GENERIC_CTZ_H_

#include "brlib.h"

/* Adapted from: http://www-graphics.stanford.edu/%7Eseander/bithacks.html
 */
static __always_inline int __ctz32_emulated(u32 n)
{
    int r = 0;
    if (!(n & 0xffff)) {
        n >>= 16;
        r += 16;
    }
    if (!(n & 0xff)) {
        n >>= 8;
        r += 8;
    }
    if (!(n & 0xf)) {
        n >>= 4;
        r += 4;
    }
    if (!(n & 3)) {
        n >>= 2;
        r += 2;
    }
    if (!(n & 1)) {
        n >>= 1;
        r += 1;
    }
    return r;
}

#define __ctz32_emulated2(n) ({   \
    int r = 0;                    \
    if (!(n & 0xffff)) {          \
        n >>= 16;                 \
        r += 16;                  \
    }                             \
    if (!(n & 0xff)) {            \
        n >>= 8;                  \
        r += 8;                   \
    }                             \
    if (!(n & 0xf)) {             \
        n >>= 4;                  \
        r += 4;                   \
    }                             \
    if (!(n & 3)) {               \
        n >>= 2;                  \
        r += 2;                   \
    }                             \
    if (!(n & 1)) {               \
        n >>= 1;                  \
        r += 1;                   \
    }                             \
    r;                            \
})

static __always_inline int __ctz64_emulated(u64 n)
{
    int r = 0;                                    /* !!!!!!!!!!!!!!!!!!!!!!!! */
    if (!(n & 0xffffffff)) {
        n >>= 32;
        r += 32;
    }
    if (!(n & 0xffff)) {
        n >>= 16;
        r += 16;
    }
    if (!(n & 0xff)) {
        n >>= 8;
        r += 8;
    }
    if (!(n & 0xf)) {
        n >>= 4;
        r += 4;
    }
    if (!(n & 3)) {
        n >>= 2;
        r += 2;
    }
    if (!(n & 1)) {
        n >>= 1;
        r += 1;
    }
    return r;
}

#define __ctz64_emulated2(n) ({   \
    int r = 0;                  \
    if (!(n & 0xffffffff)) {      \
        n >>= 32;                 \
        r += 32;                  \
    }                             \
    if (!(n & 0xffff)) {          \
        n >>= 16;                 \
        r += 16;                  \
    }                             \
    if (!(n & 0xff)) {            \
        n >>= 8;                  \
        r += 8;                   \
    }                             \
    if (!(n & 0xf)) {             \
        n >>= 4;                  \
        r += 4;                   \
    }                             \
    if (!(n & 3)) {               \
        n >>= 2;                  \
        r += 2;                   \
    }                             \
    if (!(n & 1)) {               \
        n >>= 1;                  \
        r += 1;                   \
    }                             \
    r;                            \
})

#endif  /* _GENERIC_CTZ_H_ */
