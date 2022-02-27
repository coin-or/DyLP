# Copyright (C) 2006 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Eclipse Public License.

# Author:  Andreas Waechter           IBM    2006-04-13

include BuildTools/Makemain.inc

########################################################################
#          Subdirectories and installation of .pc files                #
########################################################################

SUBDIRS = src/DylpStdLib src/Dylp doc
pkgconfiglib_DATA = dylp.pc

# Build OsiDylp only if Osi is available.

if COIN_HAS_OSI
  SUBDIRS += src/OsiDylp
  pkgconfiglib_DATA += osi-dylp.pc
endif

########################################################################
#                           Extra Targets                              #
########################################################################

test: all
	@echo "Building and running dylp unit test."
	@cd test ; $(MAKE) test


# Autotools does a good job on code, but it doesn't deal well with LaTeX
# documentation. A few rules to help it along. Note that we don't delete
# dylp.ps and dylp.pdf until maintainer-clean; these come prebuilt with the
# distribution.

doc:
	@echo "Attempting to build dylp typeset documentation."
	@cd doc ; ./build_dylpdoc --ps --pdf dylp

# Clean will get invoked in environments where it's not possible to build
# the documentation, and, quite possibly, where build_dylpdoc will not run.
# Execute ':' when build_dylpdoc fails to do nothing (successfully) so that
# the clean can keep going.

mostlyclean-local:
	@cd doc ; \
	 if test -x build_dylpdoc ; then \
	   ./build_dylpdoc --clean || : ; \
	 fi

clean-local: mostlyclean-local clean-doxygen-docs

# Normally, automake would remove build_dylpdoc, dylpabsdir.tex, and
# makefile.dylpdoc as part of distclean, because they're generated during
# configuration. But we can't allow that --- distclean-local runs *after*
# distclean, and it needs build_dylpdoc and makefile.dylpdoc. Down in
# doc/Makefile.am, the variable CONFIG_CLEAN_FILES is cleared to prevent
# distclean from removing these files. That means that if ever I generate new
# files from configure.ac, they need to be listed here.

distclean-local: clean-local
	@cd doc ; rm -f build_dylpdoc dylpabsdir.tex makefile.dylpdoc
	@cd doc ; rm -f Makefile Figures/Makefile TexMF/Makefile

maintainer-clean-local: distclean-local
	@cd doc ; rm -f dylp.pdf dylp.ps

.PHONY: doc doxydoc mostlyclean-local clean-local distclean-local \
	maintainer-clean-local


install-exec-local: install-doc

uninstall-local: uninstall-doc uninstall-doxygen-docs

########################################################################
#                         Maintainer Stuff                             #
########################################################################

# So aclocal will find the local macros in the m4 directory

ACLOCAL_AMFLAGS = -I m4

# Files that are generated and should be cleaned with make distclean
DISTCLEANFILES =

