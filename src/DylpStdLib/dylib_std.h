#ifndef _DYLIB_STD_H
#define _DYLIB_STD_H

/*
  This file is part of the support library for the Dylp LP distribution.

        Copyright (C) 2005 -- 2007 Lou Hafer

        School of Computing Science
        Simon Fraser University
        Burnaby, B.C., V5A 1S6, Canada
        lou@cs.sfu.ca

  This code is licensed under the terms of the Eclipse Public License (EPL).
*/

/*
  This file contains common definitions.

  First thing to do is haul in the Ansi C standard definitions. Takes care of
  NULL plus a few more obscure definitions. Also haul in the standard library
  declarations.
*/

#include <stddef.h>
#include <stdlib.h>

/*
  Dylp's configuration, as determined by autotools (config.h) or by various
  config*_default.h files.
*/

#include "DylpConfig.h"

/*
  A utility definition which allows for easy suppression of unused variable
  warnings from GCC. Useful when a variable is used only for assert()
  statements, and for sccsid/cvsid strings.
*/
#ifndef UNUSED
# if defined(_GNU_SOURCE) || defined(__GNUC__)
#   define UNUSED __attribute__((unused))
# else
#   define UNUSED
# endif
#endif

/*
  Memory copy functions --- memcpy, memset, and other less common ones.
*/
  
#include <string.h>

/*
  We need a boolean type. Never could understand why C doesn't have this.

  [Aug 10, 2001] For compatibility with C++, TRUE and FALSE are defined
  to be the corresponding C++ values. BOOL should be set in the compiler
  command line to be the storage type (char/short/int/long) that matches
  the size of a C++ "bool".
*/

#ifndef	__cplusplus
#define FALSE 0
#define TRUE 1
# ifdef BOOL
  typedef BOOL bool ;
# else
/*
  You're in trouble. Normally a definition for BOOL is determined by the
  autotools configure script; if not there, it should be set in one of the
  config*_default.h files. See DylpConfig.h for the logic.  If you're not
  worried about C++ compatibility, int is as good a choice as anything. If
  you're concerned about C++ compatibility, char is a good first guess. If
  that doesn't work, write a small C++ program that prints out sizeof(bool)
  and add the definition here.
*/
# warning The compile-time symbol BOOL is not defined (dylib_std.h); using int
  typedef int bool ;
# endif
#endif

#ifdef __cplusplus
#ifndef FALSE
# define FALSE false
#endif
#ifndef TRUE
# define TRUE true
#endif
#endif

/*
  flags	is used to indicate a data type composed of one-bit flags. Manipulated
  with the set of flag manipulation macros defined below.
*/

typedef unsigned int flags ;

#define setflg(zz_flgs,zz_flg) ((zz_flgs) |= (zz_flg))
#define clrflg(zz_flgs,zz_flg) ((zz_flgs) &= ~(zz_flg))
#define comflg(zz_flgs,zz_flg) ((zz_flgs) ^= (zz_flg))
#define getflg(zz_flgs,zz_flg) ((zz_flgs)&(zz_flg))
#define flgon(zz_flgs,zz_flg)  ((zz_flgs)&(zz_flg)?TRUE:FALSE)
#define flgoff(zz_flgs,zz_flg) ((zz_flgs)&(zz_flg)?FALSE:TRUE)
#define flgall(zz_flgs,zz_flg) ((((zz_flgs)&(zz_flg)) == (zz_flg))?TRUE:FALSE)


/*
  lnk_struct is a general-purpose linked list structure.

  Field		Description
  -----		-----------
  llnxt		pointer to the next list element
  llval		pointer to the associated value 
*/

typedef struct lnk_struct_tag
{ struct lnk_struct_tag *llnxt ;
  void *llval ; } lnk_struct ;

#define lnk_in(qqlnk,qqval) ((qqlnk)->llval = (void *) (qqval))
#define lnk_out(qqlnk,qqtype) ((qqtype) (qqlnk)->llval)


/* Max and min macros */

#define minn(qa,qb) (((qa) > (qb))?(qb):(qa))
#define maxx(qa,qb) (((qa) > (qb))?(qa):(qb))


/*
  Some macros to hide the memory allocation functions.

  The serious debugging versions of these macros (MALLOC_DEBUG = 2) use
  dyio_outfmt from the io library and assume the existence of a string,
  rtnnme (typically the name of the current subroutine) that's used to
  identify the origin of the message. There's enough information in the
  messages to track the allocation and deallocation of blocks, should you
  not have access to an interactive debugger with this capability.

  The casual debugging versions (MALLOC_DEBUG = 1, or, equivalently, simply
  defining MALLOC_DEBUG) only check for a return value of 0 and print a
  message to stderr with the file and line number.  This at least tells you
  when your code has core dumped because it ran out of space (as opposed
  to a bug you can actually fix).
*/

#if (MALLOC_DEBUG == 2)

#include "dylib_io.h"
extern void *zz_ptr_zz ;
extern ioid zz_chn_zz ;

/*
  Invoke this macro exactly once in a file context as 'MALLOC_DBG_INIT ;'
  to allocate the global variables used in the messages. zz_ptr_zz could
  be static but it's just as easy to make it global along with zz_chn_zz.
*/
#define MALLOC_DBG_INIT \
  void *zz_ptr_zz ; \
  ioid  zz_chn_zz

/*
  Invoke this macro exactly once, typically in the main program just after
  calling dyio_ioinit.  After calling dyio_ioinit, ioid 2 is connected to
  stdout and ioid 3 to stderr. Or use dyio_openfile to set up a specific
  file for these messages.
*/
#define MALLOC_DBG_SETCHN(chn) ( zz_chn_zz = chn )

#define MALLOC(zz_sze_zz) \
  ( zz_ptr_zz = (void *) malloc(zz_sze_zz), \
    dyio_outfmt(zz_chn_zz,FALSE,":malloc: %d bytes at %#08x in %s.\n", \
	    zz_sze_zz,zz_ptr_zz,rtnnme), \
    zz_ptr_zz )

#define CALLOC(zz_cnt_zz,zz_sze_zz) \
  ( zz_ptr_zz = (void *) calloc(zz_cnt_zz,zz_sze_zz), \
    dyio_outfmt(zz_chn_zz,FALSE,":calloc: %d (%d*%d) bytes at %#08x in %s.\n", \
	    zz_cnt_zz*zz_sze_zz,zz_cnt_zz,zz_sze_zz,zz_ptr_zz,rtnnme), \
    zz_ptr_zz )

#define REALLOC(zz_rptr_zz,zz_sze_zz) \
  ( zz_ptr_zz = (void *) realloc(zz_rptr_zz,zz_sze_zz), \
    dyio_outfmt(zz_chn_zz,FALSE, \
	   ":realloc: %#08x changed to %d bytes at %#08x in %s.\n", \
	    zz_rptr_zz,zz_sze_zz,zz_ptr_zz,rtnnme), \
    zz_ptr_zz )

#define FREE(zz_fptr_zz) \
  ( dyio_outfmt(zz_chn_zz,FALSE,":free: %#08x in %s.\n",zz_fptr_zz,rtnnme), \
    free((void *) zz_fptr_zz) )

#elif (MALLOC_DEBUG == 1)

#include <stdio.h>
static void *zz_ptr_zz ;

#define MALLOC_DBG_INIT

#define MALLOC_DBG_SETCHN(chn)

#define MALLOC(zz_sze_zz) \
  ( zz_ptr_zz = (void *) malloc(zz_sze_zz), \
    (zz_ptr_zz != 0)?0:\
      fprintf(stderr,":malloc: failed to get %d bytes at %s:%d.\n", \
	      zz_sze_zz,__FILE__,__LINE__), \
    zz_ptr_zz )

#define CALLOC(zz_cnt_zz,zz_sze_zz) \
  ( zz_ptr_zz = (void *) calloc(zz_cnt_zz,zz_sze_zz), \
    (zz_ptr_zz != 0)?0:\
      fprintf(stderr,":calloc: failed to get %d bytes at %s:%d.\n", \
	      zz_sze_zz*zz_cnt_zz,__FILE__,__LINE__), \
    zz_ptr_zz )

#define REALLOC(zz_rptr_zz,zz_sze_zz) \
  ( zz_ptr_zz = (void *) realloc(zz_rptr_zz,zz_sze_zz), \
    (zz_ptr_zz != 0)?0:\
      fprintf(stderr,":realloc: failed to get %d bytes at %s:%d.\n", \
	      zz_sze_zz,__FILE__,__LINE__), \
    zz_ptr_zz )

#define FREE(zz_fptr_zz) free((void *) zz_fptr_zz)

#else

#define MALLOC_DBG_INIT

#define MALLOC_DBG_SETCHN(chn)

#define MALLOC(zz_sze_zz) malloc(zz_sze_zz)

#define CALLOC(zz_cnt_zz,zz_sze_zz) calloc(zz_cnt_zz,zz_sze_zz)

#define REALLOC(zz_rptr_zz,zz_sze_zz) realloc(zz_rptr_zz,zz_sze_zz)

#define FREE(zz_fptr_zz) free((void *) zz_fptr_zz)

#endif


#endif /* _DYLIB_STD_H */
