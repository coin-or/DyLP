
# Copyright (C) 2006 International Business Machines.
# All Rights Reserved.
# This file is distributed under the Eclipse Public License.

## $Id$

# Author:  Andreas Waechter            IBM    2006-04-13
# 	   Lou Hafer		       SFU    many mods since then

#############################################################################
#                       Names and other basic things                        #
#############################################################################

AC_PREREQ(2.59)

AC_INIT([DyLP],[1.10.2],[coin-dylp@lists.coin-or.org])

AC_COPYRIGHT([
Copyright 2006 International Business Machines and others.
All Rights Reserved.
This file is part of the open source package Coin which is distributed
under the Eclipse Public License.])

AC_REVISION(0.10)

# List one file in the package so that the configure script can test
# whether the package is actually there
AC_CONFIG_SRCDIR(configure.ac)

# Where should everything be installed by default?  Here, we want it
# to be installed directly in 'bin', 'lib', 'include' subdirectories
# of the directory where configure is run.  The default would be
# /usr/local.
AC_PREFIX_DEFAULT([`pwd`])

#############################################################################
#  Do the tests necessary to configure compilers and initialise autotools   #
#############################################################################

AC_COIN_CREATE_LIBTOOL

#############################################################################
#                  Check which subprojects are there                        #
#############################################################################

AC_COIN_MAIN_PACKAGEDIR(Sample,Data)
AC_COIN_MAIN_PACKAGEDIR(CoinUtils)
AC_COIN_MAIN_PACKAGEDIR(Osi)
AC_COIN_MAIN_PACKAGEDIR(DyLP)

##############################################################################
#		         Documentation                                       #
##############################################################################

# Doxygen documentation. Dylp is independent of Coin code, but OsiDylp can
# benefit from links to doxygen doc'n for CoinUtils and Osi.

AC_COIN_DOXYGEN([CoinUtils Osi])

##############################################################################
#                   Finishing up by writing all the output                   #
##############################################################################

# Here list all the files that configure should create (except for the
# configuration header file)
AC_CONFIG_FILES([Makefile])

AC_CONFIG_FILES([doxydoc/doxygen.conf])

# Finally, we let configure write all the output...
AC_COIN_FINALIZE

