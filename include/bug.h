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

#ifndef _BUG_H
#define _BUG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "likely.h"

#define bug_on(expr) do {                                                      \
        if (unlikely(expr)) {                                                  \
            fprintf(stderr,                                                    \
                    "** BUG IN %s[%s:%d]: assertion \"" #expr "\" failed.\n",  \
                    __func__, __FILE__,__LINE__);                              \
            abort();                                                           \
        }                                                                      \
    } while (0)

#define warn_on(expr) ({                                                       \
        int _ret = !!(expr);                                                   \
        if (unlikely(_ret))                                                    \
            fprintf(stderr,                                                    \
                    "** WARN ON %s[%s:%d]: assertion \"" #expr "\" failed.\n", \
                    __func__, __FILE__,__LINE__);                              \
         unlikely(_ret);                                                       \
    })

#define warn(expr, format...) ({                                               \
        int _ret = !!(expr);                                                   \
        if (unlikely(_ret))                                                    \
            vfprintf(stderr, ##format);                                        \
        unlikely(_ret);                                                        \
    })

#endif /* _BUG_H */
