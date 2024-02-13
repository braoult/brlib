/* generic-bswap.h - generic bswap implementations.
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
#ifndef _GENERIC_BSWAP_H_
#define _GENERIC_BSWAP_H_

#include "brlib.h"

/* Adapted from: http://www-graphics.stanford.edu/%7Eseander/bithacks.html
 */
static __always_inline u32 __bswap32_emulated(u32 n)
{
   const u32 k = 0x00FF00FF;
   n = ((n >>  8) & k) | ((n & k) <<  8);
   n = ( n >> 16)      | ( n      << 16);
   return n;
}

static __always_inline u64 __bswap64_emulated(u64 n)
{
   const u64 k1 = 0x00FF00FF00FF00FFull;
   const u64 k2 = 0x0000FFFF0000FFFFull;
   n = ((n >>  8) & k1) | ((n & k1) <<  8);
   n = ((n >> 16) & k2) | ((n & k2) << 16);
   n = ( n >> 32)       | ( n       << 32);
   return n;
}

#endif  /* _GENERIC_BSWAP_H_ */
