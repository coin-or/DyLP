/*
  Copyright (C) 2000, International Business Machines Corporation and others.
  All Rights Reserved.

  This code is licensed under the terms of the Eclipse Public License (EPL).
*/

#include "CoinPragma.hpp"

#include <iostream>

#include "OsiUnitTests.hpp"
#include "OsiDylpSolverInterface.hpp"

using namespace OsiUnitTest ;

//----------------------------------------------------------------
// to see parameter list, call unitTest -usage
//----------------------------------------------------------------

int main (int argc, const char *argv[])
{
/*
  Start off with various bits of initialisation that don't really belong
  anywhere else.

  First off, synchronise C++ stream i/o with C stdio. This makes debugging
  output a bit more comprehensible. It still suffers from interleave of cout
  (stdout) and cerr (stderr), but -nobuf deals with that.
*/
  std::ios::sync_with_stdio() ;
/*
  Suppress an popup window that Windows shows in response to a crash. See
  note at head of file.
*/
  WindowsErrorPopupBlocker();

  testingMessage("\n  Testing OsiDylpSolverInterface\n") ;
/*
  Process command line parameters.
*/
  std::map<std::string,std::string> parms ;
  if (processParameters(argc,argv,parms) == false)
    return 1;

  std::string mpsDir = parms["-mpsDir"] ;
  std::string netlibDir = parms["-netlibDir"] ;

  /*
    Test Osi{Row,Col}Cut routines.
   */
  {
    OsiDylpSolverInterface dylpSi ;
    testingMessage("Testing OsiRowCut ...\n") ;
    OSIUNITTEST_CATCH_ERROR(OsiRowCutUnitTest(&dylpSi,mpsDir),
    			    {},dylpSi,"rowcut unittest") ;
  }
  {
    OsiDylpSolverInterface dylpSi ;
    testingMessage("Testing OsiColCut ...\n") ;
    OSIUNITTEST_CATCH_ERROR(OsiColCutUnitTest(&dylpSi,mpsDir),
    			    {},dylpSi,"colcut unittest") ;
  }
  {
    OsiDylpSolverInterface dylpSi ;
    testingMessage("Testing OsiRowCutDebugger ...\n") ;
    OSIUNITTEST_CATCH_ERROR(OsiRowCutDebuggerUnitTest(&dylpSi,mpsDir),
    			    {},dylpSi,"rowcut debugger unittest") ;
  }
/*
  Run the OSI standard tests.
*/
  {
    OsiDylpSolverInterface dylpSi ;
    testingMessage("Running OSI common tests ...\n") ;
    OSIUNITTEST_CATCH_ERROR(
        OsiSolverInterfaceCommonUnitTest(&dylpSi,mpsDir,netlibDir),
	{},dylpSi,"common unittest") ;
  }
/*
  Run the OsiDylp class test, which exercises a few things specific to OsiDylp.
*/
  testingMessage("Running Dylp-specific tests ...\n") ;
  OSIUNITTEST_CATCH_ERROR(OsiDylpSolverInterfaceUnitTest(mpsDir,netlibDir),
  			  {}, "dylp", "OsiDylp unittest");
/*
  Check to see if we're asked to run the Netlib problems. The interface here
  is a historical artifact from the period when all OsiXXX lived within the
  Osi project.
*/
  if (parms.find("-testOsiSolverInterface") != parms.end())
  { std::vector<OsiSolverInterface*> vecSi(1,new OsiDylpSolverInterface) ;
    testingMessage("Testing OsiDylpSolverInterface on Netlib problems.\n") ;
    OSIUNITTEST_CATCH_ERROR(OsiSolverInterfaceMpsUnitTest(vecSi,netlibDir),
    			    {},"dylp","netlib unittest") ;
    delete vecSi[0] ; }
  else
  { testingMessage("Use -testOsiSolverInterface to run netlib problems.\n") ; }
/*
  We're done. Report on the results.
*/
  std::cout.flush() ;
  outcomes.print() ;

  int nerrors ;
  int nerrors_expected ;
  outcomes.getCountBySeverity(TestOutcome::ERROR,nerrors,nerrors_expected) ;

  if (nerrors > nerrors_expected)
    std::cerr
      << "Tests completed with " << nerrors - nerrors_expected
      << " unexpected errors." << std::endl ;
  else
    std::cerr << "All tests completed successfully." << std::endl ;

  return (nerrors-nerrors_expected) ;
}
