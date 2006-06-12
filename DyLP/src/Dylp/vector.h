/*
  This file is a part of the OsiDylp LP distribution.

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

#ifndef _DYLP_VECTOR_H
#define _DYLP_VECTOR_H

/*
  @(#)vector.h	4.5	11/06/04
  svn/cvs: $Id$
*/

/*
  Why, you might ask, are we including ctype.h? Well, it's required by the
  ANSI C specification, so it's pretty well guaranteed to exist. And, at least
  in Solaris and Linux environments that I'm familiar with, it'll pull in the
  compile-time symbols that specify big- or little-endian, which we really
  want.
*/
#include <ctype.h>

/*
  A bunch of standard definitions.
*/
#include "dylib_std.h"

/*
  In a few rare instances, the declarations here will be unused, but for dylp
  this is a good bet.
*/
#include <math.h>

/*
  The Theory: quiet_nan is used to indicate failure (by returning NaN)
  without triggering a signal the client may not be prepared to catch. The
  idea is that any reasonable checks in the client will detect NaN fairly
  quickly.  signalling_nan is used when there's no advantage in delaying a
  signal.

  The Reality: Neither seems to trigger a signal, and many computing
  environments can't tell the difference. But it's coded into dylp, and it'd
  be much ado to change. Hence the compile-time ugliness that follows.

  In the Sun Workshop environment, quiet_nan and signalling_nan are declared
  in sunmath.h and found in libsunmath. With release 5.0, sunmath.h includes
  some declarations of type `long long', which isn't supported under the -Xc
  (strict ANSI compatibility) option for cc. So, we extract only the
  definitions we need.  Unfortunately, sunmath.h is present only in the Sun
  Workshop programming environment. Sun without Workshop has only the
  require file nan.h, which is inadequate.

  For a long while, GNU C didn't distinguish QNaN and SNaN. More recently,
  its support for IEEE 754 seems to have improved, but it's not clear that we
  can count on everyone having a recent GCC environment just yet. Here, too,
  nan.h is inadequate.  The easy way out is to simply #define them as macros
  that return the proper bit pattern. Arguably this would make more sense in
  general than Sun's implementation as functions.

  According to IEEE 754, the proper bit patterns are:

    0x7ff00000 00000000		for Inf
    0x7fffffff ffffffff		for QNaN
    0x7ff00000 00000001		for SNaN
  
  It works this way: The IEEE definition of NaN is
    Bits	Value
    63		sign --- don't care for a NaN, but nice to be positive (0)
    62:52	exponent --- must be maximum value, 0x7ff
    51:0	fraction --- must not be zero (a fraction of zero is the
		representation of infinity). Sun documentation defines QNaN
		as having bit 51 of the fraction set to 1, SNaN as having
		bit 51 set to 0.

  Creating the proper constants qualifies as a serious gross hack. And if you
  have a little-endian machine (the 80x86 family being far and away the most
  common example), you need to flip the byte order.
*/

#ifdef __DYLP_SUNWspro

  extern double quiet_nan(long dummy),signaling_nan(long dummy) ;

# else
/*
  If it's not Sun/Forte Workshop, go with what's know to work for GCC. If
  your environment doesn't match this, you'll have to give it some thought.
*/

  typedef union { unsigned char fpchr[8] ; double fpdbl ; } fpunion_t ;
/*
  Yes, all this really is needed to get all the various compilers to quit
  complaining. We need the `(unsigned char)' to prevent some compilers from
  complaining about the initialiser being out of range. Goes to the ANSI C
  rule that `Character constants not preceded by the letter L have type int.'
*/
# if (defined(__BYTE_ORDER) && (__BYTE_ORDER == __LITTLE_ENDIAN)) || \
     (!defined(__BYTE_ORDER) && \
       (defined(_LITTLE_ENDIAN) || defined(__LITTLE_ENDIAN)))
    static fpunion_t QNaNbits UNUSED = { { (unsigned char) '\376',
				    (unsigned char) '\377',
				    (unsigned char) '\377',
				    (unsigned char) '\377',
			            (unsigned char) '\377',
				    (unsigned char) '\377',
				    (unsigned char) '\377',
				    (unsigned char) '\177' } } ;
    static fpunion_t SNaNbits UNUSED = { { (unsigned char) '\001',
				    (unsigned char) '\0',
				    (unsigned char) '\0',
				    (unsigned char) '\0',
				    (unsigned char) '\0',
				    (unsigned char) '\0',
				    (unsigned char) '\360',
				    (unsigned char) '\177' } } ;
    static fpunion_t Infbits UNUSED = { { (unsigned char) '\0',
				   (unsigned char) '\0',
				   (unsigned char) '\0',
				   (unsigned char) '\0',
				   (unsigned char) '\0',
				   (unsigned char) '\0',
				   (unsigned char) '\360',
				   (unsigned char) '\177' } } ;
# elif (defined(__BYTE_ORDER) && (__BYTE_ORDER == __BIG_ENDIAN)) || \
       (!defined(__BYTE_ORDER) && \
	 (defined(_BIG_ENDIAN) || defined(__BIG_ENDIAN)))
    static fpunion_t QNaNbits UNUSED = { { (unsigned char) '\177',
				    (unsigned char) '\377',
				    (unsigned char) '\377',
				    (unsigned char) '\377',
				    (unsigned char) '\377',
				    (unsigned char) '\377',
				    (unsigned char) '\377',
				    (unsigned char) '\376' } } ;
    static fpunion_t SNaNbits UNUSED = { { (unsigned char) '\177',
				    (unsigned char) '\360',
				    (unsigned char) '\0',
				    (unsigned char) '\0',
				    (unsigned char) '\0',
				    (unsigned char) '\0',
				    (unsigned char) '\0',
				    (unsigned char) '\001' } } ;
    static fpunion_t Infbits UNUSED = { { (unsigned char) '\177',
				   (unsigned char) '\360',
				   (unsigned char) '\0',
				   (unsigned char) '\0',
				   (unsigned char) '\0',
				   (unsigned char) '\0',
				   (unsigned char) '\0',
				   (unsigned char) '\0' } } ;
#   else
/*
  You're in trouble. It could be that your machine is what the GCC folks call
  PDP_ENDIAN. Read comments above and check in the file endian.h.
*/
#   error (vector.h) Unrecognized byte ordering convention.
#   endif  /* ENDIAN options */

# define quiet_nan(zz_dummy_zz) (QNaNbits.fpdbl)
# define signaling_nan(zz_dummy_zz) (SNaNbits.fpdbl)
#endif /* !SUNWspro */

/*
  On some machines, HUGE_VAL isn't actually IEEE infinity. Fix that if
  necessary.
*/

#ifdef __DYLP_FIX_HUGE_VAL
# undef HUGE_VAL
# define HUGE_VAL (Infbits.fpdbl)
#endif

/* AW: I added this for HP machines */
#ifdef _HPUX_SOURCE
# define finite isfinite
#endif

/*
  In a Sun/Solaris environment, the definitions and functions that support
  IEEE floating point are in ieeefp.h. This seems to be true even if GNU
  compilers are being used instead of Sun Workshop compilers. In a GNU/Linux
  environment, the necessary definitions seem to live in math.h. The upshot
  is that we need to explicitly pull in ieeefp.h here for a Sun environment.

  For reasons that escape me (but likely have to do with compiling using strict
  ANSI compliance), in the Sun/Solaris environment the function isnan doesn't
  get declared from math.h. Declare it here.

  In a Microsoft environment the correct functions look to be _finite and
  _isnan from float.h.

  Sun/Solaris typedef's an enum, fpclass_t, for IEEE FP number classification
  codes, and bonsaiG and dylp expect to see it. GNU C gets clever, and uses
  an unnamed enum to create integer values for #define's of the
  classification symbols.  Pretty much everyone else sticks with simple
  #define's.

  Sun/Solaris names the classification function fpclass. Others call the
  function fpclassify.
*/

#if (defined(__DYLP_SUN) || defined(__DYLP_SUNWspro))
# include <ieeefp.h>
  extern int isnan(double dsrc) ;
#else
# if defined(_MSC_VER)
#   include <float.h>
#   define finite _finite
#   define isnan _isnan
# endif
  typedef int fpclass_t ;
# define fpclass fpclassify
#endif

/*
  There's an uglier problem. Some environments (Sun, HP, for example) define
  10 FP number class codes, separating each major category (normalised, zero,
  infinity, denormalised) into two codes by sign and separating NaN into
  quiet and signalling NaN. GNU doesn't make this separation (a far more
  sensible approach in the case of sign, arguable for NaN).

  The approach taken here to deal with the problem is to redefine each pair
  of codes from the 10 code space to be one code from the 5 code space.  But
  you really have to be cognisant of the problem and code very carefully if
  you want to make explicit use of class codes in both environments. Try
  really hard to stick to functions like isnan, isinf, finite, etc.

  The only code you need for bonsai, under normal circumstances, is FP_QNAN.
  It's used in bound_utils.c.
*/

#ifdef FP_NAN
# ifndef FP_QNAN
#  define FP_QNAN FP_NAN
# endif
#endif

/*
  Some machines don't seem to properly distinguish QNaN and SNaN, even though
  they define separate FP class codes. If your machine insists on returning
  the code for SNaN for both QNaN and SNaN, fix it here.
*/

#ifdef __DYLP_BROKEN_FPCLASS
# undef FP_QNAN
# define FP_QNAN FP_SNAN
#endif

/*
  Here are the remaining redefinitions of the FP_ codes. If any of these give
  trouble on compilation, you can safely comment out the lot of them unless
  you're using the bounds consistency check routines in bound_utils.c. And if
  you're that far into the code, you should know enough to deal with the
  problem without my help.
*/

#ifdef FP_NAN
# ifndef FP_SNAN
#  define FP_SNAN FP_NAN
# endif
# ifndef FP_NINF
#  define FP_NINF FP_INFINITE
# endif
# ifndef FP_PINF
#  define FP_PINF FP_INFINITE
# endif
# ifndef FP_NDENORM
#  define FP_NDENORM FP_SUBNORMAL
# endif
# ifndef FP_PDENORM
#  define FP_PDENORM FP_SUBNORMAL
# endif
# ifndef FP_NZERO
#  define FP_NZERO FP_ZERO
# endif
# ifndef FP_PZERO
#  define FP_PZERO FP_ZERO
# endif
# ifndef FP_NNORM
#  define FP_NNORM FP_NORMAL
# endif
# ifndef FP_PNORM
#  define FP_PNORM FP_NORMAL
# endif
#endif


/*
  Packed Vectors

  The packed vector type consists of a header plus an array of <index, value>
  pairs for the non-default entries of the vector.

  pkcoeff_struct

  Field		Description
  -----		-----------
  ndx		the column/row index for the coefficient
  val		the value of the coefficient

  pkvec_struct

  Field		Description
  -----		-----------
  ndx		the common index for all coefficients when the vector is a
		row or column from a matrix
  nme		name associated with this vector, if any
  dim		length of the vector when unpacked
  dflt		the default value of coefficients not in coeffs
  cnt		number of non-default coefficients in the coeffs array
  sze		allocated capacity (in pkcoeff_struct's) of the coeffs array
  coeffs	the array of (column/row index, coefficient) pairs

  NOTE: pkvec_struct->coeffs is indexed from 0 and sized accordingly.
*/

typedef struct { int ndx ;
		 double val ; } pkcoeff_struct ;

typedef struct { int ndx ;
		 const char *nme ;
		 int dim ;
		 double dflt ;
		 int cnt ;
		 int sze ;
		 pkcoeff_struct *coeffs ; } pkvec_struct ;

pkvec_struct *pkvec_new(int sze) ;
bool pkvec_resize(pkvec_struct *pkvec, int sze) ;
void pkvec_free(pkvec_struct *pkvec) ;

bool pkvec_check(pkvec_struct *pkvec, const char *caller) ;

double pkvec_2norm(pkvec_struct *vec) ;

double exvec_1norm(double *vec, int len),
       exvec_ssq(double *vec, int len),
       exvec_2norm(double *vec, int len),
       exvec_infnorm(double *vec, int len, int *p_jmax) ;

double pkvec_dotexvec(pkvec_struct *pkvec, double *exvec) ;

#endif /* _DYLP_VECTOR_H */

