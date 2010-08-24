/*
  Copyright (C) 2010 Lou Hafer

  This file is a portion of the COIN/OSI interface for dylp. It provides the
  methods that implement the OsiSimplex API.
*/

#ifdef _MSC_VER

/* Turn off compiler warning about long names */
# pragma warning(disable:4786)

#endif

/* Cut name lengths for readability. */

#define ODSI OsiDylpSolverInterface
#define ODWSB OsiDylpWarmStartBasis
#define CWSB CoinWarmStartBasis

#include <OsiDylpSolverInterface.hpp>
#include <OsiDylpWarmStartBasis.hpp>
#include <OsiDylpMessages.hpp>

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
  This method checks that the current method owns the solver and that the
  solver is prepared for tableau queries. If this isn't true, the method makes
  it true if at all possible.

  TODO: Really, what needs to happen here is to call dylp() with some
	combination of control options such that it does all the setup and
	returns just prior to entering the main pivot loop.
*/
bool ODSI::ensureOwnership ()
{ CoinMessageHandler *hdl = messageHandler() ;
/*
  We really should be in simplex mode 1 or 2.
*/
  if (simplex_state.simplex == 0)
    hdl->message(ODSI_NOTSIMPLEX,messages_) << CoinMessageEol ;
/*
  Is the solver already prepared and using the full system? If so, we're done.
*/
  ODSI *dylp_owner = static_cast<ODSI *>(dy_getOwner()) ;
  if (dylp_owner == this && lpprob->fullsys == true)
    return (true) ;
/*
  One or both of the above were not true. Call resolve to get dylp in the mood.
  Resolve will deal with the details of acquiring ownership, setting a warm
  start, and running dylp.
*/
  resolve() ;
/*
  Confirm that the result is as expected.
*/
  bool retval = false ;
  dylp_owner = static_cast<ODSI *>(dy_getOwner()) ;
  if (dylp_owner == this && flgon(lpprob->ctlopts,lpctlDYVALID))
  { retval = true ;
    if (lp_retval == lpOPTIMAL)
      hdl->message(ODSI_NOTOPTIMAL,messages_) << CoinMessageEol ;
    if (lpprob->fullsys == false)
      hdl->message(ODSI_NOTFULLSYS,messages_) << CoinMessageEol ; }

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

{ return (0) ; }

/*
  In order for the tableau methods to work, it must be true that this method
  owns the solver and instructed it to retain data structures after the last
  call to solve. It's more efficient if the solver is working with the full
  constraint system, but not necessary.

  We can't force any of this here, because this is a const method. As it turns
  out, it wouldn't be a good idea to do it here in any event. Consider the
  case where a client enables factorization, then clones the ODSI object and
  solves a related LP, then returns to make tableau inquiries. The client
  expects that the original solver object will be unaffected by the clone.
  The only way we can make that happen is to ensure the solver is ready at the
  head of each tableau query method.
  
  How do we do that? A bit of thought says `Might as well call resolve(),
  nothing less will do.' So our business here is to check if this object owns
  the solver, and, if it doesn't, make sure a call to resolve will succeed.
  That boils down to `Is there an active basis we can use for a warm start?'

  TODO: Now I've implemented setBasisStatus, I'm asking whether I can even
	check for a warm start. After all, setting a warm start is precisely
	the purpose of setBasisStatus.

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
  This object doesn't own the solver. Can we arrange it? The fundamental
  criteria is that we need an active basis. Arguably we should require that
  the basis is fresh, but resolve can cope with a damaged basis.
*/
  else
  { if (activeBasis.condition == ODSI::basisNone)
    { simplex_state.simplex = 0 ;
      throw CoinError("Cannot initialise solver; no active basis.",
    		      "enableFactorization","OsiDylpSolverInterface") ; } }
/*
  Do the bookkeeping.
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

  TODO: Likely I shouldn't be checking for ownership here. The proper test
	would be lp_retval == lpOPTIMAL and lpprob->ctlopts does not show
	any changes to the problem.

  Returns: true if an optimal basis is available, false otherwise.
*/
bool ODSI::basisIsAvailable () const
{ bool retval = false ;
/*
  Check that the conditions for availability of an optimal basis are
  satisfied. If so, check whether we're in fullsys mode and warn the user
  if it's otherwise.
*/
  ODSI *dylp_owner = static_cast<ODSI *>(dy_getOwner()) ;
  if (dylp_owner == this && flgon(lpprob->ctlopts,lpctlDYVALID) &&
      lp_retval == lpOPTIMAL)
  { retval = true ;
    CoinMessageHandler *hdl = messageHandler() ;
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

  const char *const archStatus = wsb->getStructuralStatus() ;
  const char *const logStatus = wsb->getArtificialStatus() ;

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
*/

void ODSI::getReducedGradient (double *cbar, double *y, const double *c)
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
    		    "getReducedGradient","OsiDylpSolverInterface") ; }
/*
  Set up to compensate for maximisation
*/
  bool maxSign = false ;
  if (getObjSense() < 0.0)
    maxSign = true ;
/*
  The solver is ready. Ask for the row duals, in the appropriate sign
  convention. (We need a negation somewhere. Asking for the true dual sign
  convention will do it.)
*/
  dy_rowDualsGivenC(lpprob,&y,c,maxSign) ;
/*
  Now calculate the reduced costs, again taking objective sense into account.
*/
  int n = getNumCols() ;
  int m = getNumRows() ;
  if (maxSign == true)
    std::transform(c,c+n,cbar,std::negate<double>()) ;
  else
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
  Calculate duals and reduced costs for a given objective function, but do
  change the values held in this ODSI. Really, all this amounts to is a call
  to getReducedGradient, plus the necessary bookkeeping to update cached
  values and tell dylp that the objective has changed, should we happen to
  call it.

  The approach here is not quite as efficient as it could be, but avoids
  duplicating the code of ODSI::setObjective. The alternative approach would
  write the duals and cbar directly to _row_price and _col_cbar,
  respectively, by passing them as parameters to getReducedGradient.  This
  would avoid the cost of tossing out the current vectors and allocating new
  space. The cost of reallocation seems a reasonable price for better code
  structure.
*/
void ODSI::setObjectiveAndRefresh (const double *c)
{
  int n = getNumCols() ;
  int m = getNumRows() ;
  double *cbar = new double[n] ;
  double *y = new double[m] ;
/*
  Do the calculation.  getReducedGradient compensates for objective sense, so
  don't fuss about that just yet.
*/
  getReducedGradient(cbar,y,c) ;
/*
  Write the new objective into the ODSI object, with all the necessary
  bookkeeping. Then restore the cached vectors.
*/
  setObjective(c) ;
  _col_cbar = cbar ;
  _row_price = y ;

  return ; }

