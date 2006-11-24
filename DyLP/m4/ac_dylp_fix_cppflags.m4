
# This macro looks for flags in CXXFLAGS and/or CFLAGS that really should be
# in CPPFLAGS because they affect the preprocessor's search path.
# See Section 4.7.1 in the autoconf manual.

# Current tests are:
#   -mno-cygwin	Sort of a special case, needs to be in preprocess, compile,
#		and link.

# Additions:
#   -wd4996	Suppress warning C4996 (deprecated function) for MSVC cl.

AC_DEFUN([AC_DYLP_FIX_CPPFLAGS],
[ case "$CXXFLAGS $CFLAGS" in
    *-mno-cygwin*)
      CXXFLAGS=`echo $CXXFLAGS | sed -e 's/-mno-cygwin//'`
      CFLAGS=`echo $CFLAGS | sed -e 's/-mno-cygwin//'`
      CPPFLAGS="-mno-cygwin $CPPFLAGS"
      LDFLAGS="-mno-cygwin $LDFLAGS"
      ;;
  esac
  case "$CXX" in
    cl* | */cl*)
      CXXFLAGS=`echo $CXXFLAGS | sed -e 's/-wd4996//'`
      CXXFLAGS="$CXXFLAGS -wd4996"
      ;;
  esac
  case "$CC" in
    cl* | */cl*)
      CFLAGS=`echo $CFLAGS | sed -e 's/-wd4996//'`
      CFLAGS="$CFLAGS -wd4996"
    ;;
  esac
])
