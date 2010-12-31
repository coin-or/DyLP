/*
  This file is a part of the Dylp LP distribution.

        Copyright (C) 2008 Lou Hafer

        School of Computing Science
        Simon Fraser University
        Burnaby, B.C., V5A 1S6, Canada
        lou@cs.sfu.ca

  This code is licensed under the terms of the Eclipse Public License (EPL).
*/

/*
  This is the main routine for the dylp unit test. The following defines
  select the test problems, a convenience during code development. Normally,
  you'll want them all set to 1 for a thorough test.
*/

#define RUN_EXMIP1 1
#define RUN_AFIRO 1
#define RUN_BOEING2 1
#define RUN_EXPRIMALRAY 1
#define RUN_EXDUALRAY 1
#define RUN_GALENETBNDS 1
#define RUN_GALENETLEQ 1
#define RUN_GALENETMIXED 1
#define RUN_GALENET 1


#include "dylp.h"

/*
  Macro cleverness to specify a default error message file. Depends on ANSI
  C merge of consecutive string constants. DYLP_ERRMSGDIR should have the
  form "path/to/distribution/DyLP/src/Dylp/", including the quotes. See
  DyLP/src/DylpStdLib/DylpConfig.h for further information.
*/

#ifdef DYLP_ERRMSGDIR
# define DYLP_ERRMSGPATH DYLP_ERRMSGDIR "dy_errmsgs.txt"
#else
# define DYLP_ERRMSGPATH "../src/Dylp/dy_errmsgs.txt"
#endif

/*
  Variables which control i/o operations.

  ttyout		i/o id for output to the user's terminal
  ttyin			i/o id for input from the user's terminal

  dy_logchn		i/o id for the execution log file
  dy_gtxecho		controls echoing of generated text to stdout
*/

ioid dy_logchn ;
bool dy_gtxecho ;





static lpret_enum do_lp (lpprob_struct *lp,
			 lptols_struct *lptols,
			 lpopts_struct *lpopts,
			 int printlvl)
/*
  This routine is a convenience wrapper which handles statistics and timing.
  It also allows for easy adjustment of the print level by making a local copy
  of the options.

  Parameters:
    lp:		lp problem to be solved
    lptols:	tolerances
    lpopts:	options
    printlvl:	desired output level

  Returns: lp status code; lpINV is used in the event of a fatal error that's
	   not really dylp's fault.
*/

{ lpret_enum lpret ;
  lpopts_struct *local_lpopts ;
  lpstats_struct *local_lpstats ;

  consys_struct *sys ;

# if MALLOC_DEBUG == 2
  const char *rtnnme = "do_lp" ;
# endif

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
  Let dylp know we're not done, and make the call to solve the lp.
*/
  lp->phase = dyINV ;
  lpret = dylp(lp,local_lpopts,lptols,local_lpstats) ;
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

  consys_struct *main_sys ;
  lpprob_struct *main_lp ;
  lpret_enum lpretval ;

  double z ;
  int errcnt,cnt ;

  lpopts_struct* main_lpopts ;
  lptols_struct* main_lptols ;

/*
  Set this to TRUE if you want to see the solutions for the test lp problems.
*/
  bool dumpsoln = TRUE ;

  char *rtnnme = argv[0] ;

  /* dy_basis.c */

  extern void dy_initbasis(int concnt, int factor_freq, double zero_tol),
	      dy_freebasis(void) ;

  /* dytest_problems.c */

  extern consys_struct *dytest_exmip1sys(lptols_struct *tols) ;
  extern consys_struct *dytest_exprimalraysys(lptols_struct *tols) ;
  extern consys_struct *dytest_exdualraysys(lptols_struct *tols) ;
  extern consys_struct *dytest_galenetbndssys(lptols_struct *tols) ;
  extern consys_struct *dytest_galenetleqsys(lptols_struct *tols) ;
  extern consys_struct *dytest_galenetmixedsys(lptols_struct *tols) ;
  extern consys_struct *dytest_galenetsys(lptols_struct *tols) ;

  extern consys_struct *dytest_afirosys(lptols_struct *tols) ;
  extern consys_struct *dytest_boeing2sys(lptols_struct *tols) ;

  /* dytest_tableau.c */

  extern int dytest_betaj(lpprob_struct *lp,
			  lptols_struct *lptols,lpopts_struct *lpopts) ;
  extern int dytest_betai(lpprob_struct *lp,
			  lptols_struct *lptols,lpopts_struct *lpopts) ;
  extern int dytest_abarj(lpprob_struct *lp,
			  lptols_struct *lptols,lpopts_struct *lpopts) ;
  extern int dytest_abari(lpprob_struct *lp,
			  lptols_struct *lptols,lpopts_struct *lpopts) ;

  /* dytest_solutions.c */

  extern int dytest_rowDuals(lpprob_struct *lp,
			     lptols_struct *lptols,lpopts_struct *lpopts) ;
  extern int dytest_allDuals(lpprob_struct *lp,
			      lptols_struct *lptols,lpopts_struct *lpopts) ;
  extern int dytest_colDuals(lpprob_struct *lp,
			     lptols_struct *lptols,lpopts_struct *lpopts) ;
  extern int dytest_colPrimals(lpprob_struct *lp,
			       lptols_struct *lptols,lpopts_struct *lpopts) ;
  extern int dytest_rowPrimals(lpprob_struct *lp,
			       lptols_struct *lptols,lpopts_struct *lpopts) ;

  /* dytest_rays.c */

  extern int dytest_primalRays(int *p_numRays,lpprob_struct *lp,
			       lptols_struct *lptols,lpopts_struct *lpopts) ;
  extern int dytest_dualRays(int *p_numRays,lpprob_struct *lp,
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
  dy_logchn = IOID_NOSTRM ;
  dy_gtxecho = TRUE ;
/*
  Announce we're running.
*/
  dyio_outfmt(ttyout,dy_gtxecho,"Dylp v%s unit test start.\n",DYLP_VERSION) ;
  dyio_flushio(ttyout,dy_gtxecho) ;
  errcnt = 0 ;
/*
  Acquire default option and tolerance structures. Allocate an
  lpprob_struct to be our top-level handle.
*/
  main_lpopts = NULL ;
  main_lptols = NULL ;
  dy_defaults(&main_lpopts,&main_lptols) ;
  main_lp = (lpprob_struct *) CALLOC(1,sizeof(lpprob_struct)) ;
/*
  Initialise the basis factorisation package with a data structure capable of
  50 constraints.  The second parameter controls how many basis updates the
  basis can hold before it requires refactoring.  Adding 5 to dylp's refactor
  interval should give a safety margin.
*/
  dy_initbasis(50,main_lpopts->factor+5,0.0) ;

#if RUN_EXMIP1 > 0
/*
  Load the exmip1 example and see if we can solve it.
*/
  dyio_outfmt(ttyout,dy_gtxecho,
	      "\n\nLoading exmip1 example from static data.\n") ;
  main_sys = dytest_exmip1sys(main_lptols) ;
  if (main_sys == NULL)
  { dyio_outfmt(ttyout,dy_gtxecho,
		"Failed to load exmip1 constraint system.\n") ;
    errcnt++ ; }
/*
  Check over the option settings, now that we know how big the constraint
  system will be.
*/
  else
  { dy_checkdefaults(main_sys,main_lpopts,main_lptols) ;
/*
  Initialise the main_lp structure to pass the problem in to dylp.
    * We need to retain the data structures (NOFREE) so that we can
      consult them when dylp returns.
    * The phase needs to be dyINV at the start.
    * We need to specify the constraint system, and its size.
    * Let dylp work with a partial system and purge at the end. This stresses
      the routines we're testing; they need to synthesize parts of the values
      they return.
    * The remaining options specify an all-logical basis, allow scaling, and
      force a cold start.
*/
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
    main_lpopts->scaling = 0 ;
    main_lpopts->forcecold = TRUE ;
/*
    main_lpopts->print.major = 1 ;
    main_lpopts->print.scaling = 1 ;
    main_lpopts->print.setup = 6 ;
    main_lpopts->print.crash = 3 ;
    main_lpopts->print.conmgmt = 3 ;
    main_lpopts->print.varmgmt = 3 ;
    main_lpopts->print.soln = 3 ;
*/
/*
  Solve.
*/
    dyio_outfmt(ttyout,dy_gtxecho,"Solving exmip1 ... ") ;
    lpretval = do_lp(main_lp,main_lptols,main_lpopts,1) ;
/*
  And the result is ...
*/
    dyio_outfmt(ttyout,dy_gtxecho,"\n  %s, z = %.12f.\n",
		dy_prtlpret(lpretval),main_lp->obj) ;
    z = 3.236842105263 ;
    if (!(fabs(main_lp->obj-z) <= main_lptols->cost))
    { errcnt++ ;
      dyio_outfmt(ttyout,dy_gtxecho,
		  "  ERROR: z = %g, expected %g, error %g, tol %g.\n",
		  main_lp->obj,z,fabs(main_lp->obj-z),main_lptols->cost) ; }
    if (dumpsoln == TRUE)
    { dy_dumpcompact(dy_logchn,dy_gtxecho,main_lp,FALSE) ; }
/*
  Test the tableau, solution, and ray routines. The tests are predominantly
  mathematical identities, with a bit of data structure consistency thrown in
  for good measure. Completely problem-independent.
*/
    errcnt += dytest_betaj(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_abarj(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_betai(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_abari(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_rowDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_colDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_allDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_colPrimals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_rowPrimals(main_lp,main_lptols,main_lpopts) ;
/*
  Call dylp to free internal structures, then free main_sys.
*/
    comflg(main_lp->ctlopts,lpctlONLYFREE|lpctlNOFREE) ;
    dylp(main_lp,main_lpopts,main_lptols,NULL) ;
    consys_free(main_sys) ;
    main_sys = NULL ;
    main_lp->consys = NULL ;
    dy_freesoln(main_lp) ; }
#endif

#if RUN_AFIRO > 0
/*
  Let's try another. Load and solve afiro.
*/
  dyio_outfmt(ttyout,dy_gtxecho,
	      "\n\nLoading afiro example from static data.\n") ;
  main_sys = dytest_afirosys(main_lptols) ;
  if (main_sys == NULL)
  { dyio_outfmt(ttyout,dy_gtxecho,
		"Failed to load afiro constraint system.\n") ;
    errcnt++ ; }
  else
  { dy_checkdefaults(main_sys,main_lpopts,main_lptols) ;
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

    dyio_outfmt(ttyout,dy_gtxecho,"Solving afiro ... ") ;
    lpretval = do_lp(main_lp,main_lptols,main_lpopts,1) ;
    dyio_outfmt(ttyout,dy_gtxecho,"\n  %s, z = %.12f.\n",
		dy_prtlpret(lpretval),main_lp->obj) ;
    z = -464.753142857143 ;
    if (!(fabs(main_lp->obj-z) <= main_lptols->cost))
    { errcnt++ ;
      dyio_outfmt(ttyout,dy_gtxecho,
		  "  ERROR: z = %g, expected %g, error %g, tol %g.\n",
		  main_lp->obj,z,fabs(main_lp->obj-z),main_lptols->cost) ; }
    if (dumpsoln == TRUE)
    { dy_dumpcompact(dy_logchn,dy_gtxecho,main_lp,FALSE) ; }

    errcnt += dytest_betaj(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_abarj(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_betai(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_abari(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_rowDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_colDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_allDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_colPrimals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_rowPrimals(main_lp,main_lptols,main_lpopts) ;

    comflg(main_lp->ctlopts,lpctlONLYFREE|lpctlNOFREE) ;
    dylp(main_lp,main_lpopts,main_lptols,NULL) ;
    consys_free(main_sys) ;
    main_sys = NULL ;
    main_lp->consys = NULL ;
    dy_freesoln(main_lp) ; }
#endif

#if RUN_BOEING2 > 0
/*
  Let's try another. Load and solve boeing2.
*/
  dyio_outfmt(ttyout,dy_gtxecho,
	      "\n\nLoading boeing2 example from static data.\n") ;
  main_sys = dytest_boeing2sys(main_lptols) ;
  if (main_sys == NULL)
  { dyio_outfmt(ttyout,dy_gtxecho,
		"Failed to load boeing2 constraint system.\n") ;
    errcnt++ ; }
  else
  { dy_checkdefaults(main_sys,main_lpopts,main_lptols) ;
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

    main_lpopts->print.phase2 = 0 ;
/*
    main_lpopts->print.major = 1 ;
    main_lpopts->print.phase1 = 4 ;
    main_lpopts->print.dual = 4 ;
    main_lpopts->print.tableau = 4 ;
    main_lpopts->print.soln = 3 ;
    main_lpopts->print.rays = 5 ;
*/

    dyio_outfmt(ttyout,dy_gtxecho,"Solving boeing2 ... ") ;
    lpretval = do_lp(main_lp,main_lptols,main_lpopts,1) ;
    dyio_outfmt(ttyout,dy_gtxecho,"\n  %s, z = %.12f.\n",
		dy_prtlpret(lpretval),main_lp->obj) ;
    z = -315.0187280152 ;
    if (!(fabs(main_lp->obj-z) <= main_lptols->cost))
    { errcnt++ ;
      dyio_outfmt(ttyout,dy_gtxecho,
		  "  ERROR: z = %g, expected %g, error %g, tol %g.\n",
		  main_lp->obj,z,fabs(main_lp->obj-z),main_lptols->cost) ; }
    if (dumpsoln == TRUE)
    { dy_dumpcompact(dy_logchn,dy_gtxecho,main_lp,FALSE) ; }

    errcnt += dytest_betaj(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_abarj(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_betai(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_abari(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_rowDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_colDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_allDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_colPrimals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_rowPrimals(main_lp,main_lptols,main_lpopts) ;

    comflg(main_lp->ctlopts,lpctlONLYFREE|lpctlNOFREE) ;
    dylp(main_lp,main_lpopts,main_lptols,NULL) ;
    consys_free(main_sys) ;
    main_sys = NULL ;
    main_lp->consys = NULL ;
    dy_freesoln(main_lp) ; }
#endif

#if RUN_EXPRIMALRAY > 0
/*
  Let's try another. Load and solve exprimalray.  The polyhedron for
  exprimalray is carefully crafted to provide an initial bounded optimum
  point with two rays that are exposed by the proper change in objective.
  Force dylp to use the full system for this test and do not allow final
  purging, lest we miss the point and rays we're aiming for.
*/
  dyio_outfmt(ttyout,dy_gtxecho,
	      "\n\nLoading exprimalray example from static data.\n") ;
  main_sys = dytest_exprimalraysys(main_lptols) ;
  if (main_sys == NULL)
  { dyio_outfmt(ttyout,dy_gtxecho,
		"Failed to load exprimalray constraint system.\n") ;
    errcnt++ ; }
  else
  { dy_checkdefaults(main_sys,main_lpopts,main_lptols) ;
    comflg(main_lp->ctlopts,lpctlONLYFREE|lpctlNOFREE) ;
    main_lp->phase = dyINV ;
    main_lp->consys = main_sys ;
    main_lp->rowsze = main_sys->rowsze ;
    main_lp->colsze = main_sys->colsze ;
    main_lpopts->forcecold = TRUE ;
    main_lpopts->fullsys = TRUE ;
    main_lpopts->finpurge.vars = FALSE ;
    main_lpopts->finpurge.cons = FALSE ;
    main_lpopts->coldbasis = ibLOGICAL ;
    main_lpopts->scaling = 2 ;

    dyio_outfmt(ttyout,dy_gtxecho,"Solving exprimalray ... ") ;

    lpretval = do_lp(main_lp,main_lptols,main_lpopts,1) ;
    dyio_outfmt(ttyout,dy_gtxecho,"\n  %s, z = %.12f.\n",
		dy_prtlpret(lpretval),main_lp->obj) ;
    z = -21 ;
    if (!(fabs(main_lp->obj-z) <= main_lptols->cost))
    { errcnt++ ;
      dyio_outfmt(ttyout,dy_gtxecho,
		  "  ERROR: z = %g, expected %g, error %g, tol %g.\n",
		  main_lp->obj,z,fabs(main_lp->obj-z),main_lptols->cost) ; }
    if (dumpsoln == TRUE)
    { dy_dumpcompact(dy_logchn,dy_gtxecho,main_lp,FALSE) ; }
/*
  Now tweak the objective to 3x1+x2+x3, giving us two rays. Solve, then test
  that we have valid primal rays. First ask for just one, then ask for five,
  expecting two.
*/
    main_sys->obj[1] = -1.0 ;
    main_sys->obj[2] = -4.0 ;
    setflg(main_lp->ctlopts,lpctlOBJCHG) ;
    main_lpopts->forcecold = FALSE ;

    dyio_outfmt(dy_logchn,dy_gtxecho,"Resolving exprimalray ...") ;
    lpretval = do_lp(main_lp,main_lptols,main_lpopts,1) ;
    dyio_outfmt(ttyout,dy_gtxecho,"\n  %s, z = %.12f.\n",
		dy_prtlpret(lpretval),main_lp->obj) ;
    if (dumpsoln == TRUE)
    { dy_dumpcompact(dy_logchn,dy_gtxecho,main_lp,FALSE) ; }

    cnt = 1 ;
    errcnt += dytest_primalRays(&cnt,main_lp,main_lptols,main_lpopts) ;
    if (cnt != 1)
    { errcnt++ ;
      dyio_outfmt(ttyout,dy_gtxecho,
		  "  ERROR: %d primal rays returned, expected %d.\n",
		  cnt,1) ; }
    cnt = 5 ;
    errcnt += dytest_primalRays(&cnt,main_lp,main_lptols,main_lpopts) ;
    if (cnt != 2)
    { errcnt++ ;
      dyio_outfmt(ttyout,dy_gtxecho,
		  "  ERROR: %d primal rays returned, expected %d.\n",
		  cnt,2) ; }
/*
  Run the remainder of the tests, to make sure they run without error when
  dylp finishes unbounded.
*/
    errcnt += dytest_betaj(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_abarj(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_betai(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_abari(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_rowDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_colDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_allDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_colPrimals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_rowPrimals(main_lp,main_lptols,main_lpopts) ;

    comflg(main_lp->ctlopts,lpctlONLYFREE|lpctlNOFREE) ;
    dylp(main_lp,main_lpopts,main_lptols,NULL) ;
    consys_free(main_sys) ;
    main_sys = NULL ;
    main_lp->consys = NULL ;
    dy_freesoln(main_lp) ; }
#endif

#if RUN_EXDUALRAY > 0
/*
  Let's try another. Load and solve exdualray. Exdualray takes advantage of
  duality: The dual polyhedron is exactly the primal polyhedron of the
  previous problem.  The symmetry should be clear from the output (objective,
  dual, and primal values are negated, but otherwise identical)
*/
  dyio_outfmt(ttyout,dy_gtxecho,
	      "\n\nLoading exdualray example from static data.\n") ;
  main_sys = dytest_exdualraysys(main_lptols) ;
  if (main_sys == NULL)
  { dyio_outfmt(ttyout,dy_gtxecho,
		"Failed to load exdualray constraint system.\n") ;
    errcnt++ ; }
  else
  { dy_checkdefaults(main_sys,main_lpopts,main_lptols) ;
    comflg(main_lp->ctlopts,lpctlONLYFREE|lpctlNOFREE) ;
    main_lp->phase = dyINV ;
    main_lp->consys = main_sys ;
    main_lp->rowsze = main_sys->rowsze ;
    main_lp->colsze = main_sys->colsze ;
    main_lpopts->forcecold = TRUE ;
    main_lpopts->fullsys = TRUE ;
    main_lpopts->finpurge.vars = FALSE ;
    main_lpopts->finpurge.cons = FALSE ;
    main_lpopts->coldbasis = ibLOGICAL ;
    main_lpopts->scaling = 2 ;
/*
    main_lpopts->print.major = 1 ;
    main_lpopts->print.phase1 = 4 ;
    main_lpopts->print.phase2 = 4 ;
    main_lpopts->print.dual = 4 ;
    main_lpopts->print.tableau = 4 ;
    main_lpopts->print.soln = 3 ;
    main_lpopts->print.rays = 5 ;
*/

    dyio_outfmt(ttyout,dy_gtxecho,"Solving exdualray ... ") ;
    lpretval = do_lp(main_lp,main_lptols,main_lpopts,1) ;
    dyio_outfmt(ttyout,dy_gtxecho,"\n  %s, z = %.12f.\n",
		dy_prtlpret(lpretval),main_lp->obj) ;
    z = 21 ;
    if (!(fabs(main_lp->obj-z) <= main_lptols->cost))
    { errcnt++ ;
      dyio_outfmt(ttyout,dy_gtxecho,
		  "  ERROR: z = %g, expected %g, error %g, tol %g.\n",
		  main_lp->obj,z,fabs(main_lp->obj-z),main_lptols->cost) ; }
    if (dumpsoln == TRUE)
    { dy_dumpcompact(dy_logchn,dy_gtxecho,main_lp,FALSE) ; }
/*
  Tweak the rhs to (-1)*(-1 -4 1) to produce a pair of rays in the dual.
*/
    main_sys->rhs[2] =  4.0 ;
    setflg(main_lp->ctlopts,lpctlRHSCHG) ;
    main_lpopts->forcecold = FALSE ;
    main_lpopts->print.force = 1 ;
    dyio_outfmt(dy_logchn,dy_gtxecho,"Resolving exdualray ...") ;
    lpretval = do_lp(main_lp,main_lptols,main_lpopts,1) ;
    dyio_outfmt(ttyout,dy_gtxecho,"\n  %s, z = %.12f.\n",
		dy_prtlpret(lpretval),main_lp->obj) ;
    if (dumpsoln == TRUE)
    { dy_dumpcompact(dy_logchn,dy_gtxecho,main_lp,FALSE) ; }
/*
  Test that we have valid dual rays. First ask for just one, then ask for
  five, expecting two.
*/
    errcnt += dytest_rowDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_colDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_allDuals(main_lp,main_lptols,main_lpopts) ;
    cnt = 1 ;
    errcnt += dytest_dualRays(&cnt,main_lp,main_lptols,main_lpopts) ;
    if (cnt != 1)
    { errcnt++ ;
      dyio_outfmt(ttyout,dy_gtxecho,
		  "  ERROR: %d dual rays returned, expected %d.\n",
		  cnt,1) ; }
    cnt = 5 ;
    errcnt += dytest_dualRays(&cnt,main_lp,main_lptols,main_lpopts) ;
    if (cnt != 2)
    { errcnt++ ;
      dyio_outfmt(ttyout,dy_gtxecho,
		  "  ERROR: %d dual rays returned, expected %d.\n",
		  cnt,2) ; }
/*
  Run the remainder of the tests, to make sure they run without error when
  dylp finishes infeasible.
*/
    errcnt += dytest_betaj(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_abarj(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_betai(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_abari(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_colPrimals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_rowPrimals(main_lp,main_lptols,main_lpopts) ;

    comflg(main_lp->ctlopts,lpctlONLYFREE|lpctlNOFREE) ;
    dylp(main_lp,main_lpopts,main_lptols,NULL) ;
    consys_free(main_sys) ;
    main_sys = NULL ;
    main_lp->consys = NULL ;
    dy_freesoln(main_lp) ; }
#endif

#if RUN_GALENETBNDS > 0
/*
  Now a sequence of three problems based on galenet that really exercise the
  dual ray code. Galenet is a straightforward primal infeasible / dual
  unbounded problem. The objective is identically zero, so it's a pure
  feasibility problem. The objective returned from dylp will be z = 48, the
  amount of primal infeasibility.

  The first version is galenetbnds. All equalities from galenet are converted
  to inequalities, all implicit bounds are converted to explicit bound
  constraints, and then all >= constraints are converted to <= constraints.
  This is the simplest possible case for returning dual rays.
*/
  dyio_outfmt(ttyout,dy_gtxecho,
	      "\n\nLoading galenetbnds example from static data.\n") ;
  main_sys = dytest_galenetbndssys(main_lptols) ;
  if (main_sys == NULL)
  { dyio_outfmt(ttyout,dy_gtxecho,
		"Failed to load galenetbnds constraint system.\n") ;
    errcnt++ ; }
  else
  { dy_checkdefaults(main_sys,main_lpopts,main_lptols) ;
    comflg(main_lp->ctlopts,lpctlONLYFREE|lpctlNOFREE) ;
    main_lp->phase = dyINV ;
    main_lp->consys = main_sys ;
    main_lp->rowsze = main_sys->rowsze ;
    main_lp->colsze = main_sys->colsze ;
    main_lpopts->forcecold = TRUE ;
    main_lpopts->fullsys = TRUE ;
    main_lpopts->finpurge.vars = FALSE ;
    main_lpopts->finpurge.cons = FALSE ;
    main_lpopts->coldbasis = ibLOGICAL ;
    main_lpopts->scaling = 2 ;
/*
    main_lpopts->print.major = 1 ;
    main_lpopts->print.phase1 = 4 ;
    main_lpopts->print.phase2 = 4 ;
    main_lpopts->print.dual = 4 ;
    main_lpopts->print.tableau = 4 ;
    main_lpopts->print.soln = 3 ;
    main_lpopts->print.rays = 5 ;
*/

    dyio_outfmt(ttyout,dy_gtxecho,"Solving galenetbnds ... ") ;
    lpretval = do_lp(main_lp,main_lptols,main_lpopts,1) ;
    dyio_outfmt(ttyout,dy_gtxecho,"\n  %s, z = %.12f.\n",
		dy_prtlpret(lpretval),main_lp->obj) ;
    z = 48.0 ;
    if (!(fabs(main_lp->obj-z) <= main_lptols->cost))
    { errcnt++ ;
      dyio_outfmt(ttyout,dy_gtxecho,
		  "  ERROR: z = %g, expected %g, error %g, tol %g.\n",
		  main_lp->obj,z,fabs(main_lp->obj-z),main_lptols->cost) ; }
    dumpsoln = TRUE ;
    if (dumpsoln == TRUE)
    { dy_dumpcompact(dy_logchn,dy_gtxecho,main_lp,FALSE) ; }
/*
  Test that we have valid dual rays. First ask for just one, then ask for
  five, expecting three.
*/
    cnt = 1 ;
    errcnt += dytest_dualRays(&cnt,main_lp,main_lptols,main_lpopts) ;
    if (cnt != 1)
    { errcnt++ ;
      dyio_outfmt(ttyout,dy_gtxecho,
		  "  ERROR: %d dual rays returned, expected %d.\n",
		  cnt,1) ; }
    cnt = 5 ;
    errcnt += dytest_dualRays(&cnt,main_lp,main_lptols,main_lpopts) ;
    if (cnt != 3)
    { errcnt++ ;
      dyio_outfmt(ttyout,dy_gtxecho,
		  "  ERROR: %d dual rays returned, expected %d.\n",
		  cnt,2) ; }
/*
  Run the remainder of the tests, to make sure they run without error when
  dylp finishes infeasible.
*/
    errcnt += dytest_betaj(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_abarj(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_betai(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_abari(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_rowDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_colDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_allDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_colPrimals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_rowPrimals(main_lp,main_lptols,main_lpopts) ;

    comflg(main_lp->ctlopts,lpctlONLYFREE|lpctlNOFREE) ;
    dylp(main_lp,main_lpopts,main_lptols,NULL) ;
    consys_free(main_sys) ;
    main_sys = NULL ;
    main_lp->consys = NULL ;
    dy_freesoln(main_lp) ; }
#endif

#if RUN_GALENETLEQ > 0
/*
  Now load and solve galenetleq --- galenetbnds, but the
  bound constraints on variables are now handled in the usual manner, as
  implicit bounds. We'll need the full ray (explicit duals y = c<B>inv(B)
  as well as duals associated with nonbasic bounded variables) in order to pass
  the mathematical tests rb < 0, rA > 0.
*/
  dyio_outfmt(ttyout,dy_gtxecho,
	      "\n\nLoading galenetleq example from static data.\n") ;
  main_sys = dytest_galenetleqsys(main_lptols) ;
  if (main_sys == NULL)
  { dyio_outfmt(ttyout,dy_gtxecho,
		"Failed to load galenetleq constraint system.\n") ;
    errcnt++ ; }
  else
  { dy_checkdefaults(main_sys,main_lpopts,main_lptols) ;
    comflg(main_lp->ctlopts,lpctlONLYFREE|lpctlNOFREE) ;
    main_lp->phase = dyINV ;
    main_lp->consys = main_sys ;
    main_lp->rowsze = main_sys->rowsze ;
    main_lp->colsze = main_sys->colsze ;
    main_lpopts->forcecold = TRUE ;
    main_lpopts->fullsys = TRUE ;
    main_lpopts->finpurge.vars = FALSE ;
    main_lpopts->finpurge.cons = FALSE ;
    main_lpopts->coldbasis = ibLOGICAL ;
    main_lpopts->scaling = 2 ;
/*
    main_lpopts->print.major = 1 ;
    main_lpopts->print.phase1 = 4 ;
    main_lpopts->print.phase2 = 4 ;
    main_lpopts->print.dual = 4 ;
    main_lpopts->print.tableau = 4 ;
    main_lpopts->print.soln = 3 ;
    main_lpopts->print.rays = 5 ;
*/

    dyio_outfmt(ttyout,dy_gtxecho,"Solving galenetleq ... ") ;
    lpretval = do_lp(main_lp,main_lptols,main_lpopts,1) ;
    dyio_outfmt(ttyout,dy_gtxecho,"\n  %s, z = %.12f.\n",
		dy_prtlpret(lpretval),main_lp->obj) ;
    z = 48.0 ;
    if (!(fabs(main_lp->obj-z) <= main_lptols->cost))
    { errcnt++ ;
      dyio_outfmt(ttyout,dy_gtxecho,
		  "  ERROR: z = %g, expected %g, error %g, tol %g.\n",
		  main_lp->obj,z,fabs(main_lp->obj-z),main_lptols->cost) ; }
    dumpsoln = TRUE ;
    if (dumpsoln == TRUE)
    { dy_dumpcompact(dy_logchn,dy_gtxecho,main_lp,FALSE) ; }
/*
  Test that we have valid dual rays. First ask for just one, then ask for
  five, expecting three.
*/
    cnt = 1 ;
    errcnt += dytest_dualRays(&cnt,main_lp,main_lptols,main_lpopts) ;
    if (cnt != 1)
    { errcnt++ ;
      dyio_outfmt(ttyout,dy_gtxecho,
		  "  ERROR: %d dual rays returned, expected %d.\n",
		  cnt,1) ; }
    cnt = 5 ;
    errcnt += dytest_dualRays(&cnt,main_lp,main_lptols,main_lpopts) ;
    if (cnt != 3)
    { errcnt++ ;
      dyio_outfmt(ttyout,dy_gtxecho,
		  "  ERROR: %d dual rays returned, expected %d.\n",
		  cnt,2) ; }
/*
  Run the remainder of the tests, to make sure they run without error when
  dylp finishes infeasible.
*/
    errcnt += dytest_betaj(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_abarj(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_betai(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_abari(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_rowDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_colDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_allDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_colPrimals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_rowPrimals(main_lp,main_lptols,main_lpopts) ;

    comflg(main_lp->ctlopts,lpctlONLYFREE|lpctlNOFREE) ;
    dylp(main_lp,main_lpopts,main_lptols,NULL) ;
    consys_free(main_sys) ;
    main_sys = NULL ;
    main_lp->consys = NULL ;
    dy_freesoln(main_lp) ; }
#endif

#if RUN_GALENETMIXED > 0
/*
  Now load and solve galenetmixed --- bound constraints on variables are
  handled implicitly, and the constraint system is a mix of <= and >=
  constraints (the equalities are handled as a pair of inequalities).  As
  with galenetleq, we'll need the full ray (explicit duals y = c<B>inv(B) as
  well as duals associated with nonbasic bounded variables) in order to pass
  the mathematical tests rb < 0, rA > 0.
*/
  dyio_outfmt(ttyout,dy_gtxecho,
	      "\n\nLoading galenetmixed example from static data.\n") ;
  main_sys = dytest_galenetmixedsys(main_lptols) ;
  if (main_sys == NULL)
  { dyio_outfmt(ttyout,dy_gtxecho,
		"Failed to load galenetmixed constraint system.\n") ;
    errcnt++ ; }
  else
  { dy_checkdefaults(main_sys,main_lpopts,main_lptols) ;
    comflg(main_lp->ctlopts,lpctlONLYFREE|lpctlNOFREE) ;
    main_lp->phase = dyINV ;
    main_lp->consys = main_sys ;
    main_lp->rowsze = main_sys->rowsze ;
    main_lp->colsze = main_sys->colsze ;
    main_lpopts->forcecold = TRUE ;
    main_lpopts->fullsys = TRUE ;
    main_lpopts->finpurge.vars = FALSE ;
    main_lpopts->finpurge.cons = FALSE ;
    main_lpopts->coldbasis = ibLOGICAL ;
    main_lpopts->scaling = 2 ;
/*
    main_lpopts->print.major = 1 ;
    main_lpopts->print.phase1 = 4 ;
    main_lpopts->print.phase2 = 4 ;
    main_lpopts->print.dual = 4 ;
    main_lpopts->print.tableau = 4 ;
    main_lpopts->print.soln = 3 ;
    main_lpopts->print.rays = 5 ;
*/

    dyio_outfmt(ttyout,dy_gtxecho,"Solving galenetmixed ... ") ;
    lpretval = do_lp(main_lp,main_lptols,main_lpopts,1) ;
    dyio_outfmt(ttyout,dy_gtxecho,"\n  %s, z = %.12f.\n",
		dy_prtlpret(lpretval),main_lp->obj) ;
    z = 48.0 ;
    if (!(fabs(main_lp->obj-z) <= main_lptols->cost))
    { errcnt++ ;
      dyio_outfmt(ttyout,dy_gtxecho,
		  "  ERROR: z = %g, expected %g, error %g, tol %g.\n",
		  main_lp->obj,z,fabs(main_lp->obj-z),main_lptols->cost) ; }
    dumpsoln = TRUE ;
    if (dumpsoln == TRUE)
    { dy_dumpcompact(dy_logchn,dy_gtxecho,main_lp,FALSE) ; }
/*
  Test that we have valid dual rays. First ask for just one, then ask for
  five, expecting three.
*/
    cnt = 1 ;
    errcnt += dytest_dualRays(&cnt,main_lp,main_lptols,main_lpopts) ;
    if (cnt != 1)
    { errcnt++ ;
      dyio_outfmt(ttyout,dy_gtxecho,
		  "  ERROR: %d dual rays returned, expected %d.\n",
		  cnt,1) ; }
    cnt = 5 ;
    errcnt += dytest_dualRays(&cnt,main_lp,main_lptols,main_lpopts) ;
    if (cnt != 3)
    { errcnt++ ;
      dyio_outfmt(ttyout,dy_gtxecho,
		  "  ERROR: %d dual rays returned, expected %d.\n",
		  cnt,2) ; }
/*
  Run the remainder of the tests, to make sure they run without error when
  dylp finishes infeasible.
*/
    errcnt += dytest_betaj(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_abarj(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_betai(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_abari(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_rowDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_colDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_allDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_colPrimals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_rowPrimals(main_lp,main_lptols,main_lpopts) ;

    comflg(main_lp->ctlopts,lpctlONLYFREE|lpctlNOFREE) ;
    dylp(main_lp,main_lpopts,main_lptols,NULL) ;
    consys_free(main_sys) ;
    main_sys = NULL ;
    main_lp->consys = NULL ;
    dy_freesoln(main_lp) ; }
#endif

#if RUN_GALENET > 0
/*
  Now load and solve the original galenet --- bound constraints on variables
  are handled implicitly, and the constraint system is a mix of <=, =, and >=
  constraints.  As with galenetleq, we'll need the full ray (explicit duals y
  = c<B>inv(B) as well as duals associated with nonbasic bounded variables)
  in order to pass the mathematical tests rb < 0, rA > 0.
*/
  dyio_outfmt(ttyout,dy_gtxecho,
	      "\n\nLoading galenet example from static data.\n") ;
  main_sys = dytest_galenetsys(main_lptols) ;
  if (main_sys == NULL)
  { dyio_outfmt(ttyout,dy_gtxecho,
		"Failed to load galenet constraint system.\n") ;
    errcnt++ ; }
  else
  { dy_checkdefaults(main_sys,main_lpopts,main_lptols) ;
    comflg(main_lp->ctlopts,lpctlONLYFREE|lpctlNOFREE) ;
    main_lp->phase = dyINV ;
    main_lp->consys = main_sys ;
    main_lp->rowsze = main_sys->rowsze ;
    main_lp->colsze = main_sys->colsze ;
    main_lpopts->forcecold = TRUE ;
    main_lpopts->fullsys = TRUE ;
    main_lpopts->finpurge.vars = FALSE ;
    main_lpopts->finpurge.cons = FALSE ;
    main_lpopts->coldbasis = ibLOGICAL ;
    main_lpopts->scaling = 2 ;
/*
    main_lpopts->print.major = 1 ;
    main_lpopts->print.phase1 = 4 ;
    main_lpopts->print.phase2 = 4 ;
    main_lpopts->print.dual = 4 ;
    main_lpopts->print.tableau = 4 ;
    main_lpopts->print.soln = 3 ;
    main_lpopts->print.rays = 5 ;
*/

    dyio_outfmt(ttyout,dy_gtxecho,"Solving galenet ... ") ;
    lpretval = do_lp(main_lp,main_lptols,main_lpopts,1) ;
    dyio_outfmt(ttyout,dy_gtxecho,"\n  %s, z = %.12f.\n",
		dy_prtlpret(lpretval),main_lp->obj) ;
    z = 46.0 ;
    if (!(fabs(main_lp->obj-z) <= main_lptols->cost))
    { errcnt++ ;
      dyio_outfmt(ttyout,dy_gtxecho,
		  "  ERROR: z = %g, expected %g, error %g, tol %g.\n",
		  main_lp->obj,z,fabs(main_lp->obj-z),main_lptols->cost) ; }
    dumpsoln = TRUE ;
    if (dumpsoln == TRUE)
    { dy_dumpcompact(dy_logchn,dy_gtxecho,main_lp,FALSE) ; }
/*
  Test that we have valid dual rays. First ask for just one, then ask for
  five, expecting two. Note that we end up at a different vertex than the
  previous expanded versions of galenet.
*/
    cnt = 1 ;
    errcnt += dytest_dualRays(&cnt,main_lp,main_lptols,main_lpopts) ;
    if (cnt != 1)
    { errcnt++ ;
      dyio_outfmt(ttyout,dy_gtxecho,
		  "  ERROR: %d dual rays returned, expected %d.\n",
		  cnt,1) ; }
    cnt = 5 ;
    errcnt += dytest_dualRays(&cnt,main_lp,main_lptols,main_lpopts) ;
    if (cnt != 2)
    { errcnt++ ;
      dyio_outfmt(ttyout,dy_gtxecho,
		  "  ERROR: %d dual rays returned, expected %d.\n",
		  cnt,2) ; }
/*
  Run the remainder of the tests, to make sure they run without error when
  dylp finishes infeasible.
*/
    errcnt += dytest_betaj(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_abarj(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_betai(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_abari(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_rowDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_colDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_allDuals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_colPrimals(main_lp,main_lptols,main_lpopts) ;
    errcnt += dytest_rowPrimals(main_lp,main_lptols,main_lpopts) ;

    comflg(main_lp->ctlopts,lpctlONLYFREE|lpctlNOFREE) ;
    dylp(main_lp,main_lpopts,main_lptols,NULL) ;
    consys_free(main_sys) ;
    main_sys = NULL ;
    main_lp->consys = NULL ;
    dy_freesoln(main_lp) ; }
#endif

/*
  Report the result before we shut down.
*/
  if (errcnt > 0)
  { dyio_outfmt(ttyout,dy_gtxecho,
		"\n  ERROR: %d total errors for all tests.\n\n", errcnt) ; }
  else
  { dyio_outfmt(ttyout,dy_gtxecho,
		"\n  All tests completed successfully.\n\n") ; }
  dy_freebasis() ;

/*
  Final cleanup. Free space used by the remaining main_* structures and close
  down any i/o.
*/
  if (main_lp != NULL)
  { dy_freesoln(main_lp) ;
    if (main_lp->consys != NULL) consys_free(main_lp->consys) ;
    FREE(main_lp) ; }
  if (main_lpopts != NULL) FREE(main_lpopts) ;
  if (main_lptols != NULL) FREE(main_lptols) ;

  if (outchn != IOID_INV && outchn != ttyout)
  { (void) dyio_closefile(outchn) ; }

  if (dy_logchn != IOID_INV && dy_logchn != IOID_NOSTRM)
  { (void) dyio_closefile(dy_logchn) ; }
  dyio_ioterm() ;
  errterm() ;

  return (errcnt) ; }

