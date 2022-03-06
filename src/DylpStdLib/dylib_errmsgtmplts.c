


//   This file is a part of the Dylp LP distribution.

//         Copyright (C) 2005 -- 2007 Lou Hafer

//         School of Computing Science
//         Simon Fraser University
//         Burnaby, B.C., V5A 1S6, Canada
//         lou@cs.sfu.ca

//   This code is licensed under the terms of the Eclipse Public License (EPL).

//   I haven't yet gone through and removed messages specific to the bonsaiG
//   MIP code. That'll happen eventually.

//   General error messages, useful pretty much anywhere

const char *dy_errmsgtmplts[] = {
  /*0*/ "<<unused>>",
  /*1*/ "internal confusion, line %d.",
  /*2*/ "%s parameter is null.",
  /*3*/ "%s parameter has invalid value '%c'.",
  /*4*/ "%s parameter has invalid value \"%s\".",
  /*5*/ "%s parameter has invalid value \"%d\".",
  /*6*/ "%s parameter has invalid value \"%#08x\".",
  /*7*/ "line %d: unrecognised %s %d.",
  /*8*/ "line %d: unable to allocate %d bytes.",
  /*9*/ "<<open>>",

//   Error messages used primarily by the basic io routines in io(io)

  /*10*/ "open failed for file \"%s\", mode \"%s\".",
  /*11*/ "close failed for file \"%s\".",
  /*12*/ "i/o error while reading from \"%s\".",
  /*13*/ "all available i/o streams are in use.",
  /*14*/ "error log file is open but has no name.",
  /*15*/ "stream %d is not active.",
  /*16*/ "file \"%s\" is not opened for input.",
  /*17*/ "file \"%s\" is not opened for output.",
  /*18*/ "failed to change error log file from \"%s\" to \"%s\".",
  /*19*/ "<<open>>",
  /*20*/ "<<open>>",
  /*21*/ "<<open>>",
  /*22*/ "<<open>>",
  /*23*/ "could not obtain mark for file \"%s\".",
  /*24*/ "could not position file \"%s\" to mark \"%ld\".",
  /*25*/ "pattern \"%s\" is too long, truncating to %d characters.",
  /*26*/ "lexeme beginning with \"%s\" is too long, truncating to %d characters.",
  /*27*/ "fixed-length string of length %d beginning with \"%s\" prematurely\n"
  /*27*/ "	truncated.",
  /*28*/ "string quoted with \"%c\", \"%c\" beginning with \"%s\" prematurely truncated.",
  /*29*/ "<<open>>",

//   Error messages used primarily by the routines of io(bnfrdr)

  /*30*/ "storage offset %d is outside legal range 0 to %d for the current node.",
  /*31*/ "node size %d is less than 0.",
  /*32*/ "null bnf reference encountered for component %d, %d components expected.",
  /*33*/ "null bnf definition in bnf reference.",
  /*34*/ "a %s is inappropriate as a component in a %s.",
  /*35*/ "invalid bnf reference type %d.",
  /*36*/ "bnf reference does not reference a generator.",
  /*37*/ "null component list encountered for alternative %d, %d alternatives\n"
  /*37*/ "	expected.",
  /*38*/ "bnf reference does not reference a non-primitive.",
  /*39*/ "bnf reference does not reference a primitive.",
  /*40*/ "saved text array index %d is outside legal range 0 to %d.",
  /*41*/ "bnf reference does not reference a terminal.",
  /*42*/ "terminal definition specifies invalid terminal type %d.",
  /*43*/ "bnf must begin with a generator, non-primitive, or primitive.",
  /*44*/ "<<open>>",
  /*45*/ "string \"%s\" cannot be converted to a number.",
  /*46*/ "base %d is not implemented for %s.",
  /*47*/ "bnf reference does not reference an immediate.",
  /*48*/ "bnf reference does not reference a literal.",
  /*49*/ "null text pointer in literal.",
  /*50*/ "no primitive is active.",
  /*51*/ "saved text array entry %d is null.",
  /*52*/ "bnf reference does not reference a label definition.",
  /*53*/ "null bnf specified for label %s in label definition.",
  /*54*/ "name %s specified for label value in label definition is not a defined\n"
  /*54*/ "	label.",
  /*55*/ "code %d specified for label %s in label definition is not valid.",
  /*56*/ "parse of bnf specified for label %s in label definition failed.",
  /*57*/ "null string obtained for label %s in label definition.",
  /*58*/ "null value obtained for label %s in label definition.",
  /*59*/ "null bnf specified for %s in label reference.",
  /*60*/ "parse of bnf specified for %s in label reference failed.",
  /*61*/ "null string obtained for %s name in label reference.",
  /*62*/ "null value obtained for socket in label reference.",
  /*63*/ "the value to be stored must be defined in a forward label reference.",
  /*64*/ "code %d specified for %s in label reference is not valid.",
  /*65*/ "null value obtained for value in label reference.",
  /*66*/ "the socket must be defined in a backward label reference.",
  /*67*/ "separator is not a primitive or terminal.",
  /*68*/ "reference requests storage but curnde is null.",
  /*69*/ "link offset %d is outside legal range 0 to %d for the current node.",
  /*70*/ "only generators, non-primitives, and primitives can be list components;\n"
  /*70*/ "	type %d is invalid.",
  /*71*/ "%s should normally be null at the completion of a parse.",
  /*72*/ "<<open>>",
  /*73*/ "<<open>>",
  /*74*/ "<<open>>",
  /*75*/ "<<open>>",
  /*76*/ "<<open>>",
  /*77*/ "<<open>>",
  /*78*/ "<<open>>",
  /*79*/ "<<open>>",
  /*80*/ "<<open>>",
  /*81*/ "<<open>>",
  /*82*/ "<<open>>",
  /*83*/ "<<open>>",
  /*84*/ "<<open>>",
  /*85*/ "<<open>>",
  /*86*/ "<<open>>",
  /*87*/ "<<open>>",
  /*88*/ "<<open>>",
  /*89*/ "<<open>>",

//   Error messages related to packed vectors

  /*90*/ "inconsistent packed vector %s (%d):\n"
  /*90*/ "	size %d, nonzero %d, coefficient array %sallocated.",
  /*91*/ "vector %s (%d) requires space for %d entries; resize specifies %d.",
  /*92*/ "vector %s (%d) has space for %d entries; %s %s (%d) has %d.",
  /*93*/ "in vector %s, default index = %d, default value = %g.",
  /*94*/ "in vector %s (%d), coeffs[%d].ndx = %d, coeffs[%d].val = %g.",
  /*95*/ "vector %#08x has null name.",
  /*96*/ "<<open>>",
  /*97*/ "<<open>>",
  /*98*/ "<<open>>",
  /*99*/ "<<open>>",

//   Error messages related to constraint system care and maintenance

  /*100*/ "[%s]: unable to attach \"%s\".",
  /*101*/ "[%s]: %s is missing.",
  /*102*/ "[%s]: %s index %d violates range %d..%d.",
  /*103*/ "[%s]: missing header for %s %d.",
  /*104*/ "[%s]: vec = %#08x not on attached vector list.",
  /*105*/ "[%s]: unable to detach \"%s\".",
  /*106*/ "[%s]: element size for vector %#08x is %d.",
  /*107*/ "[%s]: duplicate %s attach request, vec = %#08x, pvec = %#08x.",
  /*108*/ "[%s]: no reference list for attached %s %#08x.",
  /*109*/ "[%s]: pvec = %#08x not on reference list for %s %#08x.",
  /*110*/ "[%s]: empty reference at %#08x on reference list for %s %#08x.",
  /*111*/ "[%s]: unable to update references to %s from %#08x to %#08x.",
  /*112*/ "[%s]: %s failed for %s %s (%d).",
  /*113*/ "[%s]: parts flags indicate %s is present, but associated\n"
  /*113*/ "	pointer field is null.",
  /*114*/ "[%s]: unrecognised %s vector type(s) %#08x.",
  /*115*/ "[%s]: constraint system is flagged as corrupt.",
  /*116*/ "[%s]: row %s (%d) claims %d coefficients but has %d.",
  /*117*/ "<<open>>",
  /*118*/ "[%s]: %s %s (%d) has length 0.",
  /*119*/ "[%s]: coefficient a<%d,%d> = %g\n"
  /*119*/ "	appears in %s %d but not %s %d.",
  /*120*/ "[%s]: unable to properly install %s for constraint %s (%d);\n"
  /*120*/ "	required associated vectors are missing.",
  /*121*/ "[%s]: unable to add logical variable for\n"
  /*121*/ "	constraint %s (%d).",
  /*122*/ "[%s]: could not retrieve %s %s (%d).",
  /*123*/ "[%s] already has logical variables!",
  /*124*/ "[%s]: could not expand to incorporate logical variables.",
  /*125*/ "[%s]: null/incorrect %s in matrix coefficient %#08x, %s %s (%d).",
  /*126*/ "[%s]: internal confusion; %s hdr = %#08x, hdr.ndx = %d, but\n"
  /*126*/ "	mtx.hdr[%d] == %#08x.",
  /*127*/ "[%s]: null vector pointer in header (%#08x) for attached\n"
  /*127*/ "	vector type %s.",
  /*128*/ "[%s]: coefficient a<%d,%d> = %g encountered while adding %s %s.\n"
  /*128*/ "	Aborting due to invalid value.",
  /*129*/ "[%s]: addition of %s (%d) gives %d %s; allocated\n"
  /*129*/ "	capacity is only %d.",
  /*130*/ "[%s]: infinitesimal a<%d,%d> = %.10g < %.10g suppressed\n"
  /*130*/ "	while adding %s %s.",
  /*131*/ "[%s]: logical variable count %d does not equal\n"
  /*131*/ "	constraint count %d.",
  /*132*/ "[%s]: multiplication of %s %s (%d) by 0.",
  /*133*/ "[%s]: failed to set coefficient a<%d,%d> to %g.",
  /*134*/ "[%s]: %s %s (%d) claims %d coefficients but has %s pointer.",
  /*135*/ "[%s]: scaling failure.",
  /*136*/ "[%s]: %s error while duplicating %s, src = %d, dst = %d.",
  /*137*/ "[%s]: failed to duplicate constraint system.",
  /*138*/ "[%s]: failed to evaluate constraint system for scaling.",
  /*139*/ "<<open>>",
  /*140*/ "<<open>>",
  /*141*/ "<<open>>",
  /*142*/ "<<open>>",
  /*143*/ "<<open>>",
  /*144*/ "<<open>>",
  /*145*/ "<<open>>",
  /*146*/ "<<open>>",
  /*147*/ "<<open>>",
  /*148*/ "<<open>>",
  /*149*/ "<<open>>",

//   Error messages used primarily by the MPS i/o package.

  /*150*/ "expecting the keyword \"%s\", instead found \"%s\".",
  /*151*/ "mangled %s %s record.",
  /*152*/ "could not create constraint system \"%s\".",
  /*153*/ "null %s name while processing %s section.",
  /*154*/ "[%s]: unrecognised constraint type code %d (%c) at constraint %s.",
  /*155*/ "could not enter %s %s into hash table.",
  /*156*/ "could not install %s %s.%s.",
  /*157*/ "unexpected additional field \"%s\", %s %s.",
  /*158*/ "row name or 'marker' missing in entry %s.%s.",
  /*159*/ "marker keyword missing in entry %s.%s.",
  /*160*/ "can't process %s section; %s vector is missing.",
  /*161*/ "'%s' (%s) seen, but no '%s'!",
  /*162*/ "couldn't locate %s \"%s\", entry %s %s.",
  /*163*/ "unrecognised marker keyword \"%s\" at marker %s.",
  /*164*/ "null coefficient, system %s, column %s, row %s.",
  /*165*/ "cannot convert \"%s\" to number, system %s, column %s, row %s.",
  /*166*/ "couldn't locate %s \"%s\", %s %s.%s.",
  /*167*/ "unrecognised bound type code \"%s\" for variable %s (%d).",
  /*168*/ "i/o error while attempting %s section.",
  /*169*/ "unexpected eof while attempting %s section.",
  /*170*/ "cannot process mps input file \"%s\"; aborting.",
  /*171*/ "constructed constraints (Dx code) are not accepted;\n"
  /*171*/ "	define constraint %s explicitly.",
  /*172*/ "discarding non-binding constraint %s.%s.",
  /*173*/ "[%s]: attempt to multiply objective %s (%d)\n"
  /*173*/ "	by -1 for maximisation failed.",
  /*174*/ "vector resize failed while collecting coefficients for column %s.%s.",
  /*175*/ "non-binding constraint %s (%d) should not have a rhs range.",
  /*176*/ "constraint %s (%d) is supposedly a Type 3 SOS constraint,\n"
  /*176*/ "	but it has type %s instead of equality.",
  /*177*/ "[%s]: did not find objective function \"%s\".",
  /*178*/ "[%s]: %s %s (%d) has invalid type %s.",
  /*179*/ "[%s]: deleting empty constraint %s (%d).",
  /*180*/ "<<open>>",
  /*181*/ "<<open>>",
  /*182*/ "<<open>>",
  /*183*/ "<<open>>",
  /*184*/ "<<open>>",
  /*185*/ "<<open>>",
  /*186*/ "<<open>>",
  /*187*/ "<<open>>",
  /*188*/ "<<open>>",
  /*189*/ "<<open>>",
  /*190*/ "<<open>>",
  /*191*/ "<<open>>",
  /*192*/ "<<open>>",
  /*193*/ "<<open>>",
  /*194*/ "<<open>>",
  /*195*/ "<<open>>",
  /*196*/ "<<open>>",
  /*197*/ "<<open>>",
  /*198*/ "<<open>>",
  /*199*/ "<<open>>",

//   Error messages from the main program

  /*200*/ "cannot get system time.",
  /*201*/ "could not open log file %s; logging disabled.",
  /*202*/ "[%s]: could not create search tree root.",
  /*203*/ "could not initialise the arc consistency package.",
  /*204*/ "[%s]: constraint propagation for %s failed, T%d:#%d.",
  /*205*/ "[%s]: failed to create branch-and-bound search tree root.",
  /*206*/ "[%s]: failed %s at T%d from subproblem %d@%d.",
  /*207*/ "[%s]: failed to process subproblems after tour %d.",
  /*208*/ "[%s]: failed to ready new active subproblem following tour %d.",
  /*209*/ "[%s]: failed to regenerate incumbent T%d:#%d@%d.",
  /*210*/ "(T%d:#%d@%d): coefficient reduction failed for %s.",
  /*211*/ "could not %s %s signal handler.",
  /*212*/ "(T%d:#%d@%d): purging %s constraints failed for %s.",
  /*213*/ "<<open>>",
  /*214*/ "<<open>>",
  /*215*/ "<<open>>",
  /*216*/ "<<open>>",
  /*217*/ "<<open>>",
  /*218*/ "<<open>>",
  /*219*/ "<<open>>",
  /*220*/ "<<open>>",
  /*221*/ "<<open>>",
  /*222*/ "<<open>>",
  /*223*/ "<<open>>",
  /*224*/ "<<open>>",
  /*225*/ "<<open>>",
  /*226*/ "<<open>>",
  /*227*/ "<<open>>",
  /*228*/ "<<open>>",
  /*229*/ "<<open>>",


//   Error messages from the command interpreter

  /*230*/ "unexpected command text \"%s\".",
  /*231*/ "<<open>>",
  /*232*/ "indirect command file \"%s\" did not close properly.",
  /*233*/ "ambiguous command keyword \"%s\".",
  /*234*/ "unknown command keyword \"%s\".",
  /*235*/ "unrecoverable error; command interpretation aborted.",
  /*236*/ "failed to scan %s %s, %s command.",
  /*237*/ "<<open>>",
  /*238*/ "<<open>>",
  /*239*/ "<<open>>",
  /*240*/ "parser error while attempting bnf %s.",
  /*241*/ "enforcing %d <= \"%s\" <= %d; %d forced to %d.",
  /*242*/ "ignoring invalid value %g for \"%s\".",
  /*243*/ "\"%s\" defaulted to %d.",
  /*244*/ "enforcing %.2f <= \"%s\" <= %.2f; %.2f forced to %.2f.",
  /*245*/ "\"%s\" defaulted to %.8g.",
  /*246*/ "\"%s\" defaulted to \"%s\".",
  /*247*/ "<<open>>",
  /*248*/ "<<open>>",
  /*249*/ "<<open>>",
  /*250*/ "<<open>>",
  /*251*/ "<<open>>",
  /*252*/ "<<open>>",
  /*253*/ "<<open>>",
  /*254*/ "<<open>>",
  /*255*/ "<<open>>",
  /*256*/ "<<open>>",
  /*257*/ "<<open>>",
  /*258*/ "<<open>>",
  /*259*/ "<<open>>",
  /*260*/ "<<open>>",
  /*261*/ "<<open>>",
  /*262*/ "<<open>>",
  /*263*/ "<<open>>",
  /*264*/ "<<open>>",
  /*265*/ "<<open>>",
  /*266*/ "<<open>>",
  /*267*/ "<<open>>",
  /*268*/ "<<open>>",
  /*269*/ "<<open>>",
  /*270*/ "<<open>>",
  /*271*/ "<<open>>",
  /*272*/ "<<open>>",
  /*273*/ "<<open>>",
  /*274*/ "<<open>>",
  /*275*/ "<<open>>",
  /*276*/ "<<open>>",
  /*277*/ "<<open>>",
  /*278*/ "<<open>>",
  /*279*/ "<<open>>",
  /*280*/ "<<open>>",
  /*281*/ "<<open>>",
  /*282*/ "<<open>>",
  /*283*/ "<<open>>",
  /*284*/ "<<open>>",
  /*285*/ "<<open>>",
  /*286*/ "<<open>>",
  /*287*/ "<<open>>",
  /*288*/ "<<open>>",
  /*289*/ "<<open>>",
  /*290*/ "<<open>>",
  /*291*/ "<<open>>",
  /*292*/ "<<open>>",
  /*293*/ "<<open>>",
  /*294*/ "<<open>>",
  /*295*/ "<<open>>",
  /*296*/ "<<open>>",
  /*297*/ "<<open>>",
  /*298*/ "<<open>>",
  /*299*/ "<<open>>",

//   Error messages from the LP code

  /*300*/ "nonsense status %#08x for variable %s (%d).",
  /*301*/ "found only %d of the %d basic variables required to form\n"
  /*301*/ "	a basis for constraint system %s.",
  /*302*/ "[%s]: (%s)%d: could not %s an initial basis.",
  /*303*/ "[%s]: in basis pos'n %d, variable index violates %d <= %d <= %d.",
  /*304*/ "[%s]: (%s)%d: error while testing primal/dual feasibility.",
  /*305*/ "@[%s]: (%s)%d: could not %s %s variables for\n"
  /*305*/ "	inclusion in initial basis.",
  /*306*/ "[%s]: (%s)%d: error attempting to correct LP data structures\n"
  /*306*/ "	after patch of singular basis.",
  /*307*/ "[%s]: (%s)%d: aborting attempt to factor basis due to numerical\n"
  /*307*/ "	instability. Pivot parameters at %s.",
  /*308*/ "[%s]: (%s)%d: aborting attempt to factor basis, return code %s.",
  /*309*/ "[%s]: could not factor basis.",
  /*310*/ "error calculating orientation of constraint %s (%d) to objective.",
  /*311*/ "[%s] has no logical variables.",
  /*312*/ "[%s]: (%s)%d: could not sort constraints before forming active system.",
  /*313*/ "[%s]: (%s)%d: unable to load active system from %s.",
  /*314*/ "HUGE_VAL = %g, not IEEE infinity. If you really want to use IEEE\n"
  /*314*/ "	infinity, consider FIX_HUGE_VAL in vector.h, and confirm that\n"
  /*314*/ "	your environment supports the IEEE FP standard.",
  /*315*/ "variable %s (%d) is non-basic with status %s but has\n"
  /*315*/ "	an absolute value of infinity.",
  /*316*/ "[%s]: could not calculate value of primal basic variables.",
  /*317*/ "[%s]: antidegeneracy algorithm active at level %d on\n"
  /*317*/ "	entry to primal phase 1.",
  /*318*/ "[%s]: (%s)%d: could not %s phase I objective function.",
  /*319*/ "[%s] is already feasible; phase I should not have happened.",
  /*320*/ "[%s]: (%s)%d: could not calculate dot(%s,a<%d>) for %s.",
  /*321*/ "[%s]: (%s)%d: %s %s[%d] = %g, should be %g; |diff| = %g, tol = %g.",
  /*322*/ "[%s]: (%s)%d: %s (%d), status %s, violates\n"
  /*322*/ "	lb = %g < %g < %g = ub; err = %g; tol = %g.",
  /*323*/ "[%s]: (%s)%d: unexpected loss of primal feasibility ---\n"
  /*323*/ "	%s (%d), status %s, violates lb = %g <= %g <= %g = ub\n"
  /*323*/ "	err = %g; tol = %g.",
  /*324*/ "[%s]: (%s)%d: apparent unboundedness.",
  /*325*/ "[%s]: (%s)%d: variable %s (%d) = %g, lb = %g, ub = %g, status %s.\n"
  /*325*/ "	Nonbasic variables should be within bounds.",
  /*326*/ "[%s]: (%s)%d: variable %s (%d) = %g = %s, status %s.\n"
  /*326*/ "	This variable should not have been chosen to enter at %s.",
  /*327*/ "[%s]: attempted to form restricted subproblem but found no\n"
  /*327*/ "	candidates.",
  /*328*/ "[%s]: limit of %d total pivots exceeded.",
  /*329*/ "[%s]: (%s)%d: variable %s (%d) on pivot reject list pos'n %d\n"
  /*329*/ "	has status %s; should be %s with nopivot qualifier.",
  /*330*/ "[%s]: (%s)%d: variable %s (%d) has status %s,\n"
  /*330*/ "	var2basis gives pos'n %d, but basis pos'n holds index %d.",
  /*331*/ "[%s]: (%s)%d: variable %s (%d) = %g has status %s, but\n"
  /*331*/ "	lb = %g != %g = ub; err = %g, tol = %g.",
  /*332*/ "[%s](%s)%d: %s (%d), status %s,\n"
  /*332*/ "	x = %g != x<B> = %g, |x - x<B>| = %g, tol = %g.",
  /*333*/ "[%s]: (%s)%d: variable %s (%d) has status %s\n"
  /*333*/ "	but x = %g != %g = %s; err = %g; tol = %g.",
  /*334*/ "[%s]: (%s)%d: variable %s (%d) has lb = %g, ub = %g, but status %s;\n"
  /*334*/ "	it should not be free with a finite bound.",
  /*335*/ "[%s]: (%s)%d: variable %s (%d) has status %s but is within\n"
  /*335*/ "	bounds, lb = %g <= %g <= %g = ub; err = %g; tol = %g.",
  /*336*/ "[%s]: (%s)%d: for %s (%d) %s, %s -y<i> = %g != %g = cbar<i>,\n"
  /*336*/ "	diff = %g, tol = %g.",
  /*337*/ "[%s]: could not expand pivot reject list capacity from\n"
  /*337*/ "	%d entries to %d entries.",
  /*338*/ "[%s]: loss of %s feasibility should not be reported\n"
  /*338*/ "	in primal phase I.",
  /*339*/ "[%s]: (%s)%d: cycling suspected; %d pivots with no change in objective.",
  /*340*/ "[%s]: could not construct reduced right-hand-side (b - Nx<N>).",
  /*341*/ "[%s]: (%s)%d: %s accuracy check failed;\n"
  /*341*/ "	1+norm(%c) = %g; residual %g, tolerance %g.",
  /*342*/ "[%s]: %s (%d) was in degenerate set, but restored value %g\n"
  /*342*/ "	is not at lb = %g or ub = %g; %s %g, tolerance %g.",
  /*343*/ "[%s]: fatal error at pivot %d.",
  /*344*/ "[%s]: %s (%d) will overshoot feasibility from %s at pivot %d;\n"
  /*344*/ "	val = %g, lb = %g, ub = %g, delta = %g.",
  /*345*/ "[%s]: attempt to change value of fixed variable\n"
  /*345*/ "	%s (%d) = %g at pivot %d; delta = %g.",
  /*346*/ "[%s]: (%s)%d: status %s for %s (%d)\n"
  /*346*/ "	is not legal in this phase.",
  /*347*/ "[%s]: (%s)%d: unexpected loss of dual feasibility ---\n"
  /*347*/ "	%s (%d) status %s, cbar<%d> = %g has the wrong sign, tol = %g.",
  /*348*/ "@[%s]: (%s)%d: failed to price entering variable %s (%d).",
  /*349*/ "[%s]: variable %s (%d) = %g, status %s, is within bounds\n"
  /*349*/ "	lb = %g, ub = %g; it should not have been chosen to leave.",
  /*350*/ "[%s]: (%s)%d: predicted %s infeasibility = %g from iter %d\n"
  /*350*/ "	does not match starting infeasibility = %g; err = %g.",
  /*351*/ "[%s]: (%s)%d: constraint system is %d x %d.",
  /*352*/ "<<open>>",
  /*353*/ "[%s]: %s simplex returned %s; aborting.",
  /*354*/ "<<open>>",
  /*355*/ "[%s]: entering variable %s (%d) has status %s;\n"
  /*355*/ "	dual simplex is only prepared for non-basic at upper/lower bound.",
  /*356*/ "[%s]: %s cross-reference failure.\n"
  /*356*/ "	original[%d] = %d but active[%d] = %d.",
  /*357*/ "[%s]: status of leaving variable %s (%d) is %s,\n"
  /*357*/ "	%s = %g, val = %g, err = %g, tol = %g;\n"
  /*357*/ "	should be nonbasic at upper/lower bound or fixed.",
  /*358*/ "[%s]: delta<j> is 0 for dual pivot on %s (%d);\n"
  /*358*/ "	lb<i> = %g; x<i> = %g ; ub<i> = %g ; abar<i,j> = %g.",
  /*359*/ "[%s]: %s (%d) has status %s in final answer.",
  /*360*/ "[%s]: variable cross-reference failure.\n"
  /*360*/ "	active[%d] = %d; entries for logicals should be 0.",
  /*361*/ "[%s]: %s count of %d disagrees with %d entries coded as active\n"
  /*361*/ "	in cross reference vector.",
  /*362*/ "[%s]: could not locate coefficient for active\n"
  /*362*/ "	%s (%d) in constraint %s (%d).",
  /*363*/ "<<open>>",
  /*364*/ "[%s]: constraint %s (%d) should have %d coefficients\n"
  /*364*/ "	for active architectural variables, but has %d.",
  /*365*/ "[%s]: can't find coefficient for logical %s (%d)\n"
  /*365*/ "	in constraint %s (%d).",
  /*366*/ "[%s]: rhs<%d> = %g for constraint %s\n"
  /*366*/ "	should be %g; error %g; tol. %g;\n"
  /*366*/ "	original rhs<%d> for %s is %g, correction %g.",
  /*367*/ "[%s]: %s %s (%d) = %g != %g = %s %s (%d)\n"
  /*367*/ "	diff = %g, tol = %g.",
  /*368*/ "<<open>>",
  /*369*/ "[%s]: %s %s (%d) should be active but isn't; orig%s[%d] = %d.",
  /*370*/ "[%s]: expected original constraint %s (%d) in\n"
  /*370*/ "	basis pos'n %d, but found %s (%d).",
  /*371*/ "[%s]: (%s)%d: %s failed.",
  /*372*/ "[%s]: (%s)%d: %s (%d) has status %s, requires major correction to %s;\n"
  /*372*/ "	lb<i> = %g, x<i> = %g, ub<i> = %g,  x<i> - %s<i> = %g, tol. = %g.",
  /*373*/ "[%s]: inconsistent basis status after refactorisation.",
  /*374*/ "[%s]: (%s)%d: forcing refactor (bogus number);\n"
  /*374*/ "	|%s<%d>| = |%g| < bogus tol %g by %g.",
  /*375*/ "[%s]: (%s)%d: forcing refactor (bogus number);\n"
  /*375*/ "	|%s<%d>-%s<%d>| = |%g-%g| = %g < bogus tol %g by %g.",
  /*376*/ "[%s]: (%s)%d: temporarily lowering pivot tolerance from %g to %g.",
  /*377*/ "<<open>>",
  /*378*/ "<<open>>",
  /*379*/ "[%s]: (%s)%d: %s (%d) has lb = %g = ub, status %s;\n"
  /*379*/ "	status should be one of BFX, NBFX, BLLB, or BUUB.",
  /*380*/ "[%s]: %s (%d) has status %s, should be %s.",
  /*381*/ "[%s]: (%s)%d: forcing refactor on suspicion of numerical error;\n"
  /*381*/ "	|%s<%d,%d>| = |%g| < bogosity tolerance %g by %g.",
  /*382*/ "[%s]: (%s)%d: variable %s (%d) to leave at %s = %g;\n"
  /*382*/ "	this should not be!",
  /*383*/ "[%s]: (%s)%d: pivot multiplier would be below limit %g. Declaring %s.",
  /*384*/ "[%s]: (%s)%d: could not calculate reduced costs.",
  /*385*/ "[%s]: (%s)%d: pivot abar<%d,%d> = %g; should be %g, error %g (%g%%), tol %g.",
  /*386*/ "[%s]: (%s)%d: no %s variable selected\n"
  /*386*/ "	at start of pivot loop.",
  /*387*/ "[%s]: (%s)%d: %d false terminations;\n"
  /*387*/ "	retry limit exceeded; aborting.",
  /*388*/ "[%s]: (%s)%d: %s<%d> update error;\n"
  /*388*/ "	update = %g; calc = %g; |error| = %g; tol = %g.",
  /*389*/ "[%s]: (%s)%d: %s (%d) feasible %s but c<%d> = %g; should be 0.",
  /*390*/ "[%s]: (%s)%d: degenset[%d] = %d for %s (%d), %s, but\n"
  /*390*/ "	antidegeneracy level is only %d.",
  /*391*/ "[%s]: (%s)%d: failed to clear superbasics prior to return to phase I.",
  /*392*/ "[%s]: (%s)%d: could not calculate beta<%d> or abar<%d>\n"
  /*392*/ "	for dual pivot; leaving variable %s (%d).",
  /*393*/ "<<open>>",
  /*394*/ "[%s]: (%s)%d: inconsistency in rho<%d> for pivot row;\n"
  /*394*/ "	inv(B)beta<%d> = %g, ||beta<%d>||^2 = %g, err = %g, tol = %g.",
  /*395*/ "[%s]: previous return code %s; cannot hot start.",
  /*396*/ "[%s]: client %#08x does not own solver; current owner %#08x; cannot %s.",
  /*397*/ "<<open>>",
  /*398*/ "[%s]: basic <-> nonbasic conversion illegal on hot start;\n"
  /*398*/ "	attempt to convert %s (%d) from %s to %s.",
  /*399*/ "[%s]: (%s)%d: phase %s objective already installed?",
  /*400*/ "[%s]: (%s)%d: type %d evaluation routine failed for %s (%d).",
  /*401*/ "[%s]: (%s)%d: %g = dot(eta<%d>,a<%d>) != abar<%d,%d> = %g;\n"
  /*401*/ "		err = %g, tol = %g.",
  /*402*/ "[%s]: (%s)%d: logical %s (%d) has %d coefficients;\n"
  /*402*/ "	it should have exactly one.",
  /*403*/ "duplicate %s requested, but source not supplied.",
  /*404*/ "requested destination %s size of %d\n"
  /*404*/ "	is less than source size of %d; boosting to match. ",
  /*405*/ "[%s]: (%s)%d: updated objective differs from calculated objective;\n"
  /*405*/ "	z<upd> = %g, z<calc> = %g, diff = %g, tol = %g.",
  /*406*/ "[%s]: initialization/scaling of constraint system failed.",
  /*407*/ "<<open>>",
  /*408*/ "<<open>>",
  /*409*/ "<<open>>",
  /*410*/ "<<open>>",
  /*411*/ "<<open>>",
  /*412*/ "<<open>>",
  /*413*/ "<<open>>",
  /*414*/ "<<open>>",
  /*415*/ "<<open>>",
  /*416*/ "<<open>>",
  /*417*/ "<<open>>",
  /*418*/ "<<open>>",
  /*419*/ "<<open>>",
  /*420*/ "<<open>>",
  /*421*/ "<<open>>",
  /*422*/ "<<open>>",
  /*423*/ "<<open>>",
  /*424*/ "<<open>>",
  /*425*/ "<<open>>",
  /*426*/ "[%s]: logical %s (%d) for %s constraint %s (%d)\n"
  /*426*/ "	has improper %s bound %g.",
  /*427*/ "<<open>>",
  /*428*/ "<<open>>",
  /*429*/ "<<open>>",
  /*430*/ "[%s]: (%s)%d: unable to %s %s %s (%d).",
  /*431*/ "[%s]: (%s)%d: %s %s (%d) is already active as %s (%d).",
  /*432*/ "[%s]: (%s)%d: could not prep column %s (%d) for activation.",
  /*433*/ "[%s]: (%s)%d: %s variable %s (%d)\n"
  /*433*/ "	has status %s; expected NBLB, NBUB, NBFX, or NBFR.",
  /*434*/ "[%s]: (%s)%d: %s scan for %s failed.",
  /*435*/ "[%s]: (%s)%d: unexpected situation; %d %s activated,\n"
  /*435*/ "	previous simplex %s returned %s, next simplex %s.",
  /*436*/ "[%s]: (%s)%d: logical for constraint %s (%d)\n"
  /*436*/ "	has status %s; expected BUUB, BLLB, B, BUB, BLB, BFR, or BFX.",
  /*437*/ "[%s]: (%s)%d: logical for constraint %s (%d)\n"
  /*437*/ "	has status %s; expected NBLB, NBUB, or NBFX.",
  /*438*/ "[%s]: (%s)%d: variable %s (%d)\n"
  /*438*/ "	has status %s; expected BLLB or BUUB.",
  /*439*/ "[%s]: (%s)%d: unexpected %s of %s feasibility.",
  /*440*/ "[%s]: (%s)%d: failed to activate variables referenced by\n"
  /*440*/ "	activated constraints.",
  /*441*/ "[%s]: (%s)%d: unexpected situation; previous simplex %s returned %s.",
  /*442*/ "[%s]: (%s)%d: logical for newly activated constraint %s (%d) has\n"
  /*442*/ "	status %s but constraint should be %s.",
  /*443*/ "[%s]: (%s)%d: failed to solve lp relaxation, return code %s.",
  /*444*/ "[%s]: (%s)%d: %s invariant failure; should be zero.\n"
  /*444*/ "	((orig %d)-(ineligible %d))-((eligible %d)+(active %d)) = %d.",
  /*445*/ "[%s]: (%s)%d: %s %s (%d) marked as ineligible for activation.",
  /*446*/ "[%s]: (%s)%d: %d %s recorded as %s but count is %d.",
  /*447*/ "<<open>>",
  /*448*/ "<<open>>",
  /*449*/ "<<open>>",

//   Error messages from the arc consistency package.

  /*450*/ "[%s]: coefficient a<%d,%d> = a<%s,%s>= 0.",
  /*451*/ "[%s]: could not calculate bounds for constraint %s (%d).",
  /*452*/ "[%s]: failed to push constraint %s (%d).",
  /*453*/ "[%s]: constraint %s (%d) has type %s; only equality,\n"
  /*453*/ "	less-than-or-equal, or range constraints should be present.",
  /*454*/ "[%s]: the bound change of %g%% for %s %s (%d)\n"
  /*454*/ "	is less than the propagation tolerance %g; not propagated.",
  /*455*/ "unable to expand propagation stack from %d to %d entries.",
  /*456*/ "internal confusion; on_propstack indicates %s (%d) is on stack,\n"
  /*456*/ "	but search for index failed.",
  /*457*/ "[%s]: constraint %s has %s = %s; cannot\n"
  /*457*/ "	propagate from an infinite bound.",
  /*458*/ "[%s]: new %s for %s (%d) = %g weaker or equal to\n"
  /*458*/ "	old bound %g; diff %g.",
  /*459*/ "[%s]: could not propagate %s change for %s (%d).",
  /*460*/ "<<open>>",
  /*461*/ "invalid constraint type code %d, constraint %s (%d).",
  /*462*/ "constraint propagation failed for constraint %s (%d).",
  /*463*/ "[%s]: unexpected constraint type %s.",
  /*464*/ "[%s]: infinity count error at recalculation;\n"
  /*464*/ "	old %s, new %s.",
  /*465*/ "[%s]: significant error in finite part of bound at recalculation;\n"
  /*465*/ "	old %s, new %s, diff %g.",
  /*466*/ "[%s]: both bounds are infinite for %s constraint %s (%d);\n"
  /*466*/ "	cannot propagate from infinite bounds.",
  /*467*/ "constraint propagation is disabled; this call should not happen.",
  /*468*/ "snap tolerance %g is smaller than dylp bogus number tolerance %g.",
  /*469*/ "<<open>>",
  /*470*/ "<<open>>",
  /*471*/ "<<open>>",
  /*472*/ "<<open>>",
  /*473*/ "<<open>>",
  /*474*/ "<<open>>",
  /*475*/ "<<open>>",
  /*476*/ "<<open>>",
  /*477*/ "<<open>>",
  /*478*/ "<<open>>",
  /*479*/ "<<open>>",
  /*480*/ "<<open>>",
  /*481*/ "<<open>>",
  /*482*/ "<<open>>",
  /*483*/ "<<open>>",
  /*484*/ "<<open>>",
  /*485*/ "<<open>>",
  /*486*/ "<<open>>",
  /*487*/ "<<open>>",
  /*488*/ "<<open>>",
  /*489*/ "<<open>>",
  /*490*/ "<<open>>",
  /*491*/ "<<open>>",
  /*492*/ "<<open>>",
  /*493*/ "<<open>>",
  /*494*/ "<<open>>",
  /*495*/ "<<open>>",
  /*496*/ "<<open>>",
  /*497*/ "<<open>>",
  /*498*/ "<<open>>",
  /*499*/ "<<open>>",
  /*500*/ "<<open>>",
  /*501*/ "<<open>>",
  /*502*/ "<<open>>",
  /*503*/ "<<open>>",
  /*504*/ "<<open>>",
  /*505*/ "<<open>>",
  /*506*/ "<<open>>",
  /*507*/ "<<open>>",
  /*508*/ "<<open>>",
  /*509*/ "<<open>>",

//   Error messages from the 2-3 tree package.

  /*510*/ "in the %s tree, %s (%#08x) thinks its children\n"
  /*510*/ "	are of type %s; they should be %s.",
  /*511*/ "%s problem at %s tree %s (%#08x);\n"
  /*511*/ "	count is %d, reality is %d.",
  /*512*/ "purported parent (%#08x) of %s (%#08x)\n"
  /*512*/ "	in %s tree has disowned it.",
  /*513*/ "%s %d (%#08x, max %g) and %d (%#08x, max %g) of a\n"
  /*513*/ "	%s (%#08x) in the %s tree are out of order.",
  /*514*/ "%s (%#08x) in the %s tree has no parent,\n"
  /*514*/ "	but the header thinks (%#08x) is the root.",
  /*515*/ "error while freeing vertices of tree %s.",
  /*516*/ "unrecognised two-three vertex type %d at the root of the %s tree.",
  /*517*/ "failed to install leaf for content (%#08x), key %g,\n"
  /*517*/ "	in %s tree.",
  /*518*/ "in the %s tree, max at %s (%#08x) is %g,\n"
  /*518*/ "	but the largest max among %s is %g for %s (%#08x).",
  /*519*/ "leaf (%#08x) in the %s tree has no content.",
  /*520*/ "%s count of %d at %s (%#08x) in %s tree is\n"
  /*520*/ "	outside the valid range of %d to %d.",
  /*521*/ "attempting rebalance of a %s (%#08x) in %s tree and no rebalance\n"
  /*521*/ "	pattern matched.",
  /*522*/ "failed to find a leaf with value %g in %s tree.",
  /*523*/ "inconsistent header for tree %s;\n"
  /*523*/ "	root type %s, root %#08x, %d vertices, %d leaves.",
  /*524*/ "tree %s has no associated tree.",
  /*525*/ "tree %s has associated tree %s, but no associated node\n"
  /*525*/ "	was present for the deleted leaf.",
  /*526*/ "attempt to delete %sleaf %#08x from tree %s failed.",
  /*527*/ "attempt to delete leaf with key %g from tree %s failed.",
  /*528*/ "<<open>>",
  /*529*/ "<<open>>",
  /*530*/ "<<open>>",
  /*531*/ "<<open>>",
  /*532*/ "the header in the %s tree claims (%#08x) is the root, but\n"
  /*532*/ "	(%#08x) says its parent is (%#08x).",
  /*533*/ "%s (%#08x) in the %s tree claims %s (%#08x) as its child,\n"
  /*533*/ "	but the %s claims its parent is (%#08x).",
  /*534*/ "unequal number of leaves in paired trees;\n"
  /*534*/ "	%s tree has %d != %d in %s tree.",
  /*535*/ "<<open>>",
  /*536*/ "<<open>>",
  /*537*/ "<<open>>",
  /*538*/ "failed to rebalance the %s tree.",
  /*539*/ "attempt to delete leaf with key = %g from tree %s, but the\n"
  /*539*/ "	tree bounds are %g < bound < %g.",
  /*540*/ "<<open>>",
  /*541*/ "<<open>>",
  /*542*/ "failed to find leaf (%#08x) in %s tree.",
  /*543*/ "<<open>>",
  /*544*/ "%s tree is empty.",
  /*545*/ "%s tree is empty, but %s field is %g, not %g.",
  /*546*/ "<<open>>",
  /*547*/ "content field recovered from deleted node is null.",
  /*548*/ "<<open>>",
  /*549*/ "in the %s tree, the leaf (%#08x) in the %s position\n"
  /*549*/ "	has value %g, but the header value is %g.",
  /*550*/ "in the %s tree, the header specifies leaf (%#08x)\n"
  /*550*/ "	as the minimum leaf, but actually it's (%#08x).",
  /*551*/ "<<open>>",
  /*552*/ "<<open>>",
  /*553*/ "<<open>>",
  /*554*/ "<<open>>",
  /*555*/ "<<open>>",
  /*556*/ "<<open>>",
  /*557*/ "<<open>>",
  /*558*/ "<<open>>",
  /*559*/ "<<open>>",
  /*560*/ "<<open>>",
  /*561*/ "<<open>>",
  /*562*/ "<<open>>",
  /*563*/ "<<open>>",
  /*564*/ "<<open>>",
  /*565*/ "<<open>>",
  /*566*/ "<<open>>",
  /*567*/ "<<open>>",
  /*568*/ "<<open>>",
  /*569*/ "<<open>>",
  /*570*/ "<<open>>",
  /*571*/ "<<open>>",
  /*572*/ "<<open>>",
  /*573*/ "<<open>>",
  /*574*/ "<<open>>",
  /*575*/ "<<open>>",
  /*576*/ "<<open>>",
  /*577*/ "<<open>>",
  /*578*/ "<<open>>",
  /*579*/ "<<open>>",
  /*580*/ "<<open>>",
  /*581*/ "<<open>>",
  /*582*/ "<<open>>",
  /*583*/ "<<open>>",
  /*584*/ "<<open>>",
  /*585*/ "<<open>>",
  /*586*/ "<<open>>",
  /*587*/ "<<open>>",
  /*588*/ "<<open>>",
  /*589*/ "<<open>>",
  /*590*/ "<<open>>",
  /*591*/ "<<open>>",
  /*592*/ "<<open>>",
  /*593*/ "<<open>>",
  /*594*/ "<<open>>",
  /*595*/ "<<open>>",
  /*596*/ "<<open>>",
  /*597*/ "<<open>>",
  /*598*/ "<<open>>",
  /*599*/ "<<open>>",

// 	Error messages from the branch-and-bound tree routines.

  /*600*/ "(T%d:#%d@%d): cannot create this node in the\n"
  /*600*/ "	branch-and-bound search tree.",
  /*601*/ "(T%d:#%d@%d): improper status %s; should be %s.",
  /*602*/ "(T%d:#%d@%d): killing this node will leave orphans; it's not right.",
  /*603*/ "(T%d): node #%d@%d (%#08x) is an orphan;\n"
  /*603*/ "	proceeding with deletion anyway.",
  /*604*/ "(T%d): could not delete node #%d@%d (%#08x) from the\n"
  /*604*/ "	branch-and-bound search tree.",
  /*605*/ "(T%d): attempt to delete %s node from live set failed.",
  /*606*/ "analysis of objective function failed.",
  /*607*/ "<<open>>",
  /*608*/ "(T%d:#%d@%d): integrated variable forcing loop failed.",
  /*609*/ "(T%d:#%d@%d): status is %s but there are no kids.",
  /*610*/ "(T%d:#%d@%d): invalid branch specification;\n"
  /*610*/ "	branch cnt = %d, branch_vars is %snull.",
  /*611*/ "<<open>>",
  /*612*/ "(T%d:#%d@%d): unable to restore %s %s.",
  /*613*/ "(T%d:#%d@%d): failed to solve lp relaxation, return code %s.",
  /*614*/ "(T%d:#%d@%d): unexpected branch instruction %s = %s.",
  /*615*/ "<<open>>",
  /*616*/ "(T%d:#%d@%d): failed to generate %s edits.",
  /*617*/ "(T%d:#%d@%d): couldn't insert subproblem in live set.",
  /*618*/ "(T%d:#%d@%d): couldn't regenerate environment of survivor of tour.",
  /*619*/ "(T%d): couldn't recover subproblem from live set.",
  /*620*/ "(T%d:#%d@%d): couldn't resurrect subproblem.",
  /*621*/ "(T%d:#%d@%d): error while installing %s edits.",
  /*622*/ "(T%d:#%d@%d): failed resurrection attempt.",
  /*623*/ "(T%d): unable to prune subtree rooted at node #%d@%d.",
  /*624*/ "(T%d:#%d@%d): node has %s copy but not %s copy;\n"
  /*624*/ "	if one is present, both must be present.",
  /*625*/ "(T%d): not prepared to deal with full status or bounds copies\n"
  /*625*/ "	at interior node #%d@%d.",
  /*626*/ "(T%d): searched to the root without finding stored basis & bounds copies.",
  /*627*/ "(T%d): search tree node #%d@%d (%#08x) has no parent.",
  /*628*/ "(T%d): search tree node #%d@%d has no liveinfo structure.",
  /*629*/ "(T%d): leaf delete (%#08x) failed for node #%d@%d.",
  /*630*/ "(T%d:#%d@%d): inconsistency between ivf_fixed and status array;\n"
  /*630*/ "	ivf_fixed says %s (%d) fixed at %g, status is %s.",
  /*631*/ "(T%d:#%d@%d): no active variable structure to manipulate!",
  /*632*/ "(T%d:#%d@%d): inconsistent new constraint structures:\n"
  /*632*/ "	count is %d but array is %snull.",
  /*633*/ "(T%d:#%d@%d): failed to delete new constraint structures.",
  /*634*/ "<<open>>",
  /*635*/ "<<open>>",
  /*636*/ "<<open>>",
  /*637*/ "<<open>>",
  /*638*/ "<<open>>",
  /*639*/ "<<open>>",
  /*640*/ "<<open>>",
  /*641*/ "<<open>>",
  /*642*/ "<<open>>",
  /*643*/ "<<open>>",
  /*644*/ "<<open>>",
  /*645*/ "<<open>>",
  /*646*/ "<<open>>",
  /*647*/ "<<open>>",
  /*648*/ "<<open>>",
  /*649*/ "<<open>>",

// 	Error messages from the bound storage/regeneration utilities.

  /*650*/ "[%s]: %s<%s (%d)> = %g has IEEE FP class %s (%d).",
  /*651*/ "[%s]: %s<%s (%d)> = %g is inside the zero tolerance %g.",
  /*652*/ "[%s]: %s.bnd = %g has IEEE FP class %s (%d).",
  /*653*/ "[%s]: %s.bnd = %g is inside the zero tolerance %g.",
  /*654*/ "[%s]: %s.inf = %d is 1, < -%d, or > %d; invalid value.",
  /*655*/ "[%s]: currently there are %d %s, but the stored bounds arrays\n"
  /*655*/ "	have length %d.",
  /*656*/ "the %s array is null in the bounds copy\n"
  /*656*/ "	specified by key %d at T%d:#%d@%d.",
  /*657*/ "invalid magic cookie at T%d:#%d@%d.",
  /*658*/ "(T%d:#%d@%d): bound edit (%#08x) for %s (%d) has no valid edits.",
  /*659*/ "infeasible %s bound edit for %s (%d) at T%d:#%d@%d;\n"
  /*659*/ "	rewriting is not in progress; edit forced to feasibility.",
  /*660*/ "ignored weak %s bound edit for %s (%d) at T%d:#%d@%d;\n"
  /*660*/ "	rewriting is not in progress; edit strengthened.",
  /*661*/ "could not install bound edit %d (%#08x)\n"
  /*661*/ "	for %s (%d) at T%d:#%d@%d.",
  /*662*/ "(T%d:#%d@%d): cannot store a copy of the %s arrays.",
  /*663*/ "cannot delete stored copy of %s at T%d:#%d@%d.",
  /*664*/ "(T%d:#%d@%d): bounds rewriting is forced but not allowed.",
  /*665*/ "(T%d:#%d@%d): %s of %s edit information failed.",
  /*666*/ "(T%d:#%d@%d): %s %s (%d) should not have a %s but %s is present.",
  /*667*/ "<<open>>",
  /*668*/ "(T%d:#%d@%d): invalid edit counts (%d/%d) in bound edit structure.",
  /*669*/ "<<open>>",
  /*670*/ "<<open>>",
  /*671*/ "<<open>>",
  /*672*/ "<<open>>",
  /*673*/ "<<open>>",
  /*674*/ "<<open>>",

// 	Error messages from the basis storage/regeneration utilities.

  /*675*/ "no %s in stored basis copy.",
  /*676*/ "[%s]: stored %s array length %d violates\n"
  /*676*/ "	allowable range %d..%d.",
  /*677*/ "(T%d:#%d@%d:) %s %s edit information absent.",
  /*678*/ "(T%d:#%d@%d:) %s %s edit information already present.",
  /*679*/ "[%s]: variable %s (%d) has status %s;\n"
  /*679*/ "	explicit basic status should not be seen outside dylp.",
  /*680*/ "inconsistent %s edit structure: edit count is %d but array is %snull.",
  /*681*/ "[%s]: new %s size in edit structure violates\n"
  /*681*/ "	allowable range %d <= %d <= %d.",
  /*682*/ "<<open>>",
  /*683*/ "<<open>>",
  /*684*/ "<<open>>",
  /*685*/ "<<open>>",
  /*686*/ "<<open>>",
  /*687*/ "<<open>>",
  /*688*/ "<<open>>",
  /*689*/ "<<open>>",
  /*690*/ "<<open>>",
  /*691*/ "<<open>>",
  /*692*/ "<<open>>",
  /*693*/ "<<open>>",
  /*694*/ "<<open>>",
  /*695*/ "<<open>>",
  /*696*/ "<<open>>",
  /*697*/ "<<open>>",
  /*698*/ "<<open>>",
  /*699*/ "<<open>>",

// 	Error messages related to tours, integrated variable forcing & fixing

  /*700*/ "[%s]: %s could not install fixed\n"
  /*700*/ "	variables at T%d:#%d@%d.",
  /*701*/ "(T%d:#%d@%d): the monotone variable count is %d,\n"
  /*701*/ "	but the monotone variable list is %snull.",
  /*702*/ "(T%d:#%d@%d): request to fix variable %s (%d) = %d\n"
  /*702*/ "	but it is already fixed at %g.",
  /*703*/ "(T%d:#%d@%d): request to fix variable %s (%d)\n"
  /*703*/ "	but lb = %g != %g = ub.",
  /*704*/ "<<open>>",
  /*705*/ "request to fix variable %s (%d) = %g at node T%d:#%d@%d;\n"
  /*705*/ "	this value is not integral (frac = %g).",
  /*706*/ "(T%d:#%d@%d): lp return code is %s; expecting %s.",
  /*707*/ "(T%d:#%d@%d): error while processing %s instructions.",
  /*708*/ "(T%d:#%d@%d): unexpected action code %s in %s entry\n"
  /*708*/ "	for %s (%d).",
  /*709*/ "(T%d:#%d@%d): %s (%d) is to be fixed at %g, but this\n"
  /*709*/ "	is not equal to lb = %g or ub = %g.",
  /*710*/ "<<open>>",
  /*711*/ "(T%d:#%d@%d): no new incumbent this tour but branch is %s;\n"
  /*711*/ "	branch instruction %s = %s\n"
  /*711*/ "	current bounds lb = %g, ub = %g.",
  /*712*/ "(T%d:#%d@%d): failed to process branch instruction %s = %s.\"",
  /*713*/ "(T%d:#%d@%d): %s constraint not suitable for %s branch.",
  /*714*/ "(T%d:#%d@%d): branch is %s;\n"
  /*714*/ "	branch instruction %s = %s\n"
  /*714*/ "	current bounds LB = %g, UB = %g.",
  /*715*/ "(T%d:#%d@%d): parent's estimate of lb = %g is greater than\n"
  /*715*/ "	actual optimum for child's lp relaxation z<lp> = %g;\n"
  /*715*/ "	error = %g, tol = %g.",
  /*716*/ "(T%d:#%d@%d): could not remove cuts.",
  /*717*/ "<<open>>",
  /*718*/ "<<open>>",
  /*719*/ "<<open>>",
  /*720*/ "<<open>>",
  /*721*/ "<<open>>",
  /*722*/ "<<open>>",
  /*723*/ "<<open>>",
  /*724*/ "<<open>>",


// 	Error messages related to fathoming and penalties

  /*725*/ "subproblem T%d:#%d fathomed on %s; it should have\n"
  /*725*/ "	evaluated as infeasible because of the objective constraint.",
  /*726*/ "cannot perform penalty calculations at T%d:#%d@%d.",
  /*727*/ "error evaluating new integer solution at T%d:#%d@%d.",
  /*728*/ "<<open>>",
  /*729*/ "the solution at subproblem T%d:#%d@%d is not labelled integer.",
  /*730*/ "the incumbent T%d:#%d@%d has no stored %s array.",
  /*731*/ "error while winnowing the set of live subproblems; bound %g.",
  /*732*/ "[%s](T%d:#%d@%d): error while attempting fathoming.",
  /*733*/ "[%s](T%d:#%d@%d): initial lp relaxation fathomed by bound.",
  /*734*/ "[%s](T%d:#%d@%d): cannot calculate reduced costs.",
  /*735*/ "[%s](T%d:#%d@%d): cannot calculate penalties for %s (%d).",
  /*736*/ "[%s]: %s<%d,%d> = %g < 0. This should not be.",
  /*737*/ "[%s]: don't know how to calculate penalty for inactive\n"
  /*737*/ "	architectural variable %s (%d).",
  /*738*/ "[%s]: cbar<%d> = %g for %s %s (%d);\n"
  /*738*/ "	inside dual feasibility tolerance %g by %g.",
  /*739*/ "[%s]: %s variable %s (%d) has status %s but cbarj = %g; incorrect sign.",
  /*740*/ "[%s]: failed to calculate unscaled %s %s<%d>.",
  /*741*/ "[%s]: alternate calculations of abar<%d,%d> disagree.\n"
  /*741*/ "	beta<i>*a<j> = %g, (inv(B)a<j>)<%d> = %g, diff = %g, tol = %g.",
  /*742*/ "[%s]: alternate calculations of beta<%d,%d> disagree.\n"
  /*742*/ "	beta<i><k> = %g, (inv(B)e<k>)<i> = %g, diff = %g, tol = %g.",
  /*743*/ "[%s]: dot(beta<%d>,a<%d>) should be %g,\n"
  /*743*/ "	instead %g, diff = %g, tol = %g.",
  /*744*/ "<<open>>",
  /*745*/ "<<open>>",
  /*746*/ "<<open>>",
  /*747*/ "<<open>>",
  /*748*/ "<<open>>",
  /*749*/ "<<open>>",

// 	Error messages related to branching priority and tour groups
// 	(bonsai MIP code only)

  /*750*/ "(T%d): invalid priority %d recovered for %s (%d).",
  /*751*/ "(T%d:#%d@%d): %d variable(s) remain to be forced;\n"
  /*751*/ "	tour leader selection is premature.",
  /*752*/ "(T%d:#%d@%d): code %d does not specify a valid tour leader\n"
  /*752*/ "	selection strategy.",
  /*753*/ "(T%d:#%d@%d): failed to select tour leader(s).",
  /*754*/ "(T%d:#%d@%d): liveinfo structure already exists.",
  /*755*/ "empty priority specification.",
  /*756*/ "priority class %d has no members.",
  /*757*/ "null entry in member list for class %d.",
  /*758*/ "all integer variables defaulted to a single\n"
  /*758*/ "	branching priority class.",
  /*759*/ "(T%d): %s (%d) is type %s; only integral variables\n"
  /*759*/ "	have a branching priority.",
  /*760*/ "tour group #%d has only one member.",
  /*761*/ "initial processing of tour specifications failed.",
  /*762*/ "in tour #%d the station vector being processed has general\n"
  /*762*/ "	integer form; bonsai is not yet ready for this.",
  /*763*/ "empty tourclass specification.",
  /*764*/ "tour specification #%d has no %s.",
  /*765*/ "null entry (position %d) in %s list for tour\n"
  /*765*/ "	specification #%d.",
  /*766*/ "cannot hash variable name %s to index in tour specification #%d.",
  /*767*/ "duplicate entry for variable %s (%d);\n"
  /*767*/ "	it seems to be in tour groups #%d and #%d.",
  /*768*/ "failed to generate %s station(s) for tour specification #%d.",
  /*769*/ "tour specification #%d has %d members, but the station\n"
  /*769*/ "	vector being processed has only %d digits.",
  /*770*/ "tour group size in specification %d violates legal\n"
  /*770*/ "	range %d <= %d <= %d.",
  /*771*/ "duplicate %s in stations for tour specification #%d; ignored.",
  /*772*/ "stations #%d and #%d in tour specification #%d are identical.",
  /*773*/ "%s (%d) is type %s; only binary variables may appear\n"
  /*773*/ "	with a %s station specification.",
  /*774*/ "(T%d): %s (%d) is type %s; only integral variables can be\n"
  /*774*/ "	members of a tour group.",
  /*775*/ "<<open>>",
  /*776*/ "<<open>>",
  /*777*/ "<<open>>",
  /*778*/ "<<open>>",
  /*779*/ "<<open>>",
  /*780*/ "<<open>>",
  /*781*/ "<<open>>",
  /*782*/ "<<open>>",
  /*783*/ "<<open>>",
  /*784*/ "<<open>>",
  /*785*/ "<<open>>",
  /*786*/ "<<open>>",
  /*787*/ "<<open>>",
  /*788*/ "<<open>>",
  /*789*/ "<<open>>",
  /*790*/ "<<open>>",
  /*791*/ "<<open>>",
  /*792*/ "<<open>>",
  /*793*/ "<<open>>",
  /*794*/ "<<open>>",
  /*795*/ "<<open>>",
  /*796*/ "<<open>>",
  /*797*/ "<<open>>",
  /*798*/ "<<open>>",
  /*799*/ "<<open>>",

// 	Error messages related to expansion of the active subproblem.
// 	(bonsai MIP code only)

  /*800*/ "(T%d:#%d@%d): missing liveinfo or var1/var2 structures.",
  /*801*/ "(T%d:#%d@%d): this node is already a parent.",
  /*802*/ "<<open>>",
  /*803*/ "(T%d:#%d@%d): the tour %s %s (%d) has status %s;\n"
  /*803*/ "	it should be %s.",
  /*804*/ "(T%d:#%d@%d): variable %s (%d) is not the tour leader;\n"
  /*804*/ "	bonsai is not prepared to cope with a relative station specification\n"
  /*804*/ "	for other tour group members.",
  /*805*/ "(T%d:#%d@%d): unrecognized change instruction %#08x at\n"
  /*805*/ "	pos'n %d for variable %s (%d).",
  /*806*/ "(T%d:#%d@%d): cannot finalise the tour itinerary.",
  /*807*/ "(T%d:#%d@%d): error while processing tour stations.",
  /*808*/ "(T%d:#%d@%d): could not create child at station %s.",
  /*809*/ "(T%d:#%d@%d): all branch variable changes were looser\n"
  /*809*/ "	than current bounds, hence all subproblems were suppressed.\n"
  /*809*/ "	Are you sure the tour specification is correct?",
  /*810*/ "(T%d:#%d@%d): rhs ub = %g < %g = lb for %s constraint %s (%d).",
  /*811*/ "(T%d:#%d@%d): unable to verify or replace tour leader.",
  /*812*/ "(T%d:#%d@%d): failed to regenerate lp relaxation for tour parent.",
  /*813*/ "(T%d:#%d@%d): failed to generate tour environment data structures.",
  /*814*/ "<<open>>",
  /*815*/ "<<open>>",
  /*816*/ "<<open>>",
  /*817*/ "<<open>>",
  /*818*/ "<<open>>",
  /*819*/ "<<open>>",
  /*820*/ "<<open>>",
  /*821*/ "<<open>>",
  /*822*/ "<<open>>",
  /*823*/ "<<open>>",
  /*824*/ "<<open>>",
  /*825*/ "<<open>>",
  /*826*/ "<<open>>",
  /*827*/ "<<open>>",
  /*828*/ "<<open>>",
  /*829*/ "<<open>>",
  /*830*/ "<<open>>",
  /*831*/ "<<open>>",
  /*832*/ "<<open>>",
  /*833*/ "<<open>>",
  /*834*/ "<<open>>",
  /*835*/ "<<open>>",
  /*836*/ "<<open>>",
  /*837*/ "<<open>>",
  /*838*/ "<<open>>",
  /*839*/ "<<open>>",
  /*840*/ "<<open>>",
  /*841*/ "<<open>>",
  /*842*/ "<<open>>",
  /*843*/ "<<open>>",
  /*844*/ "<<open>>",
  /*845*/ "<<open>>",
  /*846*/ "<<open>>",
  /*847*/ "<<open>>",
  /*848*/ "<<open>>",
  /*849*/ "<<open>>",

// 	Error messages related to constraint analysis, box bounds,
// 	branch-on-hyperplane, and max-min-slack.

  /*850*/ "[%s]: %s (%d) mismatch in variable/coefficient classification;\n"
  /*850*/ "	row count %d, classified %d vars, %d coefficients.",
  /*851*/ "[%s]: constraint classification failed.",
  /*852*/ "(T%d:#%d@%d): unable to calculate box bounds.",
  /*853*/ "[%s]: unable to calculate off constraint bounds.",
  /*854*/ "(T%d:#%d@%d): error while scanning for BOH Type 1 candidate.",
  /*855*/ "(T%d:#%d@%d): error while attempting quick & dirty max-min-slack.",
  /*856*/ "(T%d:#%d@%d): max-min-slack lp failed, status %s.",
  /*857*/ "(T%d:#%d@%d): could not create expanded x<opt> vector.",
  /*858*/ "(T%d:#%d@%d): no valid hyperplanes after %s hyperplane pattern expansion.",
  /*859*/ "(T%d:#%d@%d): could not complete %s for max-min-slack lp.",
  /*860*/ "(T%d:#%d@%d): error while attempting scan to center.",
  /*861*/ "(T%d:#%d@%d): expected to convert %d %s,\n"
  /*861*/ "	but actually converted %d.",
  /*862*/ "(T%d:#%d@%d): failed to install %s for %g <= %s (%d) <= %g.",
  /*863*/ "[%s]: right-hand-side reduction failed.",
  /*864*/ "<<open>>",
  /*865*/ "(T%d:#%d@%d): can't install new incumbent (rounding).",
  /*866*/ "(T%d:#%d@%d): failed to solve lp relaxation of heuristic solution.",
  /*867*/ "(T%d:#%d@%d): failed to manufacture branching hyperplane from %s (%d).",
  /*868*/ "(T%d:#%d@%d): coefficient reduction cannot be performed\n"
  /*868*/ "	on %s constraint %s (%d).",
  /*869*/ "(T%d:#%d@%d): coefficient reduction failed for %s (%d).",
  /*870*/ "[%s]: integral equality %s (%d) has real rhs = %g!\n"
  /*870*/ "	Check your model and try again.",
  /*871*/ "<<open>>",
  /*872*/ "<<open>>",
  /*873*/ "<<open>>",
  /*874*/ "<<open>>",
  /*875*/ "<<open>>",
  /*876*/ "<<open>>",
  /*877*/ "<<open>>",
  /*878*/ "<<open>>",
  /*879*/ "<<open>>",
  /*880*/ "<<open>>",
  /*881*/ "<<open>>",
  /*882*/ "<<open>>",
  /*883*/ "<<open>>",
  /*884*/ "<<open>>",
  /*885*/ "<<open>>",
  /*886*/ "<<open>>",
  /*887*/ "<<open>>",
  /*888*/ "<<open>>",
  /*889*/ "<<open>>",
  /*890*/ "<<open>>",
  /*891*/ "<<open>>",
  /*892*/ "<<open>>",
  /*893*/ "<<open>>",
  /*894*/ "<<open>>",
  /*895*/ "<<open>>",
  /*896*/ "<<open>>",
  /*897*/ "<<open>>",
  /*898*/ "<<open>>",
  /*899*/ "<<open>>",

// 	Error messages generated by cutting plane management.

  /*900*/ "(T%d:#%d@%d): error in range of cuts to delete. Deleting to\n"
  /*900*/ "	%s (%d) will delete architectural constraints from %d to %d.",
  /*901*/ "(T%d:#%d@%d): suspicious constraint count %d is not equal to\n"
  /*901*/ "	size of parent system (%d) or root system (%d).",
  /*902*/ "(T%d:#%d@%d): error in range for cut edit generation. Starting\n"
  /*902*/ "	from %s (%d) will generate edits for architectural constraints\n"
  /*902*/ "	from %d to %d.",
  /*903*/ "<<open>>",
  /*904*/ "(T%d:#%d@%d): could not install cut %s.",
  /*905*/ "(T%d:#%d@%d): error while evaluating MMS branching candidates.",
  /*906*/ "<<open>>",
  /*907*/ "<<open>>",
  /*908*/ "<<open>>",
  /*909*/ "<<open>>",
  /*910*/ "<<open>>",
  /*911*/ "<<open>>",
  /*912*/ "<<open>>",
  /*913*/ "<<open>>",
  /*914*/ "<<open>>",
  /*915*/ "<<open>>",
  /*916*/ "<<open>>",
  /*917*/ "<<open>>",
  /*918*/ "<<open>>",
  /*919*/ "<<open>>",
  /*920*/ "<<open>>",
  /*921*/ "<<open>>",
  /*922*/ "<<open>>",
  /*923*/ "<<open>>",
  /*924*/ "<<open>>",
  /*925*/ "<<open>>",
  /*926*/ "<<open>>",
  /*927*/ "<<open>>",
  /*928*/ "<<open>>",
  /*929*/ "<<open>>",
  /*930*/ "<<open>>",
  /*931*/ "<<open>>",
  /*932*/ "<<open>>",
  /*933*/ "<<open>>",
  /*934*/ "<<open>>",
  /*935*/ "<<open>>",
  /*936*/ "<<open>>",
  /*937*/ "<<open>>",
  /*938*/ "<<open>>",
  /*939*/ "<<open>>",
  /*940*/ "<<open>>",
  /*941*/ "<<open>>",
  /*942*/ "<<open>>",
  /*943*/ "<<open>>",
  /*944*/ "<<open>>",
  /*945*/ "<<open>>",
  /*946*/ "<<open>>",
  /*947*/ "<<open>>",
  /*948*/ "<<open>>",
  /*949*/ "<<open>>",

// 	Error messages related to dylp's simplex/tableau and ray routines

  /*950*/ "[%s]: inactive %s %s (%d); cannot %s.",
  /*951*/ "[%s]: nonbasic variable %s (%d); cannot %s.",
  /*952*/ "[%s]: error calculating basis inverse %s %d, %s %s (%d).",
  /*953*/ "[%s]: error calculating %s %s, %s (%d).",
  /*954*/ "[%s]: cannot calculate %s rays; incompatible lp return code %s.",
  /*955*/ "[%s]: error while generating %s rays." } ; 

const int dy_errmsgCnt = (sizeof(dy_errmsgtmplts)/sizeof(char *))-1 ;
