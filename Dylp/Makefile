# Makefile for the dylp library, libDylp.

# svn/cvs: $Id$

# LibType, OptLevel, DYLP_OPTIONS, and LIBNAME are normally set in the top
# level makefile. (Which see for explanations of what they do.) You can
# override the general setting here, if you want. Each variable is
# conditionally set with `?='. This means the setting here will have an effect
# only if the variable is undefined (as, for example, if you run make in this
# directory, rather than from the top level). To override the top level
# setting, change `?=' to `:='.

export LibType ?= SHARED
export OptLevel ?= -O
export DYLP_OPTIONS ?=

export LIBNAME ?= Dylp

###############################################################################

# Use the COIN boilerplate to establish the build environment. As explained
# above, CoinDir and MakefileDir will normally be set in the top level
# makefile.

export CoinDir ?= $(shell cd ../.. ; pwd)
export MakefileDir ?= $(CoinDir)/Makefiles

include ${MakefileDir}/Makefile.coin

###############################################################################

.DELETE_ON_ERROR:

.PHONY: default install clean distclean libdepend

# Available targets:
#  * library - builds the library in TARGETDIR
#  * install - build, plus installs a symbolic link in the current directory
#	       pointing to TARGETDIR/library, copies TARGETDIR/library to the
#	       main COIN library directory, and installs a symbolic link there.
#  * clean - removes libDylp* from this directory and from the COIN library
#	     directory, and deletes TARGETDIR.
#  * distclean - as for clean, and deletes *all* TARGETDIRs

default: install

###############################################################################

# We need the dylp support library to build the dylp library.

libdepend:
	@ cd ../Lib ; $(MAKE) -f Makefile install

# Build/install the dylp library

library install: libdepend
	@ ${MAKE} -f Makefile.Dylp $@

# We need to set BOOLTYPE nd FPCONFIGFLAGS in order to suppress execution of
# the configuration programs, which may not exist when we're just doing a
# clean.

clean distclean: 
	@ ${MAKE} -f Makefile.Dylp BOOLTYPE=bogus FPCONFIGFLAGS=bogus $@
