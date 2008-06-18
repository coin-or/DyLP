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
  This file contains routines which perform unscaling, transforming various
  scaled vectors to unscaled vectors. You really want to read the typeset
  documentation for the details of the linear algebra. The attempts at ascii
  math that you see here will make much more sense.

  In the code comments, S is the diagonal matrix of column scaling factors,
  and R is the diagonal matirix of row scaling factors. Given that it's a
  diagonal matrix, entries are referenced with a single subscript.

  Probably the most important thing to mention here is that the colscale
  vector only contains entries for the architectural variables.  For the
  logical associated with row i, the appropriate column scale factor is
  1/rowscale<i>.

  I'm not entirely happy with the combination of function for unscaling
  subroutines. They combine two activities: unscaling, and a change of
  reference frame from the active system to the original system. From a
  strictly functional viewpoint, these two activities should not be tied
  together. But from a practical viewpoint, there's no point in unscaling
  unless the product is destined for use by the outside world, in which case
  change of reference frame is also necessary. And combining the two activities
  avoids multiple scans through arrays.
  
  Arguably, the focus should be reversed. Change of reference frame is
  mandatory. Scaling is optional.
*/

#define DYLP_INTERNAL

#include "dylp.h"

static char svnid[] UNUSED = "$Id$" ;



void dy_orig_cbar (int nbcnt, double *cbar, int *vndx)

/*
  This is a special purpose routine which unscales the vector of selected
  reduced costs produced by dy_pricenbvars. All we do here is walk the vectors
  and apply the unscaling.

  sc_cbar<j> = sc_c<j> - sc_c<B>sc_inv(B)sc_a<j>
	     = c<j>S<j> - c<B>S<B>inv(S<B>)inv(B)inv(R)Ra<j>S<j>
	     = c<j>S<j> - c<B>inv(B)a<j>S<j>
	     = cbar<j>S<j>

  To unscale sc_cbar<j>, we simply multiply by 1/S<j>, keeping in mind that
  if x<j> is a logical for row i, the appropriate factor is R<i>.

  Parameters:
    nbcnt:	number of entries in cbar, nbvars
    cbar:	vector of reduced costs
    vndx:	corresponding variable indices

  Note that cbar and vndx are indexed from 0, and the indices specified in
  vndx are in the frame of the original constraint system, which is what we
  need for accesses to the scaling vectors.

  Returns: undefined
*/

{ int j,k ;
  double cbarj ;
  const double *rscale,*cscale ;

/*
  Is unscaling required? If so, acquire the vectors and go to it.
*/
  if (dy_isscaled() == FALSE) return ;
  dy_scaling_vectors(&rscale,&cscale) ;
/*
  Get on with the calculation.  Recall that vndx encodes the index of a
  logical as -i.
*/
  for (k = 0 ; k < nbcnt ; k++)
  { j = vndx[k] ;
    cbarj = cbar[k] ;
    if (j > 0)
    { cbarj /= cscale[j] ; }
    else
    { cbarj *= rscale[-j] ; }
    setcleanzero(cbarj,dy_tols->dfeas) ;
    cbar[k] = cbarj ; }

  return ; }



void dy_orig_soln (double *x, double *y)

/*
  This routine unscales the primal and dual variable values before returning
  them to the user. The necessary unscaling is as follows:

    primal architectural:	x<j>S<j>
    primal logical:		x<i>/R<i>

    dual:			y<i>R<i>

  These vectors are indexed by basis position, so reference frame is
  irrelevant.

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

