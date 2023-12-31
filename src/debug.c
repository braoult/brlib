
/* debug.c - debug/log management
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

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#ifndef DEBUG_DEBUG
#define DEBUG_DEBUG
#endif

#include "debug.h"

static long long timer_start;                     /* in nanosecond */
static int level = 0;                             /* output log when < level */
static int flush = false;                         /* force flush after logs */
static FILE *stream = NULL;                       /* stream to use */

void debug_level_set(int _level)
{
    level = _level;
#   ifdef DEBUG_DEBUG_C
    log(0, "debug level set to %u\n", level);
#   endif
}

int debug_level_get(void)
{
    return level;
}

void debug_stream_set(FILE *_stream)
{
    stream = _stream;
#   ifdef DEBUG_DEBUG_C
    log(0, "stream set to %d\n", stream? fileno(stream): -1);
#   endif
}

void debug_flush_set(bool _flush)
{
    flush = _flush;
#   ifdef DEBUG_DEBUG_C
    log(0, "debug flush %s.\n", flush? "set": "unset");
#   endif
}

void debug_init(int _level, FILE *_stream, bool _flush)
{
    struct timespec timer;

    debug_stream_set(_stream);
    debug_level_set(_level);
    debug_flush_set(_flush);
    if (!clock_gettime(CLOCK_MONOTONIC, &timer)) {
        timer_start = timer.tv_sec * NANOSEC + timer.tv_nsec;
    }
    else {
        timer_start = 0;
    }
    log(0, "timer started.\n");
}

long long debug_timer_elapsed(void)
{
    struct timespec timer;

    clock_gettime(CLOCK_MONOTONIC, &timer);
    return (timer.tv_sec * NANOSEC + timer.tv_nsec) - timer_start;
}

void debug(int lev, bool timestamp, int indent, const char *src,
           int line, const char *fmt, ...)
{
    if (!stream || lev > level)
        return;

    va_list ap;

    if (indent)
        fprintf(stream, "%*s", 2*(indent-1), "");

    if (timestamp) {
        long long diff = debug_timer_elapsed();
        fprintf(stream, "%lld.%03lld ", diff/NANOSEC, (diff/1000000)%1000);
        fprintf(stream, "%010lld ", diff);
    }

    if (src) {
        if (line)
            fprintf(stream, "[%s:%u] ", src, line);
        else
            fprintf(stream, "[%s] ", src);
    }
    va_start(ap, fmt);
    vfprintf(stream, fmt, ap);
    va_end(ap);
    if (flush)
        fflush(stream);
}
