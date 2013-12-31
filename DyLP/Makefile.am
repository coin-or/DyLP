# Copyright (C) 2006 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Eclipse Public License.

## $Id: Makefile.am 34 2006-05-15 19:22:07Z andreasw $

# Author:  Andreas Waechter           IBM    2006-04-13

AUTOMAKE_OPTIONS = foreign

########################################################################
#                          Subdirectories                              #
########################################################################

SUBDIRS = src/DylpStdLib src/Dylp
if COIN_HAS_OSI
  SUBDIRS += src/OsiDylp
endif

# We don't want to compile the test subdirectory, unless the test target is
# specified.  But we need to list it as subdirectory to make sure that it is
# included in the tarball. Similarly, we don't want to attempt to build the
# documentation unless specifically requested.

if ALWAYS_FALSE
  SUBDIRS += examples
  SUBDIRS += test
  SUBDIRS += doc doc/Figures doc/TexMF
endif

########################################################################
#             Additional files to be included in tarball               #
########################################################################

# Here we need include all files that are not mentioned in other Makefiles

EXTRA_DIST = \
	AUTHORS LICENSE README NEWS \
	examples/generic.spc \
	examples/greenbeb.spc \
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
	@echo "Building and running dylp unit test."
	@cd test ; $(MAKE) test

# Doxygen documentation

doxydoc:
	doxygen doxydoc/doxygen.conf

clean-doxydoc:
	( cd doxydoc ; rm -rf html *.log *.tag )


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

clean-local: mostlyclean-local clean-doxydoc

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

########################################################################
#                  Installation of the addlibs file                    #
########################################################################

pkgconfiglibdir = $(libdir)/pkgconfig
pkgconfiglib_DATA = dylp.pc

if COIN_HAS_OSI
pkgconfiglib_DATA += osi-dylp.pc
endif

addlibsdir = $(DESTDIR)$(datadir)/coin/doc/DyLP
	
install-data-hook:
	@$(mkdir_p) "$(addlibsdir)"
if COIN_HAS_PKGCONFIG
	PKG_CONFIG_PATH=@COIN_PKG_CONFIG_PATH@ \
	$(PKG_CONFIG) --libs dylp > $(addlibsdir)/dylp_addlibs.txt
else
if COIN_CXX_IS_CL
	echo "-libpath:`$(CYGPATH_W) @abs_lib_dir@` libDyLP.lib @DYLPLIB_LIBS_INSTALLED@" > $(addlibsdir)/dylp_addlibs.txt
else
	echo -L@abs_lib_dir@ -lDyLP @DYLPLIB_LIBS_INSTALLED@ > $(addlibsdir)/dylp_addlibs.txt
endif
endif

uninstall-hook:
	rm -f $(addlibsdir)/dylp_addlibs.txt

install-exec-local: install-doc

uninstall-local: uninstall-doc

########################################################################
#                         Maintainer Stuff                             #
########################################################################

# So aclocal will find the local macros in the m4 directory

ACLOCAL_AMFLAGS = -I m4

# Files that are generated and should be cleaned with make distclean
DISTCLEANFILES =

include BuildTools/Makemain.inc
