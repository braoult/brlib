#!/usr/bin/env bash
#
# env.sh - set environment for brchess developer.
#
# Copyright (C) 2023 Bruno Raoult ("br")
# Licensed under the GNU General Public License v3.0 or later.
# Some rights reserved. See COPYING.
#
# You should have received a copy of the GNU General Public License along with this
# program. If not, see <https://www.gnu.org/licenses/gpl-3.0-standalone.html>.
#
# SPDX-License-Identifier: GPL-3.0-or-later <https://spdx.org/licenses/GPL-3.0-or-later.html>
#
# USAGE: source env.sh [arg]
#
# This file will actually be sourced if it was never sourced in current bash
# environment.

if [[ ! -v _BRLIB_ENV_ ]]; then
    export _BRLIB_ENV_=1 BRLIB_ROOT LD_LIBRARY_PATH
    BRLIB_SCRIPTDIR=$(realpath -L "$(dirname "${BASH_SOURCE[0]}")")
    BRLIB_ROOT=$(realpath -L "$(dirname "${BASH_SOURCE[0]}")/..")
    BRLIB_LIBDIR="$BRLIB_ROOT/lib"
    BRLIB_BINDIR="$BRLIB_ROOT/bin"
    LD_LIBRARY_PATH="${BRLIB_LIBDIR}${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
    PATH="$PATH:$BRLIB_BINDIR:$BRLIB_SCRIPTDIR"
    #printf "R=%s L=%s LD=%s\n" "$BRLIB_ROOT" "$BRLIB_DIR" "$LD_LIBRARY_PATH"
    unset BRLIB_LIBDIR BRLIB_BINDIR BRLIB_SCRIPTDIR
    printf "brlib environment complete.\n"
fi
