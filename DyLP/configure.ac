# Copyright (C) 2006 International Business Machines.
# All Rights Reserved.
# This file is distributed under the Common Public License.

## $Id: configure.ac,v 1.1.2.1 2006/04/19 23:25:12 andreasw Exp $

# Author:  Andreas Waechter            IBM    2006-04-13

#############################################################################
#                       Names and other basic things                        #
#############################################################################

AC_PREREQ(2.59)

AC_INIT([DyLP],[0.1],[coin-dylp@list.coin-or.org])

AC_COPYRIGHT([
Copyright 2006 International Business Machines and others.
All Rights Reserved.
This file is part of the open source package Coin which is distributed
under the Common Public License.])

AC_REVISION(0.9)

# List one file in the package so that the configure script can test
# whether the package is actually there
AC_CONFIG_SRCDIR(src/Dylp/vector.h)

# Where should everything be installed by default?  Here, we want it
# to be installed directly in 'bin', 'lib', 'include' subdirectories
# of the directory where configure is run.  The default would be
# /usr/local.
AC_PREFIX_DEFAULT([`pwd`])

#############################################################################
#                         Standard build tool stuff                         #
#############################################################################

# Get the system type
AC_CANONICAL_BUILD

# If this project depends on external projects, the Externals file in
# the source root directory contains definition of where to find those
# externals.  The following macro ensures that those externals are
# retrieved by svn if they are not there yet.
AC_COIN_PROJECTDIR_INIT

# Check if user wants to produce debugging code
AC_COIN_DEBUG_COMPILE

# Get the name of the C compiler and appropriate compiler options
AC_COIN_PROG_CC

# Get the name of the C++ compiler and appropriate compiler options
AC_COIN_PROG_CXX

# Initialize automake and libtool
AC_COIN_INIT_AUTO_TOOLS

#############################################################################
#                              COIN components                              #
#############################################################################

AC_COIN_HAS_PROJECT(DyLP)

#############################################################################
#                       Doing project specific tests                        #
#############################################################################

# Find out the C type that corresponds to the C++ bool type in size

AC_LANG_PUSH(C++)
AC_CHECK_SIZEOF(bool)
AC_LANG_POP(C++)

AC_LANG_PUSH(C)
AC_CHECK_SIZEOF(char)
if test $ac_cv_sizeof_bool = $ac_cv_sizeof_char; then
  booltype=char
else
  AC_CHECK_SIZEOF(int)
  if test $ac_cv_sizeof_bool = $ac_cv_sizeof_int; then
    booltype=int
  else
    AC_CHECK_SIZEOF(short int)
    if test $ac_cv_sizeof_bool = $ac_cv_sizeof_short_int; then
      booltype="short int"
    else
      AC_CHECK_SIZEOF(long int)
      if test $ac_cv_sizeof_bool = $ac_cv_sizeof_long_int; then
        booltype="long int"
      else
        AC_MSG_ERROR(Cannot determine C type corresponding to C++ bool type)
      fi
    fi
  fi
fi
AC_LANG_POP(C)
# just some output to be shown to the user
AC_MSG_CHECKING(for C type corresponding to C++ bool type)
AC_MSG_RESULT($booltype)
AC_DEFINE_UNQUOTED([BOOL], [$booltype],
		   [Define to the C type corresponding to the C++ bool type])

########################################################################

# We need -lm or libm.lib for the following checks
mlibflag='-lm'
case $build in
  *-cygwin* | *-mingw*)
    case $CC in
      cl) mlibflags=libm.lib
          ;;
    esac
    ;;
esac

# Find out if we have quiet_nan, and if not, how we can emulate it.

AC_CHECK_HEADERS(sunmath.h)
if test x$ac_cv_header_sunmath_h = xyes; then
  AC_CHECK_LIB(sunmath, quiet_nan,
    			  [AC_DEFINE([COIN_HAS_QUIET_NAN],[1],
    						 [Define to 1 if quiet_nan is available])])
fi

# Determine endianess
AC_C_BIGENDIAN(,,AC_MSG_ERROR(Cannot determine endianness))

# Find out what the names of finite and isnan are
AC_CHECK_HEADERS(float.h)
case $build in
  *-cygwin*) ;;
  *) AC_CHECK_HEADERS(ieeefp.h) ;;
esac
MY_C_FINITE=
AC_CHECK_DECL([finite],[MY_C_FINITE=finite],,[
#include <math.h>
#ifdef HAVE_FLOAT_H
# include <float.h>
#endif
#ifdef HAVE_IEEEFP_H
# include <ieeefp.h>
#endif])
if test -z "$MY_C_FINITE"; then
  AC_CHECK_DECL([_finite],[MY_C_FINITE=_finite],,[
#include <math.h>
#ifdef HAVE_FLOAT_H
# include <float.h>
#endif
#ifdef HAVE_IEEEFP_H
# include <ieeefp.h>
#endif])
fi
if test -z "$MY_C_FINITE"; then
  AC_CHECK_DECL([isfinite],[MY_C_FINITE=_finite],,[
#include <math.h>
#ifdef HAVE_FLOAT_H
# include <float.h>
#endif
#ifdef HAVE_IEEEFP_H
# include <ieeefp.h>
#endif])
fi
if test -z "$MY_C_FINITE"; then
  AC_MSG_WARN(Cannot find C-function for checking finite.  Check will be disabled)
else
  AC_DEFINE_UNQUOTED([DYLP_FINITE],[$MY_C_FINITE],
                     [Define to be the name of C-function for finite check])
fi

MY_C_ISNAN=
AC_CHECK_DECL([isnan],[MY_C_ISNAN=isnan],,[
#include <math.h>
#ifdef HAVE_FLOAT_H
# include <float.h>
#endif
#ifdef HAVE_IEEEFP_H
# include <ieeefp.h>
#endif])
if test -z "$MY_C_ISNAN"; then
  AC_CHECK_DECL([_isnan],[MY_C_ISNAN=_isnan],,[
#include <math.h>
#ifdef HAVE_FLOAT_H
# include <float.h>
#endif
#ifdef HAVE_IEEEFP_H
# include <ieeefp.h>
#endif])
fi
if test -z "$MY_C_ISNAN"; then
  AC_MSG_WARN(Cannot find C-function for checking isnan.  Check will be disabled)
else
  AC_DEFINE_UNQUOTED([DYLP_ISNAN],[$MY_C_ISNAN],
                     [Define to be the name of C-function for isnan check])
fi

AC_ARG_VAR(DYLP_ERRMSGDIR,[Directory containing the DyLP error message text file (dy_errmsgs.txt). Default is srcdir/src/Dylp])
AC_SUBST(DYLP_ERRMSGDIR)
if test x"$DYLP_ERRMSGDIR" = x; then
  DYLP_ERRMSGDIR=$abs_source_dir/src/Dylp
fi
AC_DEFINE_UNQUOTED([DYLP_ERRMSGDIR], ["$DYLP_ERRMSGDIR"],
		   [Set to the full path directory name for the location of the error text message file dy_errmsgs.txt])

##############################################################################
#                   VPATH links for example input files                      #
##############################################################################

# In case this is a VPATH configuration we need to make sure that the
# input files for the examples are available in the VPATH directory.

AC_COIN_VPATH_LINK(examples/plain)

##############################################################################
#                   Finishing up by writing all the output                   #
##############################################################################

# Here list all the files that configure should create (except for the
# configuration header file)
AC_CONFIG_FILES([Makefile
                 examples/Makefile
                 src/DylpStdLib/Makefile
                 src/Dylp/Makefile
		 dylp_addlibs.txt])

# Here put the location and name of the configuration header file
AC_CONFIG_HEADER([inc/config_dylp.h])

# Finally, we let configure write all the output...
AC_COIN_FINALIZE
