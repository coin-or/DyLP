# Copyright (C) 2009 Lou Hafer
# All Rights Reserved.
# This file is distributed under the Eclipse Public License.

# Author:  Lou Hafer		SFU	2009.04.15

include $(top_srcdir)/BuildTools/Makemain.inc

# See the comment for distclean-local in ../Makefile.am

CONFIG_CLEAN_FILES =

DyLPDocInstalldir = $(pkgdocdir)

DyLPDocInstall_DATA = dylp.pdf dylp.ps

# Make sure the source files are distributed. BUT NOTE that Coin doesn't handle
# distributions this way and it's highly unlikely this will work as you'd
# like.

EXTRA_DIST = $(dylpdoc_SOURCES)

# The TeX source files for the dylp documentation. dylpfigs.tex is actually a
# shell to build just the figures. Handy for presentations.

dylpdoc_SOURCES = accuracy.tex \
		  antidegenlite.tex \
		  conmgmt.tex \
		  debug.tex \
		  dual.tex \
		  dylp.tex \
		  dylpfigs.tex \
		  dynamic.tex \
		  interface.tex \
		  intro.tex \
		  lpbasis.tex \
		  notation.tex \
		  perturbed.tex \
		  pricing.tex \
		  primal.tex \
		  scaling.tex \
		  solutions.tex \
		  startup.tex \
		  statistics.tex \
		  updateformulae.tex \
		  varmgmt.tex
