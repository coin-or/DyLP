/*
  This file is part of the support library for the Dylp LP distribution.

        Copyright (C) 2005 -- 2007 Lou Hafer

        School of Computing Science
        Simon Fraser University
        Burnaby, B.C., V5A 1S6, Canada
        lou@cs.sfu.ca

  This code is licensed under the terms of the Common Public License (CPL).
*/

/*
  This file contains general purpose string manipulation routines.
*/

#include "dylib_std.h"

static char sccsid[] UNUSED = "@(#)strrtns.c	1.5	09/25/04" ;
static char svnid[] UNUSED = "$Id$" ;



int cistrcmp (const char *str1, const char *str2)

/*
  This routine compares two strings. It is insensitive to case ; both strings
  are converted to  upper case before the comparison.

  Parameters:
    str1,str2:	pointers to strings to be compared. strings should be null
		terminated

  Return value:
    -1	str1 lexicographically less than str2
     0  str1 lexicographically equal to str2
     1  str1 lexicographically greater than str2
*/

{ char cnv1,cnv2 ;

  while (!(*str1 == '\0' && *str2 == '\0'))
  { if (*str1 >= 'a' && *str1 <= 'z')
      cnv1 = *str1++-('a'-'A') ;
      else
      cnv1 = *str1++ ;
    if (*str2 >= 'a' && *str2 <= 'z')
      cnv2 = *str2++-('a'-'A') ;
      else
      cnv2 = *str2++ ;
    if (cnv1 < cnv2)
      return (-1) ;
    if (cnv1 > cnv2)
      return (1) ; }
  return (0) ; }



int cimstrcmp (const char *str1, const char *str2)

/*
  This routine compares two strings. It is insensitive to case ; both strings
  are converted to  upper case before the comparison. If str1 is shorter than
  str2 but equal up to its end, this routine reports it as equal.

  Parameters:
    str1,str2:	pointers to strings to be compared. strings should be null
		terminated

  Return value:
    -1	str1 lexicographically less than str2
     0  str1 lexicographically equal to str2 (as described above)
     1  str1 lexicographically greater than str2
*/

{ char cnv1,cnv2 ;

  while (!(*str1 == '\0' && *str2 == '\0'))
  { if (*str1 >= 'a' && *str1 <= 'z')
      cnv1 = *str1++-('a'-'A') ;
      else
      cnv1 = *str1++ ;
    if (*str2 >= 'a' && *str2 <= 'z')
      cnv2 = *str2++-('a'-'A') ;
      else
      cnv2 = *str2++ ;
    if (cnv1 < cnv2)
    { if (cnv1 == '\0')
	return (0) ;
      else
	return (-1) ; }
    if (cnv1 > cnv2)
      return (1) ; }
  return (0) ; }



int mstrcmp (const char *str1, const char *str2)

/*
  This routine compares two strings. If str1 is shorter than str2 but equal up
  to its end, this routine reports it as equal. The comparison is case
  sensitive.

  Parameters:
    str1,str2:	pointers to strings to be compared. strings should be null
		terminated

  Return value:
    -1	str1 lexicographically less than str2
     0  str1 lexicographically equal to str2 (as described above)
     1  str1 lexicographically greater than str2
*/

{ for ( ; !(*str1 == '\0' && *str2 == '\0') ; str1++,str2++)
  { if (*str1 < *str2)
    { if (*str1 == '\0')
	return (0) ;
      else
	return (-1) ; }
    if (*str1 > *str2)
      return (1) ; }
  return (0) ; }



char *strsave (const char *original)

/*
  This routine copies the string pointed to by original into a new string
  and returns a pointer to the new string.

  Parameters:
    original	pointer to string to be saved

  Return Value:
    normal:	pointer to new copy of the string
    error:	NULL
*/

{ char *copy ;

  copy = MALLOC(strlen(original)+1) ;
  if (copy != NULL)
    strcpy(copy,original) ;
  return (copy) ; }

