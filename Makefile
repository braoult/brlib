# brlib Makefile - GNU make only
#
# Copyright (C) 2021-2024 Bruno Raoult ("br")
# Licensed under the GNU General Public License v3.0 or later.
# Some rights reserved. See COPYING.
#
# You should have received a copy of the GNU General Public License along with this
# program. If not, see <https://www.gnu.org/licenses/gpl-3.0-standalone.html>.
#
# SPDX-License-Identifier: GPL-3.0-or-later <https://spdx.org/licenses/GPL-3.0-or-later.html>
#

SHELL     := /bin/bash
CC        := gcc
LD        := ld
BEAR      := bear
TOUCH     := touch
RM        := rm
MKDIR     := mkdir -p
RMDIR     := rmdir

SRCDIR    := ./src
INCDIR    := ./include
OBJDIR    := ./obj
LIBDIR    := ./lib
BINDIR    := ./bin
DEPDIR    := ./dep
TESTDIR   := ./test

SRC       := $(wildcard $(SRCDIR)/*.c)                      # brlib sources
SRC_FN    := $(notdir $(SRC))                               # source basename
OBJ       := $(addprefix $(OBJDIR)/,$(SRC_FN:.c=.o))

LIB       := br_$(shell uname -m)                           # library name
SLIB      := $(addsuffix .a, $(LIBDIR)/lib$(LIB))           # static lib
DLIB      := $(addsuffix .so, $(LIBDIR)/lib$(LIB))          # dynamic lib

DEP_FN    := $(SRC_FN) $(LIBSRC_FN)
DEP       := $(addprefix $(DEPDIR)/,$(DEP_FN:.c=.d))

TEST      := $(wildcard $(TESTDIR)/*.c)
TEST_FN   := $(notdir $(TEST))
BIN       := $(addprefix $(BINDIR)/,$(TEST_FN:.c=))

CCLSCMDS  := compile_commands.json

##################################### pre-processor flags
CPPFLAGS  := -I $(INCDIR)
#CPPFLAGS  += -DDEBUG                                       # global
#CPPFLAGS  += -DDEBUG_DEBUG_C                               # log() funcs debug
CPPFLAGS  += -DDEBUG_DEBUG                                  # activate logs funcs
CPPFLAGS  += -DDEBUG_POOL                                   # mem pools

# remove extraneous spaces (due to spaces before comments)
CPPFLAGS  := $(strip $(CPPFLAGS))

##################################### compiler flags
CFLAGS    := -std=gnu11
CFLAGS    += -O3
CFLAGS    += -g
CFLAGS    += -Wall
CFLAGS    += -Wextra
CFLAGS    += -march=native
CFLAGS    += -Wmissing-declarations
CFLAGS    += -Wno-unused-result
CFLAGS    += -fPIC
# for gprof
#CFLAGS    += -pg
# Next one may be useful for valgrind (some invalid instructions)
# CFLAGS   += -mno-tbm

CFLAGS    := $(strip $(CFLAGS))

##################################### archiver/linker/dependency flags
ARFLAGS   := rcs
LDFLAGS   := -L$(LIBDIR)
LIBS      := -l$(LIB)
DEPFLAGS   = -MMD -MP -MF $(DEPDIR)/$*.d

##################################### General targets
.PHONY: all libs compile test emacs ccls bear clean cleanall cleanallall

# default: build libraries
all: libs

# default: build libraries
libs: $(DLIB) $(SLIB)

# build objects
compile: $(OBJ)

# build test binaries
test: $(BIN)

# setup emacs projectile/ccls
emacs: $(PRJROOT) $(EMACSLSP)

# update compile-commands.json
ccls bear: $(CCLSCMDS)

# cleanup - need to think about a better system :-)
clean: cleandep cleanobj cleanlib cleanbin
cleanall: clean cleandepdir cleanobjdir cleanlibdir cleanbindir
cleanallall: cleanall cleanemacs

##################################### cleaning functions
# rmfiles - deletes a list of files in a directory if they exist.
# $(1): the directory
# $(2): the list of files to delete
# $(3): The string to include in action output - "cleaning X files."
# see: https://stackoverflow.com/questions/6783243/functions-in-makefiles
#
# Don't use wildcard like "$(DIR)/*.o", so we can control mismatches between
# list and actual files in directory.
# See rmdir below.
define rmfiles
	@#echo "rmfiles=+$(1)+"
	$(eval $@_EXIST = $(wildcard $(1)))
	@#echo "existfile=+${$@_EXIST}+"
	@if [[ -n "${$@_EXIST}" ]]; then         \
		echo "cleaning $(2) files." ;    \
		$(RM) ${$@_EXIST} ;              \
	fi
endef

# rmdir - deletes a directory if it exists.
# $(1): the directory
# $(2): The string to include in action output - "removing X dir."
#
# Don't use $(RM) -rf, to control unexpected dep files.
# See rmfile above.
define rmdir
	@#echo "rmdir +$(1)+"
	$(eval $@_EXIST = $(wildcard $(1)))
	@#echo "existdir=+${$@_EXIST}+"
	@if [[ -n "${$@_EXIST}" ]]; then         \
		echo "removing $(2) dir." ;      \
		$(RMDIR) ${$@_EXIST} ;           \
	fi
endef

##################################### dirs creation
.PHONY: alldirs

ALLDIRS   := $(DEPDIR) $(OBJDIR) $(LIBDIR) $(BINDIR)

alldirs: $(ALLDIRS)

# Here, we have something like:
# a: a
# a will be built if (1) older than a, or (2) does not exist. Here only (2).
$(ALLDIRS): $@
	@echo creating $@ directory.
	@$(MKDIR) $@

##################################### Dependencies files
.PHONY: cleandep cleandepdir

-include $(wildcard $(DEP))

# Don't use $(DEPDIR)/*.d, to control mismatches between dep and src files.
# See second rule below.
cleandep:
	$(call rmfiles,$(DEP),depend)
	@#echo cleaning dependency files.
	@#$(RM) -f $(DEP)

cleandepdir:
	$(call rmdir,$(DEPDIR),depend)
	@#[ -d $(DEPDIR) ] && echo cleaning depend files && $(RM) -f $(DEP) || true

##################################### objects
.PHONY: cleanobj cleanobjdir

cleanobj:
	$(call rmfiles,$(OBJ),brlib object)

cleanobjdir:
	$(call rmdir,$(OBJDIR),brlib objects)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR) $(DEPDIR)
	@echo compiling $< "->" $@.
	@$(CC) -c $(DEPFLAGS) $(CPPFLAGS) $(CFLAGS) $< -o $@

##################################### brlib libraries
.PHONY: cleanlib cleanlibdir

cleanlib:
	$(call rmfiles,$(DLIB) $(SLIB),library)

cleanlibdir:
	$(call rmdir,$(LIBDIR),libraries)

#$(DLIB): CFLAGS += -fPIC
$(DLIB): LDFLAGS += -shared
$(DLIB): $(OBJ) | $(LIBDIR)
	@echo "building shared library ($@)."
	@$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(SLIB): $(OBJ) | $(LIBDIR)
	@echo "building static library ($@)."
	@$(AR) $(ARFLAGS) $@ $? > /dev/null

##################################### testing
.PHONY: cleanbin cleanbindir

CUTESTSRC  := $(TESTDIR)/cutest/CuTest.c

cleanbin:
	$(call rmfiles,$(BIN),binary)

cleanbindir:
	$(call rmdir,$(BINDIR),binaries)

$(BINDIR)/%: $(TESTDIR)/%.c $(SLIB) $(DLIB) | $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(CUTESTSRC) $(LDFLAGS) $(LIBS) -o $@

##################################### pre-processed (.i) and assembler (.s) output
%.i: %.c
	@echo generating $@
	@$(CC) -E $(CPPFLAGS) $(CFLAGS) $< -o $@

%.s: %.c
	@echo generating $@
	@$(CC) -S -fverbose-asm $(CPPFLAGS) $(CFLAGS) $< -o $@

##################################### Emacs & LSP (ccls)
.PHONY: cleanemacs cleanccls

#EMACSLSP   := .dir-locals.el
CCLSFILES  := .projectile .ccls .ccls-root
CCLSCACHE  := .ccls-cache

cleanemacs:
	$(call rmfiles, $(PRJROOT) $(EMACSLSP), Emacs/ccls);

$(CCLSFILES):
	printf "creating Emacs dot-files.\n"
	@$(TOUCH) $@

cleanccls:
	$(call rmfiles, $(CCLSFILES), ccls);

# generate compile_commands.json.
# TODO: add includes and Makefile dependencies.
# also, if cclsfile is newer than sources, no need to clean objects file
# (and to run bear).
# maybe run cleanobj cleanlibobj in commands ?
$(CCLSCMDS): cleanobj $(SRC) | $(CCLSROOT)
	@echo "Generating ccls compile commands file ($@)."
	@$(BEAR) -- $(MAKE) test

##################################### valgrind (mem check)
.PHONY: memcheck

VALGRIND      := valgrind
VALGRINDFLAGS := --leak-check=full --show-leak-kinds=all
VALGRINDFLAGS += --track-origins=yes --sigill-diagnostics=yes
VALGRINDFLAGS += --quiet --show-error-list=yes
VALGRINDFLAGS += --log-file=valgrind.out
# We need to suppress libreadline leaks here. See :
# https://stackoverflow.com/questions/72840015
VALGRINDFLAGS += --suppressions=etc/libreadline.supp

memcheck: targets
	@$(VALGRIND) $(VALGRINDFLAGS) $(BINDIR)/brchess

##################################### Makefile debug
.PHONY: showflags wft

showflags:
	@echo CFLAGS: "$(CFLAGS)"
	@echo CPPFLAGS: $(CPPFLAGS)
	@echo DEPFLAGS: $(DEPFLAGS)
	@echo LDFLAGS: $(LDFLAGS)

wtf:
	@printf "OBJDIR=%s\n\n" "$(OBJDIR)"
	@printf "OBJ=%s\n\n" "$(OBJ)"
