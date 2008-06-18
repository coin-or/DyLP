/*
  This file is a part of the Dylp LP distribution.

        Copyright (C) 2008 Lou Hafer

        School of Computing Science
        Simon Fraser University
        Burnaby, B.C., V5A 1S6, Canada
        lou@cs.sfu.ca

  This code is licensed under the terms of the Common Public License (CPL).
*/

/*
  We're getting close to the O/S here. Microsoft does it differently, of
  course. There's some disagreement in the unix community about what goes
  where, but I'm hoping that sys/time.h and sys/resource.h together will
  cover it.
*/
#include <time.h>
#if defined(_MSC_VER) || defined(__MSVCRT__)
  /* Nothing to do here. */
#else
# include <sys/time.h>
# include <sys/resource.h>
#endif

/*
  This is the main routine for the dylp unit test.
*/

#include "dylp.h"

const char *osidylp_version = "1.10" ;			/* sccs! */
const char *osidylp_time ;

/*
  Macro cleverness to specify a default error message file. Depends on ANSI
  C merge of consecutive string constants. DYLP_ERRMSGDIR should have the
  form "path/to/distribution/DyLP/src/Dylp/", including the quotes. See
  DyLP/src/DylpStdLib/DylpConfig.h for further information.
*/

#ifdef DYLP_ERRMSGDIR
# define DYLP_ERRMSGPATH DYLP_ERRMSGDIR "dy_errmsgs.txt"
#else
# define DYLP_ERRMSGPATH "dy_errmsgs.txt"
#endif

/*
  Variables which control i/o operations.

  ttyout		i/o id for output to the user's terminal
  ttyin			i/o id for input from the user's terminal
  dy_cmdchn		i/o id for input from the command file
  dy_logchn		i/o id for the execution log file

  dy_cmdecho		controls echoing of command input to stdout
  dy_gtxecho		controls echoing of generated text to stdout
*/

ioid dy_cmdchn,dy_logchn ;
bool dy_cmdecho, dy_gtxecho ;

/*
  Define these as globals for the benefit of cmdint.c::process_cmds.
*/

lpopts_struct* main_lpopts ;
lptols_struct* main_lptols ;




static lpret_enum do_lp (lpprob_struct *lp,
			 lptols_struct *lptols,
			 lpopts_struct *lpopts,
			 struct timeval *elapsed, int printlvl)
/*
  This routine is a convenience wrapper which handles statistics and timing.
  It also allows for easy adjustment of the print level by making a local copy
  of the options.

  Parameters:
    lp:		lp problem to be solved
    lptols:	tolerances
    lpopts:	options
    elapsed:	time value
    printlvl:	desired output level

  Returns: lp status code; lpINV is used in the event of a fatal error that's
	   not really dylp's fault.
*/

{ lpret_enum lpret ;
  lpopts_struct *local_lpopts ;
  lpstats_struct *local_lpstats ;

  consys_struct *sys ;

  struct timeval diff/*, before,after */ ;

  /* const char *rtnnme = "do_lp" ; */

  lpret = lpINV ;
  sys = lp->consys ;

/*
  Set up options and statistics.
*/
  local_lpopts = (lpopts_struct *) MALLOC(sizeof(lpopts_struct)) ;
  memcpy(local_lpopts,lpopts,sizeof(lpopts_struct)) ;
  dy_setprintopts(printlvl,local_lpopts) ;

# ifdef DYLP_STATISTICS
  local_lpstats = NULL ;
  dy_initstats(&local_lpstats,sys) ;
# else
  local_lpstats = NULL ;
# endif
/*
  Solve the lp.
*/
  /* get_timeval(&before) ; */
  lpret = dylp(lp,local_lpopts,lptols,local_lpstats) ;
  /*
   get_timeval(&after) ;
   sub_timevals(&diff,&after,&before) ;
  */
  if (elapsed != NULL) (*elapsed) = diff ;
/*
  Dump the statistics and free the dylp statistics structure.
*/
  /* stats_lp(outpath,FALSE,lp,&diff,local_lpstats) ; */
# ifdef DYLP_STATISTICS
  dy_freestats(&local_lpstats) ;
# endif
  if (local_lpopts != NULL) FREE(local_lpopts) ;

  return (lpret) ; }



int main (int argc, char **argv)

{ bool errecho = TRUE ;

  ioid ttyin,ttyout,outchn ;
  const char *errmsgpath = DYLP_ERRMSGPATH ;
  char *errlogpath = NULL ;

/*
  These need to be globals to keep cmdint.c::process_cmds happy.

  lpopts_struct *main_lpopts ;
  lptols_struct *main_lptols ;
*/
  consys_struct *main_sys ;
  lpprob_struct *main_lp ;
  lpret_enum lpretval ;

  double z ;

  char *rtnnme = argv[0] ;

  /* dy_basis.c */

  extern void dy_initbasis(int concnt, int factor_freq, double zero_tol),
	      dy_freebasis(void) ;

  /* dytest_problems.c */

  extern consys_struct *dytest_exmip1sys(lptols_struct *tols) ;
  extern consys_struct *dytest_afirosys(lptols_struct *tols) ;
  extern consys_struct *dytest_boeing2sys(lptols_struct *tols) ;

  /* dytest_tableau.c */

  extern bool dytest_betaj(lpprob_struct *lp,
			   lptols_struct *lptols,lpopts_struct *lpopts) ;
  extern bool dytest_abarj(lpprob_struct *lp,
			   lptols_struct *lptols,lpopts_struct *lpopts) ;

  outchn = IOID_INV ;
/*
  Execute initialization routines for the i/o and error reporting packages.
*/
  errinit(errmsgpath,errlogpath,errecho) ;
  if (dyio_ioinit() != TRUE)
  { errmsg(1,rtnnme,__LINE__) ;
    exit (2) ; }
/*
  Connect ttyout to the standard output. Initialize ttyin, setting the mode to
  line-oriented. Serious internal confusion if we can't manage these. Set the
  initial command input channel to stdin.
*/
  ttyout = dyio_openfile("stdout","w") ;
  if (ttyout == IOID_INV)
  { errmsg(1,rtnnme,__LINE__) ;
    exit(3) ; }
  ttyin = dyio_openfile("stdin","r") ;
  if (ttyin == IOID_INV)
  { errmsg(1,rtnnme,__LINE__) ;
    exit(4) ; }
  (void) dyio_setmode(ttyin,'l') ;
  dy_cmdchn = ttyin ;
  dy_cmdecho = TRUE ;
  dy_gtxecho = TRUE ;
/*
  Announce we're running.
*/
  dyio_outfmt(ttyout,dy_gtxecho,"Dylp unit test start.\n") ;
  dyio_flushio(ttyout,dy_gtxecho) ;
/*
  Acquire default option and tolerance structures.
*/
  main_lpopts = NULL ;
  main_lptols = NULL ;
  dy_defaults(&main_lpopts,&main_lptols) ;
/*
  Load the exmip1 example and see if we can solve it.
*/
  dyio_outfmt(ttyout,dy_gtxecho,"Loading exmip1 example from static data.\n") ;
  main_sys = dytest_exmip1sys(main_lptols) ;
  if (main_sys == NULL)
  { dyio_outfmt(ttyout,dy_gtxecho,
		"Failed to load exmip1 constraint system.\n") ;
    return (1) ; }
/*
  Check over the option settings, now that we know how big the constraint
  system will be.
*/
  dy_checkdefaults(main_sys,main_lpopts,main_lptols) ;
/*
  Initialise the main_lp structure to pass the problem in to dylp.
*/
  main_lp = (lpprob_struct *) CALLOC(1,sizeof(lpprob_struct)) ;
  setflg(main_lp->ctlopts,lpctlNOFREE) ;
  main_lp->phase = dyINV ;
  main_lp->consys = main_sys ;
  main_lp->rowsze = main_sys->rowsze ;
  main_lp->colsze = main_sys->colsze ;
  main_lpopts->forcecold = TRUE ;
  main_lpopts->fullsys = FALSE ;
  main_lpopts->finpurge.vars = TRUE ;
  main_lpopts->finpurge.cons = TRUE ;
  main_lpopts->coldbasis = ibLOGICAL ;
  main_lpopts->scaling = 2 ;
/*
  main_lpopts->print.tableau = 5 ;

  Initialise the basis maintenance package. The second parameter controls how
  many basis updates the basis can hold before it requires refactoring.
  Adding 5 to dylp's refactor interval should give a safety margin.
*/
  dyio_outfmt(ttyout,dy_gtxecho,"Solving exmip1 ... ") ;
  dy_initbasis(2*main_sys->concnt,main_lpopts->factor+5,0.0) ;

  main_lpopts->forcecold = TRUE ;
  main_lpopts->print.scaling = 2 ;
  lpretval = do_lp(main_lp,main_lptols,main_lpopts,NULL,2) ;
/*
  And the result is ...
*/
  dyio_outfmt(ttyout,dy_gtxecho,"%s, z = %.12f.\n",
	      dy_prtlpret(lpretval),main_lp->obj) ;
  z = 3.236842105263 ;
  if (!(fabs(main_lp->obj-z) <= main_lptols->cost))
  { dyio_outfmt(ttyout,dy_gtxecho,
		"  ERROR: z = %g, expected %g, error %g, tol %g.\n",
		main_lp->obj,z,fabs(main_lp->obj-z),main_lptols->cost) ; }

  dy_dumpcompact(dy_logchn,dy_gtxecho,main_lp,FALSE) ;
  dytest_betaj (main_lp,main_lptols,main_lpopts) ;
  dytest_abarj (main_lp,main_lptols,main_lpopts) ;

/*
  Call dylp to free internal structures, then free main_sys.
*/
  comflg(main_lp->ctlopts,lpctlONLYFREE|lpctlNOFREE) ;
  dylp(main_lp,main_lpopts,main_lptols,NULL) ;
  consys_free(main_sys) ;
  main_sys = NULL ;
  main_lp->consys = NULL ;
  dy_freesoln(main_lp) ;
/*
  Let's try another. Load and solve afiro. Retain the data structures so that
  we can use them to test the tableau routines.
*/
  dyio_outfmt(ttyout,dy_gtxecho,"Loading afiro example from static data.\n") ;
  main_sys = dytest_afirosys(main_lptols) ;
  comflg(main_lp->ctlopts,lpctlONLYFREE|lpctlNOFREE) ;
  main_lp->phase = dyINV ;
  main_lp->consys = main_sys ;
  main_lp->rowsze = main_sys->rowsze ;
  main_lp->colsze = main_sys->colsze ;
  main_lpopts->forcecold = TRUE ;
  main_lpopts->fullsys = FALSE ;
  main_lpopts->finpurge.vars = TRUE ;
  main_lpopts->finpurge.cons = TRUE ;
  main_lpopts->coldbasis = ibLOGICAL ;
  main_lpopts->scaling = 2 ;
  main_lpopts->print.scaling = 2 ;
  dyio_outfmt(ttyout,dy_gtxecho,"Solving afiro ... ") ;
  lpretval = do_lp(main_lp,main_lptols,main_lpopts,NULL,1) ;
  dyio_outfmt(ttyout,dy_gtxecho,"%s, z = %.12f.\n",
	      dy_prtlpret(lpretval),main_lp->obj) ;
  z = -464.753142857143 ;
  if (!(fabs(main_lp->obj-z) <= main_lptols->cost))
  { dyio_outfmt(ttyout,dy_gtxecho,
		"  ERROR: z = %g, expected %g, error %g, tol %g.\n",
		main_lp->obj,z,fabs(main_lp->obj-z),main_lptols->cost) ; }
  dy_dumpcompact(dy_logchn,dy_gtxecho,main_lp,FALSE) ;
/*
  Test the tableau routines. These are mathematical checks, not dependent on
  the specific problem.

  Start with columns of the basis inverse, beta<j>. Test that B inv(B) = I.
  Then do ftran'd columns abar<j>. Test that B abar<j> = B (inv(B) A) = A.
*/
  dytest_betaj (main_lp,main_lptols,main_lpopts) ;
  dytest_abarj (main_lp,main_lptols,main_lpopts) ;
/*
  Call dylp to free internal structures, then free main_sys.
*/
  comflg(main_lp->ctlopts,lpctlONLYFREE|lpctlNOFREE) ;
  dylp(main_lp,main_lpopts,main_lptols,NULL) ;
  consys_free(main_sys) ;
  main_sys = NULL ;
  main_lp->consys = NULL ;
  dy_freesoln(main_lp) ;
/*
  Let's try another. Load and solve boeing2. Retain the data structures so that
  we can use them to test the tableau routines.
*/
  dyio_outfmt(ttyout,dy_gtxecho,"Loading boeing2 example from static data.\n") ;
  main_sys = dytest_boeing2sys(main_lptols) ;
  comflg(main_lp->ctlopts,lpctlONLYFREE|lpctlNOFREE) ;
  main_lp->phase = dyINV ;
  main_lp->consys = main_sys ;
  main_lp->rowsze = main_sys->rowsze ;
  main_lp->colsze = main_sys->colsze ;
  main_lpopts->forcecold = TRUE ;
  main_lpopts->fullsys = FALSE ;
  main_lpopts->finpurge.vars = TRUE ;
  main_lpopts->finpurge.cons = TRUE ;
  main_lpopts->coldbasis = ibLOGICAL ;
  main_lpopts->scaling = 2 ;
  main_lpopts->print.scaling = 2 ;
/*
  main_lpopts->print.tableau = 6 ;
  main_lpopts->print.major = 1 ;
  main_lpopts->print.setup = 4 ;
*/
  dyio_outfmt(ttyout,dy_gtxecho,"Solving boeing2 ... ") ;
  lpretval = do_lp(main_lp,main_lptols,main_lpopts,NULL,1) ;
  dyio_outfmt(ttyout,dy_gtxecho,"%s, z = %.12f.\n",
	      dy_prtlpret(lpretval),main_lp->obj) ;
  z = -315.0187280152 ;
  if (!(fabs(main_lp->obj-z) <= main_lptols->cost))
  { dyio_outfmt(ttyout,dy_gtxecho,
		"  ERROR: z = %g, expected %g, error %g, tol %g.\n",
		main_lp->obj,z,fabs(main_lp->obj-z),main_lptols->cost) ; }
  dy_dumpcompact(dy_logchn,dy_gtxecho,main_lp,FALSE) ;
/*
  Test the tableau routines. These are mathematical checks, not dependent on
  the specific problem.

  Start with columns of the basis inverse, beta<j>. Test that Binv(B) = I.
  Then do ftran'd columns abar<j>. Test that B(inv(B)A) = A.
*/
  dytest_betaj (main_lp,main_lptols,main_lpopts) ;
  dytest_abarj (main_lp,main_lptols,main_lpopts) ;
/*
  Call dylp to free internal structures, then free main_sys.
*/
  comflg(main_lp->ctlopts,lpctlONLYFREE|lpctlNOFREE) ;
  dylp(main_lp,main_lpopts,main_lptols,NULL) ;
  consys_free(main_sys) ;
  main_sys = NULL ;
  main_lp->consys = NULL ;
  dy_freesoln(main_lp) ;
/*
  Final cleanup. Free space used by the remaining main_* structures.
*/
  dy_freebasis() ;

  if (main_lp != NULL)
  { dy_freesoln(main_lp) ;
    if (main_lp->consys != NULL) consys_free(main_lp->consys) ;
    FREE(main_lp) ; }
  if (main_lpopts != NULL) FREE(main_lpopts) ;
  if (main_lptols != NULL) FREE(main_lptols) ;
/*
  Leap to here for shutdown when we opened the output file but didn't solve
  an LP.
*/
  if (outchn != IOID_INV && outchn != ttyout)
  { (void) dyio_closefile(outchn) ; }
/*
  Leap to here for shutdown in cases where we never get as far as opening
  an output file. We still need to close the log file and shut down the i/o
  subsystem.
*/
  /* NOOUTFILE_CLEANUP: */
  if (dy_logchn != IOID_INV && dy_logchn != IOID_NOSTRM)
  { (void) dyio_closefile(dy_logchn) ; }
  dyio_ioterm() ;
  errterm() ;

  return (0) ; }

