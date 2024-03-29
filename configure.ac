# Copyright (C) 2006 International Business Machines.
# All Rights Reserved.
# This file is distributed under the Eclipse Public License.

# Author:  Andreas Waechter            IBM    2006-04-13
# 	   Lou Hafer		       SFU    many mods since then

#############################################################################
#                       Names and other basic things                        #
#############################################################################

AC_INIT([DyLP],[devel],[https://github.com/coin-or/DyLP/issues/new],
	[coin-or-dylp],[https://github.com/coin-or/DyLP])


AC_COPYRIGHT([
Copyright 2006 International Business Machines and others.
All Rights Reserved.
This file is part of the open source package Coin which is distributed
under the Eclipse Public License.])

# List one file in the package so that the configure script can test
# whether the package is actually there
AC_CONFIG_SRCDIR(src/Dylp/dylp.h)

# Do some initialization work (version numbers, change prefix default, ...)
AC_COIN_INITIALIZE

#############################################################################
#                         Standard build tool stuff                         #
#############################################################################

# Get the names of the C and C++ compilers and appropriate compiler options.
AC_COIN_PROG_CC
AC_COIN_PROG_CXX

# This is a mixed C/C++ package, set the language accordingly.
AC_LANG_PUSH(C)

# Initialize libtool.
AC_COIN_PROG_LIBTOOL

# set RPATH_FLAGS to the compiler link flags required to hardcode location
# of the shared objects (expanded_libdir is set somewhere in configure before)
# (use in examples Makefile)
AC_COIN_RPATH_FLAGS([$expanded_libdir])

#############################################################################
#                   Math headers                                            #
#############################################################################

# Check for cmath/math.h, cfloat/float.h, cieeefp/ieeefp.h
AC_COIN_CHECK_MATH_HDRS

# See if we'll need an explicit libm for DylpLib

AC_COIN_CHK_LIBM([DylpLib])

# Finalize the flags. That's as much as DylpLib needs. Make sure DylpTest has
# the flags required by DylpLib.

AC_COIN_FINALIZE_FLAGS(DylpLib)
AC_COIN_CHK_HERE([DylpLib],[DylpTest],[dylp])


#############################################################################
#                     COIN-OR components                                    #
#############################################################################

# None of these are required to build and test libDylp. All are needed to
# properly test libOsiDylp and to fully link the doxygen documentation.

AC_COIN_CHK_PKG([CoinUtils],[OsiDylpLib OsiDylpTest])
AC_COIN_CHK_PKG([Osi],[OsiDylpLib OsiDylpTest])
AC_COIN_CHK_PKG([OsiTests],[OsiDylpLib OsiDylpTest],[osi-unittests])

# Just Sample will be enough to run basic tests. If Netlib is present, the
# OsiDylp test will run the Netlib problem set.

AC_COIN_CHK_PKG([Sample],[OsiDylpTest],[coindatasample],,dataonly)
AC_COIN_CHK_PKG([Netlib],[OsiDylpTest],[coindatanetlib],,dataonly)

# Finalize libs and cflags for OsiDylpLib and OsiDylpTest, then tack on
# information for dylp, which is not yet installed.

AC_COIN_FINALIZE_FLAGS([OsiDylpLib OsiDylpTest])
AC_COIN_CHK_HERE([DylpLib],[OsiDylpLib OsiDylpTest],[dylp])

#############################################################################
#                       Doing project specific tests                        #
#############################################################################

# Determine the C type that corresponds to the C++ bool type in size

AC_DYLP_EQUIV_FOR_CPP_BOOL

########################################################################

# Now determine the names in this environment for isfinite() and isnan(). The
# most common variants are finite() and isnan(); isfinite() is gaining
# ground. We check for others. After these macros finish, DYLP_ISFINITE has
# the name for isfinite(), and DYLP_ISNAN has the name for isnan().

AC_DYLP_FIND_ISFINITE
AC_DYLP_FIND_ISNAN

# Check for the presence of a quiet_nan function. To my knowledge, this will
# exist only if sunmath is present, but hey, who knows what lurks out there.

AC_CHECK_FUNC([quiet_nan],
    [AC_DEFINE([DYLP_HAS_QUIET_NAN],[1],
	[Define this symbol if the quiet_nan function exists. This function
	 should return the bit pattern for IEEE quiet NaN.])])

# And determine whether we're big-endian or little-endian. This is necessary to
# define the correct patterns for IEEE infinity and quiet_nan, in the event
# that we can't get them any other way. This will define WORDS_BIGENDIAN if
# we're on a big-endian machine, and do nothing otherwise.

AC_C_BIGENDIAN

##############################################################################
#		Dylp configuration options				     #
##############################################################################

AC_DYLP_PARANOIA([no])
AC_DYLP_STATISTICS([no])
AC_DYLP_INFO([yes])

##############################################################################
#		OsiDylp configuration options				     #
##############################################################################

AC_ODSI_INFO(yes)
AC_ODSI_STATISTICS(no)
AC_ODSI_PARANOIA(1)

##############################################################################
#		         Documentation                                       #
##############################################################################

# Doxygen documentation. Dylp is independent of Coin code, but OsiDylp can
# benefit from links to doxygen doc'n for CoinUtils and Osi.

AC_COIN_DOXYGEN([CoinUtils Osi])

##############################################################################
#                   VPATH links for example input files                      #
##############################################################################

# In case this is a VPATH configuration we need to make sure that the
# input files for the examples are available in the VPATH directory.

AC_COIN_VPATH_LINK([examples/generic.spc examples/greenbeb.spc])

##############################################################################
#                   Finishing up by writing all the output                   #
##############################################################################

# Here list all the files that configure should create (except for the
# configuration header file). First the usual stuff: DyLP's top-level Makefile,
# the code Makefiles (src/*/Makefile), and the test Makefile.

AC_CONFIG_FILES([Makefile
                 examples/Makefile
                 src/DylpStdLib/Makefile
                 src/Dylp/Makefile
                 src/OsiDylp/Makefile
                 test/Makefile])

# Files for doxygen documentation

AC_CONFIG_FILES([doxydoc/doxygen.conf])

# The pkg-config data files

AC_CONFIG_FILES([dylp.pc])
if test $coin_has_osi = yes ; then
  AC_CONFIG_FILES([osi-dylp.pc:src/OsiDylp/osi-dylp.pc.in])
fi

# The examples

AC_CONFIG_FILES([examples/plain],[chmod +x examples/plain])

# And all the stuff that goes into building the documentation. See the note in
# Makefile.am with respect to CONFIG_CLEAN_FILES and the distclean and
# distclean-local targets.

AC_CONFIG_FILES([doc/Makefile
		 doc/dylpabsdir.tex
		 doc/makefile.dylpdoc
		 doc/Figures/Makefile
		 doc/TexMF/Makefile])
AC_CONFIG_FILES([doc/build_dylpdoc],[chmod +x doc/build_dylpdoc])

# Here put the location and name of the configuration header files. Dylp needs
# config.h to be idempotent, so insist on guards.

AH_TOP([
#ifndef _DYLP_AUTO_CONFIG_H_
#define _DYLP_AUTO_CONFIG_H_
])
AH_BOTTOM([#endif /* _DYLP_AUTO_CONFIG_H_ */])
AC_CONFIG_HEADERS([src/DylpStdLib/config.h src/DylpStdLib/config_dylp.h])
if test $coin_has_osi = yes ; then
  AC_CONFIG_HEADERS([src/OsiDylp/config_osidylp.h])
fi

# Finally, we let configure write all the output...

AC_COIN_FINALIZE
