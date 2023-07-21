# Dylp

[
 ![A COIN-OR Project](https://coin-or.github.io/coin-or-badge.png)
](https://www.coin-or.org)

## Overview

dylp is an implementation of the dynamic simplex algorithm.
Briefly, dynamic simplex attempts to work with an active constraint system
which is a subset of the full constraint system.
It alternates between primal and dual simplex phases.
Between simplex phases, it deactivates variables and
constraints which are not currently useful, and scans the full constraint
system to activate variables and constraints which have become useful.
The [dylp documentation](doc/dylp.pdf) describes dylp's algorithms
in detail.

dylp is intended as a development code.
The implementation puts more weight on clarity than efficiency and is heavily
instrumented and commented.
It can be built with extensive statistics, informational printing, and
consistency checks.
It can also be built without any of these.
Fully optimised, it's decently fast.

dylp is a C code, and can be used in a pure C environment completely
independent of other COIN software.
It also has a COIN-OR [OSI](https://github.com/coin-or/Osi) interface,
OsiDylp, which takes advantage of capabilities provided by COIN
(_e.g._, enhanced input/output and constraint system preprocessing) and is
recommended if you're working in a C++ environment.

dylp is released as open source under the
[Eclipse Public License 2.0](http://www.opensource.org/licenses/EPL-2.0).

It is distributed under the auspices of the [COIN-OR Foundation](https://www.coin-or.org)

dylp is not under active development.
The source repository is https://github.com/coin-or/DyLP.
In summer 2022 it was upgraded to conform to 
COIN GitHub repository standards current at the time.

## Current Build Status

[![Windows Builds](https://github.com/coin-or/DyLP/actions/workflows/windows-ci.yml/badge.svg?branch=master)](https://github.com/coin-or/DyLP/actions/workflows/windows-ci.yml?query=branch%3Amaster)

[![Linux and MacOS Builds](https://github.com/coin-or/DyLP/actions/workflows/linux-ci.yml/badge.svg?branch=master)](https://github.com/coin-or/DyLP/actions/workflows/linux-ci.yml?query=branch%3Amaster)

## Download, Configuration, and Testing

dylp has no current releases.
Clone the git repository and use the master branch.
You can use [coinbrew](https://github.com/coin-or/coinbrew) to automate the
process of cloning the repositories for dylp and its dependencies and building
the software.
This is the best way to get started.
If you want to build in a separate directory from
the source files, use the coinbrew option `--build-dir`.
If you want to install in a specified directory, use the coinbrew option
`--prefix`.

Once you have created the necessary repositories, the minimal action
required for configuration and build is to run the commands
```
./configure
make
```
in the top level directory of the distribution.
Run
```
./configure --help
```
for a list of available configuration options.
If you're building in a directory separate from the source directory, run
configure and make from the build directory as
```
cd /path/to/build/Dylp
path/to/src/Dylp/configure
make
```
To install in a specified directory, use the `--prefix` option to configure as
```
path/to/src/Dylp/configure --prefix /path/to/install
make
make install
```

For a test of correct installation, dylp runs its own unit test and the
OsiDylp unit test.
Simply type the command
```
make test
```
in the top level build directory to build and run the unit tests.


## Examples

The dylp distribution also provides two simple main programs in the directory
`DyLP/examples`.
Be sure to execute the command 
```
make install
```
in the top level build directory before attempting to build the examples!
To build the examples, change directory to `DyLP/examples` and execute the
command
```
make examples
```
A simple shell script, `plain`, provides a convenient wrapper for both main
programs.
Execute the command '`plain --help`' for usage information.


## Dependencies

dylp's dependencies are specified in the
[`config.yml`](.coin-or/config.yml)
file used by coinbrew and in the (semi-obsolete)
[Dependencies](.coin-or/Dependencies)
file.
The master branch is specified but the latest releases should be entirely
sufficient.

## Bugs

There are undoubtably a few left somewhere.
If you run across one, please let us know.
The preferred way is to use a issue.
You can [browse existing issues](https://github.com/coin-or/DyLP/issues) to
see if your bug has already been reported and add additional information.
Otherwise, please
[submit a new issue](https://github.com/coin-or/DyLP/issues/new).

