/*
   This file is part of the support library for the Dylp LP distribution.

        Copyright (C) 2005 -- 2007 Lou Hafer

        School of Computing Science
        Simon Fraser University
        Burnaby, B.C., V5A 1S6, Canada
        lou@cs.sfu.ca

  This code is licensed under the terms of the Eclipse Public License (EPL).

 * $Id$
 *
 * Include file for the configuration of DyLP.
 *
 * On systems where the code is configured with the configure script
 * (i.e., compilation is always done with HAVE_CONFIG_H defined), this
 * header file includes the automatically generated header file.
 *
 * On systems that are compiled in other ways (e.g., with the
 * Developer Studio), a header files is included to define those
 * macros that depend on the operating system and the compiler.  The
 * macros that define the configuration of the particular user setting
 * (e.g., presence of other COIN-OR packages or third party code) are set
 * by the files config_*default.h. The project maintainer needs to remember
 * to update these file and choose reasonable defines.
 * A user can modify the default setting by editing the config_*default.h files.
 */

#ifndef __DYLPCONFIG_H__
#define __DYLPCONFIG_H__

#ifdef HAVE_CONFIG_H
#ifdef DYLPLIB_BUILD
#include "config.h"
#else
#include "config_dylp.h"
#endif

#else /* HAVE_CONFIG_H */

#ifdef DYLPLIB_BUILD
#include "config_default.h"
#else
#include "config_dylp_default.h"
#endif

#endif /* HAVE_CONFIG_H */

#endif /*__DYLPCONFIG_H__*/
