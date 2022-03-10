
dnl The body of DYLP_FIND_ISFINITE and DYLP_FIND_ISNAN is cribbed directly
dnl from the corresponding macros in coin_math.m4. I'm doing this, rather
dnl than using them directly in configure.ac, because the COIN macros will
dnl first check for a declaration in the C++ std:: namespace and insist that
dnl the language contect must be C++. Dylp is C and needs to do this check
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

  if test -z "$ac_name_of_isfinite" ; then
    for fname in __builtin_isfinite isfinite finite _finite ; do
      AC_CHECK_DECL([$fname],[ac_name_of_isfinite=$fname],,AC_COIN_MATH_HDRS)
      if test -n "$ac_name_of_isfinite" ; then
        break
      fi
    done
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
    for fname in __builtin_isnan isnan _isnan isnand _isnand nan _nan ; do
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

