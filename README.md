## dylp Overview

dylp is an implementation of the dynamic simplex algorithm. Briefly, dynamic simplex attempts to work with an active constraint system which is a subset of the full constraint system. It alternates between primal and dual simplex phases. Between simplex phases, it deactivates variables and constraints which are not currently useful, and scans the full constraint system to activate variables and constraints which have become useful. The
[dylp documentation](DyLP/doc/dylp.pdf) describes dylp's algorithms in detail.

dylp is intended as a development code. The implementation puts more weight on clarity than efficiency and is heavily instrumented and commented. It can be built with extensive statistics, informational printing, and consistency checks. It can also be built without any of these. Fully optimised, it's decently fast, with run times averaging three to four times that of clp, depending on the problem and the runtime environment.

dylp is a C code, and can be used in a pure C environment completely independent of other COIN software. It also has a
COIN-OR [OSI](http://projects.coin-or.org/Osi) interface,
OsiDylp, which takes advantage of capabilities provided by COIN (_e.g._, enhanced input/output and constraint system preprocessing) and is recommended if you're working in a C++ environment.

The [FAQ list](#dylpFAQ) answers some common questions about dylp.


## Repository Structure and Download Instructions

Dylp uses the standard COIN repository structure. Briefly, you can obtain dylp at three levels: point releases, stable versions, and development. Stable versions are indentified by a two digit number X.Y, where X is the major version number and Y is the minor number. Changes to the minor version number will normally not change the user API. Bug fixes accumulate in stable versions. Specific point releases are identified by a three digit number X.Y.Z, where Z specifies the point release. A new point release is generated for each new stable version and then as required to incorporate bug fixes. Please check the [DyLP project summary page](http://www.coin-or.org/projects/DyLP.xml) for the most recent stable version and point release.

*Point releases* are the best choice for individuals who intend to simply use dylp `as is' and should be obtained as a [tarball](http://www.coin-or.org/download/source/DyLP) with a name of the form `DyLP-X.Y.Z.tgz` (_e.g._, `DyLP-1.5.0.tgz`). Point releases never change. Upgrading to a new point release implies downloading a new tarball which completely replaces the previous point release. At any time, the most recent point release is recommended unless you have a specific reason to select an earlier release.  Bug reports will be taken seriously; see [Bugs](./WikiStart/#BugReports) below.

*Stable versions* are more suitable for individuals who expect that they will make modifications to the dylp code base or who simply wish to take advantage of the capabilities of the subversion repository for updates. As with point releases, the most recent stable version is recommended unless you have a specific reason to choose an earlier version. As an example, the command
```
svn co https://projects.coin-or.org/svn/DyLP/stable/1.5 DyLP-1.5 
```
will check out stable version 1.5 to the directory `DyLP-1.5`. If prompted, use anonymous as a user id and your email address as a password. Bug reports will be taken seriously; see [Bugs](#BugReports) below.

*Development* code is recommended only for dylp developers. It represents the bleeding edge of dylp development. Development code may not be bug-free, may not work with the stable releases of other COIN components, and in extreme cases may not even compile. As an example, the command
```
svn co https://projects.coin-or.org/svn/DyLP/trunk DyLP 
```
will check out the development version of dylp to the directory `DyLP`.  Bug reports are very much appreciated, but requests to fix something will generally be laughed at unless you demonstrate you know what you're doing by providing well-crafted code to fix the problem, in which case you'll probably receive an invitation to become a developer.

The [BuildTools](http://projects.coin-or.org/BuildTools/wiki) project page has additional details on downloading, building, installing, available options, and troubleshooting.

Within any dylp release or repository tree, you'll find this structure
  * `DyLP/src/Dylp` contains the main dylp code base;
  * `DyLP/src/DylpStdLib` contains support libraries;
  * `DyLP/src/OsiDylp` contains dylp's OSI interface ;
  * `DyLP/test` contains the dylp and OsiDylp unit test routines;
  * `DyLP/examples` contains a pair of sample main programs and a handy shell script to run them; and
  * `DyLP/doc` contains prebuilt documentation and LaTeX source for the documentation.

In the row of buttons at the top of the page, "Timeline" will get you a nice presentation of overall project activity (code, tickets, and wiki). "Browse Source" gets you into the repository. "Browse" is accurate, however. If you want to check out code, grab a tarball or use svn as mentioned above.

To find information about the change set associated with a particular commit, your best bet is to go through the Timeline button. To find information about a particular file, start with Browse Source, find the file in the repository, and select the number in the Rev column. You'll end up on a page with all commits for the file; you can retrieve the content of the file under the Rev column, and changes under the ChgSet column.


## Configuration and Testing

Dylp uses the standard COIN configuration architecture; the minimal action required for configuration is to run the command
`./configure` in the top level directory of the distribution. See the
[user configuration instructions](https://projects.coin-or.org/BuildTools/wiki/user-configure) in the BuildTools project for much more information on user configuration options.

For a test of correct installation, dylp runs its own unit test and the OsiDylp unit test. Simply type the command `make test` in the top level directory to build and run the unit tests.


## Examples

The dylp distribution also provides two simple main programs in the directory `DyLP/examples`. *Be sure to execute the command '`make install`' in the top level directory before attempting to build the examples! * To build the examples, change directory to `DyLP/examples` and execute the command '`make examples`'. A simple shell script, `plain`, provides a convenient wrapper for both main programs. Execute the command '`plain --help`' for usage information.

----------


## Included Projects

When you download a dylp stable version or point release, you'll get appropriate versions of other projects. For example, when you download release 1.5.0, you'll get [these](Dependencies) additional projects. 

== Bugs ==#BugReports

Ok, there may be a few left somewhere. If you run across one, please let us know. The preferred way is to use a issue.
You can [browse existing issues](https://github.com/coin-or/DyLP/issues) to see if your bug has already been reported and add additional information.
Otherwise, please [submit a new issues](https://github.com/coin-or/DyLP/issues/new).


## Tasks

There are many things that could be added to dylp, and it certainly has some rough edges in the Windows environment. Check the
[active issues](https://github.com/coin-or/DyLP/issues)
for development tasks looking for volunteers.

----------


## Project Manager

```
<a href = "mailto:lou`@`cs.sfu.ca">Lou Hafer</a>, Computing Science, Simon Fraser University.
```

----------
== dylp FAQs ==#dylpFAQ


### What is dylp?
Dylp implements the dynamic simplex algorithm described by Padberg in Linear Optimization and Extensions. Dylp is designed as a research code and generally chooses flexibility over speed. It can tell you an enormous amount about what it's doing, and can be compiled with extensive validation checks.


### What is dynamic simplex?
The dynamic simplex algorithm works with an active subset of the constraint system. It continuously modifies the active subset, removing loose constraints and unpromising variables, and adding violated constraints and promising variables. The primary advantage is in applications (e.g., branch-cut-price) where the constraint system is solved over and over with small variations.


### Is it reliable?
Yes. It has been used as the underlying LP solver in the bonsaiG MILP code since 1999. It solves the Netlib problems used as the validation test for the COIN-OR OSI layer. It has been tested as a solver for [cbc](http://projects.coin-or.org/Cbc).


### Is it as fast as [clp](https://projects.coin-or.org/Clp)?
The short answer is "No." The longer answer is "No, not for solving LPs from scratch. But it might have an advantage when used to repeatedly resolve the same system with small modifications." The performance ratio will depend on your system. Running the COIN OSI unitTest (which solves the Netlib LPs) on SPARC hardware with a 1MB L2 cache, it's about three times as slow as clp. On typical Intel hardware with a 256KB L2 cache, on average it's about four times slower than clp.


### Is there any documentation for dylp?
Yes. A tech report ([PDF](DyLP/doc/dylp.pdf)) ([PostScript](DyLP/doc/dylp.ps)) in the dylp distribution contains detailed descriptions of the algorithms and their implementation, documentation for interface routines, and descriptions of dylp's (many) options. The source code has extensive comments.


### Is there an OSI layer for dylp?
Yes, OsiDylp.  
[OSI](http://projects.coin-or.org/Osi) is included as an external in the DyLP distribution. OsiDylp adds significant additional functionality, including use of COIN i/o and presolve facilities, and management of multiple active solver objects.


### What platforms does dylp run on?
Most recently, dylp and OsiDylp have been tested on:
 * Solaris 10 on SPARC and x86 using !Sun/Studio 12.1 and Sun C/C++ 5.10
 * Fedora 14 using GCC 4.5.1
 * Windows 7 with Visual Studio 2008 SP1
This list is not exhaustive. Dylp participates in the COIN [TestTools](https://projects.coin-or.org/TestTools) project; check the [nightly build in action](https://projects.coin-or.org/TestTools/wiki/NightlyBuildInAction) page for an (intermittently updated) list of platforms where COIN software is tested.
Generally, dylp will run on earlier versions of the above platforms and compilers (there are issues with some versions of particular compilers). The full command parsing capabilities of the dylp standalone executable are not available in Windows environments but this is not required when dylp is used through the OsiDylp interface.

!Sun/Solaris/Studio is the primary development environment. You should be able to use dylp in any environment capable of supporting COIN. If you have difficulty building or running dylp, please tell us. It's hard to fix bugs we don't know about.


### What can the community do to help?
Use dylp. Abuse it. Let us know the results. Make a development task your own! It's still an open question as to whether the performance of this algorithm is worth the complexity of the implementation. 
