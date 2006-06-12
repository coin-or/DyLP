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

# Compile and run the fpchecks.c program to find out about certain floating
# point characteristics
AC_MSG_CHECKING(for defines for floating point characteristics)
mlibflag='-lm'
case $build in
  *-cygwin* | *-mingw*)
    case $CC in
      cl) mlibflags=libm.lib
          ;;
    esac
    ;;
esac
progname=conftest$EXEEXT
$CC $CFLAGS $srcdir/fpchecks.c -o $progname $mlibflag >/dev/null 2>&1
if test -x $progname; then :; else
  AC_MSG_ERROR(Cannot compile fpchecks test program)
fi
if (./$progname >conftest.out 2>conftest.err); then
  fpchecksout=`cat conftest.out`
  AC_MSG_RESULT($fpchecksout)
else
  fpchecksout=`cat conftest.out`
  AC_MSG_ERROR([Problems running fpchecks test program.  Output is
$fpchecksout])
fi
# Get the defines out of the output
if (grep __DYLP_SUNWspro conftest.out >/dev/null 2>&1); then
  AC_DEFINE([__DYLP_SUNWspro], [1], [Define to 1 if DyLP's fpchecks outputs -D__DYLP_SUNWspro])
fi
if (grep __DYLP_SUN conftest.out >/dev/null 2>&1); then
  AC_DEFINE([__DYLP_SUN], [1], [Define to 1 if DyLP's fpchecks outputs -D__DYLP_SUN])
fi
if (grep _GNU_SOURCE conftest.out >/dev/null 2>&1); then
  AC_DEFINE([_GNU_SOURCE], [1], [Define to 1 if DyLP's fpchecks outputs -D_GNU_SOURCE])
fi
if (grep __BIG_ENDIAN conftest.out >/dev/null 2>&1); then
  AC_DEFINE([__BIG_ENDIAN], [4321], [Define to 4321 if DyLP's fpchecks outputs -D__BIG_ENDIAN=4321])
fi
if (grep __LITTLE_ENDIAN conftest.out >/dev/null 2>&1); then
  AC_DEFINE([__LITTLE_ENDIAN], [1234], [Define to 1234 if DyLP's fpchecks outputs -D__LITTLE_ENDIAN=1234])
fi
if (grep __DYLP_FIX_HUGE_VAL conftest.out >/dev/null 2>&1); then
  AC_DEFINE([__DYLP_FIX_HUGE_VAL], [1], [Define to 1 if DyLP's fpchecks outputs -D__DYLP_FIX_HUGE_VAL])
fi
if (grep __DYLP_BROKEN_FPCLASS conftest.out >/dev/null 2>&1); then
  AC_DEFINE([__DYLP_BROKEN_FPCLASS], [1], [Define to 1 if DyLP's fpchecks outputs -D__DYLP_BROKEN_FPCLASS])
fi
# clean up
rm -f $progname conftest.out conftest.err

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
