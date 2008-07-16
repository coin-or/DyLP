/*
  This file is a part of the Dylp LP distribution.

        Copyright (C) 2005 -- 2008 Lou Hafer

        School of Computing Science
        Simon Fraser University
        Burnaby, B.C., V5A 1S6, Canada
        lou@cs.sfu.ca

  This code is licensed under the terms of the Common Public License (CPL).
*/

/*
  This file contains routines which return primal and dual solutions, unscaled,
  in the original system frame of reference. For each of the primal and dual
  variables, there are two routines:
    * dy_rowPrimals:	the values of the primal logical variables
    * dy_colPrimals:	the values of the primal architectural variables
    * dy_rowDuals:	the values of the dual variables associated with
			architectural constraints
    * dy_colDuals:	the values of the dual variables associated with
			implicit bound constraints

  The notion of row and column primals is standard. The values returned by
  rowDuals are commonly called the dual variables.  The values returned by
  colDuals are commonly called the reduced costs.

  There is another utility, dy_orig_soln, which is (at present a fairly
  specialised routine used by dylp_utils:buildsoln. It should be rewritten to
  conform to the pattern of the previous four.

  There's a routine, dy_expandxopt, which takes the primal solution generated
  by dy_orig_soln (which contains basic variables only) and expands it to a
  full solution (all architectural variables). This, too, should be pulled from
  dylp_utils and moved here (or perhaps made obsolete).
*/

#define DYLP_INTERNAL

#include "dylp.h"

static char svnid[] UNUSED = "$Id$" ;

#ifdef DYLP_PARANOIA
extern bool dy_std_paranoia (const lpprob_struct *orig_lp,
			     const char *rtnnme) ;
#endif



void dy_colDuals (lpprob_struct *orig_lp, double **p_cbar)

/*
  Returns the unscaled vector of duals associated with architectural columns
  (aka reduced costs), in the original system frame of reference.

  These are the duals associated with implicit bound constraints. See
  dy_rowDuals for the duals associated with explicit (architectural)
  constraints. (These latter are the usual notion of dual variables.)

  The algorithm is to walk the columns of orig_sys, copying over the reduced
  cost from dy_cbar when the variable is active, otherwise calculting cbar<j>
  on the spot.

  For active variables, we have

  sc_cbar<j> = sc_c<j> - sc_c<B>sc_inv(B)sc_a<j>
	     = c<j>S<j> - c<B>S<B>inv(S<B>)inv(B)inv(R)Ra<j>S<j>
	     = c<j>S<j> - c<B>inv(B)a<j>S<j>
	     = cbar<j>S<j>

  To unscale sc_cbar<j>, we simply multiply by 1/S<j>, keeping in mind that
  if x<j> is a logical for row i, the appropriate factor is R<i>.

  For inactive variables, we calculate dot(y,a<j>) using the scaled version
  of the original system, which leaves us with the same sc_abar<j>.

  Why not use the client's original system and the vector of unscaled duals
  returned by dy_rowDuals?  That would certainly be an option. One argument
  against it is the additional work involved to get the unscaled duals. The
  other argument is that maximising the independence of the two calculations
  means that the test routine (which confirms cbar<j> = c<j> - dot(y,a<j>)
  in the external frame) is marginally more convincing.

  Parameters:
    orig_lp:	the original lp problem
    p_cbar:	(i) pointer to vector; if NULL, a vector of the appropriate
		    size will be allocated
		(o) vector of reduced costs

  Returns: undefined
*/

{ int i,j,k,m,n,i_orig,j_orig,m_orig,n_orig ;
  flags statj ;
  consys_struct *orig_sys ;

  double *orig_y ;
  consys_struct *scaled_orig_sys ;
  bool scaled ;

  double cbarj ;
  double *cbar ;
  const double *rscale,*cscale ;

# ifdef DYLP_PARANOIA
  char *rtnnme = "dy_colDuals" ;

  if (dy_std_paranoia(orig_lp,rtnnme) == FALSE)
  { return ; }
  if (p_cbar == NULL)
  { errmsg(2,rtnnme,"cbar") ;
    return ; }
# endif
/*
  Is unscaling required? Acquire the scaling vectors and set up scaled_orig_sys
  accordingly.
*/
  scaled = dy_isscaled() ;
  if (scaled == TRUE)
  { dy_scaling_vectors(&rscale,&cscale) ;
    scaled_orig_sys = dy_scaled_origsys() ; }

  orig_sys = orig_lp->consys ;
  n_orig = orig_sys->varcnt ;
  m_orig = orig_sys->concnt ;
  n = dy_sys->varcnt ;
  m = dy_sys->concnt ;
/*
  Do we need a vector?
*/
  if (*p_cbar != NULL)
  { cbar = *p_cbar ;
    memset(cbar,0,(n_orig+1)*sizeof(double)) ; }
  else
  { cbar = (double *) CALLOC((n+1),sizeof(double)) ; }
/*
  Make a vector of duals that matches orig_sys, for efficient pricing of
  inactive columns.
*/
  orig_y = (double *) CALLOC((m_orig+1),sizeof(double)) ;
  for (i = 1 ; i <= m ; i++)
  { i_orig = dy_actcons[i] ;
    orig_y[i_orig] = dy_y[i] ; }
/*
  Get on with the calculation. For an active variable, we can pull the value
  from dy_cbar. For an inactive variable, we need to calculate dot(y,a<j>).
  Then we unscale and drop the result into the proper place in the result
  vector.  Since we're starting from orig_sys, we'll never reference a column
  for a logical variable.
*/
  for (j_orig = 1 ; j_orig <= n_orig ; j_orig++)
  { if (ACTIVE_VAR(j_orig))
    { j = dy_origvars[j_orig] ;
      statj = getflg(dy_status[j],vstatSTATUS) ;
      if (flgon(statj,vstatBASIC))
      { cbarj = 0.0 ; }
      else
      { cbarj = dy_cbar[j]/cscale[j_orig] ; } }
    else
    { if (scaled == TRUE)
      { cbarj = scaled_orig_sys->obj[j_orig] ; 
	cbarj -= consys_dotcol(scaled_orig_sys,j_orig,orig_y) ;
	cbarj /= cscale[j_orig] ; }
      else
      { cbarj = orig_sys->obj[j_orig] ;
	cbarj -= consys_dotcol(scaled_orig_sys,j_orig,orig_y) ; } }
    setcleanzero(cbarj,dy_tols->cost) ;
    cbar[j_orig] = cbarj ; }
/*
  Clean up a bit and we're done.
*/
  if (orig_y != NULL) FREE(orig_y) ;
  *p_cbar = cbar ;

  return ; }



void dy_rowDuals (lpprob_struct *orig_lp, double **p_y)

/*
  This routine returns the unscaled vector of row duals, commonly referred to
  as the dual variables, c<B>inv(B). The values are unscaled and returned in a
  vector matching the original system frame of reference. Duals associated with
  inactive rows are always zero. The relevant bit of unscaling is:

  sc_y<i> = - sc_c<B>sc_inv(B)
	  = - c<B>S<B>inv(S<B>)inv(B)inv(R)
	  = - c<B>inv(B)inv(R)

  So, to recover y<i> we need to postmultiply by inv(R). The appropriate row
  factor is the one associated with the original row.

  Parameters:
    orig_lp:	the original lp problem
    p_y:	(i) vector to hold the dual variables; if NULL, a vector of
		    appropriate size will be allocated
		(o) values of the dual variables, unscaled, in the original
		    system frame of reference
*/

{ int i,m,n,i_orig,m_orig,n_orig ;
  double yi ;
  double *y ;

  consys_struct *orig_sys ;

  bool scaled ;
  const double *rscale,*cscale ;

# ifndef DYLP_NDEBUG
  int j,v ;
# endif
# ifdef DYLP_PARANOIA
  char *rtnnme = "dy_rowDuals" ;

  if (dy_std_paranoia(orig_lp,rtnnme) == FALSE)
  { return ; }
  if (p_y == NULL)
  { errmsg(2,rtnnme,"y") ;
    return ; }
# endif

/*
  Is unscaling required? Acquire the scaling vectors.
  accordingly.
*/
  scaled = dy_isscaled() ;
  if (scaled == TRUE)
  { dy_scaling_vectors(&rscale,&cscale) ; }

  orig_sys = orig_lp->consys ;
  n_orig = orig_sys->varcnt ;
  m_orig = orig_sys->concnt ;
  n = dy_sys->varcnt ;
  m = dy_sys->concnt ;
/*
  Do we need a vector?
*/
  if (*p_y != NULL)
  { y = *p_y ;
    memset(y,0,(m_orig+1)*sizeof(double)) ; }
  else
  { y = (double *) CALLOC((m_orig+1),sizeof(double)) ; }
/*
  Step through the constraints of the original system. For active constraints,
  acquire and unscale the dual value.
*/
  for (i_orig = 1 ; i_orig <= m_orig ; i_orig++)
  { if (ACTIVE_CON(i_orig))
    { i = dy_origcons[i_orig] ;
      yi = dy_y[i] ;
      if (scaled == TRUE)
      { yi *= rscale[i_orig] ; }
      setcleanzero(yi,dy_tols->cost) ; }
    else
    { yi = 0.0 ; }
    y[i_orig] = yi ; }

# ifndef DYLP_NDEBUG
  if (dy_opts->print.soln >= 3)
  { dyio_outfmt(dy_logchn,dy_gtxecho,"\ty =") ;
    v = 0 ;
    for (i_orig = 1 ; i_orig <= m_orig ; i_orig++)
    { if (y[i_orig] != 0)
      { if ((++v)%4 == 0)
	{ v = 0 ;
	  dyio_outfmt(dy_logchn,dy_gtxecho,"\n\t   ") ; }
	i = dy_origcons[i_orig] ;
	j = dy_basis[i] ;
	dyio_outfmt(dy_logchn,dy_gtxecho," (%d %g %s %d)",
		    i_orig,y[i_orig],
		    consys_nme(dy_sys,'v',j,FALSE,NULL),j) ; } } }
# endif

/*
  That's it. Return the vector.
*/
  *p_y = y ;

  return ; }




void dy_orig_soln (double *x, double *y)

/*
  This routine unscales the primal and dual variable values associated with the
  rows of the active system before returning them to the user. The necessary
  unscaling is as follows:

    primal architectural:	x<j>S<j>
    primal logical:		x<i>/R<i>

    dual:			y<i>R<i>

  The vectors are indexed by basis position.

  This routine isn't really general purpose --- it's called from
  dylp_utils:buildsoln and assumes that x and y are already populated with
  scaled values. It should get a makeover to match the interface conventions of
  the other routines in the package.

  Parameters:
    x:	basic primal variables
    y:	dual variables

  Returns: undefined.
*/

{ int i,j,i_orig,j_orig ;
  double xi,yi ;
  const double *rscale,*cscale ;

/*
  Did we scale? If not, return right off. Otherwise, acquire the scaling
  vectors.
*/
  if (dy_isscaled() == FALSE) return ;
  dy_scaling_vectors(&rscale,&cscale) ;
/*
  Since we're only dealing with duals and basic primal variables, it suffices
  to step through the constraints (equivalently, basis positions).
*/
  for (i = 1 ; i <= dy_sys->concnt ; i++)
  { i_orig = dy_actcons[i] ;
    j = dy_basis[i] ;
    xi = x[i] ;
    if (j <= dy_sys->concnt)
      xi /= rscale[i_orig] ;
    else
    { j_orig = dy_actvars[j] ;
      xi *= cscale[j_orig] ; }
    setcleanzero(xi,dy_tols->zero) ;
    x[i] = xi ;
    
    yi = y[i] ;
    yi *= rscale[i_orig] ;
    setcleanzero(yi,dy_tols->cost) ;
    y[i] = yi ; }

  return ; }

