/*
  Copyright (C) 2010 Lou Hafer

  This file is a portion of the COIN/OSI interface for dylp and is licensed
  under the terms of the Eclipse Public License (EPL)

  This file provides the methods that implement the OsiSimplex Group 1 API
  (tableau access).
*/

#ifdef _MSC_VER

/* Turn off compiler warning about long names */
# pragma warning(disable:4786)

#endif

/* Cut name lengths for readability. */

#define ODSI OsiDylpSolverInterface
#define ODWSB OsiDylpWarmStartBasis
#define CWSB CoinWarmStartBasis

#include "OsiDylpSolverInterface.hpp"
#include "OsiDylpWarmStartBasis.hpp"
#include "OsiDylpMessages.hpp"

namespace {
  char svnid[] UNUSED = "$Id$" ;
}

/*! \file OsiDylpSimplex.cpp

  This file contains dylp's implementation of the OsiSimplex API.
*/

namespace {

/*
  Define constants for the integer status codes used by getBasisStatus and
  setBasisStatus so that we have them in one place.
*/
const int OsiSimplex_isFree = 0 ;
const int OsiSimplex_basic = 1 ;
const int OsiSimplex_nbub = 2 ;
const int OsiSimplex_nblb = 3 ;

}

/*
  Helper methods
*/

/*
  This method checks that the current ODSI object owns the solver and arranges
  for ownership if possible. This shouldn't be a problem in context: The
  client should not be using the simplex API without first solving an LP, and
  that should guarantee that lpprob and a warm start exist.
*/

bool ODSI::ensureOwnership () const
{ CoinMessageHandler *hdl = messageHandler() ;

  assert(lpprob && lpprob->consys) ;

/*
  We really should be in simplex mode 1 or 2, because we shouldn't be calling
  simplex API methods unless enableFactorisation or enableSimplexInterface
  has been called. And we really should be playing with a full deck, but
  that's less serious (and perhaps even reasonable, for one or two calls).
*/
  if (simplex_state.simplex == 0)
    hdl->message(ODSI_NOTSIMPLEX,messages_) << CoinMessageEol ;
  if (lpprob->fullsys == false)
    hdl->message(ODSI_NOTFULLSYS,messages_) << CoinMessageEol ;
/*
  Does this object already own the solver? If so, we're done.
*/
  ODSI *dylp_owner = static_cast<ODSI *>(dy_getOwner()) ;
  if (dylp_owner == this) return (true) ; 
/*
  If this ODSI object isn't the owner, install the active warm start in
  the lpprob object.

  It'd be possible to deal with a damaged basis by declaring activeBasis to be
  mutable. But really, is it a good idea?
*/
  if (basis_ready == false ||
      activeBasis.condition == ODSI::basisNone ||
      activeBasis.condition == ODSI::basisDamaged)
  { simplex_state.simplex = 0 ;
    hdl->message(ODSI_BADACTIVEBASIS,messages_)
        << "missing or damaged" << CoinMessageEol ;
    return (false) ; }
  dylp_owner->detach_dylp() ;
  OsiDylpWarmStartBasis *wsb =
      dynamic_cast<OsiDylpWarmStartBasis *>(activeBasis.basis) ;
  setBasisInLpprob(wsb,lpprob) ;
/*
  Initialise the solver.  After the call, dylp will be ready to pivot.

  I've argued with myself about forcing the full system here, but ultimately
  it's the user's choice. Except that other solvers simply don't offer this
  choice.
*/
  lptols_struct lcl_tols = *tolerances ;
  lpopts_struct lcl_opts = *resolveOptions ;
  lcl_opts.context = cxLOAD ;
  lcl_opts.forcewarm = true ;
  if (dyio_isactive(local_logchn)) dy_setlogchn(local_logchn) ;
  dy_setgtxecho(resolve_gtxecho) ;
  dy_checkdefaults(consys,&lcl_opts,&lcl_tols) ;

  lpret_enum lpret = dylp(lpprob,&lcl_opts,&lcl_tols,0) ;
/*
  Confirm that the result is as expected.
*/
  bool retval = false ;
  dylp_owner = static_cast<ODSI *>(dy_getOwner()) ;
  if (dylp_owner == this && flgon(lpprob->ctlopts,lpctlDYVALID))
  { retval = true ;
    if (lpret != lpOPTIMAL)
      hdl->message(ODSI_NOTOPTIMAL,messages_) << CoinMessageEol ; }

  return (retval) ; }



/*
  The methods required by the OsiSimplex API.
*/

/*
  Return ODSI's simplex capability. Defined codes are
    0: unimplemented
    1: tableau access methods only
    2: pivot control methods
*/
int ODSI::canDoSimplexInterface () const

{ return (1) ; }

/*
  In order for the tableau methods to work, it must be true that this ODSI
  object owns the solver and instructed it to retain data structures after
  the last call to solve. It's more efficient if the solver is working with
  the full constraint system, but not necessary.

  It's possible to force ownership here, but we can't guarantee it'll last.
  Consider the case where a client enables factorization, then clones
  the ODSI object and solves a related LP, then returns to make tableau
  inquiries. The client expects that the original solver object will be
  unaffected by the clone.  The only way we can make that happen is to
  ensure the solver is ready at the head of each tableau query method.

  Given the scenario above, it might seem plausible to check for an active
  basis here, so that we have something to use in a warm start. But the client
  could call setBasisStatus next, supplying the necessary warm start. So we
  can't insist that it already be present.

  About all we can do is warn if the most recent result of calling dylp failed
  to produce an optimal result, or wasn't playing with a full deck, and that's
  only useful if we already own the solver.

  Returns: undefined.
*/
void ODSI::enableFactorization() const
{ CoinMessageHandler *hdl = messageHandler() ;
/*
  Check if the solver is already in the correct state: this object owns it,
  and there are retained data structures. If that's true, check to see if
  we're optimal and playing with a full deck. These last two are optional but
  highly recommended.
*/
  ODSI *dylp_owner = static_cast<ODSI *>(dy_getOwner()) ;
  if (dylp_owner == this && flgon(lpprob->ctlopts,lpctlDYVALID))
  { if (lp_retval != lpOPTIMAL)
    { hdl->message(ODSI_NOTOPTIMAL,messages_) << CoinMessageEol ; }
    if (lpprob->fullsys != true)
    { hdl->message(ODSI_NOTFULLSYS,messages_) << CoinMessageEol ; } }
/*
  Do the bookkeeping. Take the attitude that if the client took the time to
  call enableFactorisation, they intend to do more than one or two queries,
  hence it's worthwhile to force the full system.
*/
  simplex_state.simplex = 1 ;
  simplex_state.saved_fullsys = resolveOptions->fullsys ;
  resolveOptions->fullsys = true ;

  return ; }

/*
  Undo enableFactorization. We really shouldn't be leaving
  simplex mode 1 if we weren't in it to begin with.

  Returns: undefined.
*/
void ODSI::disableFactorization() const
{ CoinMessageHandler *hdl = messageHandler() ;

  if (simplex_state.simplex != 1)
    hdl->message(ODSI_NOTSIMPLEX,messages_)
      << 1 << simplex_state.simplex << CoinMessageEol ;

  simplex_state.simplex = 0 ;
  resolveOptions->fullsys = simplex_state.saved_fullsys ;
  
  return ; }

/*
  Check if an optimal basis is available.

  The question of whether an optimal basis is available isn't really
  dependent on being in either simplex mode, and a survey of usage says
  that people will use it prior to calling enableFactorization.  Warn if
  we're not using the full system, just so the user is aware that their
  code is at best inefficient and possibly incorrect.

  Returns: true if an optimal basis is available, false otherwise.
*/
bool ODSI::basisIsAvailable () const
{ bool retval = false ;
/*
  Check that the conditions for availability of an optimal basis are
  satisfied. The last call to the solver should have produced a result of
  lpOPTIMAL, the solution should be fresh (indicating no changes to the
  constraint system), and either we own the solver or we can reload it.
*/
  if (lp_retval != lpOPTIMAL) return (retval) ;
  if (solnIsFresh == false) return (retval) ;

  ODSI *dylp_owner = static_cast<ODSI *>(dy_getOwner()) ;
  if (dylp_owner == this && flgon(lpprob->ctlopts,lpctlDYVALID))
  { retval = true ; }
  else
  if (activeBasis.basis != 0 &&
      (activeBasis.condition == ODSI::basisFresh ||
       activeBasis.condition == ODSI::basisModified))
  { retval = true ; }
/*
  If so, check whether we're in fullsys mode and warn the user
  if it's otherwise.
*/
  if (retval == true)
  { CoinMessageHandler *hdl = messageHandler() ;
    if (lpprob->fullsys == false)
      hdl->message(ODSI_NOTFULLSYS,messages_) << CoinMessageEol ; }

  return (retval) ; }

/*
  Return row and column status coded as integers.

  The strategy here is to ask for a warm start and do the format conversion.
  ODSI::getWarmStart will never consult dylp, so there's no need to ensure
  ownership of the solver here.
*/
void ODSI::getBasisStatus (int *archStatusInt, int *logStatusInt) const
{ ODWSB *wsb = 0 ;
/*
  Obtain a basis.
*/
  wsb = dynamic_cast<ODWSB *>(getWarmStart()) ;
/*
  And walk through it, converting the entries.
*/
  int n = wsb->getNumStructural() ;
  int m = wsb->getNumArtificial() ;

  const char *archStatus = wsb->getStructuralStatus() ;
  const char *logStatus = wsb->getArtificialStatus() ;

  for (int j = 0 ; j < n ; j++)
  { CWSB::Status statj ;
    statj = getStatus(archStatus,j) ;
    switch (statj)
    { case CWSB::isFree:
      { archStatusInt[j] = OsiSimplex_isFree ;
        break ; }
      case CWSB::basic:
      { archStatusInt[j] = OsiSimplex_basic ;
        break ; }
      case CWSB::atUpperBound:
      { archStatusInt[j] = OsiSimplex_nbub ;
        break ; }
      case CWSB::atLowerBound:
      { archStatusInt[j] = OsiSimplex_nblb ;
        break ; }
      default:
      { CoinMessageHandler *hdl = messageHandler() ;
        hdl->message(ODSI_CONFUSION,messages_) << __LINE__ << CoinMessageEol ;
	break ; } } }
/*
  A free logical is an error; let it fall into the default.
*/
  for (int i = 0 ; i < m ; i++)
  { CWSB::Status stati ;
    stati = getStatus(logStatus,i) ;
    switch (stati)
    { case CWSB::basic:
      { logStatusInt[i] = OsiSimplex_basic ;
        break ; }
      case CWSB::atUpperBound:
      { logStatusInt[i] = OsiSimplex_nbub ;
        break ; }
      case CWSB::atLowerBound:
      { logStatusInt[i] = OsiSimplex_nblb ;
        break ; }
      default:
      { CoinMessageHandler *hdl = messageHandler() ;
        hdl->message(ODSI_CONFUSION,messages_) << __LINE__ << CoinMessageEol ;
	break ; } } }

  delete wsb ;

  return ; }

/*
  Set a warm start and call resolve to update the basis factorisation and
  solution vectors.

  This does require a call to dylp, so we need to establish ownership. But
  since we're about to set a basis, we don't care. We can just call resolve
  and let it deal with the details.

  Setting the warm start is a local matter for this ODSI object.
*/
int ODSI::setBasisStatus (const int *archStatusInt, const int *logStatusInt)
{
/*
  Translate the status vectors into the appropriate format for a warm start.
  CWSB status vectors are packed four per byte.
*/
  int n = getNumCols() ;
  int m = getNumRows() ;

  char *archStatus = new char[(n+3)/4] ;
  char *logStatus = new char[(m+3)/4] ;

  for (int j = 0 ; j < n ; j++)
  { int statj ;
    statj = archStatusInt[j] ;
    switch (statj)
    { case OsiSimplex_isFree:
      { setStatus(archStatus,j,CWSB::isFree) ;
        break ; }
      case OsiSimplex_basic:
      { setStatus(archStatus,j,CWSB::basic) ;
        break ; }
      case OsiSimplex_nbub:
      { setStatus(archStatus,j,CWSB::atUpperBound) ;
        break ; }
      case OsiSimplex_nblb:
      { setStatus(archStatus,j,CWSB::atLowerBound) ;
        break ; }
      default:
      { CoinMessageHandler *hdl = messageHandler() ;
        hdl->message(ODSI_CONFUSION,messages_) << __LINE__ << CoinMessageEol ;
	break ; } } }
/*
  A free logical is an error; let it fall into the default.
*/
  for (int i = 0 ; i < m ; i++)
  { int stati ;
    stati = logStatusInt[i] ;
    switch (stati)
    { case OsiSimplex_basic:
      { setStatus(logStatus,i,CWSB::basic) ;
        break ; }
      case OsiSimplex_nbub:
      { setStatus(logStatus,i,CWSB::atUpperBound) ;
        break ; }
      case OsiSimplex_nblb:
      { setStatus(logStatus,i,CWSB::atLowerBound) ;
        break ; }
      default:
      { CoinMessageHandler *hdl = messageHandler() ;
        hdl->message(ODSI_CONFUSION,messages_) << __LINE__ << CoinMessageEol ;
	break ; } } }
/*
  Create a CWSB object and feed it to setWarmStart.
*/
  CoinWarmStartBasis wsb(n,m,archStatus,logStatus) ;
  delete[] archStatus ;
  delete[] logStatus ;
  bool result = setWarmStart(&wsb) ;
  if (result == false)
  { CoinMessageHandler *hdl = messageHandler() ;
    hdl->message(ODSI_CWSBREJECT,messages_) << CoinMessageEol ;
    return (1) ; }
/*
  And resolve.
*/
  resolve() ;
/*
  Confirm that the result is as expected.
*/
  int retval = 0 ;
  ODSI *dylp_owner = static_cast<ODSI *>(dy_getOwner()) ;
  if (dylp_owner == this && flgon(lpprob->ctlopts,lpctlDYVALID))
  { CoinMessageHandler *hdl = messageHandler() ;
    if (lp_retval == lpOPTIMAL)
      hdl->message(ODSI_NOTOPTIMAL,messages_) << CoinMessageEol ;
    if (lpprob->fullsys == false)
      hdl->message(ODSI_NOTFULLSYS,messages_) << CoinMessageEol ;
    retval = 1 ; }

  return (retval) ; }

/*
  Calculate duals and reduced costs for a given objective function, without
  changing the values held in this ODSI. We have to consult dylp to calculate
  y = c<B>inv(B) (the row duals), but there's no point in going there for
  the reduced costs --- it'd be a make-work project translating in and out
  of dylp's interior frame of reference.

  Note that the algebra here is independent of maximisation/minimisation. It's
  only when we get to interpretation of the results (`Are we optimal?') that
  maximisation or minimisation is relevant.
*/
void ODSI::getReducedGradient (double *cbar, double *y, const double *c) const
{
/*
  Make sure this object owns dylp and it's ready to work.
*/
  bool retval = ensureOwnership() ;
  if (retval == false)
  { CoinMessageHandler *hdl = messageHandler() ;
    hdl->message(ODSI_BADSTATE,messages_)
        << "tableau queries" << CoinMessageEol ;
    throw CoinError("Cannot query solver for row duals.",
    		    "getReducedGradient","OsiDylpSolverInterface",
		    "OsiDylpSimplex",__LINE__) ; }
/*
  Ask for the row duals. Because dylp uses 1-based indexing, we really need
  a vector of size m+1 for y. It's not safe to pass in the vector provided
  as a parameter; let dylp do the allocation. For the objective we have
  the same issue, but it's not written and we can safely fudge the address.
*/
  int n = getNumCols() ;
  int m = getNumRows() ;
  double *oneBased = 0 ;
  dy_rowDualsGivenC(lpprob,&oneBased,idx_vec(c),false) ;
  CoinCopyN(&oneBased[1],m,y) ;
  FREE(oneBased) ;
/*
  Now calculate the reduced costs.
*/
  CoinMemcpyN(c,n,cbar) ;
  pkvec_struct *ai = pkvec_new(n) ;
  for (int i = 0 ; i < m ; i++)
  { if (y[i] != 0)
    { consys_getrow_pk(consys,idx(i),&ai) ;
      for (int l = 0 ; l < ai->cnt ; l++)
      { int j = inv(ai->coeffs[l].ndx) ;
	cbar[j] -= y[i]*ai->coeffs[l].val ; } } }
  if (ai) pkvec_free(ai) ;
/*
  Groom the vector to eliminate tiny values and we're done.
*/
  for (int j = 0 ; j < n ; j++) setcleanzero(cbar[j],tolerances->cost) ;

  return ; }


/*
  Return the vector of basic variable indices. We don't have to ask dylp;
  this information is available in lpprob.basis.

  Dylp reports basis information only for the active constraints, so the
  strategy here is to initialise the vector supplied as a parameter to an
  all-logical basis. Then drop in the entries mentioned in lpprob.basis.

  Dylp reports the logical for constraint i as -i; translate to the standard
  coding of n+i. Don't forget to translate dylp's 1-based indices to Coin's
  0-based indices.
*/
void ODSI::getBasics (int *index) const
{ int n = getNumCols() ;
  int m = getNumRows() ;

/*
  Initialise index as if we have an all-logical basis, with all logicals in
  natural position.
*/
  CoinIotaN(index,m,n) ;
/*
  Now correct the entries mentioned in lpprob.basis.
*/
  int basislen = lpprob->basis->len ;
  basisel_struct *els = lpprob->basis->el ;
  for (int k = 1 ; k <= basislen ; k++)
  { int i = inv(els[k].cndx) ;
    int j = els[k].vndx ;
    /*
    std::cout
      << "  Dylp says x<" << els[k].vndx
      << "> basic for constraint " << els[k].cndx << "." << std::endl ;
    */
    if (j < 0)
    { j = n+(-j) ; }
    index[i] = inv(j) ; }

  return ; }

/*
  Ask dylp for a column of the basis inverse, beta<k> = inv(B)e<k>. (Note
  that this is not the same as asking for the column matching a particular
  basic variable x<j>; rather, we're asking for the column for the variable
  that's basic for constraint k.)
*/
void ODSI::getBInvCol (int col, double *betak) const
{ const bool verbose = false ;
/*
  Make sure this object owns dylp and it's ready to work.
*/
  bool retval = ensureOwnership() ;
  if (retval == false)
  { CoinMessageHandler *hdl = messageHandler() ;
    hdl->message(ODSI_BADSTATE,messages_)
        << "tableau queries" << CoinMessageEol ;
    throw CoinError("Cannot query solver for column of basis inverse.",
    		    "getBInvCol","OsiDylpSolverInterface",
		    "OsiDylpSimplex",__LINE__) ; }
/*
  Call dylp to retrieve beta<k> and report the result. Because dylp uses
  1-based indexing, we really need an array of size (m+1). It's not safe
  to pass in &betak[-1].
*/
  if (verbose)
    std::cout << "  Asking dylp for column " << idx(col) << "." << std::endl ;
  double *oneBased = 0 ;
  retval = dy_betak(lpprob,idx(col),&oneBased) ;
  if (retval == false)
  { CoinMessageHandler *hdl = messageHandler() ;
    hdl->message(ODSI_FAILEDCALL,messages_)
	<< "getBInvCol" << "dy_betak" << CoinMessageEol ;
    throw CoinError("Failed query to solver for column of basis inverse.",
    		    "getBInvCol","OsiDylpSolverInterface",
		    "OsiDylpSimplex",__LINE__) ; }
  int m = getNumRows() ;
  CoinCopyN(&oneBased[1],m,betak) ;
  FREE(oneBased) ;
  
  return ; }


/*
  Ask dylp for a row of the basis inverse, beta<i> = e<i>inv(B)
*/
void ODSI::getBInvRow (int row, double *betai) const
{ const bool verbose = false ;
/*
  Make sure this object owns dylp and it's ready to work.
*/
  bool retval = ensureOwnership() ;
  if (retval == false)
  { CoinMessageHandler *hdl = messageHandler() ;
    hdl->message(ODSI_BADSTATE,messages_)
        << "tableau queries" << CoinMessageEol ;
    throw CoinError("Cannot query solver for row of basis inverse.",
    		    "getBInvRow","OsiDylpSolverInterface",
		    "OsiDylpSimplex",__LINE__) ; }
/*
  Call dylp to retrieve beta<k> and report the result. Because dylp uses
  1-based indexing, we really need an array of size (m+1). It's not safe
  to pass in &betak[-1].
*/
  if (verbose)
    std::cout << "  Asking dylp for row " << idx(row) << "." << std::endl ;
  double *oneBased = 0 ;
  retval = dy_betai(lpprob,idx(row),&oneBased) ;
  if (retval == false)
  { CoinMessageHandler *hdl = messageHandler() ;
    hdl->message(ODSI_FAILEDCALL,messages_)
	<< "getBInvRow" << "dy_betai" << CoinMessageEol ;
    throw CoinError("Failed query to solver for row of basis inverse.",
    		    "getBInvRow","OsiDylpSolverInterface",
		    "OsiDylpSimplex",__LINE__) ; }
  int m = getNumRows() ;
  CoinCopyN(&oneBased[1],m,betai) ;
  FREE(oneBased) ;
  
  return ; }

/*
  Ask dylp for a column of the tableau, abar<j> = inv(B)a<j>.
*/
void ODSI::getBInvACol (int col, double *abarj) const
{ const bool verbose = false ;
/*
  Make sure this object owns dylp and it's ready to work.
*/
  bool retval = ensureOwnership() ;
  if (retval == false)
  { CoinMessageHandler *hdl = messageHandler() ;
    hdl->message(ODSI_BADSTATE,messages_)
        << "tableau queries" << CoinMessageEol ;
    throw CoinError("Cannot query solver for tableau column.",
    		    "getBInvACol","OsiDylpSolverInterface",
		    "OsiDylpSimplex",__LINE__) ; }
/*
  Call dylp to retrieve abar<j> and report the result. Because dylp uses
  1-based indexing, we really need an array of size (m+1). It's not safe
  to pass in &abarj[-1].
*/
  if (verbose)
    std::cout << "  Asking dylp for column " << idx(col) << "." << std::endl ;
  double *oneBased = 0 ;
  retval = dy_abarj(lpprob,idx(col),&oneBased) ;
  if (retval == false)
  { CoinMessageHandler *hdl = messageHandler() ;
    hdl->message(ODSI_FAILEDCALL,messages_)
	<< "getBInvACol" << "dy_abarj" << CoinMessageEol ;
    throw CoinError("Failed query to solver for tableau column.",
    		    "getBInvACol","OsiDylpSolverInterface",
		    "OsiDylpSimplex",__LINE__) ; }
  int m = getNumRows() ;
  CoinCopyN(&oneBased[1],m,abarj) ;
  FREE(oneBased) ;
  
  return ; }

/*
  Ask dylp for a row of the tableau, abar<i> = e<i>(inv(B)(B N I)). Osi wants
  this returned in two pieces: inv(B)(B N) and (optionally) inv(B)I. Clearly,
  the last component is simply e<i>inv(B) = beta<i>.
*/
void ODSI::getBInvARow (int row, double *abari, double *betai) const
{ const bool verbose = false ;
/*
  Make sure this object owns dylp and it's ready to work.
*/
  bool retval = ensureOwnership() ;
  if (retval == false)
  { CoinMessageHandler *hdl = messageHandler() ;
    hdl->message(ODSI_BADSTATE,messages_)
        << "tableau queries" << CoinMessageEol ;
    throw CoinError("Cannot query solver for tableau row.",
    		    "getBInvRow","OsiDylpSolverInterface",
		    "OsiDylpSimplex",__LINE__) ; }
/*
  Call dylp to retrieve abar<i> and beta<i> and report the result. Because
  dylp uses 1-based indexing, we need arrays of size (n+1). It's
  not safe to pass in &foo[-1], so let dylp allocate its own arrays.
*/
  if (verbose)
    std::cout << "  Asking dylp for row " << idx(row) << "." << std::endl ;
  double *oneBasedAbari = 0 ;
  double *oneBasedBetai = 0 ;
  retval = dy_abari(lpprob,idx(row),&oneBasedAbari,&oneBasedBetai) ;
  if (retval == false)
  { CoinMessageHandler *hdl = messageHandler() ;
    hdl->message(ODSI_FAILEDCALL,messages_)
	<< "getBInvARow" << "dy_abari" << CoinMessageEol ;
    throw CoinError("Failed query to solver for tableau row.",
    		    "getBInvARow","OsiDylpSolverInterface",
		    "OsiDylpSimplex",__LINE__) ; }
  int n = getNumCols() ;
  int m = getNumRows() ;
  CoinCopyN(&oneBasedAbari[1],n,abari) ;
  if (betai != 0)
  { CoinCopyN(&oneBasedBetai[1],m,betai) ; }
  FREE(oneBasedAbari) ;
  FREE(oneBasedBetai) ;
  
  return ; }

