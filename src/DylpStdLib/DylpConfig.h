/*
   This file is part of the support library for the Dylp LP distribution.

        Copyright (C) 2005 -- 2007 Lou Hafer

        School of Computing Science
        Simon Fraser University
        Burnaby, B.C., V5A 1S6, Canada
        lou@cs.sfu.ca
*/
/*
  This code is licensed under the terms of the Eclipse Public License (EPL).

  Include file for the configuration of DyLP.

  On systems where the code is configured with the configure script (i.e.,
  compilation is always done with HAVE_CONFIG_H defined), this header file
  includes the automatically generated header file.

  On systems that are compiled in other ways (e.g., with the Developer
  Studio), a header file is included to define those macros that depend
  on the operating system and the compiler.  The macros that define
  the configuration of the particular user setting (e.g., presence
  of other COIN-OR packages or third party code) are set by the files
  config_*default.h. The project maintainer needs to remember to update
  these files and choose reasonable defines.  A user can modify the default
  settings by editing the config_*default.h files.
*/

#ifndef __DYLPCONFIG_H__
#define __DYLPCONFIG_H__

#ifdef HAVE_CONFIG_H

#  ifdef DYLPLIB_BUILD
#    include "config.h"
#    ifdef DYLPLIB_EXPORT
#      undef DYLPLIB_EXPORT
#    endif
#    ifdef DLL_EXPORT
#      define DYLPLIB_EXPORT __declspec(dllexport)
#    elif defined(__GNUC__) && __GNUC__ >= 4
#      define DYLPLIB_EXPORT __attribute__((__visibility__("default")))
#    else
#      define DYLPLIB_EXPORT
#    endif
#  else /* DYLPLIB_BUILD */
#    include "config_dylp.h"
#  endif

#else /* HAVE_CONFIG_H */

#  ifndef DYLPLIB_EXPORT
#    if defined(_WIN32) && defined(DLL_EXPORT)
#      ifdef DYLPLIB_BUILD
#        define DYLPLIB_EXPORT __declspec(dllexport)
#      else
#        define DYLPLIB_EXPORT __declspec(dllimport)
#      endif
#    elif defined(__GNUC__) && __GNUC__ >= 4
#      ifdef DYLPLIB_BUILD
#        define DYLPLIB_EXPORT __attribute__((__visibility__("default")))
#      else
#        define DYLPLIB_EXPORT
#      endif
#    else
#      define DYLPLIB_EXPORT
#    endif
#    ifdef DYLPLIB_BUILD
#      include config_default.h
#    else
#      include config_dylp_default.h
#    endif
#  endif

#endif /* HAVE_CONFIG_H */

#endif /*__DYLPCONFIG_H__*/




