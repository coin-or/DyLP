#ifndef _DYLIB_ERRS_H
#define _DYLIB_ERRS_H

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

void errinit(const char *elogpath, bool errecho),
     errterm(void) ;

void errmsg(int errid, ... ),
     dywarn(int errid, ... ) ;

#endif /* _DYLIB_ERRS_H */
