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
  This is an error reporting/logging package. It contains routines to
  print formatted error and warning messages. The client passes a numeric
  identifier, along with any necessary parameters. The routines will look
  up the identifier in an array of error message templates and pass the
  message, with parameters, to vfprintf.

  If the conditional compilation variable DYLP_NDEBUG is defined, the warn
  routine is converted to a complete noop.

  In addition to reporting error messages via stderr, the package has
  provision for optionally logging error messages into a separate file,
  specified by elogchn.
*/

#include "dylib_std.h"

#include <stdio.h>
#include <stdarg.h>

#ifndef MALLOC
#define MALLOC(zz_sze_zz) malloc(zz_sze_zz)
#endif

/*
  Declarations for the error message template array.
*/

extern const char *dy_errmsgs[] ;
extern int dy_errmsgCnt ;

/*
  elogchn is the channel used to access an error log file. errecho controls
  whether error messages go to stderr.
*/

static FILE *elogchn ;
static char *elogname ;
static bool errecho ;

/*
  The text of an error message (the unexpanded pattern) can be no longer than
  MAXERRTXT.
*/

#define MAXERRTXT 240



void errinit (const char *elogpath, bool echo)

/*
  This routine initializes the error reporting facilities. It attempts to
  open an error logging file, if given, and records whether error messages
  should be echoed to stderr in addition to the log file.

  Parameters:
    elogpath:	path name for logging error messages (the default is to not
                log error messages)
    echo:	TRUE if error messages should be echoed to stderr, FALSE
		otherwise.

  Returns: undefined
*/

{ const char *rtnnme = "errinit" ;

/*
  If a log file name has been supplied, try to open it. Keep the name locally
  for the benefit of errlogq, should it be called.
*/
  if (elogpath == NULL)
  { elogchn = NULL ;
    elogname = NULL ; }
  else
  { elogname = (char *) MALLOC(strlen(elogpath)+1) ;
    strcpy(elogname,elogpath) ;
    elogchn = fopen(elogname,"w") ;
    if (elogchn == NULL)
    { fprintf(stderr,"\n%s: couldn't open error logging file \"%s\".\n",
	      rtnnme,elogname) ;
      perror(rtnnme) ; } }
  
  errecho = echo ;

  return ; }


void errterm (void)

/*
  This routine cleans up data structures owned by the error package and closes
  the error log file, if present. Don't close stdin!

  ANSI stdio doesn't seem to provide a standard method for checking if a file
  is closed. A bit of testing says that ftell should be ok for testing normal
  files. Since we don't want to be closing stdin, stdout, or stderr, it's
  actually a feature to report an error for them.


  Parameters: none

  Returns: undefined.
*/

{ char *rtnnme = "errterm" ;

  if (elogchn != NULL && elogchn != stdout && elogchn != stderr &&
      ftell(elogchn) >= 0 )
  { if (fclose(elogchn) < 0)
    { fprintf(stderr,"\n%s: couldn't close error log file \"%s\".\n",
	      elogname,rtnnme) ;
      perror(rtnnme) ; }
    elogchn = NULL ; }
  if (elogname != NULL)
  { FREE(elogname) ;
    elogname = NULL ; }

  return ; }



/*
  These next two routines are exported only to io.c in the default setup.
  They are used to allow the user to control the error log files through
  io.c
*/

FILE *errlogq (char **elogpath)

/*
  This routine is here to keep the compartmentalisation of errs.c. Its sole
  purpose in life is to return the stdio FILE handle and path name for the
  error log file.

  Parameters:
    elogpath	(o) Will be set to point to the name of the error log file,
		    or NULL if no error log file is specified.

  Returns: the file handle, which will be NULL if no error log file exists.
*/

{ *elogpath = elogname ;
  return (elogchn) ; }


bool reseterrlogchn (const char *newpath, FILE *newchn, bool echo, bool close)

/*
  Another routine solely dedicated to compartmentalisation. If newpath is
  non-NULL, the routine will reset the log file, opening it if newchn is NULL,
  otherwise using newchn. The errecho flag is set to echo.

  Parameters:
    newpath:	new path for the log file; ignored if NULL
    newchn:	new handle for the log file channel; ignored if NULL
    echo:	TRUE if errors should be echoed to stderr, FALSE otherwise.
    close:	TRUE of the old file should be closed, FALSE otherwise.
  
  Returns: TRUE if the new file is opened successfully, FALSE otherwise
	   (note that failure to close the old file still gets a TRUE return)
*/

{ bool success ;
  const char *rtnnme = "reseterrlogchn" ;

/*
  If newchn is NULL, try to open the specified path. Retain the previous
  settings if we can't open the new file. If newchn isn't NULL, use it
  without question. There's the possibility the user has called us only to
  change the echo, in which case newpath will be NULL.
*/
  success = TRUE ;
  if (newpath != NULL)
  { if (newchn == NULL)
    { newchn = fopen(newpath,"w") ;
      if (newchn == NULL)
      { fprintf(stderr,"\n%s: couldn't open error logging file \"%s\".\n",
		rtnnme,newpath) ;
	perror(rtnnme) ;
	fprintf(stderr,"\n%s: retaining previous file \"%s\".\n",
		rtnnme,elogname) ;
	success = FALSE ; } }

    if (success == TRUE)
    { if (close == TRUE)
      { if (fclose(elogchn) == EOF)
	{ fprintf(stderr,
		  "\n%s: couldn't close previous error logging file \"%s\".\n",
		  rtnnme,elogname) ;
	  perror(rtnnme) ; } }
      elogchn = newchn ;
      if (elogname != NULL) FREE(elogname) ;
      elogname = (char *) MALLOC(strlen(newpath)+1) ;
      strcpy(elogname,newpath) ; } }

  errecho = echo ;

  return (success) ; }



static void errOrWarn (char *msgType, int errid, va_list varargs)

/*
  The implementation for errmsg and warn. First check that errid specifies a
  valid message. If it does not, construct something appropriate and print it.
  If the errid is valid, invoke vfprintf to print the specified message.

  Parameter:
    msgType:	type of message ("error", "warning", ...)
    errid:	error number

  Returns: undefined
*/

{ char *ident ;

/*
  Construct the prefix and copy it into the main message buffer. We'll be
  a few chars too big in the size estimate for pfxBuf. No harm done.
*/
  char *pfxTmplt = "%%s (%s): " ;
  int pfxLen = strlen(pfxTmplt)+strlen(msgType) ;
  char pfxBuf[pfxLen] ;
  sprintf(pfxBuf,pfxTmplt,msgType) ;
  pfxLen = strlen(pfxBuf) ;

  char msgBuf[MAXERRTXT+pfxLen+1] ;
  strcpy(msgBuf,pfxBuf) ;
/*
  Flush stdout and elogchn, so that any buffered normal output appears before
  the error message.
*/
  fflush(stdout) ;
  if (elogchn != NULL) fflush(elogchn) ;
/*
  Check that the error id is within range and points to a defined error
  message (i.e., not '<<open>>'). If either fails, construct an appropriate
  error message.
*/
  bool badErr = FALSE ;
  if (errid <= 0 || errid > dy_errmsgCnt)
  { sprintf(msgBuf+pfxLen,"error message %d out of range 1 .. %d.",
      errid,dy_errmsgCnt) ;
    badErr = TRUE ; }
  else if (strcmp(dy_errmsgs[errid],"<<open>>") == 0)
  { sprintf(msgBuf+pfxLen,"error message %d is '%s'.",
      errid,dy_errmsgs[errid]) ;
    badErr = TRUE ; }
  else
  { strcpy(msgBuf+pfxLen,dy_errmsgs[errid]) ; }
/*
  Print as appropriate. The constructed messages for badErr need only the
  ident. Because each call to vfprintf leaves the varargs object in an
  undefined state, make a copy for reuse.

  It's helpful to think about va_start as creating an object which can be
  used once and then destroyed with va_end. We can't actually create the
  object here (that has to be done in the calling routines errmsg and warn,
  which have access to the original variable parameters on the stack). What
  we can do here is make a copy, use it, and destroy it. The original object
  passed in via the varargs parameter is created and destroyed up in the
  calling routines.
*/
  if (badErr == TRUE)
  { ident = va_arg(varargs,char *) ;
    if (errecho == TRUE)
    { putc('\n',stderr) ;
      fprintf(stderr,msgBuf,ident) ;
      putc('\n',stderr) ; }
    if (elogchn != NULL)
    { putc('\n',elogchn) ;
      fprintf(elogchn,msgBuf,ident) ;
      putc('\n',elogchn) ; } }
  else
  { va_list origVarargs ;
    va_copy(origVarargs,varargs) ;
    if (errecho == TRUE)
    { putc('\n',stderr) ;
      vfprintf(stderr,msgBuf,varargs) ;
      putc('\n',stderr) ; }
    if (elogchn != NULL) 
    { putc('\n',elogchn) ;
      vfprintf(elogchn,msgBuf,origVarargs) ;
      putc('\n',elogchn) ; }
    va_end(origVarargs) ; }
/*
  Flush the logged error message, so that the user will definitely see it.
*/
  if (elogchn != NULL) fflush(elogchn) ;

  return ; }


void errmsg (int errid, ... )

/*
  Actual Call:
    errmsg(errid,ident,arg1, ... ,argn)

  errmsg is the error reporting function used by clients of this package.
  It prints a message with the format "ident (error): error message".
  errid identifies a printf-style error message template which is held in
  the dy_errmsgs array.

  A variable number of parameters may follow ident; they are assumed to
  be compatible with the way vfprintf will interpret the error message
  template. The message is printed to stderr and to the error log file,
  if it is open.

  Parameters:
    errid:	number of the generic error message
    ident:	string used to prefix the error message (usually a routine
		name or similar identifier)
    arg1,...:	the arguments to be spliced into the error message template

  Returns: undefined
*/

{ va_list varargs ; 

  va_start(varargs,errid) ;
  errOrWarn("error",errid,varargs) ;
  va_end(varargs) ;

  return ; }



#ifdef DYLP_NDEBUG

void dywarn (int errid, ... )

{ return ; }

#else

void dywarn (int errid, ... )

/*
  Actual Call:
    dywarn(errid,ident,arg1, ... ,argn)

  Warn is functionally identical to errmsg, but prints the message
  "ident (warning): warning message".

  Parameters:
    errid:	number of the generic error message
    ident:	string used to prefix the error message (usually a routine
		name or similar identifier)
    arg1,...:	the arguments to be spliced into the error message template

  Returns: undefined
*/

{ va_list varargs ; 

  va_start(varargs,errid) ;
  errOrWarn("warn",errid,varargs) ;
  va_end(varargs) ;

  return ; }

#endif /* DYLP_NDEBUG */

