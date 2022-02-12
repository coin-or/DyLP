# Copyright (C) 2022 Lou Hafer
# All Rights Reserved.
# This file is distributed under the Eclipse Public License.

# Author:  Lou Hafer	SFU (retired)

include $(top_srcdir)/BuildTools/Makemain.inc

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
libOsiDylp_la_LIBADD = ../Dylp/libDylp.la $(OSIDYLPLIB_LFLAGS)

# This is for libtool

libOsiDylp_la_LDFLAGS = $(LT_LDFLAGS)

# Here list all include flags. We need DYLPLIB_BUILD to properly access the
# underlying dylp configuration file. Arguably, this should be separated as
# for other OsiXXX.

AM_CPPFLAGS = -I$(srcdir)/../Dylp -I$(srcdir)/../DylpStdLib \
              -DDYLPLIB_BUILD $(OSIDYLPLIB_CFLAGS)

########################################################################
#                Headers that need to be installed                     #
########################################################################

# Here list all the header files that are required by a user of the library,
# and that therefore should be installed in pkgincludedir

includecoindir = $(pkgincludedir)
includecoin_HEADERS = \
	OsiDylpMessages.hpp \
	OsiDylpSolverInterface.hpp \
	OsiDylpWarmStartBasis.hpp

