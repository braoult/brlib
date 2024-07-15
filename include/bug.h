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

#ifndef BUG_H
#define BUG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

#define bug_on_always(expr) do {                                      \
        if (expr) {                                                   \
            fprintf(stderr,                                           \
                    "** BUG ON %s[%s:%d]: assertion '%s' failed.\n",  \
                    __func__, __FILE__,__LINE__, #expr);                    \
            abort();                                                  \
            /* not reached */                                         \
        }                                                             \
    } while (0)

#define warn(expr, args...) ({                                        \
        int _ret = !!(expr);                                          \
        if (_ret)                                                     \
            fprintf(stderr, ##args);                                  \
        _ret;                                                         \
    })

#define warn_on_always(expr) ({                                       \
            warn(expr,                                                \
                 "** WARN ON %s[%s:%d]: assertion '%s' failed.\n",    \
                 __func__, __FILE__,__LINE__, #expr);                       \
        })

#ifdef BUG_ON

#define bug_on(expr)          bug_on_always(expr)

#define warn_on(expr)         warn_on_always(expr)
#define warn_on_or_eval(expr) warn_on(expr)

#else

#define bug_on(expr)

#define warn_on(expr)
#define warn_on_or_eval(expr) (expr)

#endif  /* BUG_ON */

#endif /* BUG_H */
