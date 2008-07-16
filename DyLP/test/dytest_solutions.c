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
  This file contains routines to test dylp's solution routines: dy_colPrimals,
  dy_rowPrimals, dy_colDuals, dy_rowDuals.
*/

#include "dylp.h"

extern ioid dy_logchn ;
extern bool dy_gtxecho ;



bool dytest_rowDuals (lpprob_struct *main_lp, lptols_struct *main_lptols,
		      lpopts_struct *main_lpopts)
/*
  This routine checks the dual variables returned by dy_rowDuals. It checks
  that y<i> = c<B>(inv(B))<i>. Columns of the basis inverse are obtained from
  the routine dy_betaj.

  Parameters:
    main_lp:	 the lp problem structure
    main_lptols: the lp tolerance structure
    main_lpopts: the lp options structure

  Returns: TRUE if y = c<B>inv(B), FALSE otherwise.
*/

{ int i,j,k,m,n ;
  consys_struct *sys ;
  flags *status ;

  double *y ;

  double *cB ;
  int *basis2sys ;
  basisel_struct *basisVec ;
  int basisLen ;

  double *betai ;
  double cBdotbetai ;
  int errcnt ;

  char *rtnnme = "dytest_rowDuals" ;

/*
  Do a little initialisation. Mention that we've started.
*/
  sys = main_lp->consys ;
  m = sys->concnt ;
  n = sys->varcnt ;

# ifndef DYLP_NDEBUG
  if (main_lpopts->print.soln >= 1)
  { dyio_outfmt(dy_logchn,dy_gtxecho,
		"\n%s: checking y = c<B>inv(B) using %s (%d x %d).",
		rtnnme,sys->nme,m,n) ;
    if (main_lpopts->print.soln >= 2)
    { dyio_outfmt(dy_logchn,dy_gtxecho,
		  "  basis contains %d entries.\n",main_lp->basis->len) ; } }
# endif
/*
  Acquire the row duals and the status vector.
*/
  y = NULL ;
  dy_rowDuals(main_lp,&y) ;
  status = main_lp->status ;
/*
  Make a vector c<B> of objective coefficients in basis order. This is
  considerably easier than creating a basis matrix (as is done for tableau
  testing). By construction, the basic variables for inactive constraints
  are the logicals, which have an objective coefficient of zero, and this
  is how cB and basis2sys are initialised. All that need be done for c<B>
  is to change the entries that are associated with architecturals. For the
  basis, we need to set all entries (logicals can be basic out of natural
  position). Recall that basic logicals are represented by negative indices.
*/
  cB = (double *) MALLOC((m+1)*sizeof(double)) ;
  basis2sys = (int *) MALLOC((m+1)*sizeof(int)) ;
  for (i = 1 ; i <= m ; i++)
  { cB[i] = 0.0 ;
    basis2sys[i] = -i ; }
  basisLen = main_lp->basis->len ;
  basisVec = main_lp->basis->el ;
  for (k = 1 ; k <= basisLen ; k++)
  { i = basisVec[k].cndx ;
    j = basisVec[k].vndx ;
    if (j > 0)
    { cB[i] = sys->obj[j] ;
      basis2sys[i] = j ; }
    else
    { basis2sys[i] = j ; } }
# ifndef DYLP_NDEBUG
  if (main_lpopts->print.soln >= 5)
  { dyio_outfmt(dy_logchn,dy_gtxecho,"\n\tc<B> =") ;
    k = 0 ;
    for (i = 1 ; i <= m ; i++)
    { if (cB[i] != 0)
      { if ((++k)%4 == 0)
	{ k = 0 ;
	  dyio_outfmt(dy_logchn,dy_gtxecho,"\n\t      ") ; }
	j = basis2sys[i] ;
	dyio_outfmt(dy_logchn,dy_gtxecho," (%d %g  %s %d)",
		    i,cB[i],consys_nme(sys,'v',j,FALSE,NULL),j) ; } } }
#   endif
/*
  Now step through the rows (equivalently, walk the basis) and see if
  y<i> = c<B>beta<j>, where beta<j> is the column of inv(B) such that
  x<j> is basic in pos'n i.
*/
  errcnt = 0 ;
  betai = NULL ;
  for (i = 1 ; i <= m ; i++)
  { j = basis2sys[i] ;
    if (dy_betaj(main_lp,j,&betai) == FALSE)
    { errcnt++ ;
      if (j < 0)
      { j = n-j ; }
      errmsg(952,rtnnme,sys->nme,"column",i,"variable",
	     consys_nme(sys,'v',j,FALSE,NULL),j-n) ;
      continue ; }
    cBdotbetai = 0 ;
    for (k = 1 ; k <= m ; k++)
    { 
      /*
         dyio_outfmt(dy_logchn,dy_gtxecho,
    		  "\n %s (%d) %g * %g",
		  consys_nme(sys,'c',k,FALSE,NULL),k,cB[k],betai[k]) ;
      */
      cBdotbetai += cB[k]*betai[k] ; }
    if (fabs(cBdotbetai-y[i]) > main_lptols->cost)
    { errcnt++ ;
      if (j < 0)
      { j = n-j ; }
      dyio_outfmt(dy_logchn,dy_gtxecho,
		  "\n  ERROR: pos'n %d %s (%d) c<B> dot beta<j> = %g; ",
		    i,consys_nme(sys,'v',j,FALSE,NULL),j-n,cBdotbetai) ;
	dyio_outfmt(dy_logchn,dy_gtxecho,"expected %g; err %g, tol %g.",
		    y[i],(cBdotbetai-y[i]),main_lptols->zero) ; } }
/*
  Free up space and report the result.
*/
  if (cB != NULL) FREE(cB) ;
  if (basis2sys != NULL) FREE(basis2sys) ;
  if (betai != NULL) FREE(betai) ;
  if (y != NULL) FREE(y) ;

  if (errcnt != 0)
  { dyio_outfmt(dy_logchn,dy_gtxecho,
		"\n%s: found %d errors testing y = c<B>inv(B).\n",
		rtnnme,errcnt) ;
    return (FALSE) ; }
  else
  { dyio_outfmt(dy_logchn,dy_gtxecho,"\n%s: pass y = c<B>inv(B).\n",rtnnme) ;
    return (TRUE) ; } }



bool dytest_colDuals (lpprob_struct *main_lp, lptols_struct *main_lptols,
		      lpopts_struct *main_lpopts)
/*
  This routine checks the dual variables returned by dy_colDuals (more
  usually called the reduced costs of the architectural variables).
  
  It checks that cbar<N> = c<N> - yN, where y is the vector of row duals
  returned by dy_rowDuals and N is the set of nonbasic architectural columns
  of A (or the matching index set, as appropriate).

  Parameters:
    main_lp:	 the lp problem structure
    main_lptols: the lp tolerance structure
    main_lpopts: the lp options structure

  Returns: TRUE if cbar<N> = c<N> - yN, FALSE otherwise.
*/

{ int j,m,n ;
  consys_struct *sys ;
  flags *status ;
  flags statj ;
  double *obj ;

  double *y ;
  double *cbarN ;

  double cbarj ;

  int errcnt ;
  bool staterr ;
  char *errstring ;

  char *rtnnme = "dytest_colDuals" ;

/*
  Do a little initialisation. Mention that we've started.
*/
  sys = main_lp->consys ;
  m = sys->concnt ;
  n = sys->varcnt ;

# ifndef DYLP_NDEBUG
  if (main_lpopts->print.soln >= 1)
  { dyio_outfmt(dy_logchn,dy_gtxecho,
		"\n%s: checking cbar<N> = c<N> - yN using %s (%d x %d).",
		rtnnme,sys->nme,m,n) ; }
# endif
/*
  Acquire the row duals, column duals, status vector, and objective.
*/
  y = NULL ;
  dy_rowDuals(main_lp,&y) ;
  cbarN = NULL ;
  dy_colDuals(main_lp,&cbarN) ;
  status = main_lp->status ;
  obj = sys->obj ;
/*
  Now step through the columns checking that cbar<j> = c<j> = dot(y,a<j>).
  Also check to see that the sign is correct for the status of the variable
  in a minimisation problem.  For status values not listed (vstatSB and any
  of the basic status codes), there's no `correct' sign.
*/
  errcnt = 0 ;
  for (j = 1 ; j <= n ; j++)
  { cbarj = obj[j] - consys_dotcol(sys,j,y) ;
    statj = getflg(status[j],vstatSTATUS) ;
    if (fabs(cbarj-cbarN[j]) > main_lptols->cost)
    { errcnt++ ;
      dyio_outfmt(dy_logchn,dy_gtxecho,
		  "\nERROR: col %s (%d) %s cbar<%d> = %g; expected %g;",
		  consys_nme(sys,'v',j,FALSE,NULL),j,dy_prtvstat(statj),
		  j,cbarj,cbarN[j]) ;
      dyio_outfmt(dy_logchn,dy_gtxecho," error %g, tol %g.",
		  fabs(cbarj),main_lptols->cost) ; }
    staterr = FALSE ;
    switch (statj)
    { case vstatNBLB:
      { if (cbarj < -main_lptols->zero)
	{ staterr = TRUE ;
	  errstring = "positive" ; }
	break ; }
      case vstatNBUB:
      { if (cbarj > main_lptols->zero)
	{ staterr = TRUE ;
	  errstring = "negative" ; }
	break ; }
      case vstatNBFR:
      { if (fabs(cbarj) > main_lptols->zero)
	{ staterr = TRUE ;
	  errstring = "zero" ; }
	break ; }
      default:
      { break ; } }
    if (staterr == TRUE)
    { dyio_outfmt(dy_logchn,dy_gtxecho,
		  "\nERROR: col %s (%d) %s cbar<%d> = %g; should be %s.",
		  consys_nme(sys,'v',j,FALSE,NULL),j,dy_prtvstat(statj),
		  j,cbarj,errstring) ; } }
/*
  Free up space and report the result.
*/
  if (y != NULL) FREE(y) ;
  if (cbarN != NULL) FREE(cbarN) ;

  if (errcnt != 0)
  { dyio_outfmt(dy_logchn,dy_gtxecho,
		"\n%s: found %d errors testing cbar<N> = c<N> - yN.\n",
		rtnnme,errcnt) ;
    return (FALSE) ; }
  else
  { dyio_outfmt(dy_logchn,dy_gtxecho,
		"\n%s: pass cbar<N> = c<N> - yN.\n",rtnnme) ;
    return (TRUE) ; } }

