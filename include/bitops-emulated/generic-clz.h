/* generic-clz.h - generic clz implementations.
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
#ifndef _GENERIC_CLZ_H_
#define _GENERIC_CLZ_H_

#include "brlib.h"

/* Adapted from: http://www-graphics.stanford.edu/%7Eseander/bithacks.html
 */
static __always_inline int __clz32_emulated(u32 n)
{
    uint r, q;

    r  = (n > 0xFFFF) << 4; n >>= r;
    q  = (n > 0xFF  ) << 3; n >>= q; r |= q;
    q  = (n > 0xF   ) << 2; n >>= q; r |= q;
    q  = (n > 0x3   ) << 1; n >>= q; r |= q;
    r |= (n >> 1);
    return 31 - r;
}

static __always_inline int __clz64_emulated(u64 n)
{
    uint r, q;

    r = (n > 0xFFFFFFFF) << 5; n >>= r;
    q = (n > 0xFFFF)     << 4; n >>= q; r |= q;
    q = (n > 0xFF  )     << 3; n >>= q; r |= q;
    q = (n > 0xF   )     << 2; n >>= q; r |= q;
    q = (n > 0x3   )     << 1; n >>= q; r |= q;
    r |= (n >> 1);
    return 63 - r;
}

#endif  /* _GENERIC_CLZ_H_ */
