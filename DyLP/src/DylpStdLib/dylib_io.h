#ifndef _DYLIB_IO_H
#define _DYLIB_IO_H

/*
  This file is part of the support library  for the OsiDylp LP distribution.

        Copyright (C) 2005 Lou Hafer

        School of Computing Science
        Simon Fraser University
        Burnaby, B.C., V5A 1S6, Canada
        lou@cs.sfu.ca

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St., Fifth Floor, Boston, MA  02110-1301  USA
*/

/*
  @(#)io.h	2.4	03/18/04
  svn/cvs: $Id$
*/

#include "dylib_std.h"
#ifdef _DYLIB_FORTRAN
#include "dylib_fortran.h"
#endif


/*
  Common definitions for the i/o library packages.
*/

/*
  The file i/o routines in io.c use an integer i/o id to specify a stream.
  The only reason to have this typedef is clarity in the code (and the off
  chance that it might someday become a more complex type). i/o id's are
  positive integers, between 1 and FOPEN_MAX-2 (see io.c for details).
*/

typedef int ioid ;

#define IOID_NOSTRM ((ioid) 0)
#define IOID_INV ((ioid) -1)

/*
  The lexeme structure, used for strings.
  
  Field		Description
  -----		-----------
  class		The class of the lexeme.
  string	The value of the lexeme.

  The values for class are drawn from the following set, defined below as the
  enum lexclass.

  Value		Description
  -----		-----------
  LCNIL		Null lexeme.
  LCNUM		A number.
  LCID		An identifier.
  LCDEL		A delimiter.
  LCFS		A fixed-length string.
  LCQS		A quoted string.
  LCEOF		Indicates end-of-file while trying to assemble a lexeme.
  LCERR		Indicates I/O error while trying to assemble a lexeme.
*/

typedef enum {LCNIL,LCNUM,LCID,LCDEL,LCFS,LCQS,LCEOF,LCERR} lexclass ;

#ifdef	__cplusplus
typedef struct { lexclass clazz ;
		 char *string ; } lex_struct ;
#else
typedef struct { lexclass class ;
		 char *string ; } lex_struct ;
#endif

extern bool ioinit(void) ;
extern void ioterm(void) ;

extern ioid openfile(const char *path, const char *mode) ;
extern bool isactive(ioid id) ;
extern bool closefile(ioid id) ;
extern bool DyLPsetmode(ioid id, char mode), ttyq(ioid id) ;

extern bool chgerrlog(const char *path, bool echo) ;

extern const char *idtopath(ioid id) ;
extern ioid pathtoid(const char *path, const char *mode) ;

extern long mark(ioid id) ;
extern bool backup(ioid id, long there) ;

extern bool scan(ioid id, const char pattern[], bool rwnd, bool wrap) ;
extern lex_struct *scanlex(ioid id),
		  *scanstr(ioid id,
			   lexclass stype, int fslen, char qschr, char qechr) ;

extern void flushio(ioid id, bool echo),
	    outfmt(ioid id, bool echo, const char *pattern, ... ),
	    outchr(ioid id, bool echo, char chr) ;
extern int outfxd(char *buffer, int fldsze, char lcr,
		  const char *pattern, ... ) ;

#ifdef _DYLIB_FORTRAN
extern void outfmt_(integer *ftnid, logical *ftnecho, char *pattern, ... ) ;
#endif

#endif /* _DYLIB_IO_H */
