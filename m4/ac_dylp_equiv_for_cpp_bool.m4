
# AC_DYLP_EQUIV_FOR_CPP_BOOL
AC_DEFUN([AC_DYLP_EQUIV_FOR_CPP_BOOL],
[ AC_MSG_NOTICE([Determining C type equivalent for C++ bool.])

  AC_LANG_PUSH(C++)
  AC_COMPUTE_INT([dylp_sizeof_cpp_bool],
    [(long) (sizeof (bool))],[],
    [AC_MSG_FAILURE([cannot compute sizeof (bool) for C++])])
  AC_LANG_POP(C++)

# Uncomment to force a particular value to test the code below.
# dylp_sizeof_cpp_bool=8

  AC_MSG_NOTICE([C++ bool is $dylp_sizeof_cpp_bool bytes.])

  AC_LANG_PUSH(C)
  dylp_booltype="no"
  AC_COMPUTE_INT([dylp_sizeof_c_bool],
    [(long) (sizeof (char))],[],
    [AC_MSG_FAILURE([cannot compute sizeof (char) for C])])
  if test $dylp_sizeof_cpp_bool -eq $dylp_sizeof_c_bool ; then
    dylp_booltype="char"
  fi
  if test $dylp_booltype = "no"; then
    AC_COMPUTE_INT([dylp_sizeof_c_bool],
      [(long) (sizeof (int))],[],
      [AC_MSG_FAILURE([cannot compute sizeof (int) for C])])
    if test $dylp_sizeof_cpp_bool = $dylp_sizeof_c_bool ; then
      dylp_booltype="int"
    fi
  fi
  if test $dylp_booltype = "no"; then
    AC_COMPUTE_INT([dylp_sizeof_c_bool],
      [(long) (sizeof (short int))],[],
      [AC_MSG_FAILURE([cannot compute sizeof (short int) for C])])
    if test $dylp_sizeof_cpp_bool = $dylp_sizeof_c_bool ; then
      dylp_booltype="short int"
    fi
  fi
  if test $dylp_booltype = "no"; then
    AC_COMPUTE_INT([dylp_sizeof_c_bool],
      [(long) (sizeof (long int))],[],
      [AC_MSG_FAILURE([cannot compute sizeof (long int) for C])])
    if test $dylp_sizeof_cpp_bool = $dylp_sizeof_c_bool ; then
      dylp_booltype="long int"
    fi
  fi
  if test $dylp_booltype = "no"; then
    dylp_booltype="char"
    AC_MSG_WARN([Cannot determine C type to match C++ bool. Defaulting to
		 char. Dylp will compile, but will certainly crash when
		 executed.])
  fi
  AC_DEFINE_UNQUOTED([BOOL],[$dylp_booltype],
      [Define to the C type whose size in bytes matches the size in bytes
       of the the C++ bool type.])
  AC_LANG_POP(C)

  AC_MSG_NOTICE([C $dylp_booltype will be used as bool by dylp.])
])

