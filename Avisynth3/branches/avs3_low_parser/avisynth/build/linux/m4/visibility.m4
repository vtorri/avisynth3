dnl Detection and configuration of the visibility feature of gcc
dnl Vincent Torri 2005-12-28
dnl
dnl AC_CHECK_GCC_VISIBILITY([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Check the visibility feature of gcc and define VISIBILITY_CFLAGS
dnl
AC_DEFUN([AC_CHECK_GCC_VISIBILITY],
   [VISIBILITY_CFLAGS=""
    VISIBILITY_CPPFLAGS=""
    AC_MSG_CHECKING([whether ${CXX} supports -fvisibility=hidden])
    save_CFLAGS=${CFLAGS}
    CFLAGS="$CFLAGS -fvisibility=hidden -fvisibility-inlines-hidden"
    AC_COMPILE_IFELSE(
       [AC_LANG_PROGRAM(
          [[
int a __attribute__ ((visibility("hidden")));
int b __attribute__ ((visibility("default")));
#pragma GCC visibility push(hidden)
extern void f(int);
#pragma GCC visibility pop
#pragma GCC visibility push(default)
extern void g(int);
#pragma GCC visibility pop
          ]],
          [[]]
        )],
       [VISIBILITY_CFLAGS="-fvisibility=hidden -fvisibility-inlines-hidden"
        VISIBILITY_CPPFLAGS="-DHAVE_GCC_VISIBILITY_FEATURE"
        m4_if([$1], [], [:], [$1])
        AC_MSG_RESULT("yes")],
       [m4_if([$2], [], [:], [$2])
        AC_MSG_RESULT("no")])
    CFLAGS=${save_CFLAGS}
    AC_SUBST(VISIBILITY_CFLAGS)
    AC_SUBST(VISIBILITY_CPPFLAGS)
   ])
