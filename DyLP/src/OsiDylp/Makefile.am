# Copyright (C) 2006 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Eclipse Public License.

## $Id: Makefile.am 1152 2007-12-28 03:59:07Z andreasw $

# Author:  Andreas Waechter           IBM    2006-04-13
#	   Lou Hafer		      SFU    many modifications since

AUTOMAKE_OPTIONS = foreign

########################################################################
#                             libOsiDylp                               #
########################################################################

# Name of the library compiled in this directory. We want it installed in
# $libdir
lib_LTLIBRARIES = libOsiDylp.la

# List all source files for this library, including headers
libOsiDylp_la_SOURCES = \
	OsiDylpMessages.cpp OsiDylpMessages.hpp \
	OsiDylpPresolve.cpp \
	OsiDylpSimplex.cpp \
	OsiDylpSolverInterface.cpp OsiDylpSolverInterface.hpp \
	OsiDylpWarmStartBasis.cpp OsiDylpWarmStartBasis.hpp

# List all additionally required libraries
if DEPENDENCY_LINKING
libOsiDylp_la_LIBADD = $(OSIDYLPLIB_LIBS) ../Dylp/libDylp.la
endif

# This is for libtool (on Windows)
libOsiDylp_la_LDFLAGS = $(LT_LDFLAGS)

# Here list all include flags, relative to this "srcdir" directory.  This
# "cygpath" stuff is necessary to compile with native compilers on Windows.
AM_CPPFLAGS = \
	-I`$(CYGPATH_W) $(srcdir)/../Dylp` \
	-I`$(CYGPATH_W) $(srcdir)/../DylpStdLib`  \
	$(COINUTILS_CFLAGS) $(OSI_CFLAGS)

# This line is necessary to allow VPATH compilation
DEFAULT_INCLUDES = -I. -I`$(CYGPATH_W) $(srcdir)` -I$(top_builddir)/src/DylpStdLib

########################################################################
#                Headers that need to be installed                     #
########################################################################

# Here list all the header files that are required by a user of the library,
# and that therefore should be installed in 'include/coin'
includecoindir = $(includedir)/coin
includecoin_HEADERS = \
	OsiDylpMessages.hpp \
	OsiDylpSolverInterface.hpp \
	OsiDylpWarmStartBasis.hpp

