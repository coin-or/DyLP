/*
  Copyright (C) 2000, International Business Machines Corporation and others.
  All Rights Reserved.

  This code is licensed under the terms of the Eclipse Public License (EPL).
*/

#include "CoinPragma.hpp"

#include "OsiConfig.h"

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <cstdio>
#include <iostream>
#include <map>

#include "OsiUnitTests.hpp"
#include "OsiCuts.hpp"
#include "OsiRowCutDebugger.hpp"

#include "CoinError.hpp"
#include "CoinHelperFunctions.hpp"

#include "OsiDylpSolverInterface.hpp"

using namespace OsiUnitTest ;

namespace {

/*
  Print the help text.
*/
void printHelp (std::string myName)

{ std::cout
    << "Usage: " << myName
    << " [-nobuf] [-mpsDir=V1] [-netlibDir=V2]"
    << " [-testOsiSolverInterface] " << std::endl ;
  std::cout << "  where:" << std::endl ;
  std::cout
    << "    "
    << "-mpsDir: directory containing mps test files." << std::endl
    << "\t" << "Default value V1=\"../../Data/Sample\"" << std::endl ;
  std::cout
    << "    "
    << "-netlibDir: directory containing netlib files." << std::endl
    << "\t" << "Default value V2=\"../../Data/Netlib\"" << std::endl ;
  std::cout
    << "    "
    << "-testOsiSolverInterface: "
    << "run each OSI on the netlib problem set." << std::endl
    << "\t"
    << "Default is to not run the netlib problem set." << std::endl ;
  std::cout
    << "    "
    << "-nobuf: use unbuffered output." << std::endl
    << "\t" << "Default is buffered output." << std::endl ;
  std::cout
    << "    "
    << "-cerr2cout: redirect cerr to cout." << std::endl
    << "\t" << "Default is separate cout & cerr." << std::endl ;
  std::cout
    << "  "
    << "Parameters that take a value need the `='; "
    << "no whitespace, please." << std::endl ;
  std::cout << std::endl ;

  return ; }


/*
  Utility routine to process command line parameters. An unrecognised parameter
  will trigger the help message and a return value of false.
  
  This should be replaced with the one of the standard CoinUtils parameter
  mechanisms.
*/
bool processParameters (int argc, const char **argv,
			std::map<std::string,std::string> &parms)

{
  assert(argc >= 1);
  assert(argv != NULL);
/*
  Initialise the parameter keywords.
*/
  std::set<std::string> definedKeyWords ;
  definedKeyWords.insert("cerr2cout") ;
  definedKeyWords.insert("mpsDir") ;
  definedKeyWords.insert("netlibDir") ;
  definedKeyWords.insert("testOsiSolverInterface") ;
  definedKeyWords.insert("nobuf") ;
  definedKeyWords.insert("help") ;
/*
  Set default values for data directories. These match the typical `classic
  external' coin packaging.
*/
  const char dirsep =  CoinFindDirSeparator() ;
  std::string pathTmp ;

  pathTmp = ".." ;
  pathTmp += dirsep ;
  pathTmp += ".." ;
  pathTmp += dirsep ;
  pathTmp += "Data" ;
  pathTmp += dirsep ;
/*
  Adjust for Windows in classic package.
*/
  if (dirsep == '\\')
    pathTmp = "..\\..\\" + pathTmp ;

  parms["mpsDir"] = pathTmp + "Sample"  ;
  parms["netlibDir"] = pathTmp + "Netlib" ;

/*
  Read the command line parameters and fill a map of parameter keys and
  associated data. The parser allows for parameters which are only a keyword,
  or parameters of the form keyword=value (no spaces).
*/
  for (int i = 1 ; i < argc ; i++)
  { std::string parm(argv[i]) ;
    std::string key,value ;
    std::string::size_type eqPos = parm.find('=');

    if (eqPos == std::string::npos)
    { key = parm ; }
    else
    { key = parm.substr(0,eqPos) ;
      value = parm.substr(eqPos+1) ; }
/*
  Is the specifed key valid?  We'll allow for `--', `-', or nothing at all,
  as long as the keyword is valid. Anything starting with `h' is interpreted
  as `help'. Print the help message end return.
*/
    if (key.find("--") == 0)
    { key = key.substr(2) ; }
    else
    if (key.find('-') == 0)
    { key = key.substr(1) ; }
    if (key.find('h') == 0 || key.find('?') == 0)
    { printHelp(argv[0]) ;
      return (false) ; }
    if (definedKeyWords.find(key) == definedKeyWords.end())
    { std::cerr << "Undefined parameter \"" << key << "\"." << std::endl ;
      printHelp(argv[0]) ;
      return (false) ; }
/*
  Valid keyword; stash the value for later reference.
*/
    parms[key]=value ; }
/*
  Tack the directory separator onto the data directories so we don't have to
  worry about it later.
*/
  parms["mpsDir"] += dirsep ;
  parms["netlibDir"] += dirsep ;
/*
  Did the user request unbuffered i/o?
*/
  if (parms.find("nobuf") != parms.end())
  { setbuf(stderr,0) ;
    setbuf(stdout,0) ; }
/*
  Did the user request a redirect for cerr? This must occur before any i/o is
  performed.
*/
  if (parms.find("cerr2cout") != parms.end())
  { std::cerr.rdbuf(std::cout.rdbuf()) ; }

  return (true) ; }


/*
  The worker routine that actually runs the tests. Pulled out of main
  to make the surrounding try-catch more obvious there.
*/
int runTests (std::map<std::string,std::string> parms)

{ int errCnt = 0 ;
  int totalErrCnt = 0 ;

  std::string mpsDir = parms["mpsDir"] ;
  std::string netlibDir = parms["netlibDir"] ;
/*
  Test Osi{Row,Col}Cut routines.
*/
  std::cout << std::endl ;
  { OsiDylpSolverInterface dylpSi ;
    testingMessage("Testing OsiRowCut ...\n") ;
    OsiRowCutUnitTest(&dylpSi,mpsDir) ; }
  { OsiDylpSolverInterface dylpSi ;
    testingMessage("Testing OsiColCut ...\n") ;
    OsiColCutUnitTest(&dylpSi,mpsDir) ; }
  { OsiDylpSolverInterface dylpSi ;
    testingMessage("Testing OsiRowCutDebugger ...\n") ;
    OsiRowCutDebuggerUnitTest(&dylpSi,mpsDir) ; }
/*
  Run the OSI standard tests.
*/
  { std::cout
      << std::endl
      << "  Running OSI standard tests ... "
      << std::endl << std::endl ;
    OsiDylpSolverInterface dylpSi ;
    errCnt = OsiSolverInterfaceCommonUnitTest(&dylpSi,mpsDir,netlibDir) ;
    std::cout
	<< std::endl
	<< "  OSI standard tests completed, " << errCnt << " errors."
	<< std::endl ;
    totalErrCnt += errCnt ; }
/*
  Run the OsiDylp class test, which exercises a few things specific to
  OsiDylp.
*/
  totalErrCnt += OsiDylpSolverInterfaceUnitTest(mpsDir,netlibDir);
/*
  Check to see if we're asked to run the Netlib problems. The interface here
  is a historical artifact from the period when all OsiXXX lived within the
  Osi project.
*/
  if (parms.find("testOsiSolverInterface") != parms.end())
  { std::vector<OsiSolverInterface*> vecSi(1,new OsiDylpSolverInterface) ;
    testingMessage("Testing OsiDylpSolverInterface on Netlib problems.\n") ;
    errCnt = OsiSolverInterfaceMpsUnitTest(vecSi,netlibDir) ;
    if (errCnt != 0)
    { std::cerr
	<< "OsiDylp unitTest: " << errCnt << " errors after Netlib tests."
	<< std::endl ;
      totalErrCnt += errCnt ; }
    delete vecSi[0] ; }

  return (totalErrCnt) ; }

}	// end file-local namespace



/*
  unitTest [-nobuf] [-mpsDir=V1] [-netlibDir=V2] [-testOsiSolverInterface]

  where:
    -nobuf: remove buffering on cout (stdout); useful to keep cout and cerr
        messages synchronised when redirecting output to a file or pipe.
    -mpsDir: directory containing mps test files
        Default value V1="../../Data/Sample"    
    -netlibDir: directory containing netlib files
        Default value V2="../../Data/Netlib"
    -testOsiSolverInterface
        If specified, then OsiSolveInterface::unitTest
        is skipped over and not run.

  All parameters are optional. The `=' is required. No whitespace, please.
*/

int main (int argc, const char *argv[])

{ int totalErrCnt = 0 ;

  testingMessage("\n  Testing OsiDylpSolverInterface\n") ;

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

/*
  Process command line parameters.
*/
  std::map<std::string,std::string> parms ;

  if (processParameters(argc,argv,parms) == false) return (1) ;
/*
  Wrap the entire test sequence in a try-catch block to avoid a crash due to
  uncaught exceptions.
*/
  try
  { totalErrCnt += runTests(parms) ; }
  catch (CoinError& error)
  { std::cout.flush() ;
    std::cerr << "Caught CoinError exception: " ;
    error.print(true) ;
    totalErrCnt += 1 ; }
  catch (...)
  { std::cout.flush() ;
    std::cerr
      << "Unknown exception caught in main method. This is not good."
      << std::endl ;
    totalErrCnt += 1 ; }
/*
  We're done. Report on the results.
*/
  if (totalErrCnt)
  { std::cout.flush() ;
    std::cerr
      << "Tests completed with " << totalErrCnt << " errors." << std::endl ; 
  } else
  { testingMessage("All tests completed successfully\n") ; }

  return (totalErrCnt) ; }

