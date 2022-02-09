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
  This file contains routines to test dylp's ray routines: dy_primalRays and
  dy_dualRays.
*/

#include "dylp.h"

#include <math.h>

extern ioid dy_logchn ;
extern bool dy_gtxecho ;



int dytest_primalRays (int *p_numRays,
		       lpprob_struct *main_lp, lptols_struct *main_lptols,
		       lpopts_struct *main_lpopts)

/*
  This routine checks the primal rays returned by dy_primalRays. For a ray r,
  the first test is that dot(c,r) < 0 (remember, dylp minimises). Then, for a
  constraint ax <= b, we should have dot(a,r) <= 0. For a constraint ax >= b,
  the test is dot(a,r) >= 0.

  It's up to the calling routine to determine if the number of rays is as
  expected. In particular, it's not an error if dy_primalRays returns fewer
  rays than requested.  If dy_primalRays returns zero rays, this is treated
  as the degenerate case of `all rays pass' and the routine will return
  TRUE.

  Parameters:
    p_numRays:	 (i) the number of rays to request from dy_primalRays
		 (o) the number of rays returned by dy_primalRays
    main_lp:	 the lp problem structure
    main_lptols: the lp tolerance structure
    main_lpopts: the lp options structure

  Returns: 0 if all rays returned tested as valid rays, error count otherwise.
*/

{ int m,n,i,j,k ;
  consys_struct *sys ;

  double **rays ;
  int reqRays,rcvRays ;
  double *rayk ;
  double aidotrayk ;
  bool error ;
  int errcnt ;

  char *rtnnme = "dytest_primalRays" ;

/*
  Do a little initialisation. Mention that we've started.
*/
  sys = main_lp->consys ;
  n = sys->varcnt ;
  m = sys->concnt ;

# ifndef DYLP_NDEBUG
  if (main_lpopts->print.rays >= 1)
  { dyio_outfmt(dy_logchn,dy_gtxecho,
		"%s: checking primal rays using %s (%d x %d).\n",
		rtnnme,sys->nme,m,n) ; }
# endif
/*
  Ask for the requested number of rays.
*/
  rays = NULL ;
  reqRays = *p_numRays ;
  rcvRays = reqRays ;
  *p_numRays = 0 ;
  if (dy_primalRays(main_lp,&rcvRays,&rays) == FALSE)
  { errmsg(955,rtnnme,sys->nme,"primal") ;
    if (rays != NULL)
    { for (k = 0 ; k < rcvRays ; k++)
      { if (rays[k] != NULL) FREE(rays[k]) ; }
      FREE(rays) ; }
    return (1) ; }
  *p_numRays = rcvRays ;
# ifndef DYLP_NDEBUG
  if (main_lpopts->print.rays >= 2)
  { dyio_outfmt(dy_logchn,dy_gtxecho,"\n    requested %d rays, received %d.",
		reqRays,rcvRays) ; }
# endif
/*
  Now test each ray. Check first that we actually have a nonzero ray, then
  check dot(c,r) < 0, and finally that dot(a<i>,r) <= 0 for a<i>x <= b<i>
  (dot(a<i>,r) >= 0 for a<i>x >= b<i>).
*/
  errcnt = 0 ;
  for (k = 0 ; k < rcvRays ; k++)
  { rayk = rays[k] ;
    if (rayk == NULL)
    { dyio_outfmt(dy_logchn,dy_gtxecho,"\n  ERROR: ray %d is NULL.",k) ;
      errcnt++ ;
      continue ; }
    aidotrayk = exvec_1norm(rayk,n) ;
    if (fabs(aidotrayk) <= 0.0)
    { dyio_outfmt(dy_logchn,dy_gtxecho,"\n  ERROR: ray %d is zero.",k) ;
      FREE(rayk) ;
      rays[k] = NULL ;
      continue ; }
/*
  Check dot(c,r) < 0.
*/
      aidotrayk = 0 ;
      if (main_lpopts->print.rays >= 5)
      { dyio_outchr(dy_logchn,dy_gtxecho,'\n') ; }
      for (j = 1 ; j <= n ; j++)
      { if (rayk[j] != 0.0)
	{ aidotrayk += rayk[j]*sys->obj[j] ;
	  if (main_lpopts->print.rays >= 5)
	  { dyio_outfmt(dy_logchn,dy_gtxecho," (%s (%d) %g*%g)",
			consys_nme(sys,'v',j,FALSE,NULL),
			j,rayk[j],sys->obj[j]) ; } } }
      if (aidotrayk >= 0)
      { dyio_outfmt(dy_logchn,dy_gtxecho,
		    "\n  ERROR: dot(c,ray[%d]) = %g; should be < 0.",
		    k,aidotrayk) ;
	errcnt++ ; }
      else
      { if (main_lpopts->print.rays >= 2)
	{ dyio_outfmt(dy_logchn,dy_gtxecho,"\n    dot(c,ray[%d]) = %g.",
		      k,aidotrayk) ; } }
/*
  Check dot(a<i>,r) <= 0 or >= 0, as appropriate for the constraint. We need
  dot(a<i>,r) = 0 for range constraints and equalities.
*/
    for (i = 1 ; i <= m ; i++)
    { aidotrayk = consys_dotrow(sys,i,rayk) ;
      error = FALSE ;
      if (sys->ctyp[i] != contypLE)
      { if (aidotrayk < -main_lptols->zero)
	{ error = TRUE ; } }
      if (sys->ctyp[i] != contypGE)
      { if (aidotrayk > main_lptols->zero)
	{ error = TRUE ; } }
      if (error == TRUE)
      { dyio_outfmt(dy_logchn,dy_gtxecho,
		    "\n  ERROR: a<%d> dot ray<%d> = %g ; should be %s 0.",
		    i,k,aidotrayk,((sys->ctyp[i] == contypGE)?">=":"<=")) ;
	errcnt++ ; } }
    FREE(rayk) ;
    rays[k] = NULL ; }
/*
  We're done. Clean up and go home.
*/
  FREE(rays) ;
  if (errcnt != 0)
  { dyio_outfmt(dy_logchn,dy_gtxecho,
	  "\n%s: found %d errors in %d rays testing cr < 0, Ar <= 0.\n",
	   rtnnme,errcnt,rcvRays) ; }
  else
  { dyio_outfmt(dy_logchn,dy_gtxecho,
		"\n%s: pass cr < 0, Ar <= 0.\n",rtnnme) ; }

  return (errcnt) ; }



int dytest_dualRays (int *p_numRays,
		     lpprob_struct *main_lp, lptols_struct *main_lptols,
		     lpopts_struct *main_lpopts)

/*
  This routine checks the dual rays returned by dy_dualRays. For a ray r,
  we do two tests:
    * for the dual objective b, dot(r,b) < 0
    * for a dual constraint ya<j> >= c, dot(r,a<j>) >= 0.

  Note that the math doesn't necessarily hold unless the constraint system
  and dual ray contain *all* components. The ray must have components for the
  usual row duals (c<B>inv(B)) and for the duals associated with implicit
  bound constraints. Which implies we need to fabricate coefficients as
  needed to make implicit bound constraints explicit in the constraint system.

  It's up to the calling routine to determine if the number of rays is as
  expected. In particular, it's not an error if dy_dualRays returns fewer
  rays than requested.  If dy_dualRays returns zero rays, this is treated
  as the degenerate case of `all rays pass' and the routine will return
  TRUE.

  Parameters:
    p_numRays:	 (i) the number of rays to request from dy_dualRays
		 (o) the number of rays returned by dy_dualRays
    main_lp:	 the lp problem structure
    main_lptols: the lp tolerance structure
    main_lpopts: the lp options structure

  Returns: 0 if all rays returned tested as valid rays, error count otherwise.
*/

{ int m,n,i,j,k,l ;
  consys_struct *sys ;

  double **rays ;
  int reqRays,rcvRays ;
  double *rayk ;
  double ajdotrayk ;

  bool fullRay ;

  int bposj ;
  flags *status ;
  flags statj ;
  double *x ;
  double xj ;

  int errcnt ;

  char *rtnnme = "dytest_dualRays" ;

/*
  Do a little initialisation. Mention that we've started.
*/
  sys = main_lp->consys ;
  n = sys->varcnt ;
  m = sys->concnt ;

# ifndef DYLP_NDEBUG
  if (main_lpopts->print.rays >= 1)
  { dyio_outfmt(dy_logchn,dy_gtxecho,
		"%s: checking dual rays using %s (%d x %d).",
		rtnnme,sys->nme,m,n) ; }
# endif
/*
  Ask for the requested number of rays. Go for the true dual sign convention,
  lest we go completely mad.
*/
  rays = NULL ;
  reqRays = *p_numRays ;
  rcvRays = reqRays ;
  *p_numRays = 0 ;
  fullRay = TRUE ;
  if (dy_dualRays(main_lp,fullRay,&rcvRays,&rays,TRUE) == FALSE)
  { errmsg(955,rtnnme,sys->nme,"dual") ;
    if (rays != NULL)
    { for (k = 0 ; k < rcvRays ; k++)
      { if (rays[k] != NULL) FREE(rays[k]) ; }
      FREE(rays) ; }
    return (1) ; }
  *p_numRays = rcvRays ;
# ifndef DYLP_NDEBUG
  if (main_lpopts->print.rays >= 2)
  { dyio_outfmt(dy_logchn,dy_gtxecho,"\n    requested %d rays, received %d.",
		reqRays,rcvRays) ; }
# endif
/*
  Now test each ray. Check first that we actually have a nonzero ray, then
  check dot(r,b) < 0, and then  dot(r,a<j>) >= 0 for all columns j.
*/
  errcnt = 0 ;
  if (rcvRays > 0)
  { for (k = 0 ; k < rcvRays ; k++)
    { rayk = rays[k] ;
      if (rayk == NULL)
      { dyio_outfmt(dy_logchn,dy_gtxecho,"\n  ERROR: ray %d is NULL.",k) ;
	errcnt++ ;
	continue ; }
      ajdotrayk = exvec_1norm(rayk,m) ;
      if (fabs(ajdotrayk) <= 0.0)
      { dyio_outfmt(dy_logchn,dy_gtxecho,"\n  ERROR: ray %d is zero.",k) ;
	FREE(rayk) ;
	rays[k] = NULL ;
	continue ; }

#     ifndef DYLP_NDEBUG
      if (main_lpopts->print.rays >= 3)
      { dyio_outfmt(dy_logchn,dy_gtxecho,"\n      ray<%d> non-zeros:",k) ;
	j = 0 ;
	for (i = 1 ; i <= m ; i++)
	{ if (rayk[i] != 0)
	  { if (j == 0)
	    { dyio_outfmt(dy_logchn,dy_gtxecho,"\n\t") ; }
	    dyio_outfmt(dy_logchn,dy_gtxecho," (%s (%d) %g)",
			consys_nme(sys,'c',i,FALSE,NULL),i,rayk[i]) ;
	    j = (j+1)%3 ; } }
	if (fullRay == TRUE)
	{ i = j ;
	  dyio_outfmt(dy_logchn,dy_gtxecho," *") ;
	  for (j = 1 ; j <= n ; j++)
	  { if (rayk[m+j] != 0)
	    { if (i == 0)
	      { dyio_outfmt(dy_logchn,dy_gtxecho,"\n\t") ; }
	      dyio_outfmt(dy_logchn,dy_gtxecho," (%s (%d) %g)",
			  consys_nme(sys,'v',j,FALSE,NULL),j,rayk[m+j]) ;
	      i = (i+1)%3 ; } } } }
#     endif
/*
  Check dot(r,b) < 0. For the first m elements, it's just a straightforward
  dot product of ray elements with the rhs values.
*/
      ajdotrayk = 0 ;
      if (main_lpopts->print.rays >= 5)
      { dyio_outchr(dy_logchn,dy_gtxecho,'\n') ; }
      for (i = 1 ; i <= m ; i++)
      { if (rayk[i] != 0.0)
	{ ajdotrayk += rayk[i]*sys->rhs[i] ;
	  if (main_lpopts->print.rays >= 5)
	  { dyio_outfmt(dy_logchn,dy_gtxecho," (%s (%d) %g*%g)",
			consys_nme(sys,'c',i,FALSE,NULL),
			i,rayk[i],sys->rhs[i]) ; } } }
/*
  For the remaining components corresponding to architecturals, we're
  synthesizing bound constraints on the fly. If the ray component corresponds
  to a variable x<j> that's NBUB or NBFX, use u<j> as the rhs value; NBLB,
  -l<j>.  Check that ray components are zero for variables that are not NBLB,
  NBUB, or NBFX. Serious confusion if it's otherwise, eh?
*/
      status = main_lp->status ;
      x = main_lp->x ;
      if (fullRay == TRUE)
      { if (main_lpopts->print.rays >= 5)
	  dyio_outchr(dy_logchn,dy_gtxecho,'\n') ;
	l = 0 ;
	for (j = 1 ; j <= n ; j++)
	{ statj = status[j] ;
	  if (((int) statj) > 0)
	  { if (main_lpopts->print.rays >= 5)
	    { dyio_outfmt(dy_logchn,dy_gtxecho," (%s (%d) %s",
			  consys_nme(sys,'v',j,FALSE,NULL),j,
			  dy_prtvstat(statj)) ; }
	    if (flgon(statj,vstatNBUB|vstatNBFX))
	    { ajdotrayk += rayk[m+j]*sys->vub[j] ;
	      if (main_lpopts->print.rays >= 5 &&
		  (sys->vub[j] != 0.0 || rayk[m+j] != 0))
	      { dyio_outfmt(dy_logchn,dy_gtxecho," %g*%g)",
			  rayk[m+j],sys->vub[j]) ; } }
	    else
	    if (flgon(statj,vstatNBLB))
	    { ajdotrayk -= rayk[m+j]*sys->vlb[j] ;
	      if (main_lpopts->print.rays >= 5 &&
		  (sys->vlb[j] != 0.0 || rayk[m+j] != 0))
	      { dyio_outfmt(dy_logchn,dy_gtxecho," %g*(-%g))",
			    rayk[m+j],sys->vlb[j]) ; } }
	    else
	    if (rayk[m+j] != 0.0)
	    { dyio_outfmt(dy_logchn,dy_gtxecho,
			  "\n  ERROR: %s (%d) %s not tight at a bound!.",
			  consys_nme(sys,'v',j,FALSE,NULL),j,
			  dy_prtvstat(statj)) ;
	      errcnt++ ; } }
/*
  Let's not forget that there's a nonbasic dual that's driving this ray,
  which will correspond to a basic primal variable. Keep count, because there
  should be at most one nonzero coefficient here and it better not be within
  bounds. Use the closest bound to the variable's value.
*/
	  else
	  if (rayk[m+j] != 0)
	  { l++ ;
	    bposj = -((int) statj) ;
	    xj = x[bposj] ;
	    if (main_lpopts->print.rays >= 5)
	    { dyio_outfmt(dy_logchn,dy_gtxecho," (%s (%d) %s",
			  consys_nme(sys,'v',j,FALSE,NULL),j,
			  dy_prtvstat(vstatB)) ; }
	    if (xj > sys->vub[j])
	    { ajdotrayk += rayk[m+j]*sys->vub[j] ;
	      if (main_lpopts->print.rays >= 5)
	      { dyio_outfmt(dy_logchn,dy_gtxecho," %g*%g)",
			  rayk[m+j],sys->vub[j]) ; } }
	    else
	    if (xj < sys->vlb[j])
	    { ajdotrayk -= rayk[m+j]*sys->vlb[j] ;
	      if (main_lpopts->print.rays >= 5)
	      { dyio_outfmt(dy_logchn,dy_gtxecho," %g*(-%g))",
			  rayk[m+j],sys->vlb[j]) ; } }
	    else
	    { dyio_outfmt(dy_logchn,dy_gtxecho,
			  "\n  ERROR: %s (%d) %s = %g, lb = %g, ub = %g,",
			  consys_nme(sys,'v',j,FALSE,NULL),j,
			  dy_prtvstat(vstatB),xj,sys->vlb[j],sys->vub[j]) ;
	      dyio_outfmt(dy_logchn,dy_gtxecho,
			  " should not be in bounds to spawn dual ray!") ;
	      errcnt++ ; } } }
/*
  There should be at most one coefficient for nonbasic duals. `At most',
  because the basic variable for the constraint that spawned this ray might
  be a logical.
*/

	if (l > 1)
	{ dyio_outfmt(dy_logchn,dy_gtxecho,
		      "\n  ERROR: ray[%d] has %d nonzeroes matching",k,l) ;
	  dyio_outfmt(dy_logchn,dy_gtxecho,
		      " nonbasic duals; should be at most 1.") ;
	  errcnt++ ; } }
/*
  Ok, we've added the contribution of the row and column duals. What's the
  result?
*/
      if (ajdotrayk >= 0)
      { dyio_outfmt(dy_logchn,dy_gtxecho,
		    "\n  ERROR: dot(ray[%d],b) = %g; should be < 0.",
		    k,ajdotrayk) ;
	errcnt++ ; }
      else
      { if (main_lpopts->print.rays >= 2)
	{ dyio_outfmt(dy_logchn,dy_gtxecho,"\n    dot(ray[%d],b) = %g.",
		      k,ajdotrayk) ; } }
/*
  Now test dot(r,a<j>) > 0. Again, it's straightforward for the explicit
  component.  Then, look at the entry in the full ray which matches the
  column we're processing; if it's nonzero, we need to synthesize bound
  constraints. An upper bound constraint should look like x<j> <= u<j>, a
  lower bound constraint -x<j> <= -l<j>. As with the objective, we need to do
  a little work to figure out which bound constraint to use. There's no need
  to redo the ray coefficient consistency tests.
*/
      for (j = 1 ; j <= n ; j++)
      { ajdotrayk = consys_dotcol(sys,j,rayk) ;
	if (main_lpopts->print.rays >= 5)
	{ dyio_outfmt(dy_logchn,dy_gtxecho,
		      "\n\t  dotcol = %g",ajdotrayk) ; }
	if (fullRay == TRUE)
	{ statj = status[j] ;
	  if (((int) statj) > 0)
	  { if (main_lpopts->print.rays >= 5)
	    { dyio_outfmt(dy_logchn,dy_gtxecho," (%s (%d) %s",
			consys_nme(sys,'v',j,FALSE,NULL),j,
			dy_prtvstat(statj)) ; }
	    if (flgon(statj,vstatNBUB|vstatNBFX))
	    { ajdotrayk += rayk[m+j] ;
	      if (main_lpopts->print.rays >= 5)
	      { dyio_outfmt(dy_logchn,dy_gtxecho," %g*%g)",
			    rayk[m+j],1.0) ; } }
	    else
	    if (flgon(statj,vstatNBLB))
	    { ajdotrayk -= rayk[m+j] ;
	      if (main_lpopts->print.rays >= 5)
	      { dyio_outfmt(dy_logchn,dy_gtxecho," %g*(%g))",
			    rayk[m+j],-1.0) ; } } }
	  else
	  { bposj = -((int) statj) ;
	    xj = x[bposj] ;
	    if (main_lpopts->print.rays >= 5)
	    { dyio_outfmt(dy_logchn,dy_gtxecho," (%s (%d) %s",
			  consys_nme(sys,'v',j,FALSE,NULL),j,
			  dy_prtvstat(vstatB)) ; }
	    if (xj > sys->vub[j])
	    { ajdotrayk += rayk[m+j] ;
	      if (main_lpopts->print.rays >= 5)
	      { dyio_outfmt(dy_logchn,dy_gtxecho," %g*%g)",
			    rayk[m+j],1.0) ; } }
	    else
	    if (xj < sys->vlb[j])
	    { ajdotrayk -= rayk[m+j] ;
	      if (main_lpopts->print.rays >= 5)
	      { dyio_outfmt(dy_logchn,dy_gtxecho," %g*(%g))",
			    rayk[m+j],-1.0) ; } } } }
	if (ajdotrayk < -main_lptols->zero)
	{ dyio_outfmt(dy_logchn,dy_gtxecho,
		"\n  ERROR: dot(ray<%d>, a<%s (%d)>) = %g ; should be >= 0.",
		k,consys_nme(sys,'v',j,FALSE,NULL),j,ajdotrayk) ;
	  errcnt++ ; }
	else
	{ if (main_lpopts->print.rays >= 3)
	  { dyio_outfmt(dy_logchn,dy_gtxecho,
		      "\n      dot(ray<%d>, a<%s (%d)>) = %g.",
		      k,consys_nme(sys,'v',j,FALSE,NULL),j,ajdotrayk) ; } } }
      FREE(rayk) ;
      rays[k] = NULL ; } }
/*
  We're done. Clean up and go home.
*/
  FREE(rays) ;
  if (errcnt != 0)
  { dyio_outfmt(dy_logchn,dy_gtxecho,
	  "\n%s: found %d errors in %d rays testing rA >= 0.\n",
	   rtnnme,errcnt,rcvRays) ; }
  else
  { dyio_outfmt(dy_logchn,dy_gtxecho,"\n%s: pass rb < 0, rA >= 0.\n",rtnnme) ; }

  return (errcnt) ; }

