/* debug.h - debug/log management.
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

#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdbool.h>

#include <br.h>

#define NANOSEC  1000000000                       /* nano sec in sec */
#define MILLISEC 1000000                          /* milli sec in sec */

#define _printf __attribute__ ((format (printf, 6, 7)))

#ifdef DEBUG_DEBUG

/**
 * debug_level_set - get debug level.
 * @level: integer debug level.
 *
 * @Return: void.
 */
void debug_level_set(int level);

/**
 * debug_level_get - get debug level.
 *
 * @Return: current level debug (integer).
 */
int debug_level_get(void);

/**
 * debug_stream_set - set debug output stream.
 * @stream: (FILE *) output stream
 *
 * @Return: void.
 */
void debug_stream_set(FILE *stream);

/**
 * debug_flush_set - set debug flush option.
 * @flush: boolean, if true, flush after each output.
 *
 * @Return: void.
 */
void debug_flush_set(bool flush);

/**
 * debug_init - init debug module.
 * @level: debug level (integer).
 * @stream: (FILE *) output stream
 * @flush: boolean, if true, flush after each output.
 *
 * @Return: void.
 */
void debug_init(int level, FILE *stream, bool flush);

/**
 * debug_timer_elapsed - get current elapsed debug timer value.
 *
 * @Return: long long, timer elapsed.
 */
long long debug_timer_elapsed(void);

/**
 * debug - log function.
 * @level: log level
 * @timestamp: boolean, print timestamp if true
 * @indent: indent level (2 spaces each)
 * @src: source file/func name (or NULL)
 * @line: line number
 *
 * @Return: void.
 */
void _printf debug(int level, bool timestamp,
                   int indent, const char *src,
                   int line, const char *fmt, ...);

#else  /* DEBUG_DEBUG */

static inline void debug_init(__unused int level,
                              __unused FILE *stream,
                              __unused bool flush) {}
static inline void debug_level_set(__unused int level) {}
static inline int debug_level_get(void) {return 0;}
static inline void debug_stream_set(__unused FILE *stream) {}
static inline long long debug_timer_elapsed(void) {return 0LL;}
static inline void debug_flush_set(__unused bool level) {}
static inline void _printf debug(__unused int level, __unused bool timestamp,
                                 __unused int indent, __unused const char *src,
                                 __unused int line, __unused const char *fmt, ...) {}

#endif  /* DEBUG_DEBUG */

#undef _printf

/**
 * log - simple log (no function name, no indent, no timestamp)
 * @level:      log level
 * @fmt:        printf format string
 * @args:       subsequent arguments to printf
 */
#define log(level, fmt, args...) \
    debug((level), false, 0, NULL, 0, fmt, ##args)

/**
 * log_i - log with indent (no function name, no timestamp)
 * @level:      log level
 * @fmt:        printf format string
 * @args:       subsequent arguments to printf
 *
 * Output example:
 * >>>>val=2
 */
#define log_i(level, fmt, args...) \
    debug((level), false, (level), NULL, 0, fmt, ##args)

/**
 * log_f - log with function name (no indent name, no timestamp)
 * @level:      log level
 * @fmt:        printf format string
 * @args:       subsequent arguments to printf
 *
 * Output example:
 * [function] val=2
 */
#define log_f(level, fmt, args...) \
    debug((level), false, 0, __func__, 0, fmt, ##args)

/**
 * log_if - log with function name and line number (no indent name, no timestamp)
 * @level:      log level
 * @fmt:        printf format string
 * @args:       subsequent arguments to printf
 *
 * Output example:
 * >>>> [function:15] val=2
 */
#define log_if(level, fmt, args...) \
    debug((level), false, (level), __func__, __LINE__, fmt, ##args)

/**
 * log_it - log with function name, line number, indent, and timestamp
 * @level:      log level
 * @fmt:        printf format string
 * @args:       subsequent arguments to printf
 *
 * Output example:
 * >>>> [function:15] val=2
 */
#define log_it(level, fmt, args...) \
    debug((level), true, (level), __func__, __LINE__, fmt, ##args)

#endif /* DEBUG_H */
