#ifndef OsiDylpSolverInterface_H
#define OsiDylpSolverInterface_H

/*
  Copyright (C) 2002, 2003, 2004.
  Lou Hafer, Stephen Tse, International Business Machines Corporation and
  others. All Rights Reserved.
  Copyright (C) 2005 -- 2010 Lou Hafer

  This file is a portion of the COIN/OSI interface for dylp and is licensed
  under the terms of the Eclipse Public License (EPL)
*/

/*! \file OsiDylpSolverInterface.hpp 
    \brief Declarations of the COIN OSI API for the dylp solver.

  This file contains the declaration of the class OsiDylpSolverInterface
  (ODSI), an implementation of the COIN OSI API for the dylp LP solver.
  The documentation here most often provides only brief descriptions of
  methods. See the OsiSolverInterface documentation for additional details.
*/

/*
  sccs: @(#)OsiDylpSolverInterface.hpp	1.12	09/16/04
  cvs: $Id: OsiDylpSolverInterface.hpp 1312 2008-10-10 00:26:32Z lou $
*/

#include "OsiConfig.h"
#include <CoinPackedMatrix.hpp>
#include <OsiSolverInterface.hpp>
#include <CoinWarmStart.hpp>
#include <CoinMessageHandler.hpp>
#include <CoinMpsIO.hpp>
#include <CoinPresolveMatrix.hpp>

#include "OsiDylpConfig.h"

#define DYLP_INTERNAL
extern "C" {
#include "dylp.h"
}

class OsiDylpWarmStartBasis ;

/*! \brief Enum to specify cold/warm/hot start */

typedef enum { startInvalid = 0,
	       startCold = 1, startWarm, startHot } ODSI_start_enum ;


/*! \class OsiDylpSolverInterface
    \brief COIN OSI API for dylp

  The class OsiDylpSolverInterface (ODSI) implements the public functions
  defined for the COIN OsiSolverInterface (OSI) API.

  <h3>OsiDylpSolverInterface Principles for Users</h3>

  In addition to the principles outlined for the OsiSolverInterface class,
  ODSI maintains the following:

  <strong>Construction of a Constraint System</strong>:
  A constraint system can be batch loaded from a file (MPS format) or from
  a data structure, or it can be built incrementally. When building a
  constraint system incrementally, keep in mind that you must create a row
  or column (addRow or addCol, respectively) before you can adjust other
  properties (row or column bounds, objective, variable values, <i>etc</i>.)

  <strong>Existence of a Solution</strong>:
  For proper operation, OSI requires that a SI maintain a basic primal solution
  at all times after a problem has been loaded.

  <p>
  When a problem is loaded, ODSI generates a basic primal solution (primal
  variable values and a matching basis).
  The solution is not necessarily primal or dual feasible.
  In terms of the objective function, this solution is pessimistic, but
  not necessarily worst-case.
  ODSI does not generate matching values for the dual variables (row prices).

  <p>
  Any successful call to dylp (<i>i.e.</i>, a call that results in an optimal,
  infeasible, or unbounded result, or that terminates on iteration limit) will
  replace the existing solution with the result of the call to dylp.

  <p>
  It is possible to specify initial values for the primal and dual variables
  using setColSolution() and setRowPrice(). To specify an initial basis,
  see the documentation for the CoinWarmStartBasis and OsiDylpWarmStartBasis
  classes. When these functions are used, it is the responsibility of the
  client to ensure validity and consistency.

  <strong>Maintenance of an LP Basis</strong>
  Skirting the edges of the principle that changing the problem invalidates
  the solution, OsiDylp will maintain a valid basis across two common
  operations used in branch-and-cut: deletion of a loose constraint and
  deletion of a nonbasic variable. Arguably the set of allowable
  modifications could be increased.

  <strong>Assignment</strong>
  Assignment (#operator=()) works pretty much as you'd expect, with one
  exception. Only one ODSI object can control the dylp solver at a time,
  so hot start information is not copied on assignment.

  Detailed implementation comments are contained in OsiDylpSolverInterface.cpp,
  which is not normally scanned when generating COIN OSI API documentation.
*/

class OSIDYLPLIB_EXPORT
OsiDylpSolverInterface: virtual public OsiSolverInterface

{ friend void OsiDylpSolverInterfaceUnitTest(const std::string &mpsDir,
					     const std::string &netLibDir) ;

/*
  Consult the COIN OSI documentation or relevant source code for details
  not covered here. Supported functions are listed first, followed by
  unsupported functions.
*/

public:

/*! \name Constructors and Destructors */
//@{

  /*! \brief Default constructor */

  OsiDylpSolverInterface() ;

  /*! \brief Copy constructor */

  OsiDylpSolverInterface(const OsiDylpSolverInterface &src) ;

  /*! \brief Clone the solver object */

  OsiSolverInterface* clone(bool copyData = true) const ;

  /*! \brief Assignment */

  OsiDylpSolverInterface &operator=(const OsiDylpSolverInterface &rhs) ;

  /*! \brief Destructor */

  ~OsiDylpSolverInterface() ;

  /*! \brief Reset the solver object to the state produced by the default
	     constructor.
  */

  void reset() ;

//@}

/*! \name Methods to load a problem */
//@{

  /*! \brief Read a problem description in MPS format from a file.  */

  int readMps(const char *filename, const char *extension = "mps") ;

  /*! \brief Read a problem description in MPS format from a file, including
	     SOS information.
  */

  int readMps(const char *filename, const char *extension,
	      int &numberSets, CoinSet **&sets) ;

  /*! \brief Write the problem into the specified file in MPS format.
  
    \p objsense == 1 forces the file to be written as a maximisation problem,
    while -1 forces a minimisation problem. The default of 0 writes the file
    as maximisation or minimisation using the solver's current setting.
  */

  void writeMps(const char *basename,
		const char *extension = "mps",
		double objsense = 0.0) const ;

  /*! \brief Load a problem description (OSI packed matrix, row sense,
	  parameters unaffected).
  */
  void loadProblem(const CoinPackedMatrix &matrix,
		   const double *collb, const double *colub, const double *obj,
		   const char *rowsen, const double *rowrhs,
		   const double *rowrng) ;

  /*! \brief Load a problem description (OSI packed matrix, row bounds,
	  parameters unaffected).
  */
  void loadProblem(const CoinPackedMatrix &matrix,
		   const double *collb, const double *colub, const double *obj,
		   const double *rowlb, const double *rowub) ;
  
  /*! \brief Load a problem description (standard column-major packed
	  matrix, row sense, parameters unaffected)
  */
  void loadProblem(const int colcnt, const int rowcnt,
	       const int *start, const int *index, const double *value,
	       const double *collb, const double *colub, const double *obj,
	       const char *sense, const double *rhsin, const double *range) ;

  /*! \brief Load a problem description (standard column-major packed
	   matrix, row bounds, parameters unaffected)
  */
  void loadProblem(const int colcnt, const int rowcnt,
		   const int *start, const int *index, const double *value,
		   const double *collb, const double *colub, const double *obj,
		   const double *row_lower, const double *row_upper) ;

  /*! \brief Load a problem description (OSI packed matrix, row sense,
	  parameters destroyed).
  */
  void assignProblem(CoinPackedMatrix *&matrix,
		     double *&collb, double *&colub, double *&obj, 
		     char *&rowsen, double *&rowrhs, double *&rowrng) ;

  /*! \brief Load a problem description (OSI packed matrix, row bounds,
	  parameters destroyed).
  */
  void assignProblem(CoinPackedMatrix *&matrix,
		     double *&collb, double *&colub, double *&obj,
		     double *&rowlb, double *&rowub) ;

//@}

/*! \name Methods to obtain problem information */

//@{

  /*! \brief Get the number of columns (variables) */

  int getNumCols() const ;

  /*! \brief Get the number of rows (constraints) */

  int getNumRows() const ;

  /*! \brief Get the number of non-zero coefficients */

  int getNumElements() const ;

  /*! \brief Get the number of integer variables

    Counts both binary and general integer variables.
  */

  int getNumIntegers() const ;

  /*! \brief Get the column (variable) lower bound vector */

  const double* getColLower() const ;

  /*! \brief Get the column (variable) upper bound vector */

  const double* getColUpper() const ;

  /*! \brief Return true if the variable is continuous */

  bool isContinuous(int colIndex) const ;

  /*! \brief Return true if the variable is binary */

  bool isBinary(int colIndex) const ;

  /*! \brief Return true if the variable is general integer */

  bool isIntegerNonBinary (int colIndex) const ;

  /*! \brief Return true if the variable is integer (general or binary) */

  bool isInteger (int colIndex) const ;

  /*! \brief Get the row sense (constraint type) vector */

  const char* getRowSense() const ;

  /*! \brief Get the row (constraint) right-hand-side vector */

  const double* getRightHandSide() const ;

  /*! \brief Get the row (constraint) range vector */

  const double* getRowRange() const ;

  /*! \brief Get the row (constraint) lower bound vector */

  const double* getRowLower() const ;

  /*! \brief Get the row (constraint) upper bound vector */

  const double* getRowUpper() const ;

  /*! \brief Get the objective function coefficient vector */

  const double* getObjCoefficients() const ;

  /*! \brief Get the objective function sense (min/max)
  
    A value of 1 indicates minimisation; -1 indicates maximisation.
  */

  double getObjSense() const ;

  /*! \brief Get a pointer to a row-major copy of the constraint matrix */

  const CoinPackedMatrix *getMatrixByRow() const ;

  /*! \brief Get a pointer to a column-major copy of the constraint matrix */

  const CoinPackedMatrix *getMatrixByCol() const ;
//@}

/*! \name Methods for row and column names.

  Only the set methods need to be overridden to ensure consistent names
  between OsiDylp and the OSI base class.
*/
//@{

  /*! \brief Set the objective function name */

  void setObjName (std::string name) ;

  /*! \brief Set a row name

    Quietly does nothing if the name discipline (#OsiNameDiscipline) is
    auto. Quietly fails if the row index is invalid.
  */
  void setRowName(int ndx, std::string name) ;

  /*! \brief Set a column name

    Quietly does nothing if the name discipline (#OsiNameDiscipline) is
    auto. Quietly fails if the column index is invalid.
  */
  void setColName(int ndx, std::string name) ;

//@}

/*! \name Methods to modify the problem */
//@{

  /*! \brief Set a single variable to be continuous. */

  void setContinuous(int index) ;
  using OsiSolverInterface::setContinuous ;

  /*! \brief Set a single variable to be integer. */

  void setInteger(int index) ;
  using OsiSolverInterface::setInteger ;

  /*! \brief Set the lower bound on a column (variable) */

  void setColLower(int index, double value) ;
  using OsiSolverInterface::setColLower ;

  /*! \brief Set the upper bound on a column (variable) */

  void setColUpper(int index, double value) ;
  using OsiSolverInterface::setColUpper ;

  /*! \brief Set the lower bound on a row (constraint) */

  void setRowLower(int index, double value) ;

  /*! \brief Set the upper bound on a row (constraint) */

  void setRowUpper(int index, double value) ;

  /*! \brief Set the type of a row (constraint) */

  void setRowType(int index, char rowsen, double rowrhs, double rowrng) ;

  /*! \brief Set an objective function coefficient */

  void setObjCoeff (int index, double value) ;

  /*! Set the objective coefficients for all columns.  */

  void setObjective(const double * array);

  /*! \brief Set the sense (min/max) of the objective
  
    Use 1 for minimisation, -1 for maximisation. (The default is minimisation;
    the objective is multiplied by -1 to maximise.)
  */

  void setObjSense(double sense) ;

  /*! \brief Set the value of the primal variables in the problem solution */

  void setColSolution(const double *colsol) ;

  /*! \brief Set the value of the dual variables in the problem solution */

  void setRowPrice(const double*) ;

  /* For overload resolution with OSI::addCol functions. */

  using OsiSolverInterface::addCol ;

  /*! \brief Add a column (variable) to the problem */

  void addCol(const CoinPackedVectorBase &vec,
	      const double collb, const double colub, const double obj) ;

  /*! \brief Remove column(s) (variable(s)) from the problem */

  void deleteCols(const int num, const int *colIndices) ;

  /* For overload resolution with OSI::addRow functions. */

  using OsiSolverInterface::addRow ;

  /*! \brief Add a row (constraint) to the problem */

  void addRow(const CoinPackedVectorBase &row,
	      const double rowlb, const double rowub) ;

  /*! \brief Add a row (constraint) to the problem */

  void addRow(const CoinPackedVectorBase &row,
	      const char rowsen, const double rowrhs, const double rowrng) ;

  /*! \brief Delete row(s) (constraint(s)) from the problem */

  void deleteRows(const int num, const int *rowIndices) ;

  /*! \brief Apply a row (constraint) cut (add one constraint) */

  void applyRowCut(const OsiRowCut &cut) ;

  /*! \brief Apply a column (variable) cut (adjust one or more bounds) */

  void applyColCut(const OsiColCut &cut) ;
//@}

/*! \name Solve methods */
//@{

  /*! \brief Solve an lp from scratch */

  void initialSolve() ;

  /*! \brief Get an empty OsiDylpWarmStartBasis object */

  CoinWarmStart *getEmptyWarmStart () const ;

  /*! \brief Build a warm start object for the current lp solution. */

  CoinWarmStart *getWarmStart() const ;

  /*! \brief Apply a warm start object.
  
    By definition, a null parameter is a request to synch the warm start basis
    with the solver. ODSI interprets a 0x0 basis as a request to remove warm
    start information.
  */

  bool setWarmStart(const CoinWarmStart *warmStart) ;

  /*! \brief Call dylp to reoptimize (warm start). */

  void resolve() ;

  /*! \brief Create a hot start snapshot. */

  void markHotStart() ;

  /*! \brief Call dylp to reoptimize (hot start). */

  void solveFromHotStart() ;

  /*! \brief Delete the hot start snapshot. */

  void unmarkHotStart() ;

//@}

/*! \name Methods returning solver termination status */
//@{

  /*! \brief True if dylp abandoned the problem */

  bool isAbandoned() const ;

  /*! \brief True if dylp reported an optimal solution */

  bool isProvenOptimal() const ;

  /*! \brief True if dylp reported the problem to be primal infeasible */

  bool isProvenPrimalInfeasible() const ;

  /*! \brief True if dylp reported the problem to be dual infeasible (primal
	     unbounded)
  */
  bool isProvenDualInfeasible() const ;

  /*! \brief True if dylp reached the iteration limit */

  bool isIterationLimitReached() const ;

  /*! \brief Get the number of iterations for the last lp */

  int getIterationCount() const ;

  /*! \brief Is the primal objective limit reached?
  
    Put in different terms, quit when the objective value becomes better
    than the given limit for an acceptable value.
  */
  bool isPrimalObjectiveLimitReached() const ;

  /*! \brief Is the dual objective limit reached?
  
    Put in different terms, quit when the objective value becomes worse
    than the given limit for an acceptable value.
  */
  bool isDualObjectiveLimitReached() const ;
//@}


/*! \name Methods to set/get solver parameters */
//@{

  /*! \brief Get dylp's value for infinity */

  double getInfinity() const ;

  /*! \brief Set an OSI integer parameter */

  bool setIntParam(OsiIntParam key, int value) ;

  /*! \brief Set an OSI double parameter */

  bool setDblParam(OsiDblParam key, double value) ;

  /*! \brief Set an OSI string parameter */

  bool setStrParam(OsiStrParam key, const std::string& value) ;

  /*! \brief Set an OSI hint */

  bool setHintParam(OsiHintParam key, bool sense = true,
		    OsiHintStrength strength = OsiHintTry, void *info = 0) ;

  /*! \brief Get an OSI integer parameter */

  bool getIntParam(OsiIntParam key, int &value) const ;

  /*! \brief Get an OSI double parameter */

  bool getDblParam(OsiDblParam key, double &value) const ;

  /*! \brief Get an OSI string parameter */

  bool getStrParam(OsiStrParam key, std::string &value) const ;

  /* For overload resolution with OSI::getHintParam functions. */

  using OsiSolverInterface::getHintParam ;

  /*! \brief Get an OSI hint */

  bool getHintParam(OsiHintParam key, bool &sense,
		    OsiHintStrength &strength, void *&info) const ;

  /*! \brief Change the language for OsiDylp messages */

  void newLanguage(CoinMessages::Language language)
  { setOsiDylpMessages(language) ; }

  /*! \brief An alias for OsiDylpSolverInterface::newLanguage. */

  void setLanguage(CoinMessages::Language language)
  { setOsiDylpMessages(language) ; }

//@}

/*! \name Methods to obtain solution information */
//@{

  /*! \brief Get the objective function value for the solution */

  double getObjValue() const ;

  /*! \brief Return the vector of primal variables for the solution */

  const double* getColSolution() const ; 

  /*! \brief Return the vector of dual variables for the solution */

  const double* getRowPrice() const ;

  /*! \brief Return the vector of reduced costs for the solution */

  const double* getReducedCost() const ;

  /*! \brief Return the vector of row activity for the solution */

  const double* getRowActivity() const ;

  /*! \brief Get as many dual rays as the solver can provide
  
    If \c fullRay is false (the default), the ray will contain only the
    components associated with the row duals. If \c fullRay is set to \c true,
    the ray will also contain the components associated with nonbasic
    variables.
  */

  std::vector<double *> getDualRays(int maxNumRays, bool fullRay) const ;

  /*! \brief Get as many primal rays as the solver can provide */

  std::vector<double *> getPrimalRays(int maxNumRays) const ;

//@}

/*! \name Simplex API methods */
//@{

  /*! \brief Return the simplex implementation level.  */
  int canDoSimplexInterface() const ;

  /*! \brief Prepare the solver for the use of tableau access methods.

    In order for the tableau methods to work, the ODSI object invoking them
    must own the solver; the most recent call to optimise the problem must
    have resulted in an optimal solution; and the solver must be holding
    retained data structures for that optimal solution. It's much more
    efficient if the solver is using the full system, but it's not mandatory.

    Because this is a const method, we can't force any of this; we can only
    check.
  */
  void enableFactorization() const ;

  /*! \brief Undo the effects of #enableFactorization.

     Even if #resolve was invoked by #enableFactorization, little needs to
     be done here. Ownership of the solver is transferred by invocation, so
     there's no need to explicitly give it back.
  */
  void disableFactorization() const ;

  /*! \brief Check if an optimal basis is available. */
  bool basisIsAvailable () const ;

  /*! \brief Retrieve status information for architectural and logical
     	     variables.

    Retrieve status vectors for architectural (also called structural or
    column) and logical (also called artificial or row) variables. Returns the
    same information as #getWarmStart, but in a different format.
  */
  void getBasisStatus (int *archStatus, int *logStatus) const ;

  /*! \brief Set a basis and update the factorization and solution.

    Provides the combined functionality of #setWarmStart followed by
    #resolve. As with #getBasisStatus, the status vectors are coded as
    integers.
  */
  int setBasisStatus (const int *archStatus, const int *logStatus) ;

  /*! \brief Calculate duals and reduced costs for the given objective
	     coefficients.

    The solver's objective coefficient vector is not changed
    (cf. #setObjectiveAndRefresh)
  */
  virtual void getReducedGradient(double *columnReducedCosts, 
				  double *duals, const double *c) const ;

  /*! \brief Get indices of basic variables */
  virtual void getBasics(int *index) const ;

  /*! \brief Get a column of the basis inverse */
  virtual void getBInvCol(int col, double *betak) const ;

  /*! \brief Get a column of the tableau */
  virtual void getBInvACol(int col, double *abarj) const ;

  /*! \brief Get a row of the basis inverse */
  virtual void getBInvRow(int row, double *betai) const ;

  /*! \brief Get a row of the tableau */
  virtual void getBInvARow(int row, double *abari, double *betai = 0) const ;

//@}


/*! \name Debugging Methods */
//@{

  /*! \brief Activate the row cut debugger
  
    Activate the debugger for a model known to the debugger. The debugger
    will consult an internal database for an optimal solution vector.
  */

  void activateRowCutDebugger (const char * modelName) ;

  /*! \brief Activate the row cut debugger
  
    Activate the debugger for a model not included in the debugger's internal
    database. \p solution must be a full solution vector, but only the integer
    variables need to be correct. The debugger will fill in the continuous
    variables by solving an lp relaxation with the integer variables fixed as
    specified. If the given values for the continuous variables should be
    preserved, set \p keepContinuous to true.
  */

  void activateRowCutDebugger (const double *solution,
  			       bool keepContinuous = false) ;

/*! \brief Define to enable paranoid checks.

  When non-zero, enables various paranoid checks. 
    1: bounds checks on indices
    2: test for equality after structure copy operations and test for
       basis internal consistency.

  An error will cause a throw. Configuration should set this symbol to 0 for
  an optimised build, 2 if --enable-osidylp-paranoia is requested.

  In particular, this symbol must be defined as >= 1 in order for OsiCbc(dylp)
  to pass the OsiCbc unit test.
*/

#ifndef ODSI_PARANOIA
# define ODSI_PARANOIA 1
#endif

#if ODSI_PARANOIA >= 1
  /*! \brief Check that a row or column index is in range

    Check that a row or column index is in range for the current constraint
    system. This routine will throw an error if there is no constraint system
    or if the index is out of range.

    NOTE that ODSI_PARANOIA must be 1 or greater in order for OsiCbc(dylp)
    to pass the OsiCbc unit test.
  */
  void indexCheck (int k, bool isCol, std::string rtnnme) ;
#endif

//@}

/*! \name Dylp-specific methods */
//@{

  /*! \brief Process an options (.spc) file */

  void dylp_controlfile(const char* name, const bool silent,
			const bool mustexist = true) ;

  /*! \brief Establish a log file */

  void dylp_logfile(const char* name, bool echo = false) ;

  /*! \brief Establish an output (solution and/or statistics) file */

  void dylp_outfile(const char* name) ;

  /*! \brief Print the solution and/or statistics to the output file. */

  void dylp_printsoln(bool wantSoln, bool wantStats) ;

  /*! \brief Set the language for messages */

  void setOsiDylpMessages(CoinMessages::Language local_language) ;

//@}

/*! \name Unsupported functions */
//@{


  /*! \brief Invoke the solver's built-in branch-and-bound algorithm. */

  void branchAndBound() ;

//@}

/*! \name Dylp data structures

  These structures contain dylp control options and tolerances.

  Dylp existed before OSI, and (as with all solvers) it has its own unique
  options. Just be aware that if you access these structures directly you
  lose solver independence. See the dylp documentation and dylp.h for details.
*/
//@{

  /*! \brief Solver options for an initial solve. */
  lpopts_struct *initialSolveOptions ;

  /*! \brief Solver options for a resolve. */
  lpopts_struct *resolveOptions ;

  /*! \brief Solver numeric tolerances. */
  lptols_struct *tolerances ;

//@}

private:

/*
  Private implementation state and helper functions. If you're contemplating
  using any of these, you should have a look at the code.
  See OsiDylpSolverInterface.cpp for descriptions.
*/ 
/*! \name Dylp data structures

  These fields hold pointers to the data structures which are used to pass an
  lp problem to dylp.
*/
//@{

  /*! \brief The constraint system */
  consys_struct *consys ;
  /*! \brief The lp problem */
  lpprob_struct *lpprob ;
  /*! \brief The statistics structure */
  lpstats_struct *statistics ;

//@}

/*! \name Dylp residual control variables */
//@{

  /*! \brief Number of outstanding ODSI objects. */
  static int reference_count ;
  /*! \brief Basis maintenance package is initialised. */
  static bool basis_ready ;

//@}


/*! \name Solver instance control variables

  These variables maintain state for individual ODSI instances.
*/
//@{

  /*! \brief Output stream for this ODSI instance
  
    Holds the ioid of the stream that will be used to write out the solution
    and statistics.
  */

  ioid local_outchn ;

  /*! \brief Log stream for this ODSI instance

    Holds the ioid of the stream that will be used for dylp log information.
  */

  ioid local_logchn ;

  /*! \brief Controls output of log information to stdout
	     during initialSolve()
  */

  bool initial_gtxecho ;

  /*! \brief Controls output of log information to stdout
	     during resolve() and solveFromHotStart()
  */

  bool resolve_gtxecho ;

  /*! \brief Result of last call to solver for this ODSI instance
  
    The default value is lpINV (<i>i.e.</i>, the code is not valid). A call
    to dylp will set lp_retval to the dylp return code. Errors in the
    interface's interaction with other dylp routines will set this value to
    the return code given by the routine, or lpFATAL if the routine does not
    return anything more specific.
  */

  lpret_enum lp_retval ;

  /*! \brief Objective function sense for this ODSI instance

    Coded 1.0 to minimize (default), -1.0 to maximize.
  */

  double obj_sense ;

  /*! \brief The value of infinity */

  double odsiInfinity ;

  /*! \brief Solver name (dylp).  */

  const std::string solvername ;

  /*! \brief Array for info blocks associated with hints. */

  mutable void *info_[OsiLastHintParam] ;

  /*! \brief Allow messages from CoinMpsIO package. */

  bool mps_debug ;

  /*! \brief Warm start object used as a fallback for hot start
  
    If some other ODSI object uses the underlying solver between calls
    to #solveFromHotStart(), the solver must be reloaded. This basis is kept
    for just such a situation.
  */

  CoinWarmStart *hotstart_fallback ;

  /*! \brief Codes for basis condition

    - basisNone: no basis exists
    - basisFresh: the basis is in sync with the solver
    - basisModified: `good' constraint system modifications have occurred
    - basisDamaged: `bad' constraint system modifications have occurred

    `Good' modifications are deletion of a loose constraint (specifically, a
    constraint with a basic logical) or a variable at bound (specifically, a
    nonbasic variable). `Bad' modifications are deletion of a tight constraint
    (specifically, a constraint with a nonbasic logical) or deletion of a
    variable not at bound (specifically, a basic variable). Bad modifications
    will in general cause the basis to be primal and/or dual infeasible after
    it's patched up.

    A subtle point: basisModified will also be used in situations where ODSI
    has constructed a basis but not set it into an lpprob structure. This is
    the case when a solution is invented for a newly loaded problem.
  */

  enum basisCondition
  { basisNone = 0, basisFresh, basisModified, basisDamaged } ;

  /*! \brief Active basis

    The active basis is set with each successful return from the solver
    (where successful means a result of optimal, infeasible, unbounded, or
    iterlim), or by an explicit call to #setWarmStart() with a valid basis.
    By definition, calling #setWarmStart() with a null parameter is a request
    to synch the active basis with the solver (a noop for ODSI). Calling
    #setWarmStart() with an empty (0x0) basis is taken as a request to delete
    activeBasis.

    Condition will take a value from the #basisCondition enum (which see).

    Balance records whether we have an excess or shortage of basic variables.
    Deletion of tight constraints will result in an excess. Deletion of basic
    variables will result in a shortage.
  */

  struct
  { CoinWarmStart *basis ;
    basisCondition condition ;
    int balance ; } activeBasis ;

  /*! \brief The most recent solution from dylp is valid.

    True if the solution held in #lpprob is valid. False if changes to the
    constraint system have rendered the solution invalid.
  */
  bool solnIsFresh ;

  /*! \brief State related to the OsiSimplex interface.

    - simplex is set using the same coding as #canDoSimplexInterface: 0 is off,
      1 is tableau methods, 2 is pivot control.
  */

  mutable struct
  { int simplex ;
    bool saved_fullsys ; } simplex_state ;

//@}



/*! \name Cached problem information

  Problem information is cached for efficiency, to avoid repeated
  reconstruction of OSI structures from dylp structures.
*/
//@{

  mutable double _objval ;
  mutable double* _col_obj ;
  mutable double* _col_x ;
  mutable double* _col_cbar ;

  mutable double* _row_rhs ;
  mutable double* _row_lower ;
  mutable double* _row_upper ;
  mutable char* _row_sense ;
  mutable double* _row_range ;
  mutable double* _row_lhs ;
  mutable double* _row_price ;

  mutable CoinPackedMatrix* _matrix_by_col ;
  mutable CoinPackedMatrix* _matrix_by_row ;

//@}

/*! \name Data for presolve

  Data related to the use of the CoinPresolve capabilities (which see for
  further information).
*/
//@{

/*! \brief The presolve object

  In more detail, #preObj_ is loaded with the original system. Presolve
  transformations are applied to convert it to a presolved system.
*/
  CoinPresolveMatrix *preObj_ ;

/*! \brief List of postsolve actions

  The list of postsolve (reverse) transformations required to convert the
  presolved system back to the original system. Built as presolve
  transformations are applied.
*/
  const CoinPresolveAction *postActions_ ;

/*! \brief The postsolve object

  In more detail, #postObj_ is loaded with the presolved system and its
  optimal basis. The postsolve transformations held by #postActions_ are
  applied to convert back to the original system. For ODSI, our only interest
  is the basis.
*/
  CoinPostsolveMatrix *postObj_ ;

  /// Limit for iterations of the major presolve loop
  int passLimit_ ;

  /// true if presolve should consider integrality
  bool keepIntegers_ ;

  /// Saved copy of original problem
  consys_struct *savedConsys_ ;

  /// Saved pointers to cached structural vectors
  mutable double* saved_col_obj ;
  mutable double* saved_row_rhs ;
  mutable double* saved_row_lower ;
  mutable double* saved_row_upper ;
  mutable char* saved_row_sense ;
  mutable double* saved_row_range ;
  mutable CoinPackedMatrix* saved_matrix_by_col ;
  mutable CoinPackedMatrix* saved_matrix_by_row ;

//@}

/*! \name Helper functions for presolve

  Functions used to access the CoinPresolve capabilities. There are no public
  functions associated with presolve --- the only control is the
  OsiDoPresolveInInitial and OsiDoPresolveInResolve hints. The functions
  declared here do the work. See OsiDylpPresolve.cpp for additional
  explanation.
*/
//@{
  /// Create and load a presolve object.
  CoinPresolveMatrix *initialisePresolve(bool keepIntegers) ;

  /// Perform presolve transformations
  void doPresolve() ;

  /// Decide whether presolve was effective enough to use
  bool evalPresolve() ;

  /// Save the original problem
  void saveOriginalSys() ;

  /// Load the presolved problem into the ODSI object
  void installPresolve() ;

  /// Create and load a postsolve object
  CoinPostsolveMatrix *initialisePostsolve(CoinPresolveMatrix *&preObj) ;

  /// Apply the postsolve transforms from #postActions_
  void doPostsolve() ;

  /// Reload the original constraint system with the postsolved basis
  void installPostsolve() ;

  /// Delete presolve information
  void destruct_presolve() ;

//@}

/*! \name Helper functions for the simplex API */

//@{

  /// Ensure that the solver is ready for simplex operations
  bool ensureOwnership () const ;

//}@

/*! \name Helper functions for problem construction */

//@{
  void construct_lpprob() ;
  void construct_options() ;
  void construct_consys(int cols, int rows) ;
  void dylp_ioinit() ;
  void gen_rowparms(int rowcnt,
		    double *rhs, double *rhslow, contyp_enum *ctyp,
		    const double *rowlb, const double *rowub) ;
  void gen_rowparms(int rowcnt,
	       double *rhs, double *rhslow, contyp_enum *ctyp,
	       const char *sense, const double *rhsin, const double *range) ;
  void load_problem(const CoinMpsIO &mps) ;
  void load_problem(const CoinPackedMatrix &matrix,
	 const double* col_lower, const double* col_upper, const double* obj,
	 const contyp_enum *ctyp, const double* rhs, const double* rhslow) ;
  void load_problem (const int colcnt, const int rowcnt,
	 const int *start, const int *lens,
	 const int *index, const double *value,
	 const double* col_lower, const double* col_upper, const double* obj,
	 const contyp_enum *ctyp, const double* rhs, const double* rhslow) ;
//@}

/*! \name Helper functions for invoking dylp */
//@{
  /*! \brief Common core method to invoke dylp */
  lpret_enum do_lp (ODSI_start_enum start, bool echo) ;

  /*! \brief Install a basis in the lp problem structure */
  void setBasisInLpprob (const OsiDylpWarmStartBasis *wsb,
  			 lpprob_struct *lpprob) const ;
//@}

/*! \name Destructor helpers */
//@{
  void destruct_primal_cache() ;
  void destruct_dual_cache() ;
  void destruct_col_cache(bool structure) ;
  void destruct_row_cache(bool structure) ;
  void destruct_cache(bool rowStructure, bool colStructure) ;
  void destruct_problem(bool preserve_interface) ;
  void detach_dylp() const ;
//@}


/*! \name Helper functions for problem modification */
/*
  There are separate groups for member and static methods so that doxygen
  won't promote the group to the top level.
*/
//@{
  
  void add_col(const CoinPackedVectorBase& coin_coli,
	       vartyp_enum vtypi, double vlbi,
	       double vubi, double obji, const std::string *nme) ;
  void add_row(const CoinPackedVectorBase& coin_rowi, 
	       char clazzi, contyp_enum ctypi,
	       double rhsi, double rhslowi, const std::string *nme) ;
  void calc_objval() ;
  contyp_enum bound_to_type(double lower, double upper) ;
  void gen_rowiparms(contyp_enum* ctypi, double* rhsi, double* rhslowi, 
			    char sensei, double rhsini, double rangei) ;
  void gen_rowiparms(contyp_enum* ctypi, double* rhsi, double* rhslowi, 
			    double rowlbi, double rowubi) ;
  void unimp_hint(bool dylpSense, bool hintSense,
		 OsiHintStrength hintStrength, const char *msgString) ;
  void pessimal_primal() ;
  void reduceActiveBasis() ;

//@}

/*! \name Helper functions for problem modification */
//@{
  static contyp_enum sense_to_type(char type) ;
  static char type_to_sense(contyp_enum type) ;
//@}

/*! \name Copy helpers

  Copy function templates for simple vectors and fixed-size objects, and
  specializations for various complex structures.
*/
//@{
  template<class T> static void copy(const T* src, T* dst, int n) ;
  template<class T> static T* copy(const T* src, int n) ;
  template<class T> static T* copy(const T* src) ;
/*
  Specializations for more complicated structures.
*/
  static basis_struct* copy_basis(const basis_struct* src, int dstsze) ;
  static void copy_basis(const basis_struct* src, basis_struct* dst) ;
  static lpprob_struct* copy_lpprob(const lpprob_struct* src) ;
//@}

#ifndef _MSC_VER
/*! \name Copy verification functions

  Copy verification functions, to check that two structures are identical.
*/
//@{
  template<class T> static void assert_same(const T& t1, const T& t2,
					    bool exact) ;
  template<class T> static void assert_same(const T* t1, const T* t2,
					    int n, bool exact) ;

  static void assert_same(double d1, double d2, bool exact) ;

  static void assert_same(const basis_struct& b1, 
  			  const basis_struct& b2, bool exact) ;
  static void assert_same(const consys_struct& c1, const 
			  consys_struct& c2, bool exact) ;
  static void assert_same(const conbnd_struct& c1, const 
			  conbnd_struct& c2, bool exact) ;
  static void assert_same(const lpprob_struct& l1, 
			  const lpprob_struct& l2, bool exact) ;
  static void assert_same(const OsiDylpSolverInterface& o1, 
			  const OsiDylpSolverInterface& o2, bool exact) ;
//@}
#endif	/* ! _MSC_VER */

/*! \name Vector helper functions

  The inline methods are documented here, because this is the only place they
  appear.
*/
//@{

  /*! \brief Convert 0-based vector pointer to 1-based vector pointer

    For cases where it's inconvenient to adjust indices, the alternative is to
    adjust the pointer to the vector so it points to vector[-1]. Be careful!
  */
  template<class T> inline static T* idx_vec (T* vec) { return (vec-1) ; }

  /*! \brief Convert 0-based index to 1-based index */
  inline static int idx (int i) { return (i+1) ; }

  /*! \brief Convert 1-based vector pointer to 0-based vector pointer

    For cases where it's inconvenient to adjust indices, the alternative is to
    adjust the pointer to the vector so it points to vector[1].
  */
  template<class T> inline static T* inv_vec (T* vec) { return (vec+1) ; }

  /*! \brief Convert 1-based index to 0-based index */
  inline static int inv (int i) { return (i-1) ; }

  static pkvec_struct* packed_vector(
    const CoinShallowPackedVector vector, int dimension) ;
  static void packed_vector(
    const CoinShallowPackedVector vector, int dimension, pkvec_struct *dst) ;
//@}

/*! \name File i/o helper routines */
//@{
  static std::string make_filename(const char *filename, 
				   const char *ext1, const char *ext2) ;
//@}

} ;


/*
  OsiDylpSolverInterfaceTest.cpp
*/

/*! \relates OsiDylpSolverInterface
    \brief Unit test for OsiDylpSolverInterface.

    Performs various tests to see if ODSI is functioning correctly. Not
    an exhaustive test, but it'll (usually) catch gross problems.
*/

void OsiDylpSolverInterfaceUnitTest(const std::string & mpsDir,
				    const std::string &netLibDir) ;

#endif // OsiDylpSolverInterface_H
