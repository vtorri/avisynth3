dnl PKG_GET_VARIABLE([VARIABLE], [PKG_VARIABLE], [MODULES])
AC_DEFUN([PKG_GET_VARIABLE],
   [AC_REQUIRE([PKG_PROG_PKG_CONFIG])
    AC_MSG_CHECKING($2 for $3)
    _PKG_CONFIG([$1], [variable=$2], [$3])
    if test -n "pkg_cv_[]$1"; then
       $1="${pkg_cv_[]$1}"
       AC_MSG_RESULT(${$1})
    else
       AC_MSG_RESULT(not found)
    fi
   AC_SUBST($1)
   ]
)
