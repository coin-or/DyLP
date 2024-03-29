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
  This file contains static data definitions for several test problems, as well
  as some utility routines for loading the static data into a constraint
  system.
*/

#include "dylp.h"

extern ioid dy_logchn ;
extern bool dy_gtxecho ;

/*
  The exmip1 example used throughout COIN unit tests. This example is drawn
  from the OSL documentation, "Passing Your Model Using Mathematical
  Programming System (MPS) Format." It's probably not possible to find this
  any longer, unless you happen to know someone old enough to have worked
  with OSL.
*/

static const int exmip1_rowcnt = 6 ;
static const int exmip1_colcnt = 8 ;
static const int exmip1_coeffcnt = 17 ;
static const int exmip1_maxColLen = 3 ;
static const char *exmip1_objname = "OBJ" ;
static const int exmip1_objndx = 1 ;

static const char *exmip1_rowname[] = { "bogus",
  "OBJ", "ROW01", "ROW02", "ROW03", "ROW04", "ROW05"
  } ;
static const char exmip1_rowsense[] = { 'B',
  'N', 'G', 'L', 'E', 'R', 'R'
  } ;
static const double exmip1_rowlb[] = { -42.42,
  -1e100, 2.5, -1e100, 4.0, 1.8, 3.0
  } ;
static const double exmip1_rowub[] = { -42.42,
  1e100, 1e100, 2.1, 4.0, 5.0, 15.0
  } ;

static const char *exmip1_colname[] = { "bogus",
  "COL01", "COL02", "COL03", "COL04", "COL05", "COL06", "COL07", "COL08"
  } ;
static const double exmip1_collb[] = { -42.42,
  2.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0
  } ;
static const double exmip1_colub[] = { -42.42,
  1e100, 4.1, 1.0, 1.0, 4.0, 1e100, 1e100, 4.3
  } ;

static const int exmip1_rowndx[] = {-42,
  1, 2, 6, 2, 3, 3, 4, 2, 5, 1, 2, 6, 4, 5, 1, 2, 6
  } ;
static const int exmip1_colndx[] = {-42,
  1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 5, 6, 7, 8, 8, 8
  } ;
static const double exmip1_coeff[] = { -42.42,
  1.0, 3.0, 5.6, 1.0, 2.0, 1.1, 1.0, -2.0, 2.8, 2.0, -1.0, 1.0, 1.0, -1.2, -1.0,
  -1.0, 1.9
  } ;



/*
  The exprimalray example is used to test the routine that returns primal
  rays. The coefficient matrix is:

     6 x1 +  15 x2 +   8 x3 >=  210	face1
     9 x1 -   6 x2 -   7 x3 <=   30	face2
  -  3 x1 +  21 x2 -   4 x3 <=  180	face3
  -216 x1 + 315 x2 - 136 x3 <= 1218	face4
    66 x1 + 336 x2 -  83 x3 <= 4305	face5

      x1, x2, x3 free

  The overall shape is a cone formed by face1, face2, and face3, with origin
  at (10,10,0), opening upward (increasing x3). Face4 forms an extreme point
  with faces 1 and 3 at (8,10,3/2), cutting off ray13 emanating from
  (10,10,0) and forming two more rays emanating from (8,10,3/2). Face5 forms
  an extreme point with faces 2 and 3 at (13,11,3), cutting off ray 23
  emanating from (10,10,0) and forming two more rays emanating from
  (13,11,3).

  ray12 = ( 1 -2  3)
  ray13 = (-4  0  3)	[ truncated by face4 at (8,10,3/2) ]
  ray23 = ( 3  1  3)	[ truncated by face5 at (13,11,3) ]

  ray14 = (-40  -8  45)
  ray34 = (-28   8  63)

  ray25 = (10  1  12)
  ray35 = ( 7  9  42)

  An objective of min x1 + x2 + 2x3 will find (10,10,0), the base of the cone.

  An objective of min (1 0 1) will find (8,10,3/2) with z = 9.5. An objective
  of min (1 1 1) will want to go unbounded from (8,10,3/2) along ray14. An
  objective of min (3 1 1) will want to go unbounded from (8,10,3/2) along
  both of ray14 and ray34.

  An objective of min (-1 -1 1) (coded below) will find (13,11,3). An
  objective of min (-1 -4 1) will want to go unbounded along either of ray25
  or ray35. This portion of the polytope works better in exdualray (we don't
  run afoul of the orthant constraints in the dual).
*/


static const int exprimalray_rowcnt = 6 ;
static const int exprimalray_colcnt = 3 ;
static const int exprimalray_coeffcnt = 18 ;
static const int exprimalray_maxColLen = 5 ;
static const char *exprimalray_objname = "obj" ;
static const int exprimalray_objndx = 1 ;

static const char *exprimalray_rowname[] = { "bogus",
  "obj", "face1", "face2", "face3", "face4", "face5"
  } ;
static const char exprimalray_rowsense[] = { 'B',
  'N', 'G', 'L', 'L', 'L', 'L'
  } ;
static const double exprimalray_rowlb[] = { -42.42,
  -1e100, 210, -1e100, -1e100, -1e100, -1e100
  } ;
static const double exprimalray_rowub[] = { -42.42,
  1e100, 1e100, 30, 180, 1218, 4305
  } ;

static const char *exprimalray_colname[] = { "bogus",
  "x1", "x2", "x3"
  } ;
static const double exprimalray_collb[] = { -42.42,
  -1e100, -1e100, -1e100
  } ;
static const double exprimalray_colub[] = { -42.42,
  1e100, 1e100, 1e100
  } ;

static const int exprimalray_rowndx[] = { -42,
  1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6
  } ;
static const int exprimalray_colndx[] = { -42,
  1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3
  } ;
static const double exprimalray_coeff[] = { -42.42,
   -1.0,  6.0,  9.0, -3.0, -216.0,  66.0,
   -1.0, 15.0, -6.0, 21.0,  315.0, 336.0,
    1.0,  8.0, -7.0, -4.0, -136.0, -83.0
  } ;



/*
  The exdualray example is used to test the routine that returns dual
  rays. It uses the polyhedron of exprimalray to define the dual space,
  resulting in this primal coefficient matrix:

   -6 face1 +   9 face2 -   3 face3 - 216 face4 +  66.0 face5 >=  1.0	x1
  -15 face1 -   6 face2 +  21 face3 + 315 face4 + 336.0 face5 >=  1.0	x2
   -8 face1 -   7 face2 -   4 face3 - 136 face4 -  83.0 face5 >= -1.0	x3

      0 <= face1, face2, face3, face4, face5 <= infty

  The objective of c = (-210 30 180 1218 4305) coded into the arrays below
  completes the definition of the dual polytope. With b = (1 1 -1) as shown,
  there should be a finite minimum at y = (13,11,3).

  To translate the primal objective of exprimalray to the rhs of exdualray,
  note first that the primal min objective is negated to get a max objective,
  which is then used as the rhs here according to the standard primal -> dual
  transform. (Specified using rowlb!)
*/


static const int exdualray_rowcnt = 4 ;
static const int exdualray_colcnt = 5 ;
static const int exdualray_coeffcnt = 20 ;
static const int exdualray_maxColLen = 3 ;
static const char *exdualray_objname = "obj" ;
static const int exdualray_objndx = 1 ;

static const char *exdualray_rowname[] = { "bogus",
  "obj", "x1", "x2", "x3"
  } ;
static const char exdualray_rowsense[] = { 'B',
  'N', 'G', 'G', 'G'
  } ;
static const double exdualray_rowub[] = { -42.42,
  1e100, 1e100, 1e100, 1e100
  } ;
static const double exdualray_rowlb[] = { -42.42,
  -1e100, 1.0, 1.0, -1.0
  } ;
/*
static const char exdualray_rowsense[] = { 'B',
  'N', 'L', 'G', 'G'
  } ;
static const double exdualray_rowub[] = { -42.42,
  1e100, -1.0, 1e100, 1e100
  } ;
static const double exdualray_rowlb[] = { -42.42,
  -1e100, -1e100, 1.0, -1.0
  } ;
*/

static const char *exdualray_colname[] = { "bogus",
  "face1", "face2", "face3", "face4", "face5"
  } ;
static const double exdualray_collb[] = { -42.42,
  0, 0, 0, 0, 0
  } ;
static const double exdualray_colub[] = { -42.42,
  1e100, 1e100, 1e100, 1e100, 1e100
  } ;

static const int exdualray_rowndx[] = { -42,
  1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4
  } ;
static const int exdualray_colndx[] = { -42,
  1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5
  } ;
static const double exdualray_coeff[] = { -42.42,
    -210.0,   -6.0,  -15.0,   -8.0,
      30.0,    9.0,   -6.0,   -7.0,
     180.0,   -3.0,   21.0,   -4.0,
    1218.0, -216.0,  315.0, -136.0,
    4305.0,   66.0,  336.0,  -83.0
  } ;
/* Correct for constraint x1 a G constraint
static const double exdualray_coeff[] = { -42.42,
    -210.0,    6.0,  -15.0,   -8.0,
      30.0,   -9.0,   -6.0,   -7.0,
     180.0,    3.0,   21.0,   -4.0,
    1218.0,  216.0,  315.0, -136.0,
    4305.0,  -66.0,  336.0,  -83.0
  } ;
*/


/*
  galenet, from Data/Infeas.

  This problem is a small network flow problem: three sources, two
  intermediate nodes, three sinks. It's a feasibility problem (objective
  coefficients are all zero, hence duals and reduced costs are also zero)
  and it's primal infeasible and dual unbounded.

  It was used in the OsiClp unit test, and it turns out to be good from several
  viewpoints. It's small enough to do manual checks, it has a nice mix of <=,
  =, and >= constraints, and it has bounded variables.

  Three more versions of this problem follow:
    * galenetmixed, in which the equalities are converted to a pair of <= and
      >= constraints;
    * galenetleq, in which all constraints are converted to <= constraints;
      and
    * galenetbnds, in which all bounds are converted to explicit inequalities,
      then all inequalities are converted to <= constraints.

  Galenetbnds is the easy case for dual ray generation, with all constraints
  explicit. This makes it easy to recreate the canonical dual without worrying
  about handling implicit bound constraints.

  Galenetbnds and galenetleq both exercise the unscaled alternatives in the
  various solution generation routines.

  Galenetmixed turns out to exercise some interesting cases. In addition to
  the inversion required to convert to >= constraints in the original system,
  there's one case where dylp's BLLB slack attached to the internal <=
  constraint must be viewed as a BUUB surplus in order to successfully generate
  the correct ray.
*/

/*
  First galenet as originally formulated, with a mix of >=, =, and <=
  constraints.
*/

static const int galenet_rowcnt = 9 ;
static const int galenet_colcnt = 8 ;
static const int galenet_coeffcnt = 16 ;
static const int galenet_maxColLen = 2 ;
static const char *galenet_objname = "COST" ;
static const int galenet_objndx = 9 ;

static const char *galenet_rowname[] = { "bogus",
  "S1", "S2", "S3", "NODE4", "NODE5", "D6", "D7", "D8", "COST"
  } ;
static const char galenet_rowsense[] = { 'B',
  'L', 'L', 'L', 'E', 'E', 'G', 'G', 'G', 'N'
  } ;
static const double galenet_rowlb[] = { -42.42,
  -1e100, -1e100, -1e100, 0.0, 0.0, 10., 20., 30., -1e100
  } ;
static const double galenet_rowub[] = { -42.42,
  20., 20., 20., 0.0, 0.0, 1e100, 1e100, 1e100, 1e100
  } ;

static const char *galenet_colname[] = { "bogus",
  "T14", "T24", "T25", "T35", "T46", "T47", "T57", "T58"
  } ;
static const double galenet_collb[] = { -42.42,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
  } ;
static const double galenet_colub[] = { -42.42,
  30., 20., 10., 10., 10., 2., 20., 30.
  } ;

static const int galenet_rowndx[] = {-42,
  1, 4, 2, 4, 2, 5, 3, 5, 6, 4, 7, 4, 7, 5, 8, 5
  } ;
static const int galenet_colndx[] = {-42,
  1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8
  } ;
static const double galenet_coeff[] = { -42.42,
  1., 1., 1., 1., 1., 1., 1., 1., 1., -1., 1., -1., 1., -1., 1., -1.
  } ;

/*
  Galenetmixed, with the equalities converted to a pair of inequalities.
*/

static const int galenetmixed_rowcnt = 11 ;
static const int galenetmixed_colcnt = 8 ;
static const int galenetmixed_coeffcnt = 24 ;
static const int galenetmixed_maxColLen = 3 ;
static const char *galenetmixed_objname = "COST" ;
static const int galenetmixed_objndx = 11 ;

static const char *galenetmixed_rowname[] = { "bogus",
  "S1", "S2", "S3", "NODE4U", "NODE4L", "NODE5U", "NODE5L", "D6", "D7", "D8",
  "COST"
  } ;
static const char galenetmixed_rowsense[] = { 'B',
  'L', 'L', 'L', 'L', 'G', 'L', 'G', 'G', 'G', 'G', 'N'
  } ;
static const double galenetmixed_rowlb[] = { -42.42,
  -1e100, -1e100, -1e100, -1e100, 0.0, -1e100, 0.0, 10., 20., 30., -1e100
  } ;
static const double galenetmixed_rowub[] = { -42.42,
  20., 20., 20., 0.0, 1e100, 0.0, 1e100, 1e100, 1e100, 1e100, 1e100
  } ;

static const char *galenetmixed_colname[] = { "bogus",
  "T14", "T24", "T25", "T35", "T46", "T47", "T57", "T58"
  } ;
static const double galenetmixed_collb[] = { -42.42,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
  } ;
static const double galenetmixed_colub[] = { -42.42,
  30., 20., 10., 10., 10., 2., 20., 30.
  } ;

static const int galenetmixed_rowndx[] = {-42,
  1, 4, 5, 2, 4, 5, 2, 6, 7, 3, 6, 7, 8, 4, 5, 9, 4, 5, 9, 6, 7, 10, 6, 7
  } ;
static const int galenetmixed_colndx[] = {-42,
  1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8
  } ;
static const double galenetmixed_coeff[] = { -42.42,
  1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., -1., -1., 1., -1., -1.,
  1., -1., -1., 1., -1., -1.
  } ;

/*
  Galenetleq, with equalities converted to inequalities and >= constraints
  converted to <= constraints.
*/

static const int galenetleq_rowcnt = 11 ;
static const int galenetleq_colcnt = 8 ;
static const int galenetleq_coeffcnt = 24 ;
static const int galenetleq_maxColLen = 3 ;
static const char *galenetleq_objname = "COST" ;
static const int galenetleq_objndx = 11 ;

static const char *galenetleq_rowname[] = { "bogus",
  "S1", "S2", "S3", "NODE4U", "NODE4L", "NODE5U", "NODE5L", "D6", "D7", "D8",
  "COST"
  } ;
static const char galenetleq_rowsense[] = { 'B',
  'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'N'
  } ;
static const double galenetleq_rowlb[] = { -42.42,
  -1e100, -1e100, -1e100, -1e100, -1e100, -1e100, -1e100, -1e100, -1e100,
  -1e100, -1e100
  } ;
static const double galenetleq_rowub[] = { -42.42,
  20., 20., 20., 0.0, 0.0, 0.0, 0.0, -10., -20., -30., 1e100
  } ;

static const char *galenetleq_colname[] = { "bogus",
  "T14", "T24", "T25", "T35", "T46", "T47", "T57", "T58"
  } ;
static const double galenetleq_collb[] = { -42.42,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
  } ;
static const double galenetleq_colub[] = { -42.42,
  30., 20., 10., 10., 10., 2., 20., 30.
  } ;

static const int galenetleq_rowndx[] = {-42,
  1, 4, 5, 2, 4, 5, 2, 6, 7, 3, 6, 7, 8, 4, 5, 9, 4, 5, 9, 6, 7, 10, 6, 7
  } ;
static const int galenetleq_colndx[] = {-42,
  1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8
  } ;
static const double galenetleq_coeff[] = { -42.42,
  1., 1., -1.0, 1., 1., -1.0, 1., 1., -1., 1., 1., -1., -1., -1., 1.0, -1., -1.,
  1.0, -1., -1., 1., -1., -1., 1.
  } ;


/*
  Galenetbnds, with bounds on variables made into explicit <= constraints.
  Useful when checking just how rA > 0 fails in the presence of implicit
  bounds.
*/

static const int galenetbnds_rowcnt = 27 ;
static const int galenetbnds_colcnt = 8 ;
static const int galenetbnds_coeffcnt = 40 ;
static const int galenetbnds_maxColLen = 5 ;
static const char *galenetbnds_objname = "COST" ;
static const int galenetbnds_objndx = 27 ;

static const char *galenetbnds_rowname[] = { "bogus",
  "S1", "S2", "S3", "NODE4U", "NODE4L", "NODE5U", "NODE5L", "D6", "D7", "D8",
  "T14UB", "T14LB", "T24UB", "T24LB", "T25UB", "T25LB", "T35UB", "T35LB",
  "T46UB", "T46LB", "T47UB", "T47LB", "T57UB", "T57LB", "T58UB", "T58LB",
  "COST"
  } ;
static const char galenetbnds_rowsense[] = { 'B',
  'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L',
  'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'N'
  } ;
static const double galenetbnds_rowlb[] = { -42.42,
  -1e100, -1e100, -1e100, -1e100, -1e100, -1e100, -1e100, -1e100, -1e100,
  -1e100, -1e100, -1e100, -1e100, -1e100, -1e100, -1e100, -1e100, -1e100,
  -1e100, -1e100, -1e100, -1e100, -1e100, -1e100, -1e100, -1e100, -1e100
  } ;
static const double galenetbnds_rowub[] = { -42.42,
  20., 20., 20., 0.0, 0.0, 0.0, 0.0, -10., -20., -30., 30., 0.0, 20., 0.0, 10.,
  0.0, 10., 0.0, 10., 0.0, 2., 0.0, 20., 0.0, 30., 0.0, 1e100
  } ;

static const char *galenetbnds_colname[] = { "bogus",
  "T14", "T24", "T25", "T35", "T46", "T47", "T57", "T58"
  } ;
static const double galenetbnds_collb[] = { -42.42,
  -1e100, -1e100, -1e100, -1e100, -1e100, -1e100, -1e100, -1e100
  } ;
static const double galenetbnds_colub[] = { -42.42,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100
  } ;

static const int galenetbnds_rowndx[] = {-42,
  1, 4, 5, 11, 12, 2, 4, 5, 13, 14, 2, 6, 7, 15, 16, 3, 6, 7, 17, 18, 8, 4, 5,
  19, 20, 9, 4, 5, 21, 22, 9, 6, 7, 23, 24, 10, 6, 7, 25, 26
  } ;
static const int galenetbnds_colndx[] = {-42,
  1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6,
  6, 6, 6, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8
  } ;
static const double galenetbnds_coeff[] = { -42.42,
  1., 1., -1.0, 1., -1., 1., 1., -1.0, 1., -1., 1., 1., -1., 1., -1., 1., 1.,
  -1., 1., -1., -1., -1., 1.0, 1., -1., -1., -1., 1.0, 1., -1., -1., -1., 1.,
  1., -1., -1., -1., 1., 1., -1.
  } ;



/*
  The afiro example, smallest of the Netlib problems.
*/

static const int afiro_rowcnt = 28 ;
static const int afiro_colcnt = 32 ;
static const int afiro_coeffcnt = 88 ;
static const int afiro_maxColLen = 4 ;
static const char *afiro_objname = "COST" ;
static const int afiro_objndx = 1 ;

static const char *afiro_rowname[] = { "bogus",
  "COST", "R09", "R10", "R12", "R13", "R19", "R20", "R22", "R23", "X05", "X17",
  "X18", "X19", "X20", "X21", "X27", "X40", "X41", "X42", "X43", "X44", "X45",
  "X46", "X47", "X48", "X49", "X50", "X51"
  } ;
static const char afiro_rowsense[] = { 'B',
  'N', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L',
  'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L'
  } ;
static const double afiro_rowlb[] = { -42.42,
  -1e100, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 44., -1e100, -1e100, -1e100,
  -1e100, -1e100, -1e100, -1e100, -1e100, -1e100, -1e100, -1e100, -1e100,
  -1e100, -1e100, -1e100, -1e100, -1e100, -1e100, -1e100
  } ;
static const double afiro_rowub[] = { -42.42,
  1e100, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 44., 80., 80., 0.0, 0.0, 0.0, 0.0,
  500., 500., 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 310., 300.
  } ;

static const char *afiro_colname[] = { "bogus",
  "X01", "X02", "X03", "X04", "X06", "X07", "X08", "X09", "X10", "X11", "X12",
  "X13", "X14", "X15", "X16", "X22", "X23", "X24", "X25", "X26", "X28", "X29",
  "X30", "X31", "X32", "X33", "X34", "X35", "X36", "X37", "X38", "X39"
  } ;
static const double afiro_collb[] = { -42.42,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0
  } ;
static const double afiro_colub[] = { -42.42,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100
  } ;

static const int afiro_rowndx[] = {-42,
  25, 2, 3, 10, 15, 2, 1, 23, 2, 27, 3, 26, 4, 5, 11, 26, 4, 5, 12, 26, 4, 5,
  13, 26, 4, 5, 14, 22, 11, 22, 12, 22, 13, 22, 14, 15, 4, 1, 24, 4, 28, 5, 23,
  6, 7, 16, 21, 6, 1, 25, 6, 22, 6, 27, 7, 24, 8, 9, 17, 24, 8, 9, 18, 24, 8, 9,
  19, 24, 8, 9, 20, 22, 17, 22, 18, 22, 19, 22, 20, 21, 9, 1, 26, 9, 28, 8, 9,
  1
  } ;
static const int afiro_colndx[] = {-42,
  1, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8,
  8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16, 16, 16,
  17, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 21, 21, 22, 22, 22, 22, 23, 23,
  23, 23, 24, 24, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 29, 30, 30,
  31, 31, 32, 32
  } ;
static const double afiro_coeff[] = { -42.42,
  .301, -1., -1.06, 1., -1., 1., -.4, -1., 1., 1., 1., .301, -1., -1.06, 1.,
  .313, -1., -1.06, 1., .313, -1., -.96, 1., .326, -1., -.86, 1., 2.364, -1.,
  2.386, -1., 2.408, -1., 2.429, -1., 1.4, 1., -.32, -1., 1., 1., 1., .109, -1.,
  -.43, 1., -1., 1., -.6, -1., 1., -1., 1., 1., 1., .109, -.43, 1., 1., .108,
  -.43, 1., 1., .108, -.39, 1., 1., .107, -.37, 1., 1., 2.191, -1., 2.219, -1.,
  2.249, -1., 2.279, -1., 1.4, -1., -.48, -1., 1., 1., 1., 1., 10.
  } ;



/*
  boeing2, the smallest problem that's a good mix of L, G, E, and R
  constraints.
*/

static const int boeing2_rowcnt = 167 ;
static const int boeing2_colcnt = 143 ;
static const int boeing2_coeffcnt = 1339 ;
static const int boeing2_maxColLen = 24 ;
static const char *boeing2_objname = "OBJECTIV" ;
static const int boeing2_objndx = 3 ;

static const char *boeing2_rowname[] = { "bogus",
  "REVENUES", "ACOCOSTS", "OBJECTIV", "FUELAVAL", "SYSTDEPT", "ACMILES",
  "ASMILES", "PASSNGRS", "RPMILES", "LFRPMASM", "ATONMILE", "RTONMILE",
  "LFTNMILE", "FLAV*1", "FLAV*2", "FLAV*3", "FLAV*4", "LF1003S1", "LF1003B1",
  "LF1003C1", "LF1005S1", "LF1005B1", "LF1005C1", "LF1011S1", "LF1011S2",
  "LF1011B1", "LF1011B2", "LF1011C1", "LF1011C2", "LF1013S1", "LF1013S2",
  "LF1013B1", "LF1013B2", "LF1013C1", "LF1013C2", "LF1015S1", "LF1015B1",
  "LF1015C1", "LF1017S1", "LF1017B1", "LF1017C1", "LF1019S1", "LF1019S2",
  "LF1019B1", "LF1019B2", "LF1019C1", "LF1019C2", "LF1021S1", "LF1021B1",
  "LF1021C1", "LF1002S1", "LF1002B1", "LF1002C1", "LF1004S1", "LF1004B1",
  "LF1004C1", "LF1006S1", "LF1006S2", "LF1006B1", "LF1006B2", "LF1006C1",
  "LF1006C2", "LF1008S1", "LF1008S2", "LF1008B1", "LF1008B2", "LF1008C1",
  "LF1008C2", "LF1010S1", "LF1010S2", "LF1010S3", "LF1010B1", "LF1010B2",
  "LF1010B3", "LF1010C1", "LF1010C2", "LF1010C3", "LF1012S1", "LF1012B1",
  "LF1012C1", "LF1014S1", "LF1014S2", "LF1014B1", "LF1014B2", "LF1014C1",
  "LF1014C2", "LF1100S1", "LF1100S2", "LF1100S3", "LF1100S4", "LF1100S5",
  "LF1100B1", "LF1100B2", "LF1100B3", "LF1100B4", "LF1100B5", "LF1100C1",
  "LF1100C2", "LF1100C3", "LF1100C4", "LF1100C5", "LF1102S1", "LF1102S2",
  "LF1102S3", "LF1102S4", "LF1102B1", "LF1102B2", "LF1102B3", "LF1102B4",
  "LF1102C1", "LF1102C2", "LF1102C3", "LF1102C4", "LF1200S1", "LF1200B1",
  "LF1200C1", "LF1201S1", "LF1201B1", "LF1201C1", "NOPTCLE0", "CONTBOS1",
  "CONTBOS2", "CONTBOS3", "CONTBOS4", "CONTORD1", "CONTORD2", "CONTORD3",
  "CONTORD4", "CONTLGA2", "CONTLGA4", "CONTCLE1", "CONTCLE2", "CONTCLE3",
  "CONTCLE4", "DMBOSORD", "DMBOSLGA", "DMBOSCLE", "DMORDBOS", "DMORDLGA",
  "DMORDCLE", "DMLGABOS", "DMLGAORD", "DMLGACLE", "DMCLEBOS", "DMCLEORD",
  "DMCLELGA", "MSBOSORD", "MSBOSLGA", "MSBOSCLE", "MSORDBOS", "MSORDLGA",
  "MSORDCLE", "MSLGABOS", "MSLGAORD", "MSLGACLE", "MSCLEBOS", "MSCLEORD",
  "MSCLELGA", "DCBOSORD", "DCBOSCLE", "DCORDBOS", "DCORDLGA", "DCLGAORD",
  "DCLGACLE", "DCCLELGA", "MCORDBOS", "MCLGAORD"
  } ;
static const char boeing2_rowsense[] = { 'B',
  'G', 'G', 'N', 'L', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'E', 'E',
  'E', 'E', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
  'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
  'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
  'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
  'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
  'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
  'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G',
  'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'R',
  'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'G', 'G', 'G', 'G',
  'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
  'G', 'G'
  } ;
static const double boeing2_rowlb[] = { -42.42,
  0.0, 0.0, -1e100, -1e100, 50., 0.0, 0.0, 9431., 0.0, 0.0, 0.0, 0.0, 0.0, 30.,
  45., 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  24., 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  241.0, 1881.0, 113.0, 241.0, 412.0, 495.0, 2194.0, 569.0, 413.0, 104.0, 569.0,
  327.0, 3., 7., 1., 3., 4., 5., 7., 5., 4., 1., 6., 3., 0.0, 12.8, 19.2, 10.4,
  36.0, 12.8, 0.0, 1., 2.
  } ;
static const double boeing2_rowub[] = { -42.42,
  1e100, 1e100, 1e100, 100000., 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 1e100, 30., 45., 0.0, 0.0, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 302., 2352., 142., 302., 515., 619., 2743., 712., 517., 131., 712.,
  409., 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 1e100, 12., 16., 24., 13., 45., 16., 5., 1e100, 1e100
  } ;

static const char *boeing2_colname[] = { "bogus",
  "PBOSORD0", "PBOSORD1", "PBOSORD2", "PBOSORD3", "PBOSORD4", "PBOSLGA0",
  "PBOSLGA1", "PBOSLGA2", "PBOSLGA3", "PBOSCLE0", "PBOSCLE1", "PBOSCLE2",
  "PBOSCLE3", "PORDBOS0", "PORDBOS1", "PORDBOS2", "PORDBOS3", "PORDBOS4",
  "PORDLGA0", "PORDLGA1", "PORDLGA2", "PORDLGA3", "PORDCLE0", "PORDCLE1",
  "PORDCLE2", "PORDCLE3", "PLGABOS0", "PLGABOS1", "PLGABOS2", "PLGABOS3",
  "PLGABOS4", "PLGABOS5", "PLGAORD0", "PLGAORD1", "PLGAORD2", "PLGAORD3",
  "PLGACLE0", "PLGACLE1", "PLGACLE2", "PCLEBOS0", "PCLEBOS1", "PCLEBOS2",
  "PCLEBOS3", "PCLEBOS4", "PCLEORD0", "PCLEORD1", "PCLEORD2", "PCLEORD3",
  "PCLEORD4", "PCLELGA0", "PCLELGA1", "PCLELGA2", "PCLELGA3", "BBOSORD0",
  "BBOSORD1", "BBOSCLE0", "BBOSCLE1", "BORDBOS0", "CBOSORD0", "CBOSORD1",
  "CBOSORD2", "CBOSORD3", "CBOSORD4", "CBOSCLE0", "CBOSCLE1", "CBOSCLE2",
  "CBOSCLE3", "CORDBOS0", "CORDBOS1", "CORDBOS2", "CORDBOS3", "CORDBOS4",
  "CORDLGA0", "CORDLGA1", "CORDLGA2", "CORDLGA3", "CLGAORD0", "CLGAORD1",
  "CLGAORD2", "CLGAORD3", "CLGACLE0", "CLGACLE1", "CLGACLE2", "CCLELGA0",
  "CCLELGA1", "CCLELGA2", "CCLELGA3", "GRDTIMO1", "GRDTIMN1", "GRDTIMO2",
  "GRDTIMN2", "GRDTIMN3", "GRDTIMN4", "N1003AC1", "N1003AC2", "N1003AC3",
  "N1003AC4", "N1005AC1", "N1005AC2", "N1005AC3", "N1005AC4", "N1011AC1",
  "N1011AC2", "N1011AC3", "N1011AC4", "N1013AC2", "N1013AC4", "N1015AC2",
  "N1015AC4", "N1017AC2", "N1017AC4", "N1019AC2", "N1019AC4", "N1021AC1",
  "N1021AC2", "N1021AC3", "N1021AC4", "N1002AC1", "N1002AC2", "N1002AC3",
  "N1002AC4", "N1004AC2", "N1004AC4", "N1006AC1", "N1006AC2", "N1006AC3",
  "N1006AC4", "N1008AC2", "N1008AC4", "N1010AC2", "N1010AC4", "N1012AC2",
  "N1012AC4", "N1014AC2", "N1014AC4", "N1100AC2", "N1100AC4", "N1102AC2",
  "N1102AC4", "N1200AC2", "N1200AC4", "N1201AC2", "N1201AC4"
  } ;
static const double boeing2_collb[] = { -42.42,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -100., 0.0,
  -90., -45., -45., 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
  } ;
static const double boeing2_colub[] = { -42.42,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100, 1e100,
  0., 1e100, 0., 0., 0., 7., 7., 2., 2., 7., 7., 2., 2., 7., 7., 2., 2., 14.,
  2., 7., 2., 7., 2., 7., 2., 7., 7., 2., 2., 7., 7., 2., 2., 7., 2., 7., 7.,
  2., 2., 14., 2., 14., 2., 7., 2., 14., 2., 7., 7., 7., 7., 14., 7., 14., 7.
  } ;

static const int boeing2_rowndx[] = {-42,
  1, 3, 8, 9, 10, 135, 18, 1, 3, 8, 9, 10, 135, 24, 25, 1, 3, 8, 9, 10, 135, 30,
  31, 1, 3, 8, 9, 10, 135, 87, 88, 89, 1, 3, 8, 9, 10, 135, 102, 103, 1, 3, 8,
  9, 10, 136, 30, 1, 3, 8, 9, 10, 136, 87, 1, 3, 8, 9, 10, 136, 102, 103, 104,
  1, 3, 8, 9, 10, 136, 114, 1, 3, 8, 9, 10, 137, 21, 1, 3, 8, 9, 10, 137, 24, 1,
  3, 8, 9, 10, 137, 87, 88, 1, 3, 8, 9, 10, 137, 102, 1, 3, 8, 9, 10, 138, 57,
  58, 1, 3, 8, 9, 10, 138, 63, 64, 1, 3, 8, 9, 10, 138, 69, 70, 71, 1, 3, 8, 9,
  10, 138, 90, 91, 1, 3, 8, 9, 10, 138, 104, 105, 1, 3, 8, 9, 10, 139, 54, 1, 3,
  8, 9, 10, 139, 63, 1, 3, 8, 9, 10, 139, 69, 70, 1, 3, 8, 9, 10, 139, 104, 1,
  3, 8, 9, 10, 140, 51, 1, 3, 8, 9, 10, 140, 57, 1, 3, 8, 9, 10, 140, 69, 1, 3,
  8, 9, 10, 140, 90, 1, 3, 8, 9, 10, 141, 64, 1, 3, 8, 9, 10, 141, 71, 1, 3, 8,
  9, 10, 141, 82, 1, 3, 8, 9, 10, 141, 88, 89, 90, 91, 1, 3, 8, 9, 10, 141, 105,
  1, 3, 8, 9, 10, 141, 117, 1, 3, 8, 9, 10, 142, 31, 1, 3, 8, 9, 10, 142, 39, 1,
  3, 8, 9, 10, 142, 42, 43, 1, 3, 8, 9, 10, 142, 88, 89, 1, 3, 8, 9, 10, 143,
  36, 1, 3, 8, 9, 10, 143, 42, 1, 3, 8, 9, 10, 143, 88, 1, 3, 8, 9, 10, 144, 58,
  1, 3, 8, 9, 10, 144, 70, 71, 1, 3, 8, 9, 10, 144, 81, 82, 1, 3, 8, 9, 10, 144,
  91, 1, 3, 8, 9, 10, 144, 103, 104, 105, 1, 3, 8, 9, 10, 145, 25, 1, 3, 8, 9,
  10, 145, 43, 1, 3, 8, 9, 10, 145, 48, 1, 3, 8, 9, 10, 145, 89, 1, 3, 8, 9, 10,
  145, 103, 1, 3, 8, 9, 10, 146, 70, 1, 3, 8, 9, 10, 146, 78, 1, 3, 8, 9, 10,
  146, 81, 1, 3, 8, 9, 10, 146, 103, 104, 1, 3, 12, 13, 159, 19, 1, 3, 12, 13,
  159, 26, 27, 1, 3, 12, 13, 160, 22, 1, 3, 12, 13, 160, 26, 1, 3, 12, 13, 161,
  59, 60, 1, 3, 12, 13, 159, 20, 1, 3, 12, 13, 159, 28, 29, 1, 3, 12, 13, 159,
  34, 35, 1, 3, 12, 13, 159, 97, 98, 99, 1, 3, 12, 13, 159, 110, 111, 1, 3, 12,
  13, 160, 23, 1, 3, 12, 13, 160, 28, 1, 3, 12, 13, 160, 97, 98, 1, 3, 12, 13,
  160, 110, 1, 3, 12, 13, 161, 61, 62, 1, 3, 12, 13, 161, 67, 68, 1, 3, 12, 13,
  161, 75, 76, 77, 1, 3, 12, 13, 161, 100, 101, 1, 3, 12, 13, 161, 112, 113, 1,
  3, 12, 13, 162, 56, 1, 3, 12, 13, 162, 67, 1, 3, 12, 13, 162, 75, 76, 1, 3,
  12, 13, 162, 112, 1, 3, 12, 13, 163, 35, 1, 3, 12, 13, 163, 41, 1, 3, 12, 13,
  163, 46, 47, 1, 3, 12, 13, 163, 98, 99, 1, 3, 12, 13, 164, 38, 1, 3, 12, 13,
  164, 46, 1, 3, 12, 13, 164, 98, 1, 3, 12, 13, 165, 76, 1, 3, 12, 13, 165, 80,
  1, 3, 12, 13, 165, 85, 1, 3, 12, 13, 165, 111, 112, 14, 2, 3, 14, 2, 3, 15, 2,
  3, 15, 2, 3, 16, 2, 3, 17, 2, 3, 2, 3, 4, 5, 6, 7, 10, 14, 11, 13, 18, 19,
  121, 125, 147, 2, 3, 4, 5, 6, 7, 10, 15, 18, 122, 126, 147, 2, 3, 4, 5, 6, 16,
  11, 13, 20, 123, 127, 2, 3, 4, 5, 6, 17, 11, 13, 20, 124, 128, 2, 3, 4, 5, 6,
  7, 10, 14, 11, 13, 21, 22, 120, 121, 131, 149, 2, 3, 4, 5, 6, 7, 10, 15, 21,
  120, 122, 132, 149, 2, 3, 4, 5, 6, 16, 11, 13, 23, 120, 123, 133, 2, 3, 4, 5,
  6, 17, 11, 13, 23, 120, 124, 134, 2, 3, 4, 5, 6, 7, 10, 14, 11, 13, 24, 25,
  26, 27, 120, 121, 125, 149, 147, 157, 2, 3, 4, 5, 6, 7, 10, 15, 24, 25, 120,
  122, 126, 149, 147, 157, 2, 3, 4, 5, 6, 16, 11, 13, 28, 29, 120, 123, 127, 2,
  3, 4, 5, 6, 17, 11, 13, 28, 29, 120, 124, 128, 2, 3, 4, 5, 6, 7, 10, 15, 30,
  31, 122, 126, 148, 147, 154, 2, 3, 4, 5, 6, 17, 11, 13, 34, 35, 124, 128, 167,
  2, 3, 4, 5, 6, 7, 10, 15, 36, 120, 129, 132, 155, 2, 3, 4, 5, 6, 17, 11, 13,
  38, 120, 130, 134, 2, 3, 4, 5, 6, 7, 10, 15, 39, 129, 126, 154, 2, 3, 4, 5, 6,
  17, 11, 13, 41, 130, 128, 167, 2, 3, 4, 5, 6, 7, 10, 15, 42, 43, 120, 129,
  126, 155, 154, 157, 2, 3, 4, 5, 6, 17, 11, 13, 46, 47, 120, 130, 128, 167, 2,
  3, 4, 5, 6, 7, 10, 14, 11, 13, 48, 49, 120, 131, 125, 157, 2, 3, 4, 5, 6, 7,
  10, 15, 48, 120, 132, 126, 157, 2, 3, 4, 5, 6, 16, 11, 13, 50, 120, 133, 127,
  2, 3, 4, 5, 6, 17, 11, 13, 50, 120, 134, 128, 2, 3, 4, 5, 6, 7, 10, 14, 11,
  13, 51, 52, 120, 125, 131, 152, 2, 3, 4, 5, 6, 7, 10, 15, 51, 120, 126, 132,
  152, 2, 3, 4, 5, 6, 16, 11, 13, 53, 120, 127, 133, 2, 3, 4, 5, 6, 17, 11, 13,
  53, 120, 128, 134, 2, 3, 4, 5, 6, 7, 10, 15, 54, 126, 129, 151, 2, 3, 4, 5, 6,
  17, 11, 13, 56, 128, 130, 2, 3, 4, 5, 6, 7, 10, 14, 11, 13, 57, 58, 59, 60,
  120, 125, 121, 152, 150, 156, 2, 3, 4, 5, 6, 7, 10, 15, 57, 58, 120, 126, 122,
  152, 150, 156, 2, 3, 4, 5, 6, 16, 11, 13, 61, 62, 120, 127, 123, 166, 2, 3, 4,
  5, 6, 17, 11, 13, 61, 62, 120, 128, 124, 166, 2, 3, 4, 5, 6, 7, 10, 15, 63,
  64, 126, 122, 151, 150, 153, 2, 3, 4, 5, 6, 17, 11, 13, 67, 68, 128, 124, 166,
  2, 3, 4, 5, 6, 7, 10, 15, 69, 70, 71, 120, 126, 122, 152, 151, 150, 158, 156,
  153, 2, 3, 4, 5, 6, 17, 11, 13, 75, 76, 77, 120, 128, 124, 166, 2, 3, 4, 5, 6,
  7, 10, 15, 78, 120, 132, 129, 158, 2, 3, 4, 5, 6, 17, 11, 13, 80, 120, 134,
  130, 2, 3, 4, 5, 6, 7, 10, 15, 81, 82, 120, 132, 122, 158, 156, 153, 2, 3, 4,
  5, 6, 17, 11, 13, 85, 86, 120, 134, 124, 2, 3, 4, 5, 6, 7, 10, 15, 87, 88, 89,
  90, 91, 120, 148, 149, 147, 155, 154, 153, 157, 152, 150, 156, 2, 3, 4, 5, 6,
  17, 11, 13, 97, 98, 99, 100, 101, 120, 167, 166, 2, 3, 4, 5, 6, 7, 10, 15,
  102, 103, 104, 105, 120, 149, 147, 148, 157, 158, 156, 151, 150, 153, 2, 3, 4,
  5, 6, 17, 11, 13, 110, 111, 112, 113, 120, 166, 2, 3, 4, 5, 6, 7, 10, 15, 114,
  122, 129, 148, 2, 3, 4, 5, 6, 17, 11, 13, 116, 124, 130, 2, 3, 4, 5, 6, 7, 10,
  15, 117, 129, 122, 153, 2, 3, 4, 5, 6, 17, 11, 13, 119, 130, 124
  } ;
static const int boeing2_colndx[] = {-42,
  1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7,
  7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10,
  10, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 13, 13, 13,
  13, 13, 13, 13, 14, 14, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15, 15, 15,
  15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 17, 17, 17, 17, 17, 18,
  18, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19, 19, 19, 19, 20, 20, 20, 20, 20,
  20, 20, 21, 21, 21, 21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22, 22, 23, 23,
  23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25,
  26, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 28,
  28, 28, 29, 29, 29, 29, 29, 29, 29, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
  31, 31, 31, 31, 31, 31, 31, 32, 32, 32, 32, 32, 32, 32, 33, 33, 33, 33, 33,
  33, 33, 34, 34, 34, 34, 34, 34, 34, 35, 35, 35, 35, 35, 35, 35, 35, 36, 36,
  36, 36, 36, 36, 36, 36, 37, 37, 37, 37, 37, 37, 37, 38, 38, 38, 38, 38, 38,
  38, 39, 39, 39, 39, 39, 39, 39, 40, 40, 40, 40, 40, 40, 40, 41, 41, 41, 41,
  41, 41, 41, 41, 42, 42, 42, 42, 42, 42, 42, 42, 43, 43, 43, 43, 43, 43, 43,
  44, 44, 44, 44, 44, 44, 44, 44, 44, 45, 45, 45, 45, 45, 45, 45, 46, 46, 46,
  46, 46, 46, 46, 47, 47, 47, 47, 47, 47, 47, 48, 48, 48, 48, 48, 48, 48, 49,
  49, 49, 49, 49, 49, 49, 50, 50, 50, 50, 50, 50, 50, 51, 51, 51, 51, 51, 51,
  51, 52, 52, 52, 52, 52, 52, 52, 53, 53, 53, 53, 53, 53, 53, 53, 54, 54, 54,
  54, 54, 54, 55, 55, 55, 55, 55, 55, 55, 56, 56, 56, 56, 56, 56, 57, 57, 57,
  57, 57, 57, 58, 58, 58, 58, 58, 58, 58, 59, 59, 59, 59, 59, 59, 60, 60, 60,
  60, 60, 60, 60, 61, 61, 61, 61, 61, 61, 61, 62, 62, 62, 62, 62, 62, 62, 62,
  63, 63, 63, 63, 63, 63, 63, 64, 64, 64, 64, 64, 64, 65, 65, 65, 65, 65, 65,
  66, 66, 66, 66, 66, 66, 66, 67, 67, 67, 67, 67, 67, 68, 68, 68, 68, 68, 68,
  68, 69, 69, 69, 69, 69, 69, 69, 70, 70, 70, 70, 70, 70, 70, 70, 71, 71, 71,
  71, 71, 71, 71, 72, 72, 72, 72, 72, 72, 72, 73, 73, 73, 73, 73, 73, 74, 74,
  74, 74, 74, 74, 75, 75, 75, 75, 75, 75, 75, 76, 76, 76, 76, 76, 76, 77, 77,
  77, 77, 77, 77, 78, 78, 78, 78, 78, 78, 79, 79, 79, 79, 79, 79, 79, 80, 80,
  80, 80, 80, 80, 80, 81, 81, 81, 81, 81, 81, 82, 82, 82, 82, 82, 82, 83, 83,
  83, 83, 83, 83, 84, 84, 84, 84, 84, 84, 85, 85, 85, 85, 85, 85, 86, 86, 86,
  86, 86, 86, 87, 87, 87, 87, 87, 87, 87, 88, 88, 88, 89, 89, 89, 90, 90, 90,
  91, 91, 91, 92, 92, 92, 93, 93, 93, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 96, 96,
  96, 96, 96, 96, 96, 96, 96, 96, 96, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97,
  97, 98, 98, 98, 98, 98, 98, 98, 98, 98, 98, 98, 98, 98, 98, 98, 98, 99, 99,
  99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 100, 100, 100, 100, 100, 100, 100,
  100, 100, 100, 100, 100, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101,
  101, 101, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102,
  102, 102, 102, 102, 102, 102, 102, 103, 103, 103, 103, 103, 103, 103, 103,
  103, 103, 103, 103, 103, 103, 103, 103, 104, 104, 104, 104, 104, 104, 104,
  104, 104, 104, 104, 104, 104, 105, 105, 105, 105, 105, 105, 105, 105, 105,
  105, 105, 105, 105, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106,
  106, 106, 106, 106, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107,
  107, 107, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108,
  109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 110, 110, 110,
  110, 110, 110, 110, 110, 110, 110, 110, 110, 111, 111, 111, 111, 111, 111,
  111, 111, 111, 111, 111, 111, 112, 112, 112, 112, 112, 112, 112, 112, 112,
  112, 112, 112, 112, 112, 112, 112, 113, 113, 113, 113, 113, 113, 113, 113,
  113, 113, 113, 113, 113, 113, 114, 114, 114, 114, 114, 114, 114, 114, 114,
  114, 114, 114, 114, 114, 114, 114, 115, 115, 115, 115, 115, 115, 115, 115,
  115, 115, 115, 115, 115, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116,
  116, 116, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 118,
  118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118,
  119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 120, 120,
  120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 121, 121, 121, 121, 121,
  121, 121, 121, 121, 121, 121, 121, 122, 122, 122, 122, 122, 122, 122, 122,
  122, 122, 122, 122, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123,
  124, 124, 124, 124, 124, 124, 124, 124, 124, 124, 124, 124, 124, 124, 124,
  124, 124, 124, 124, 124, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
  125, 125, 125, 125, 125, 125, 126, 126, 126, 126, 126, 126, 126, 126, 126,
  126, 126, 126, 126, 126, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
  127, 127, 127, 127, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
  128, 128, 128, 128, 129, 129, 129, 129, 129, 129, 129, 129, 129, 129, 129,
  129, 129, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130, 130,
  130, 130, 130, 130, 130, 130, 130, 131, 131, 131, 131, 131, 131, 131, 131,
  131, 131, 131, 131, 131, 131, 131, 132, 132, 132, 132, 132, 132, 132, 132,
  132, 132, 132, 132, 132, 133, 133, 133, 133, 133, 133, 133, 133, 133, 133,
  133, 133, 134, 134, 134, 134, 134, 134, 134, 134, 134, 134, 134, 134, 134,
  134, 134, 134, 135, 135, 135, 135, 135, 135, 135, 135, 135, 135, 135, 135,
  135, 136, 136, 136, 136, 136, 136, 136, 136, 136, 136, 136, 136, 136, 136,
  136, 136, 136, 136, 136, 136, 136, 136, 136, 136, 137, 137, 137, 137, 137,
  137, 137, 137, 137, 137, 137, 137, 137, 137, 137, 137, 138, 138, 138, 138,
  138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138,
  138, 138, 138, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139,
  139, 139, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 141,
  141, 141, 141, 141, 141, 141, 141, 141, 141, 141, 142, 142, 142, 142, 142,
  142, 142, 142, 142, 142, 142, 142, 143, 143, 143, 143, 143, 143, 143, 143,
  143, 143, 143
  } ;
static const double boeing2_coeff[] = { -42.42,
  .075, -.075, 1., .86441, -.86441, 1., -1., .075, -.075, 1., .87605, -.87605,
  1., -1., -1., .075, -.075, 1., .91637, -.91637, 1., -1., -1., .075, -.075, 1.,
  .91722, -.91722, 1., -1., -1., -1., .075, -.075, 1., .87605, -.87605, 1., -1.,
  -1., .027, -.027, 1., .18557, -.18557, 1., -1., .027, -.027, 1., .18557,
  -.18557, 1., -1., .027, -.027, 1., 1.60685, -1.60685, 1., -1., -1., -1., .027,
  -.027, 1., .18557, -.18557, 1., -1., .053, -.053, 1., .56156, -.56156, 1.,
  -1., .053, -.053, 1., .56156, -.56156, 1., -1., .053, -.053, 1., .60273,
  -.60273, 1., -1., -1., .053, -.053, 1., .56156, -.56156, 1., -1., .075, -.075,
  1., .87605, -.87605, 1., -1., -1., .075, -.075, 1., .91637, -.91637, 1., -1.,
  -1., .075, -.075, 1., .91722, -.91722, 1., -1., -1., -1., .075, -.075, 1.,
  .87605, -.87605, 1., -1., -1., .075, -.075, 1., .91637, -.91637, 1., -1., -1.,
  .068, -.068, 1., .7308, -.7308, 1., -1., .068, -.068, 1., .7308, -.7308, 1.,
  -1., .068, -.068, 1., .73165, -.73165, 1., -1., -1., .068, -.068, 1., .7308,
  -.7308, 1., -1., .035, -.035, 1., .3145, -.3145, 1., -1., .035, -.035, 1.,
  .3145, -.3145, 1., -1., .035, -.035, 1., .3145, -.3145, 1., -1., .035, -.035,
  1., .3145, -.3145, 1., -1., .027, -.027, 1., .18557, -.18557, 1., -1., .027,
  -.027, 1., .18557, -.18557, 1., -1., .027, -.027, 1., .18557, -.18557, 1.,
  -1., .027, -.027, 1., 1.6077, -1.6077, 1., -1., -1., -1., -1., .027, -.027,
  1., .18557, -.18557, 1., -1., .027, -.027, 1., .18557, -.18557, 1., -1., .068,
  -.068, 1., .7308, -.7308, 1., -1., .068, -.068, 1., .7308, -.7308, 1., -1.,
  .068, -.068, 1., .73165, -.73165, 1., -1., -1., .068, -.068, 1., .73165,
  -.73165, 1., -1., -1., .037, -.037, 1., .41715, -.41715, 1., -1., .037, -.037,
  1., .41715, -.41715, 1., -1., .037, -.037, 1., .41715, -.41715, 1., -1., .053,
  -.053, 1., .56156, -.56156, 1., -1., .053, -.053, 1., .60273, -.60273, 1.,
  -1., -1., .053, -.053, 1., .60273, -.60273, 1., -1., -1., .053, -.053, 1.,
  .56156, -.56156, 1., -1., .053, -.053, 1., 1.23087, -1.23087, 1., -1., -1.,
  -1., .035, -.035, 1., .3145, -.3145, 1., -1., .035, -.035, 1., .3145, -.3145,
  1., -1., .035, -.035, 1., .3145, -.3145, 1., -1., .035, -.035, 1., .3145,
  -.3145, 1., -1., .035, -.035, 1., .3145, -.3145, 1., -1., .037, -.037, 1.,
  .41715, -.41715, 1., -1., .037, -.037, 1., .41715, -.41715, 1., -1., .037,
  -.037, 1., .41715, -.41715, 1., -1., .037, -.037, 1., 1.0453, -1.0453, 1.,
  -1., -1., .75, -.75, .86441, -.86441, 1., -1., .75, -.75, .87605, -.87605, 1.,
  -1., -1., .53, -.53, .56156, -.56156, 1., -1., .53, -.53, .56156, -.56156, 1.,
  -1., .75, -.75, .87605, -.87605, 1., -1., -1., .75, -.75, .86441, -.86441, 1.,
  -1., .75, -.75, .87605, -.87605, 1., -1., -1., .75, -.75, .91637, -.91637, 1.,
  -1., -1., .75, -.75, .91722, -.91722, 1., -1., -1., -1., .75, -.75, .87605,
  -.87605, 1., -1., -1., .53, -.53, .56156, -.56156, 1., -1., .53, -.53, .56156,
  -.56156, 1., -1., .53, -.53, .60273, -.60273, 1., -1., -1., .53, -.53, .56156,
  -.56156, 1., -1., .75, -.75, .87605, -.87605, 1., -1., -1., .75, -.75, .91637,
  -.91637, 1., -1., -1., .75, -.75, .91722, -.91722, 1., -1., -1., -1., .75,
  -.75, .87605, -.87605, 1., -1., -1., .75, -.75, .91637, -.91637, 1., -1., -1.,
  .68, -.68, .7308, -.7308, 1., -1., .68, -.68, .7308, -.7308, 1., -1., .68,
  -.68, .73165, -.73165, 1., -1., -1., .68, -.68, .7308, -.7308, 1., -1., .68,
  -.68, .7308, -.7308, 1., -1., .68, -.68, .7308, -.7308, 1., -1., .68, -.68,
  .73165, -.73165, 1., -1., -1., .68, -.68, .73165, -.73165, 1., -1., -1., .37,
  -.37, .41715, -.41715, 1., -1., .37, -.37, .41715, -.41715, 1., -1., .37,
  -.37, .41715, -.41715, 1., -1., .37, -.37, .41715, -.41715, 1., -1., .37,
  -.37, .41715, -.41715, 1., -1., .37, -.37, .41715, -.41715, 1., -1., .37,
  -.37, 1.0453, -1.0453, 1., -1., -1., 1., .65, .65, 1., -.05, -.05, 1., .275,
  .275, 1., -.02889, -.02889, 1., -.03611, -.03611, 1., -.01333, -.01333, .01,
  .01, 7.98429, 1., .86441, 351.81396, 211.088376, 2.32729, 25.93224, 12.96612,
  305., 12., 1., -1., 1., .02, .02, 2.51914, 1., .86441, 113.23743, 67.942458,
  2.10966, 98., 1., -1., 1., .03, .03, 3.15178, 1., .86441, 2.10966, 34.57631,
  17.288155, 30., 1., -1., .04, .04, 1.99337, 1., .86441, 2.10966, 12.96612,
  6.48306, 11.25, 1., -1., 4.04337, 4.04337, 5.83404, 1., .56156, 228.55299,
  137.131794, 1000., 16.84665, 8.423325, 305., 12., 1., 1., -1., 1., 1.60964,
  1.60964, 1.82258, 1., .56156, 73.56374, 44.138244, 2000., 98., 1., 1., -1.,
  1., 1.93119, 1.93119, 2.27351, 1., .56156, 3000., 22.4622, 11.2311, 30., 1.,
  1., -1., 1.34618, 1.34618, 1.41795, 1., .56156, 1500., 8.42333, 4.211665,
  11.25, 1., 1., -1., 1.1, 1.1, 9.91398, 2., .87605, 356.55371, 213.932226, 25.,
  26.2816, 13.1408, 305., 305., 12., 12., 2., 1., -1., 1., 1., 1., 1.2, 1.2,
  3.07692, 2., .87605, 114.76299, 68.857794, 25., 98., 98., 2., 1., -1., 1., 1.,
  1., 1.3, 1.3, 3.83055, 2., .87605, 26., 35.04214, 17.52107, 30., 30., 2., 1.,
  -1., 2.34647, 2.34647, 2.3665, 2., .87605, 27., 13.1408, 6.5704, 11.25, 11.25,
  2., 1., -1., 2.36783, 2.36783, 3.16965, 2., .91637, 120.04449, 72.026694,
  2.65943, 98., 98., 1., -1., 1., 1., 1., 1.90292, 1.90292, 2.4431, 2., .91637,
  2.65943, 13.74556, 6.87278, 11.25, 11.25, 1., -1., 1., 1.36416, 1.36416,
  1.49045, 1., .41715, 54.64705, 32.78823, 1.25093, 98., 1., 1., -1., 1.,
  1.14401, 1.14401, 1.14359, 1., .41715, 1.25093, 6.25729, 3.128645, 11.25, 1.,
  1., -1., 1.64736, 1.64736, 2.21183, 1., .7308, 95.73444, 57.440664, 1.85313,
  98., 1., -1., 1., 1.33312, 1.33312, 1.73951, 1., .7308, 1.85313, 10.96196,
  5.48098, 11.25, 1., -1., 1., 2.55381, 2.55381, 2.7448, 2., .73165, 95.8463,
  57.50778, 2.30477, 98., 98., 2., 1., -1., 1., 1., 1., 2.14431, 2.14431,
  2.09214, 2., .73165, 2.30477, 10.97477, 5.487385, 11.25, 11.25, 2., 1., -1.,
  1., 3.12679, 3.12679, 4.07994, 1., .3145, 128.00075, 76.80045, 1.34295,
  9.43495, 4.717475, 305., 12., 1., 1., -1., 1., 1.18965, 1.18965, 1.25435, 1.,
  .3145, 41.19926, 24.719556, 1.05384, 98., 1., 1., -1., 1., 1.42472, 1.42472,
  1.55704, 1., .3145, 1.05384, 12.57993, 6.289965, 30., 1., 1., -1., 1.0003,
  1.0003, .94855, 1., .3145, 1.05384, 4.71747, 2.358735, 11.25, 1., 1., -1.,
  3.12679, 3.12679, 4.07994, 1., .3145, 128.00075, 76.80045, 1.34295, 9.43495,
  4.717475, 305., 12., 1., 1., -1., 1., 1.18965, 1.18965, 1.25435, 1., .3145,
  41.19926, 24.719556, 1.05384, 98., 1., 1., -1., 1., 1.42472, 1.42472, 1.55704,
  1., .3145, 1.05384, 12.57993, 6.289965, 30., 1., 1., -1., 1.0003, 1.0003,
  .94855, 1., .3145, 1.05384, 4.71747, 2.358735, 11.25, 1., 1., -1., 1.64736,
  1.64736, 2.21183, 1., .7308, 95.73444, 57.440664, 1.85313, 98., 1., -1., 1.,
  1.33312, 1.33312, 1.73951, 1., .7308, 1.85313, 10.96196, 5.48098, 11.25, 1.,
  -1., 7.17016, 7.17016, 9.91398, 2., .87605, 356.55371, 213.932226, 3.12813,
  26.2816, 13.1408, 305., 305., 12., 12., 2., 1., -1., 1., 1., 1., 2.79929,
  2.79929, 3.07692, 2., .87605, 114.76299, 68.857794, 2.58202, 98., 98., 2., 1.,
  -1., 1., 1., 1., 3.35591, 3.35591, 3.83055, 2., .87605, 2.58202, 35.04214,
  17.52107, 30., 30., 2., 1., -1., 1., 2.34647, 2.34647, 2.3665, 2., .87605,
  2.58202, 13.1408, 6.5704, 11.25, 11.25, 2., 1., -1., 1., 2.36783, 2.36783,
  3.16965, 2., .91637, 120.04449, 72.026694, 2.65943, 98., 98., 1., -1., 1., 1.,
  1., 1.90292, 1.90292, 2.4431, 2., .91637, 2.65943, 13.74556, 6.87278, 11.25,
  11.25, 1., -1., 1., 3.27428, 3.27428, 3.70262, 3., .91722, 120.15637,
  72.093822, 3.11107, 98., 98., 98., 2., 1., -1., 1., 1., 1., 1., 1., 1.,
  2.71411, 2.71411, 2.79573, 3., .91722, 3.11107, 13.75836, 6.87918, 11.25,
  11.25, 11.25, 2., 1., -1., 1., 1.36416, 1.36416, 1.49045, 1., .41715,
  54.64705, 32.78823, 1.25093, 98., 1., 1., -1., 1., 1.14401, 1.14401, 1.14359,
  1., .41715, 1.25093, 6.25729, 3.128645, 11.25, 1., 1., -1., 2.08463, 2.08463,
  2.44827, 2., .60273, 78.95706, 47.374236, 2.05723, 98., 98., 1., 1., -1., 1.,
  1., 1., 1.71382, 1.71382, 1.84718, 2., .60273, 2.05723, 9.04089, 4.520445,
  11.25, 11.25, 1., 1., -1., 6.07357, 6.07357, 6.77953, 5., 1.79328, 234.91937,
  140.951622, 5.69309, 98., 98., 98., 98., 98., 4., 1., 1., 1., 1., 1., 1., 1.,
  1., 1., 1., 5.06059, 5.06059, 5.16223, 5., 1.79328, 5.69309, 26.89915,
  13.449575, 11.25, 11.25, 11.25, 11.25, 11.25, 4., 1., 1., 5.16712, 5.16712,
  6.24657, 4., 1.79242, 234.80756, 140.884536, 5.24145, 98., 98., 98., 98., 2.,
  1., 1., 1., 1., 1., 1., 1., 1., 1., 4.24939, 4.24939, 4.8096, 4., 1.79242,
  5.24145, 26.88635, 13.443175, 11.25, 11.25, 11.25, 11.25, 2., 1., .72047,
  .72047, .95782, 1., .18557, 24.31007, 14.586042, .8063, 98., 1., -1., 1.,
  .5698, .5698, .70359, 1., .18557, .8063, 2.7836, 1.3918, 11.25, 1., -1.,
  .72047, .72047, .95782, 1., .18557, 24.31007, 14.586042, .8063, 98., 1., -1.,
  1., .5698, .5698, .70359, 1., .18557, .8063, 2.7836, 1.3918, 11.25, 1., -1.
  } ;





#define INFTY_CHECK(zz_val_zz,zz_InftyIn_zz,zz_InftyOut_zz) \
(((zz_val_zz) == (zz_InftyIn_zz))?(zz_InftyOut_zz):\
    (((zz_val_zz) == (-zz_InftyIn_zz))?(-zz_InftyOut_zz):(zz_val_zz)))


static consys_struct *load_consys (
    const int m, const int n, const int q, const int maxColLen,
    const double infty,
    const char *probname,
    const int objndx, const char *objname,
    const char **rowname, const char *rowsense,
    const double *rlb, const double *rub,
    const char **colname, const double *vlb, const double *vub,
    const int *colndx, const int *rowndx, const double *coeff)
/*
  A routine to load a problem into a constraint system from initialised data
  arrays.  This routine has far too many parameters, but it'll do for the
  task at hand.  The idea is that a fairly simple awk script (or equivalent)
  has preprocessed an MPS file into a set of static arrays. This routine
  simply loads the arrays into a consys.

  Rows and columns are assumed to be numbered from 1, and position [0] of
  all arrays is garbage. This goes along with dylp's convention of 1-based
  indexing.

  The objective function is assumed to be included in (colndx,rowndx,coeff)
  just like any other row. A separate objective vector is built along with
  the constraint system, and the objective row is deleted from the constraint
  system at the end of the routine.

  Infinity is a never-ending pain. It's expected that the data arrays will
  use the value 1e100 as infinity (well outside any reasonable value, but
  well within the range of a float). This is (almost) hardwired --- a single
  local variable, mpsInfinity, is initialised to 1e100. Any occurrences of
  mpsInfinity in the input data will be replaced by the value infty passed in
  as a parameter.

  The rlb and rub arrays are expected to contain correct values as if for a
  range constraint. This routine will take care of dylp's idiosyncracies for
  G (>=) constraints.  It's expected that the preprocessing script will
  change the rowsense letter to R to indicate an honest range constraint (i.e.,
  finite lower and upper bounds).

  The routine will attempt to free the constraint system in the event of an
  error during construction. There's a nontrivial chance of failure.

  Parameters:
    m,n,q:	number of rows, columns, and coefficients, respectively
    maxColLen:	maximum number of nonzeros in a column
    infty:	the desired value for infinity (either IEEE infinity, or
		the classic finite infinity, DBL_MAX)
    probname:	the problem name
    objndx:	index of the objective function
    objname:	name of the objective function
    rowname:	names for rows (constraints)
    rowsense:	the rowsense letters N, E, L, G, from the MPS ROWS section,
		modified to include R for range constraints.
    rlb,rub:	row lower and upper bounds, respectively. See notes above.
    colname:	names for columns (variables)
    vlb,vub:	column lower and upper bounds, respectively.
    colndx,rowndx,coeff:
		three correlated arrays specifying the nonzero coefficients
		of the constraint matrix, in column-major order.

  Returns: a pointer to a constraint system, or NULL if an error occurs.
*/

{ int i,j,k,curCol,v ;
  const double mpsInfty = 1e100 ;

  pkvec_struct *ai,*aj ;
  pkcoeff_struct *coeffj ;

  consys_struct *sys ;
  char typlett ;
  contyp_enum ctypi ;
  double rlbi,rubi,cj,aij,vlbj,vubj ;

  bool retval ;

  int printlvl = 0 ;
  const char *rtnnme = "load_consys" ;

/*
  Define the desired set of attached vectors and any options.
*/
  const flags parts = CONSYS_OBJ |
		      CONSYS_VUB | CONSYS_VLB | CONSYS_VTYP |
		      CONSYS_RHS | CONSYS_RHSLOW | CONSYS_CTYP ;
  const flags opts = CONSYS_WRNATT ;

/*
  Start off by creating an empty constraint system with the required capacity.
  Insert the objective name while we're at it. Note that we have one extra row,
  the objective, which we'll filter out as the need arises.
*/
  sys = consys_create(probname,parts,opts,m,n,infty) ;
  if (sys == NULL)
  { dy_errmsg(152,rtnnme,probname) ;
    return (NULL) ; }
  consys_chgnme(sys,'o',0,objname) ;
/*
  Insert empty rows, excepting the objective function, which is kept
  separately. We need to convert the rowsense letter to the appropriate
  contyp_enum value, and we need to convert any mpsInfty values to the actual
  infty value.
*/
  ai = pkvec_new(0) ;
  for (i = 1 ; i <= m ; i++)
  { ai->nme = rowname[i] ;
    typlett = rowsense[i] ;
    switch (typlett)
    { case 'L':
      { ctypi = contypLE ;
	break ; }
      case 'E':
      { ctypi = contypEQ ;
	break ; }
      case 'R':
      { ctypi = contypRNG ;
	break ; }
      case 'G':
      { ctypi = contypGE ;
	break ; }
      case 'N':
      { ctypi = contypNB ;
	break ; }
      default:
      { ctypi = contypINV ;
	dy_errmsg(154,rtnnme,sys->nme,((unsigned) typlett),typlett,ai->nme) ;
	break ; } }
/*
  One of dylp's idiosyncracies is that it uses rhs to hold the right-hand-side
  for both >= and <= constraints. Because the rhs and rhslow arrays are
  uniformly assumed to be rhslow[i] <= ax <= rhs[i], we need a special case for
  a >= constraint.
*/
    if (ctypi == contypGE)
    { rlbi = INFTY_CHECK(rub[i],mpsInfty,infty) ;
      rubi = INFTY_CHECK(rlb[i],mpsInfty,infty) ; }
    else
    { rlbi = INFTY_CHECK(rlb[i],mpsInfty,infty) ;
      rubi = INFTY_CHECK(rub[i],mpsInfty,infty) ; }
    retval = consys_addrow_pk(sys,'c',ctypi,ai,rubi,rlbi,NULL,NULL) ;
#   ifndef DYLP_NDEBUG
    if (printlvl >= 2)
    { typlett = (ctypi == contypGE)?'>':'<' ;
      dyio_outfmt(dy_logchn,dy_gtxecho,
		  "%s: installing row %g <= %s (%d) <= %g ",
		  rtnnme,rlb[i],ai->nme,i,rub[i]) ;
      dyio_outfmt(dy_logchn,dy_gtxecho,"as %g %c= %s (%d, %s) %c= %g.\n",
		  rlbi,typlett,consys_nme(sys,'c',ai->ndx,FALSE,NULL),ai->ndx,
		  consys_prtcontyp(ctypi),typlett,rubi) ; }
#   endif
    if (retval == FALSE)
    { dy_errmsg(156,rtnnme,"row",sys->nme,ai->nme) ; }
    if (retval == FALSE || ctypi == contypINV)
    { retval = FALSE ;
      break ; } }
  if (ai != NULL) pkvec_free(ai) ;
  if (retval == FALSE)
  { consys_free(sys) ;
    return (NULL) ; }
/*
  Now load the coefficient matrix. We need to scan the column-major arrays
  that specify the coefficients and load the columns into memory.

  The approach is to scan along, loading a vector with coefficients, until we
  hit a change of column index. At that point, the current column is added to
  the constraint system and we start again.

  Do a bit of prep before we start into the scan loop: Allocate a packed
  vector large enough for the longest column, and make it look like colndx[1]
  is the current column, and reset column length and objective coefficient.
*/
  aj = pkvec_new(maxColLen) ;
  coeffj = aj->coeffs ;
  curCol = colndx[1] ;
  v = 0 ;
  cj = 0.0 ;

  for (k = 1 ; k <= q ; k++)
  { i = rowndx[k] ;
    if (i == objndx)
    { cj = INFTY_CHECK(coeff[k],mpsInfty,infty) ; }
    else
    { coeffj[v].ndx = i ;
      aij = INFTY_CHECK(coeff[k],mpsInfty,infty) ;
      coeffj[v].val = coeff[k] ;
      v++ ; }
/*
  Have we just processed the last coefficient for this column? If so, install
  it.
*/
    if (k+1 > q || colndx[k+1] != curCol)
    { j = curCol ;
      aj->cnt = v ;
      aj->nme = colname[j] ;
      vlbj = INFTY_CHECK(vlb[j],mpsInfty,infty) ;
      vubj = INFTY_CHECK(vub[j],mpsInfty,infty) ;
      retval = consys_addcol_pk(sys,vartypCON,aj,cj,vlbj,vubj) ;
#     ifndef DYLP_NDEBUG
      if (printlvl >= 2)
      { dyio_outfmt(dy_logchn,dy_gtxecho,
	      "%s: installing column %g <= %s (%d) <= %g, %d coefficients, ",
	      rtnnme,vlb[j],aj->nme,j,vub[j],aj->cnt) ;
	dyio_outfmt(dy_logchn,dy_gtxecho,"as %g <= %s (%d) <= %g, cj = %g.\n",
	      vlbj,consys_nme(sys,'v',aj->ndx,FALSE,NULL),aj->ndx,vubj,cj) ; }
#     endif
      if (retval == FALSE)
      { dy_errmsg(156,rtnnme,"column",sys->nme,aj->nme) ;
	break ; }
      if (k+1 <= q)
      { v = 0 ;
	cj = 0.0 ;
        curCol = colndx[k+1] ; } } }

  if (aj != NULL) pkvec_free(aj) ;
  if (retval == FALSE)
  { consys_free(sys) ;
    return (NULL) ; }
/*
  One final act: delete the objective function from the constraint system.
*/
  retval = consys_delrow_stable(sys,objndx) ;
  if (retval == FALSE)
  { dy_errmsg(112,rtnnme,sys->nme,"delete","constraint",
	      consys_nme(sys,'c',objndx,FALSE,NULL),objndx) ;
    consys_free(sys) ;
    return (NULL) ; }

  { return (sys) ; } }
    



/*
  Convenience routines to create specific constraint systems.
*/


consys_struct *dytest_exmip1sys (lptols_struct *tols)
/*
  Create a constraint system loaded with the exmip1 example.

  Parameters:
    tols:	lptols_struct, used to specify infinity

  Returns: pointer to a loaded constraint system, or NULL if there's an error.
*/

{ consys_struct *sys ;

  sys = load_consys(exmip1_rowcnt,exmip1_colcnt,exmip1_coeffcnt,
		    exmip1_maxColLen,tols->inf,"exmip1",
		    exmip1_objndx,exmip1_objname,
		    exmip1_rowname,exmip1_rowsense,exmip1_rowlb,exmip1_rowub,
		    exmip1_colname,exmip1_collb,exmip1_colub,
		    exmip1_colndx,exmip1_rowndx,exmip1_coeff) ;
  
  return (sys) ; }


consys_struct *dytest_exprimalraysys (lptols_struct *tols)
/*
  Create a constraint system loaded with the exprimalray example.

  Parameters:
    tols:	lptols_struct, used to specify infinity

  Returns: pointer to a loaded constraint system, or NULL if there's an error.
*/

{ consys_struct *sys ;

  sys = load_consys(exprimalray_rowcnt,exprimalray_colcnt,exprimalray_coeffcnt,
		    exprimalray_maxColLen,tols->inf,"exprimalray",
		    exprimalray_objndx,exprimalray_objname,
		    exprimalray_rowname,exprimalray_rowsense,
		    exprimalray_rowlb,exprimalray_rowub,
		    exprimalray_colname,exprimalray_collb,exprimalray_colub,
		    exprimalray_colndx,exprimalray_rowndx,exprimalray_coeff) ;
  
  return (sys) ; }

consys_struct *dytest_exdualraysys (lptols_struct *tols)
/*
  Create a constraint system loaded with the exdualray example.

  Parameters:
    tols:	lptols_struct, used to specify infinity

  Returns: pointer to a loaded constraint system, or NULL if there's an error.
*/

{ consys_struct *sys ;

  sys = load_consys(exdualray_rowcnt,exdualray_colcnt,exdualray_coeffcnt,
		    exdualray_maxColLen,tols->inf,"exdualray",
		    exdualray_objndx,exdualray_objname,
		    exdualray_rowname,exdualray_rowsense,
		    exdualray_rowlb,exdualray_rowub,
		    exdualray_colname,exdualray_collb,exdualray_colub,
		    exdualray_colndx,exdualray_rowndx,exdualray_coeff) ;
  
  return (sys) ; }

consys_struct *dytest_galenetsys (lptols_struct *tols)
/*
  Create a constraint system loaded with the galenet example.

  Parameters:
    tols:	lptols_struct, used to specify infinity

  Returns: pointer to a loaded constraint system, or NULL if there's an error.
*/

{ consys_struct *sys ;

  sys = load_consys(galenet_rowcnt,galenet_colcnt,galenet_coeffcnt,
		    galenet_maxColLen,tols->inf,"galenet",
		    galenet_objndx,galenet_objname,
		    galenet_rowname,galenet_rowsense,
		    galenet_rowlb,galenet_rowub,
		    galenet_colname,galenet_collb,galenet_colub,
		    galenet_colndx,galenet_rowndx,galenet_coeff) ;
  
  return (sys) ; }

consys_struct *dytest_galenetmixedsys (lptols_struct *tols)
/*
  Create a constraint system loaded with the galenetmixed example.

  Parameters:
    tols:	lptols_struct, used to specify infinity

  Returns: pointer to a loaded constraint system, or NULL if there's an error.
*/

{ consys_struct *sys ;

  sys = load_consys(galenetmixed_rowcnt,galenetmixed_colcnt,
		    galenetmixed_coeffcnt,
		    galenetmixed_maxColLen,tols->inf,"galenetmixed",
		    galenetmixed_objndx,galenetmixed_objname,
		    galenetmixed_rowname,galenetmixed_rowsense,
		    galenetmixed_rowlb,galenetmixed_rowub,
		    galenetmixed_colname,galenetmixed_collb,galenetmixed_colub,
		    galenetmixed_colndx,galenetmixed_rowndx,
		    galenetmixed_coeff) ;
  
  return (sys) ; }

consys_struct *dytest_galenetleqsys (lptols_struct *tols)
/*
  Create a constraint system loaded with the galenetleq example.

  Parameters:
    tols:	lptols_struct, used to specify infinity

  Returns: pointer to a loaded constraint system, or NULL if there's an error.
*/

{ consys_struct *sys ;

  sys = load_consys(galenetleq_rowcnt,galenetleq_colcnt,galenetleq_coeffcnt,
		    galenetleq_maxColLen,tols->inf,"galenetleq",
		    galenetleq_objndx,galenetleq_objname,
		    galenetleq_rowname,galenetleq_rowsense,
		    galenetleq_rowlb,galenetleq_rowub,
		    galenetleq_colname,galenetleq_collb,galenetleq_colub,
		    galenetleq_colndx,galenetleq_rowndx,galenetleq_coeff) ;
  
  return (sys) ; }

consys_struct *dytest_galenetbndssys (lptols_struct *tols)
/*
  Create a constraint system loaded with the galenetbnds example.

  Parameters:
    tols:	lptols_struct, used to specify infinity

  Returns: pointer to a loaded constraint system, or NULL if there's an error.
*/

{ consys_struct *sys ;

  sys = load_consys(galenetbnds_rowcnt,galenetbnds_colcnt,galenetbnds_coeffcnt,
		    galenetbnds_maxColLen,tols->inf,"galenetbnds",
		    galenetbnds_objndx,galenetbnds_objname,
		    galenetbnds_rowname,galenetbnds_rowsense,
		    galenetbnds_rowlb,galenetbnds_rowub,
		    galenetbnds_colname,galenetbnds_collb,galenetbnds_colub,
		    galenetbnds_colndx,galenetbnds_rowndx,galenetbnds_coeff) ;
  
  return (sys) ; }


consys_struct *dytest_afirosys (lptols_struct *tols)
/*
  Create a constraint system loaded with the afiro example.

  Parameters:
    tols:	lptols_struct, used to specify infinity

  Returns: pointer to a loaded constraint system, or NULL if there's an error.
*/

{ consys_struct *sys ;

  sys = load_consys(afiro_rowcnt,afiro_colcnt,afiro_coeffcnt,
		    afiro_maxColLen,tols->inf,"afiro",
		    afiro_objndx,afiro_objname,
		    afiro_rowname,afiro_rowsense,afiro_rowlb,afiro_rowub,
		    afiro_colname,afiro_collb,afiro_colub,
		    afiro_colndx,afiro_rowndx,afiro_coeff) ;
  
  return (sys) ; }


consys_struct *dytest_boeing2sys (lptols_struct *tols)
/*
  Create a constraint system loaded with the boeing2 example.

  Parameters:
    tols:	lptols_struct, used to specify infinity

  Returns: pointer to a loaded constraint system, or NULL if there's an error.
*/

{ consys_struct *sys ;

  sys = load_consys(boeing2_rowcnt,boeing2_colcnt,boeing2_coeffcnt,
		    boeing2_maxColLen,tols->inf,"boeing2",
		    boeing2_objndx,boeing2_objname,
		    boeing2_rowname,boeing2_rowsense,boeing2_rowlb,boeing2_rowub,
		    boeing2_colname,boeing2_collb,boeing2_colub,
		    boeing2_colndx,boeing2_rowndx,boeing2_coeff) ;
  
  return (sys) ; }
