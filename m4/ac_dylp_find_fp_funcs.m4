
dnl The body of DYLP_FIND_ISFINITE and DYLP_FIND_ISNAN is cribbed directly
dnl from the corresponding macros in coin_math.m4. I'm doing this, rather
dnl than using them directly in configure.ac, because the COIN macros will
dnl first check for a declaration in the C++ std:: namespace and insist that
dnl the language context must be C++. Dylp is C and needs to do this check
dnl in a C context.

# AC_DYLP_FIND_ISFINITE
# ------------------------------------------------------
# Determines the name of the finite() function in this environment. This is the
# function that recognises whether an IEEE floating point value is finite.  The
# variable ac_name_of_finite will be set to the proper name on return and
# DYLP_ISFINITE will be defined to the same value.
# ------------------------------------------------------

AC_DEFUN([AC_DYLP_FIND_ISFINITE],
[ AC_MSG_NOTICE([Checking for proper name for isfinite().])

  ac_name_of_isfinite=

dnl The usual tests for a symbol using CHECK_DECL. Generally these seem to
dnl be adequate but there was a report of failure on an Apple M1 system.

  if test -z "$ac_name_of_isfinite" ; then
    for fname in isfinite finite _finite __builtin_isfinite ; do
      AC_CHECK_DECL([$fname],[ac_name_of_isfinite=$fname],,AC_COIN_MATH_HDRS)
      if test -n "$ac_name_of_isfinite" ; then
        break
      fi
    done
  fi
  if test -n "$ac_name_of_isfinite" ; then
    AC_MSG_NOTICE([CHECK_DECL found $ac_name_of_isfinite.])
  fi

dnl Hence just go for it --- try to compile and run a call to isfinite(). If
dnl it works, declare victory. DYLPLIB_LFLAGS will contain -lm, if we need it
dnl (set by CHK_LIBM).

  if test -z "$ac_name_of_isfinite" ; then
    temp=
    coin_save_LIBS="$LIBS"
    LIBS=$DYLPLIB_LFLAGS
    AC_RUN_IFELSE(
      [AC_LANG_PROGRAM([AC_COIN_MATH_HDRS],
	 [double dvar = 1.0 ;
	  if (isfinite(dvar))
	  { return (0) ; }
	  else
	  { return (1) ; }])],
      [temp='isfinite'])
    if test -n "$temp" ; then
      AC_MSG_NOTICE([Just try isfinite(double) worked.])
      ac_name_of_isfinite=$temp
    fi
    LIBS="$coin_save_LIBS"
  fi

dnl Try the exotic test that @mandeluna sent in for Apple M1, augmented to
dnl actually compile and run a program. Really, this should never execute,
dnl the 'just go for it' test should have succeeded.

  if test -z "$ac_name_of_isfinite" ; then
    temp=
    coin_save_LIBS="$LIBS"
    LIBS=$DYLPLIB_LFLAGS
    AC_RUN_IFELSE(
      [AC_LANG_PROGRAM([AC_COIN_MATH_HDRS],
      [ double dvar = 1.0 ;
#if __has_builtin(__builtin_isfinite)
  if (isfinite(dvar))
  { return (0) ; }
  else
  { return (1) ; }
#else
  choke me
#endif
      ])],
      [temp='isfinite'])
    if test -n "$temp" ; then
      AC_MSG_NOTICE([Exotic M1 test found $temp!])
      ac_name_of_isfinite=$temp
    fi
    LIBS="$coin_save_LIBS"
  fi

  if test -z "$ac_name_of_isfinite" ; then
    AC_MSG_WARN([Cannot find a C function to check if an IEEE floating point
		 value is finite. There is no hope of building dylp on this
		 system.])
    ac_name_of_isfinite="<<none found>>"
  fi
  AC_DEFINE_UNQUOTED([DYLP_ISFINITE],
      [$ac_name_of_isfinite],
      [Define to be the name of the C function used to check that an IEEE
       floating point value is finite.])
  AC_MSG_NOTICE([Using $ac_name_of_isfinite as isfinite().])
])

dnl FIND_ISNAN doesn't seem to have the same difficulty as FIND_ISFINITE,
dnl but keep the above additions in mind.

# AC_DYLP_FIND_ISNAN
# ------------------------------------------------------
# Determines the name of the isnan() function in this environment. This is the
# function that recognises whether an IEEE floating point value is NaN.  The
# variable ac_name_of_isnan will be set to the proper name on return and
# DYLP_ISNAN will be defined to the same value.
# ------------------------------------------------------

AC_DEFUN([AC_DYLP_FIND_ISNAN],
[ AC_MSG_NOTICE([Checking for proper name for isnan().])

  ac_name_of_isnan=

  if test -z "$ac_name_of_isnan" ; then
    for fname in isnan _isnan isnand _isnand nan _nan __builtin_isnan ; do
      AC_CHECK_DECL([$fname],[ac_name_of_isnan=$fname],,AC_COIN_MATH_HDRS)
      if test -n "$ac_name_of_isnan" ; then
        break
      fi
    done
  fi

  if test -z "$ac_name_of_isnan" ; then
    AC_MSG_WARN([Cannot find a C function to check if an IEEE floating point
		 value is NaN. There is no hope of building dylp on this
		 system.])
    ac_name_of_isnan="<<none found>>"
  fi
  AC_DEFINE_UNQUOTED([DYLP_ISNAN],
      [$ac_name_of_isnan],
      [Define to be the name of the C function used to check that an IEEE
       floating point value is NaN.])
  AC_MSG_NOTICE([Using $ac_name_of_isnan as isnan().])
])

