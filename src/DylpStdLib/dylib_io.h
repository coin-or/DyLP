#ifndef _DYLIB_IO_H
#define _DYLIB_IO_H

/*
  This file is part of the support library for the Dylp LP distribution.

        Copyright (C) 2005 -- 2007 Lou Hafer

        School of Computing Science
        Simon Fraser University
        Burnaby, B.C., V5A 1S6, Canada
        lou@cs.sfu.ca

  This code is licensed under the terms of the Eclipse Public License (EPL).
*/


#include "dylib_std.h"


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
  DY_LCNIL	Null lexeme.
  DY_LCNUM	A number.
  DY_LCID	An identifier.
  DY_LCDEL	A delimiter.
  DY_LCFS	A fixed-length string.
  DY_LCQS	A quoted string.
  DY_LCEOF	Indicates end-of-file while trying to assemble a lexeme.
  DY_LCERR	Indicates I/O error while trying to assemble a lexeme.
*/

typedef enum {DY_LCNIL,DY_LCNUM,DY_LCID,DY_LCDEL,DY_LCFS,DY_LCQS,
	      DY_LCEOF,DY_LCERR} lexclass ;

#ifdef	__cplusplus
typedef struct { lexclass clazz ;
		 char *string ; } lex_struct ;
#else
typedef struct { lexclass class ;
		 char *string ; } lex_struct ;
#endif

extern DYLPLIB_EXPORT bool dyio_ioinit(void) ;
extern DYLPLIB_EXPORT void dyio_ioterm(void) ;

extern DYLPLIB_EXPORT ioid
  dyio_openfile(const char *path, const char *mode) ;

extern DYLPLIB_EXPORT bool
  dyio_isactive(ioid id),
  dyio_closefile(ioid id),
  dyio_setmode(ioid id, char mode), dyio_ttyq(ioid id) ;

extern DYLPLIB_EXPORT bool dyio_chgerrlog(const char *path, bool echo) ;

extern DYLPLIB_EXPORT const char *dyio_idtopath(ioid id) ;
extern DYLPLIB_EXPORT ioid dyio_pathtoid(const char *path, const char *mode) ;

extern long dyio_mark(ioid id) ;
extern bool dyio_backup(ioid id, long there) ;

extern bool dyio_scan(ioid id, const char pattern[], bool rwnd, bool wrap) ;
extern lex_struct
  *dyio_scanlex(ioid id),
  *dyio_scanstr(ioid id, lexclass stype, int fslen, char qschr, char qechr) ;

extern DYLPLIB_EXPORT void
  dyio_flushio(ioid id, bool echo),
  dyio_outfmt(ioid id, bool echo, const char *pattern, ... ),
  dyio_outchr(ioid id, bool echo, char chr) ;

extern DYLPLIB_EXPORT int
  dyio_outfxd(char *buffer, int fldsze, char lcr, const char *pattern, ... ) ;

#endif /* _DYLIB_IO_H */
