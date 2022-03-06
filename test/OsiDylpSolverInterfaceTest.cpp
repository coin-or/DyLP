/*! \legal
  Copyright (C) 2002, 2003, 2004.
  Lou Hafer, Stephen Tse, International Business Machines Corporation and
  others. All Rights Reserved.

  This file is a portion of the COIN/OSI interface for dylp and is licensed
  under the terms of the Eclipse Public License (EPL).
*/

#include "DylpConfig.h"

#include "CoinPragma.hpp"

/* 
  Rudimentary tests for the Dylp OSI layer. The assumption is that
  OsiSolverInterfaceCommonUnitTest checks basic functionality. The routine
  test_starts does a cursory check of cold/warm/hot starts.

  These tests need to be sharpened considerably, if they are to have any
  teeth.
*/

#include <iostream>
#include <iomanip>
#include "OsiDylpSolverInterface.hpp"
#include "OsiDylpWarmStartBasis.hpp"
#include "OsiDylpMessages.hpp"
#include "OsiUnitTests.hpp"

#include "CoinFloatEqual.hpp"

namespace {
  char sccsid[] UNUSED = "@(#)OsiDylpSolverInterfaceTest.cpp	1.11	09/25/04" ;
  char cvsid[] UNUSED = "$Id: OsiDylpSolverInterfaceTest.cpp 1224 2008-05-02 21:14:00Z lou $" ;
}


void test_starts (const std::string& mpsDir)

/*
  This routine makes a number of checks for warm and hot start capabilities.
    * Create and attempt to set an empty warm start object.
    * Create an ODSI object and solve the exmip1 sample MPS file with
      initialSolve.
    * Get a warm start object, then destroy the ODSI object. Create a new ODSI
      object, clone the saved warm start, install it, and resolve. Check that
      the objective is the same and that we did not pivot.
    * Change the objective sense and resolve from hot start.
*/

{ OsiDylpSolverInterface *osi = new OsiDylpSolverInterface ;
  OsiHintStrength strength ;
  bool sense ;
  void *p_info ;

  CoinRelFltEq eq ;

  double exmip1MinObj = 3.23684210526 ;
  double exmip1MaxObj = 4.5 ;
  std::streamsize old_prec = std::cout.precision() ;

  OSIUNITTEST_ASSERT_ERROR(osi != NULL, return, "dylp", "creating ODSI");
/*
  Read in exmip1 and solve it.
*/
  std::cout << "Boosting verbosity." << std::endl ;
  osi->setHintParam(OsiDoReducePrint,false) ;

  std::string exmpsfile = mpsDir+"exmip1" ;
  std::string probname ;
  std::cout << "Reading mps file \"" << exmpsfile << "\"" << std::endl ;
  OSIUNITTEST_ASSERT_ERROR(osi->readMps(exmpsfile.c_str(), "mps") == 0, return, "dylp", "reading exmip1");
  OSIUNITTEST_ASSERT_ERROR(osi->getStrParam(OsiProbName,probname), {}, "dylp", "get problem name");
  std::cout << "Solving " << probname << " ... " << std::endl ;
  osi->initialSolve() ;
  double val = osi->getObjValue() ;
  OSIUNITTEST_ASSERT_ERROR(eq(val,exmip1MinObj),
    std::cout
      << "Incorrect objective " << std::setprecision(12) << val
      << "; expecting " << exmip1MinObj
      << ", diff " << val-exmip1MinObj << "." << std::endl ;
      std::cout.precision(old_prec),
    "dylp", "initial solve of exmip1");
  std::cout << "And the answer is " << val << "." << std::endl;
/*
  Grab a warm start object for later use. No point in continuing if this
  fails.
*/
  std::cout << "Getting a warm start object ... " << std::endl ;
  CoinWarmStart *ws = osi->getWarmStart() ;
  OSIUNITTEST_ASSERT_ERROR(ws != NULL, return, "dylp", "acquire warm start");
/*
  Brief interruption for an idiot check: are the signs of the reduced costs
  correct in the solution, given minimisation? Easy to test with status info
  from the warm start object.
*/
  { const double *cbar = osi->getReducedCost() ;
    std::cout << "Performing sanity test on reduced costs." << std::endl ;
    const CoinWarmStartBasis *wsb = dynamic_cast<CoinWarmStartBasis *>(ws) ;
    bool signsok = true;
    for (int j = 0 ; j < osi->getNumCols() ; j++)
    { switch (wsb->getStructStatus(j))
      { case CoinWarmStartBasis::atUpperBound:
	{ if (cbar[j] > 0)
	  { std::cout
	      << "Sign error! " << "Var " << j
	      << " at upper bound, cbar = " << cbar[j] << "." << std::endl ;
	    signsok = false; }
	  break ; }
        case CoinWarmStartBasis::atLowerBound:
	{ if (cbar[j] < 0)
	  { std::cout
	      << "Sign error! " << "Var " << j
	      << " at lower bound, cbar = " << cbar[j] << "." << std::endl ;
	  signsok = false; }
	  break ; }
        case CoinWarmStartBasis::basic:
	{ if (fabs(cbar[j]) > 1.0e-5)
	  { std::cout
	      << "Value error! " << "Var " << j
	      << " is basic, cbar = " << cbar[j]
	      << ", should be zero" << "." << std::endl ;
	  signsok = false; }
	  break ; }
	default:
	{ break ; } } }
    OSIUNITTEST_ASSERT_ERROR(signsok == true, {}, "dylp", "reduced costs sanity check");
  }
/*
  Back to our regular programming. Create an empty warm start object and
  set it as the warm start. Then call resolve(). The call to setWarmStart
  should remove the warm start information in the solver, and the call to
  resolve() should throw.
*/
  { std::cout
      << "Checking behaviour for empty warm start object." << std::endl ;
    std::cout << "Acquiring ... " ;
    CoinWarmStart *emptyWS = osi->getEmptyWarmStart() ;
    OSIUNITTEST_ASSERT_ERROR(emptyWS != NULL, return, "dylp", "acquire empty warmstart");
    std::cout << "setting ... " ;
    OSIUNITTEST_ASSERT_ERROR(osi->setWarmStart(emptyWS) == true, return, "dylp", "install empty warmstart");
    std::cout << "calling resolve (throw expected) ... " ;
    bool throwSeen = false ;
    try
    { osi->resolve() ; }
    catch (CoinError &ce)
    { std::cout << std::endl << ce.methodName() << ":" << ce.message() ;
      throwSeen = true ; }
    if (throwSeen)
    { std::cout << std::endl << " caught ... success!" << std::endl ; }
    else
    { std::cout << " no throw! ... FAILURE!" ; }
    OSIUNITTEST_ASSERT_ERROR(throwSeen, {}, "dylp", "resolve from empty warmstart");
    delete emptyWS ; }
/*
  Make sure that the warm start information is sufficient (and persistent) by
  discarding the current ODSI object and then installing the warm start
  information in a new ODSI object.
*/
  std::cout << "Discarding current ODSI object ... " << std::endl ;
  delete osi ;
  osi = 0 ;
/*
  We've discarded the first solver. Clone the original warm start object and
  destroy the original.
*/
  std::cout << "Cloning warm start ... " << std::endl ;
  CoinWarmStart *ws_clone = ws->clone() ;
  OSIUNITTEST_ASSERT_ERROR(ws_clone, return, "dylp", "clone warmstart");
  delete ws ;
  ws = ws_clone ;
  ws_clone = 0 ;
/*
  Create a second solver, and read in exmip1.  Install the cloned warm start
  in the new solver.
*/
  int level = 5 ;
  level |= 0x10 ;
  std::cout << "Creating new ODSI object ... " << std::endl ;
  osi = new OsiDylpSolverInterface ;
  OSIUNITTEST_ASSERT_ERROR(osi != NULL, return, "dylp", "create second ODSI");

  osi->setHintParam(OsiDoReducePrint,false,OsiForceDo,&level) ;
  osi->getHintParam(OsiDoReducePrint,sense,strength,p_info) ;
  std::cout << "Verbosity now maxed at "
	    << *reinterpret_cast<int *>(p_info) << "." << std::endl ;

  OSIUNITTEST_ASSERT_ERROR(osi->readMps(exmpsfile.c_str(), "mps") == 0, return, "dylp", "reading exmip1");
  std::cout << "Installing cloned warm start object ... " << std::endl ;
  OSIUNITTEST_ASSERT_ERROR(osi->setWarmStart(ws) == true, return, "dylp", "install valid warmstart after deleting original solver");
/*
  Resolve. Check that we did not pivot (much) and that the objective hasn't
  changed. Set the print level quite high (we need to do this at some
  point).
*/
  std::cout << "Resolving the lp ... " << std::endl ;
  osi->resolve() ;
  val = osi->getObjValue() ;
  OSIUNITTEST_ASSERT_ERROR(eq(val,exmip1MinObj),
    std::cout
    << "Incorrect objective " << std::setprecision(12) << val
    << "; expecting " << exmip1MinObj
    << ", diff " << val-exmip1MinObj << "." << std::endl ;
    std::cout.precision(old_prec),
    "dylp", "resolve exmip1 from optimal basis");
  if (eq(val,exmip1MinObj))
    OSIUNITTEST_ASSERT_ERROR(osi->getIterationCount() <= 1, {}, "dylp", "resolve exmip1 from optimal basis");
  { std::cout
      << std::endl << "And the answer is " << val << " after "
      << osi->getIterationCount() << " pivots." << std::endl ; }
  delete ws ;
  ws = 0 ;
/*
  Flip the objective and do a hot start.
*/
  osi->setHintParam(OsiDoReducePrint,true,OsiForceDo) ;
  std::cout << "Reducing verbosity." << std::endl ;
  std::cout << "Changing objective sense to maximisation ..." ;
  osi->setObjSense(-1.0) ;
  std::cout << " attempting hot start ..." ;
  osi->markHotStart() ;
  osi->solveFromHotStart() ;
  val = osi->getObjValue() ;
  OSIUNITTEST_ASSERT_ERROR(eq(val,exmip1MaxObj),
    std::cout
    << "Incorrect objective " << std::setprecision(12) << val
    << "; expecting " << exmip1MaxObj
    << ", diff " << exmip1MaxObj-val << "." << std::endl ;
    std::cout.precision(old_prec),
    "dylp", "solve exmip1 with flipped obj from hotstart");
  if (eq(val,exmip1MaxObj))
  { std::cout
      << std::endl << "And the answer is " << val << "." << std::endl ; }
/*
  Another brief interruption for an idiot check: are the signs of the reduced
  costs correct in the solution, given maximisation?
*/
  { const double *cbar = osi->getReducedCost() ;
    ws = osi->getWarmStart() ;
    const CoinWarmStartBasis *odsi_wsb = 
    			dynamic_cast<CoinWarmStartBasis *>(ws) ;
    std::cout
      << "Performing sanity test on reduced costs." << std::endl ;
    bool signsok = true;
    for (int j = 0 ; j < osi->getNumCols() ; j++)
    { switch (odsi_wsb->getStructStatus(j))
      { case CoinWarmStartBasis::atUpperBound:
	{ if (cbar[j] < 0)
	  { std::cout
	      << "Sign error! " << "Var " << j
	      << " at upper bound, cbar = " << cbar[j] << "." << std::endl ;
	  signsok = false; }
	  break ; }
        case CoinWarmStartBasis::atLowerBound:
	{ if (cbar[j] > 0)
	  { std::cout
	      << "Sign error! " << "Var " << j
	      << " at lower bound, cbar = " << cbar[j] << "." << std::endl ;
	  signsok = false; }
	  break ; }
        case CoinWarmStartBasis::basic:
	{ if (fabs(cbar[j]) > 1.0e-5)
	  { std::cout
	      << "Value error! " << "Var " << j
	      << " is basic, cbar = " << cbar[j] << ", should be zero"
	      << "." << std::endl ;
	  signsok = false; }
	  break ; }
	default:
	{ break ; } } }
    delete ws ;
    OSIUNITTEST_ASSERT_ERROR(signsok, {}, "dylp", "signs in reduced costs for maximization");
    }
/*
  Turn off printing, to make sure we can get dylp to shut up.
*/
  level = 0 ;
  osi->setHintParam(OsiDoReducePrint,true,OsiForceDo,&level) ;
  osi->getHintParam(OsiDoReducePrint,sense,strength,p_info) ;
  std::cout << "Verbosity now at "
	    << *reinterpret_cast<int *>(p_info) << "." << std::endl ;
/*
  And return to minimisation.
*/
  std::cout << "And back to minimisation ..." ;
  osi->setObjSense(1.0) ;
  std::cout << " attempting hot start ..." ;
  osi->solveFromHotStart() ;
  val = osi->getObjValue() ;
  OSIUNITTEST_ASSERT_ERROR(eq(val,exmip1MinObj),
    std::cout
    << "Incorrect objective " << std::setprecision(12) << val
    << "; expecting " << exmip1MinObj
    << ", diff " << val-exmip1MinObj << "." << std::endl ;
    std::cout.precision(old_prec),
    "dylp", "solve exmip1 without flipped obj from hotstart");
  if (eq(val,exmip1MinObj))
  { std::cout
      << std::endl << "And the answer is " << val << "." << std::endl ; }

  delete osi ;
}


/*! OsiDylp unit test driver

  This is the unit test routine for OsiDylpSolverInterface. It tests for
  problems that have been uncovered and fixed already. If it fails, you've
  probably tickled a new bug. Please file a bug report.
*/

void OsiDylpSolverInterfaceUnitTest (const std::string &mpsDir,
				     const std::string &netLibDir)

{ std::cout
    << "  Starting OsiDylp specific tests ... "
    << std::endl << std::endl ;
  std::cout
    << "Test multi-language facility in message handler ... "
    << std::endl ;
  OsiDylpSolverInterface* osi = new OsiDylpSolverInterface ;
  osi->handler_->setLogLevel(3) ;
  osi->handler_->message(ODSI_TEST_MSG,osi->messages_) ;
  osi->newLanguage(CoinMessages::uk_en) ;
  osi->handler_->message(ODSI_TEST_MSG,osi->messages_) ;
  osi->handler_->finish() ;
/*
  Test the reset function.
*/
  std::cout << "Testing reset ... " << std::endl ;
  OsiDylpSolverInterface* osi2 = new OsiDylpSolverInterface ;
  osi->reset() ;
# ifndef _MSC_VER
  osi->assert_same(*osi,*osi2,true) ;
# endif
  delete osi ;
  delete osi2 ;
  std::cout << "Testing cold/warm/hot start ... " << std::endl ;
  test_starts(mpsDir) ;

  std::cout
    << std::endl << "  OsiDylp specific tests completed."
    << std::endl << std::endl ;
}
