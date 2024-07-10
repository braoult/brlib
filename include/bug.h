/* bug.h - bug_on/warn_on/warn functions.
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

/* we allow multiple inclusions, so that BUG_ON/WARN_ON can change during
 * compilation.
 */
#ifdef BUG_H

#undef BUG_H

/* reset all macros */
#undef bug_on
#undef warn_on
#undef warn

#endif  /* BUG_H */

#define BUG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

#ifndef BUG_ON
#define BUG_ON 0
#endif
#ifndef WARN_ON
#define WARN_ON 0
#endif

#include "likely.h"

#if (BUG_ON + 0 == 1)
#define bug_on(expr) do {                                                      \
        if (unlikely(expr)) {                                                  \
            fprintf(stderr,                                                    \
                    "** BUG IN %s[%s:%d]: assertion \"" #expr "\" failed.\n",  \
                    __func__, __FILE__,__LINE__);                              \
            abort();                                                           \
        }                                                                      \
    } while (0)
#else
#define bug_on(expr)
#endif

#if (WARN_ON + 0 == 1)
#define warn_on(expr) ({                                                       \
        int _ret = !!(expr);                                                   \
        if (unlikely(_ret))                                                    \
            fprintf(stderr,                                                    \
                    "** WARN ON %s[%s:%d]: assertion \"" #expr "\" failed.\n", \
                    __func__, __FILE__,__LINE__);                              \
        _ret;                                                                  \
    })
#else
#define warn_on(expr) ({ 0; })
#endif

#define warn(expr, args...) ({                                                 \
        int _ret = !!(expr);                                                   \
        if (unlikely(_ret))                                                    \
            fprintf(stderr, ##args);                                           \
        _ret;                                                                  \
    })
