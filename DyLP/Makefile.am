# Copyright (C) 2006 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Common Public License.

## $Id: Makefile.am 34 2006-05-15 19:22:07Z andreasw $

# Author:  Andreas Waechter           IBM    2006-04-13

AUTOMAKE_OPTIONS = foreign

########################################################################
#                          Subdirectories                              #
########################################################################

SUBDIRS = src/DylpStdLib src/Dylp examples doc

########################################################################
#             Additional files to be included in tarball               #
########################################################################

# Here we need include all files that are not mentioned in other Makefiles

EXTRA_DIST = \
	examples/generic.spc \
	examples/greenbeb.spc \
	examples/Makefile.in \
	examples/mpsio.c \
	examples/odsi+dylp.cpp \
	examples/osi_dylp.c \
	examples/osi+dylp.cpp \
	examples/plain \
	examples/README

########################################################################
#                           Extra Targets                              #
########################################################################

test: all
	if test -r ../Osi/test/Makefile; then \
	  cd ../Osi/test; \
	  $(MAKE) test; \
	else \
	  echo "We don't have Osi to run the test"; \
	fi

########################################################################
#                  Installation of the addlibs file                    #
########################################################################

addlibsfile = dylp_addlibs.txt
addlibsdir = $(prefix)/share/doc/coin/$(PACKAGE_NAME)/

install-exec-local: install-doc
	$(install_sh_DATA) $(addlibsfile) $(addlibsdir)/$(addlibsfile)

uninstall-local: uninstall-doc
	rm -f $(DESTDIR)$(libdir)/$(addlibsfile)

########################################################################
#                         Maintainer Stuff                             #
########################################################################

# So aclocal will find the local macros in the m4 directory

ACLOCAL_AMFLAGS = -I m4

# Files that are generated and should be cleaned with make distclean
DISTCLEANFILES =

include ../BuildTools/Makemain.inc
