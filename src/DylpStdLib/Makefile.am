# Copyright (C) 2022 Lou Hafer
# All Rights Reserved.
# This file is distributed under the Eclipse Public License.

# Author:  Lou Hafer (SFU retired) 2022-02-10

include $(top_srcdir)/BuildTools/Makemain.inc

########################################################################
#                            libDylpStdLib                             #
########################################################################

# Name of the library compiled in this directory.  We don't want it to be
# installed since it will be included in the libDylp library

noinst_LTLIBRARIES = libDylpStdLib.la

# List all source files for this library, including headers
libDylpStdLib_la_SOURCES = \
	DylpConfig.h \
	dylib_binsrch.c \
	dylib_bnfrdr.c dylib_bnfrdr.h \
	dylib_bnfrdrio.c \
	dylib_errs.c dylib_errs.h \
	dylib_fortran.h \
	dylib_hash.c dylib_hash.h \
	dylib_io.c dylib_io.h \
	dylib_keytab.h \
	dylib_littab.c \
	dylib_std.h \
	dylib_strrtns.c dylib_strrtns.h

# This is for libtool
libDylpStdLib_la_LDFLAGS = $(LT_LDFLAGS)

# DylpStdLib is self-contained, but it needs DYLPLIB_BUILD to access the
# configuration in config.h. Give it access to DYLPLIB_CFLAGS just in case.

AM_CPPFLAGS = -DDYLPLIB_BUILD $(DYLPLIB_CFLAGS)

########################################################################
#                Headers that need to be installed                     #
########################################################################

# Here list all the header files that are required by a user of the library,
# and that therefore should be installed in pkgincludedir.

includecoindir = $(pkgincludedir)
includecoin_HEADERS = \
	dylib_errs.h \
	dylib_hash.h \
	dylib_io.h \
	dylib_std.h \
	dylib_strrtns.h

install-exec-local:
	$(install_sh_DATA) config_dylp.h $(DESTDIR)$(includecoindir)/DylpConfig.h

uninstall-local:
	rm -f $(DESTDIR)$(includecoindir)/DylpConfig.h
