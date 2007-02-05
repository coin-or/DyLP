
# This macro makes corrections to CPPFLAGS, CFLAGS, AND CXXFLAGS. It does
# additions and deletions of options that are problematic on some systems,
# and it looks for flags in CXXFLAGS and/or CFLAGS that really should be
# in CPPFLAGS because they affect the preprocessor's search path.
# (See Section 4.7.1 in the autoconf manual.)

# Current tests are:
#   -mno-cygwin	Sort of a special case, needs to be in preprocess, compile,
#		and link. (Should be handled properly in BuildTools as of
#		07.02.01).

# Additions:
#   -wd4996	Suppress warning C4996 (deprecated function) for MSVC cl.

# Deletions:
#   -pedantic-errors    On some systems, standard system files will fail to
#               compile when this is specified.

AC_DEFUN([AC_DYLP_FIX_CPPFLAGS],
[
# Transfer flags from CFLAGS/CXXFLAGS to CPPFLAGS.
  case "$CXXFLAGS $CFLAGS" in
    *-mno-cygwin*)
      CXXFLAGS=`echo $CXXFLAGS | sed -e 's/-mno-cygwin//g'`
      CFLAGS=`echo $CFLAGS | sed -e 's/-mno-cygwin//g'`
      CPPFLAGS="-mno-cygwin $CPPFLAGS"
      LDFLAGS="-mno-cygwin $LDFLAGS"
      ;;
  esac
# Add flags. Strip the option first, then add once, to avoid repetition.
  case "$CXX" in
    cl* | */cl*)
      CXXFLAGS=`echo $CXXFLAGS | sed -e 's/-wd4996//g'`
      CXXFLAGS="$CXXFLAGS -wd4996"
      ;;
  esac
  case "$CC" in
    cl* | */cl*)
      CFLAGS=`echo $CFLAGS | sed -e 's/-wd4996//g'`
      CFLAGS="$CFLAGS -wd4996"
    ;;
  esac
# Deletions.
  case "$build" in
    *-darwin*)
      CXXFLAGS=`echo $CXXFLAGS | sed -e 's/-[-]*pedantic-errors//g'`
      CFLAGS=`echo $CFLAGS | sed -e 's/-[-]*pedantic-errors//g'`
    ;;
  esac
])
