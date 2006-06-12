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

/*
  This code is meant to compile and link using the command

    cc -lm fpchecks.c
  
  where cc should be replaced with the name of your local C compiler. Asking
  for libmath should be safe (if redundant) in any C environment.

  The first thing that this file does is to try and identify the computing
  environment and set some macro names that bonsaiG/dylp will recognize. The
  approach here is to look for macro names defined by the programming
  environment.

  Environments known to bonsaiG/dylp are:

    * Sun Solaris/Workshop: __SUNPRO_C => __DYLP_SUNWspro
    * Sun Solaris/Gnu: __GNUC__, __sun => __DYLP_SUN
    * Other Gnu: __GNUC__ => _GNU_SOURCE
  
  _GNU_SOURCE is meaningful to GCC compilers (features.h). You'd think
  _ISOC99_SOURCE would suffice to bring in all IEEE FP support functions in
  the Gnu environment, but that doesn't seem to hold uniformly.  _GNU_SOURCE
  seems like overkill, but it's robust.  Recently (as of 3.0?) IEEE 754
  floating point support looks to be much improved. When it looks like we can
  count on seeing a GCC environment at 3.0 or later, the whole issue should
  be reviewed.

  __DYLP_SUNWspro and __DYLP_SUN are meaningful only in osidylp code.  It's
  likely that bonsaiG/dylp should be more fussy about the hardware in a Linux
  environment. Your mileage may vary.

  Once the environment is settled, the code checks various characteristics of
  the floating point computation environment and reports on the proper values
  for macro definitions in the dylp makefile. Specifically, we try to
  determine:

    * If your machine is big-endian or little-endian. bonsaiG/dylp assume a
      big-endian machine by default (a consequence of growing up on SPARC
      hardware).

    * The extent to which your machine supports IEEE floating point `special'
      values used by bonsaiG/dylp: infinity (Inf) and not-a-number (NaN).
  
  In case it needs repeating: big-endian machines store the most significant
  bits of a multibyte number in the the lower numbered memory locations (big
  end first).  little-endian machines store the most significant bits in the
  higher numbered memory location. Given the usual conventions for writing
  numbers (most significant digits to the left) and writing memory locations
  (lower numbered addresses to the left), multibyte numbers tend to look
  scrambled when printed out a byte at a time on a little-endian machine.

  NOTE: There is one more endian type recognised by GCC, PDP_ENDIAN. dylp
  isn't prepared to support this. It also seems to be rare.
*/
/*
  Rather than go through the bother required to get this string into the
  binary, we'll settle for a comment.

  sccs: sccsid[] = "@(#)fpchecks.c	1.3	09/25/04" ;
  svn/cvs: svnid[] = "$Id$" ;
*/

#if defined(__SUNPRO_C) || defined (__SUNPRO_CC)
# define __DYLP_SUNWspro
#else
# if defined(__GNUC__)
#   define _GNU_SOURCE
#   ifdef __sun
#     define __DYLP_SUN
#   endif
# endif
#endif

/* AW: I added this for HP machines */
#ifdef _HPUX_SOURCE
# define finite isfinite
#endif

#include <string.h>
#include <stdio.h>
#include <math.h>

void exit (int status) ;

/*
  Why ctype.h? Well, it's part of the ANSI C specification, so it really should
  exist. And, for at least Solaris and Linux environments that I'm familiar
  with, it'll pull in the include file with endian macros.
*/
#include <ctype.h>

/*
  We'll use this union type to construct IEEE FP bit patterns.
*/

typedef union { unsigned char fpchr[8] ; double fpdbl ; } fpunion_t ;

/*
  In the Sun Workshop environment, there are functions that return the IEEE
  FP bit patters for Inf (infinity), SNaN (signaling_nan), and QNaN
  (quiet_nan). Unfortunately, they are in the libsunmath, which is not linked
  by default. Since this program should build with a vanilla compile and link
  on all systems, we'll use these macros to fake the functions in all
  environments.
*/

# define infinity() (QNaNbits->fpdbl)
# define quiet_nan(zz_dummy_zz) (QNaNbits->fpdbl)
# define signaling_nan(zz_dummy_zz) (SNaNbits->fpdbl)

/*
  Make IEEE floating point bit patterns for infinity (Inf), signalling NaN
  (SNaN), and quiet NaN (QNaN), in big-endian and little-endian format.

  We need the `(unsigned char)' here to prevent some compilers from
  complaining about the initialiser being out of range. Goes to the ANSI C
  rule that `Character constants not preceded by the letter L have type int.'
*/


static fpunion_t LE_QNaNbits = { { (unsigned char) '\376',
				   (unsigned char) '\377',
				   (unsigned char) '\377',
				   (unsigned char) '\377',
				   (unsigned char) '\377',
				   (unsigned char) '\377',
				   (unsigned char) '\377',
				   (unsigned char) '\177' } } ;
static fpunion_t LE_SNaNbits = { { (unsigned char) '\001',
				   (unsigned char) '\0',
				   (unsigned char) '\0',
				   (unsigned char) '\0',
				   (unsigned char) '\0',
				   (unsigned char) '\0',
				   (unsigned char) '\360',
				   (unsigned char) '\177' } } ;
static fpunion_t LE_Infbits = { { (unsigned char) '\0',
				  (unsigned char) '\0',
				  (unsigned char) '\0',
				  (unsigned char) '\0',
				  (unsigned char) '\0',
				  (unsigned char) '\0',
				  (unsigned char) '\360',
				  (unsigned char) '\177' } } ;

static fpunion_t BE_QNaNbits = { { (unsigned char) '\177',
				   (unsigned char) '\377',
				   (unsigned char) '\377',
				   (unsigned char) '\377',
				   (unsigned char) '\377',
				   (unsigned char) '\377',
				   (unsigned char) '\377',
				   (unsigned char) '\376' } } ;
static fpunion_t BE_SNaNbits = { { (unsigned char) '\177',
				   (unsigned char) '\360',
				   (unsigned char) '\0',
				   (unsigned char) '\0',
				   (unsigned char) '\0',
				   (unsigned char) '\0',
				   (unsigned char) '\0',
				   (unsigned char) '\001' } } ;
static fpunion_t BE_Infbits = { { (unsigned char) '\177',
				  (unsigned char) '\360',
				  (unsigned char) '\0',
				  (unsigned char) '\0',
				  (unsigned char) '\0',
				  (unsigned char) '\0',
				  (unsigned char) '\0',
				  (unsigned char) '\0' } } ;

/*
  finite and fpclass functions

  finite() checks for IEEE infinity. fpclass is a more general function that
  returns the class of the FP number (normalized, denormalized, zero, infinity,
  NaN, with variations).

  In the Sun environment, finite and fpclass are defined in ieeefp.h. Be
  aware that fpclass returns a 10-value enum, fpclass_t. There's another
  function, fp_class, which returns a 5-value enum, fp_class_type. This
  function is only found in the Workshop file sunmath.h. BonsaiG and dylp
  expect the 10-value model.

  Sun/Solaris typedef's an enum for IEEE FP number classification codes. GNU C
  gets clever, and uses an unnamed enum to create integer values for #define's.
  Pretty much everyone else sticks with simple #define's. Sun/Solaris names the
  classification function fpclass. Others call the function fpclassify. 
*/

#if (defined(__DYLP_SUN) || defined(__DYLP_SUNWspro))
# include <ieeefp.h>
#else
  typedef int fpclass_t ;
# define fpclass fpclassify
#endif

/*
  There's an uglier problem. Some environments (Sun, HP, for example) define
  10 FP number class codes, separating each major category (normalised, zero,
  infinity, denormalised) into two codes by sign and separating not-a-number
  into quiet and signalling NaN. Others don't (a far more sensible approach in
  the case of sign, arguable for NaN). Unfortunately, to make the strong bounds
  consistency checks in bound_utils.c work in both circumstances, they need
  to be rewritten using if-then-else instead of case. With any luck you
  shouldn't need them, and if you're working at that level, you're capable of
  the necessary rewrite on your own. The following definitions take care of
  making sure FP_QNAN and FP_SNAN are available, which is all bonsaiG needs in
  normal operation.
*/
#ifdef FP_NAN
# ifndef FP_QNAN
#  define FP_QNAN FP_NAN
# endif
# ifndef FP_SNAN
#  define FP_SNAN FP_NAN
# endif
#endif


/*
  The main routine. This will print out a list of defines to be used during
  compilation. Given the -v option, it will print explanatory messages.
*/

int main (int argc, char *argv[])

{ int i,verbose,nonnull,nonan ;
  double val ;
  long dummy ;
  fpunion_t *Infbits,*QNaNbits,*SNaNbits ;
  unsigned char *valchr ;

verbose = 0 ;
if (argc > 1)
{ if (argc == 2 && !strcmp(argv[1],"-v"))
    verbose = 1 ;
  else
  { printf("usage: %s [-v]\n",argv[0]) ;
    exit (1) ; } }
/*
  Print the appropriate define for the programming environment. Force
  _GNU_SOURCE so we're sure we have all the IEEE math functions we need.
  If we don't have Forte/Workshop or GCC, don't force anything.
*/
nonnull = 0 ;
# if defined(__DYLP_SUNWspro)
    printf("-D__DYLP_SUNWspro") ;
    nonnull = 1 ;
    if (verbose) printf("\nSun Workshop programming environment.\n") ;
# elif defined(__DYLP_SUN)
    printf("-D__DYLP_SUN -D_GNU_SOURCE") ;
    nonnull = 1 ;
    if (verbose) printf("\nSun/Gnu programming environment.\n") ;
# elif defined(_GNU_SOURCE) 
    printf("-D_GNU_SOURCE") ;
    nonnull = 1 ;
    if (verbose) printf("\nLinux/Gnu programming environment.\n") ;
# endif

/*
  Check data sizes. We're betting that a long is 4 bytes and a double is 8.
  Coping with other sizes isn't an insuperable problem, but this program
  would need to be modified to be more flexible.
*/
/* TODO: AW: I took this out for now to compile on 64 bit machines
  if (sizeof(long) != 4)
  { printf("\nsizeof(long) = %d bytes, expecting 4 bytes.", sizeof(long)) ;
    printf("\nThis program can't cope.") ;
    exit (1) ; }
*/
  if (sizeof(double) != 8)
  { printf("\nsizeof(double) = %d bytes, expecting 8 bytes.", sizeof(double)) ;
    printf("\nThis program can't cope.") ;
    exit (1) ; }
/*
  Do we have the usual macros for endedness? In a Linux/GCC environment, the
  symbols will start with two underscores and are pulled in with ctype.h. In
  a Sun environment (Workshop or GCC) the symbol with one underscore seems to
  be defined at some very basic level (i.e., math.h isn't necessary to get it).
*/
# if (defined(__BYTE_ORDER) && (__BYTE_ORDER == __BIG_ENDIAN)) || \
     (!defined(__BYTE_ORDER) && \
       (defined(_BIG_ENDIAN) || defined(__BIG_ENDIAN)))
  if (verbose) printf("Your system is big-endian.\n") ;
  Infbits = &BE_Infbits ;
  QNaNbits = &BE_QNaNbits ;
  SNaNbits = &BE_SNaNbits ;
# elif (defined(__BYTE_ORDER) && (__BYTE_ORDER == __LITTLE_ENDIAN)) || \
     (!defined(__BYTE_ORDER) && \
       (defined(_LITTLE_ENDIAN) || defined(__LITTLE_ENDIAN)))
  if (verbose) printf("\nYour system is little-endian.\n") ;
  Infbits = &LE_Infbits ;
  QNaNbits = &LE_QNaNbits ;
  SNaNbits = &LE_SNaNbits ;
# else
/*
  Test for big- or little-endedness. A four byte value should do it. Set the
  IEEE FP bit patterns accordingly. The particular values for __BIG_ENDIAN
  and __LITTLE_ENDIAN are GCC compatible.
*/
  if (verbose) printf("\nChecking byte ordering ...") ;
  dummy = 0x00010203L ;
  valchr = (unsigned char *) &dummy ;
  if (verbose)
  { printf("\n0x00010203L bit pattern is stored as ") ;
    for (i = 0 ; i <= 2 ; i++) printf("%x ",(unsigned int) valchr[i]) ;
    printf("%x\n",(unsigned int) valchr[3]) ; }
  if (valchr[0] == 0x00)
  { if (nonnull) printf(" ") ;
    printf("-D__BIG_ENDIAN=4321") ;
    nonnull = 1 ;
    if (verbose) printf("Your system is big-endian.\n") ;
    Infbits = &BE_Infbits ;
    QNaNbits = &BE_QNaNbits ;
    SNaNbits = &BE_SNaNbits ;
  }
  else
  { if (nonnull) printf(" ") ;
    printf("-D__LITTLE_ENDIAN=1234") ;
    nonnull = 1 ;
    if (verbose) printf("\nYour system is little-endian.\n") ;
    Infbits = &LE_Infbits ;
    QNaNbits = &LE_QNaNbits ;
    SNaNbits = &LE_SNaNbits ; }
# endif
/*
  Now test that we have proper behaviour for infinity.
*/
  if (verbose)
  { printf("\nInf = %g, finite says %d, isnan says %d, fpclass says %d.\n",
	   Infbits->fpdbl,finite(Infbits->fpdbl),isnan(Infbits->fpdbl),
	   fpclass(Infbits->fpdbl)) ;
    printf("Inf bit pattern is ") ;
    for (i = 0 ; i <= 6 ; i++) printf("%x ",(unsigned int) Infbits->fpchr[i]) ;
    printf("%x\n",(unsigned int) Infbits->fpchr[7]) ;

    printf("HUGE_VAL = %g, finite says %d, isnan says %d, fpclass says %d.\n",
	   HUGE_VAL,finite(HUGE_VAL),isnan(HUGE_VAL),fpclass(HUGE_VAL)) ; }

  if (HUGE_VAL != Infbits->fpdbl)
  { if (nonnull) printf(" ") ;
    printf("-D__DYLP_FIX_HUGE_VAL") ;
    nonnull = 1 ;
    if (verbose) printf("\nHUGE_VAL is not IEEE infinity.\n") ; }
/*
  Check whether we have distinct QNaN and SNaN bit patterns. This test is
  pointless if we have only FP_NAN to begin with, in which case we check to
  see which NaN patterns are recognised. The final case (unrecognised NaN
  bit pattern) should never occur, but there's no harm in having it. Nor
  have I ever encountered a system that recognises only QNaN or only SNaN.
  Both would be violations of the IEEE standard, as I understand it.
*/
#ifndef FP_NAN
  if (verbose)
  { printf("\nSystem defines both QNaN and SNaN.\n") ;
    printf("QNaN = %g, finite says %d, isnan says %d, fpclass says %d.\n",
	   QNaNbits->fpdbl,finite(QNaNbits->fpdbl),isnan(QNaNbits->fpdbl),
	   fpclass(QNaNbits->fpdbl)) ;
    printf("QNaN bit pattern is ") ;
    for (i = 0 ; i <= 6 ; i++) printf("%x ",(unsigned int) QNaNbits->fpchr[i]) ;
    printf("%x\n",(unsigned int) QNaNbits->fpchr[7]) ;
    printf("SNaN = %g, finite says %d, isnan says %d, fpclass says %d.\n",
	   SNaNbits->fpdbl,finite(SNaNbits->fpdbl),isnan(SNaNbits->fpdbl),
	   fpclass(SNaNbits->fpdbl)) ;
    printf("SNaN bit pattern is ") ;
    for (i = 0 ; i <= 6 ; i++) printf("%x ",(unsigned int) SNaNbits->fpchr[i]) ;
    printf("%x\n",(unsigned int) SNaNbits->fpchr[7]) ; }

  if (fpclass(QNaNbits->fpdbl) == fpclass(SNaNbits->fpdbl))
  { if (nonnull) printf(" ") ;
    printf("-D__DYLP_BROKEN_FPCLASS") ;
    nonnull = 1 ;
    if (verbose)
      printf("\nfpclass does not distinguish between QNaN and SNaN.\n") ; }
  else
  { if (verbose) printf("fpclass distinguishes between QNaN and SNaN.\n") ; }
#else
  if (verbose)
  { nonan = 1 ;
    printf("System defines only NaN.\n") ;
    if (isnan(QNaNbits->fpdbl) != 0)
    { printf("System recognises the QNaN bit pattern.\n") ;
      nonan = 0 ;
      printf("NaN = %g, finite says %d, isnan says %d, fpclass says %d.\n",
	     QNaNbits->fpdbl,finite(QNaNbits->fpdbl),isnan(QNaNbits->fpdbl),
	     fpclass(QNaNbits->fpdbl)) ;
      printf("NaN bit pattern is ") ;
      for (i = 0 ; i <= 6 ; i++)
	printf("%x ",(unsigned int) QNaNbits->fpchr[i]) ;
      printf("%x\n",(unsigned int) QNaNbits->fpchr[7]) ; }
    if (isnan(SNaNbits->fpdbl) != 0)
    { printf("System recognises the SNaN bit pattern.\n") ;
      nonan = 0 ;
      printf("NaN = %g, finite says %d, isnan says %d, fpclass says %d.\n",
	     SNaNbits->fpdbl,finite(SNaNbits->fpdbl),isnan(SNaNbits->fpdbl),
	     fpclass(SNaNbits->fpdbl)) ;
      printf("NaN bit pattern is ") ;
      for (i = 0 ; i <= 6 ; i++)
	printf("%x ",(unsigned int) SNaNbits->fpchr[i]) ;
      printf("%x\n",(unsigned int) SNaNbits->fpchr[7]) ; }
    if (nonan)
    { printf("NaN does not conform to IEEE standards!\n") ;
      printf("You're in for some code hacking, I'm afraid.\n") ;
      val = HUGE_VAL-HUGE_VAL ;
      printf("NaN = %g, finite says %d, isnan says %d, fpclass says %d.\n",
	     val,finite(val),isnan(val),fpclass(val)) ;
      printf("NaN bit pattern is ") ;
      for (i = 0 ; i <= 6 ; i++)
	printf("%x ",(unsigned int) SNaNbits->fpchr[i]) ;
      printf("%x\n",(unsigned int) SNaNbits->fpchr[7]) ; } }
#endif

  exit (0) ;
}
