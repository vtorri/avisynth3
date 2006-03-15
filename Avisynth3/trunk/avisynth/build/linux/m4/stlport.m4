dnl Configure path for STLport
dnl Vincent Torri 2005-12-16
dnl
dnl AM_CHECK_STLPORT(platform [, ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for STLport 5.0 and define STLPORT_LIBS and STLPORT_CFLAGS.
dnl
AC_DEFUN([AM_CHECK_STLPORT],
   [
    dnl
    dnl Get the prefix where STLport is installed.
    dnl
    AC_ARG_WITH(
       [stlport-path],
       AC_HELP_STRING(
          [--with-stlport-path=PATH (default is /usr/local)],
          [STLport Library prefix]),
       [with_stlport_arg="yes"],
       [with_stlport_arg="no"])

    dnl
    dnl Name of the thread library option.
    dnl
    AC_ARG_WITH(
       [stlport-lib-name],
       AC_HELP_STRING(
          [--with-stlport-lib-name=name],
          [STLport library name]),
       [with_stlport_lib_name_arg="yes"],
       [with_stlport_lib_name_arg="no"])

    dnl
    dnl Name of the thread library with debug symbols option.
    dnl
    AC_ARG_WITH(
       [stlport-lib-debug-name],
       AC_HELP_STRING(
          [--with-stlport-lib-debug-name=name],
          [STLport library name with debug symbols]),
       [with_stlport_lib_debug_name_arg="yes"],
       [with_stlport_lib_debug_name_arg="no"])

    dnl
    dnl Get the name of the library with respect
    dnl to the platform and the version of STLport.
    dnl
    case "$1" in
       *bsd* | linux* | irix* | solaris* )
          if test x"${core_debug_mode}" = x"yes" ; then
             if test x"${with_stlport_lib_debug_name_arg}" = x"yes" ; then
                STLPORT_LIB_NAME=${with_stlport_lib_name}
             else
                STLPORT_LIB_NAME="stlportg"
             fi
          else
             if test x"${with_stlport_lib_name_arg}" = x"yes" ; then
                STLPORT_LIB_NAME=${with_stlport_lib_name}
             else
                STLPORT_LIB_NAME="stlport"
             fi
          fi
       ;;
       [[cC]][[yY]][[gG]][[wW]][[iI]][[nN]]*|mingw32*|mks*)
          if test x"${core_debug_mode}" = x"yes" ; then
             STLPORT_LIB_NAME="stlportg"
          else
             STLPORT_LIB_NAME="stlport"
          fi
       ;;
       darwin*|raphsody*)
          STLPORT_LIB_NAME=""
       ;;
       beos)
          STLPORT_LIB_NAME=""
       ;;
       *)
          STLPORT_LIB_NAME=""
       ;;
    esac

    dnl
    dnl Check STLport library.
    dnl
    dnl We set the path to use.
    if test "${with_stlport_path+set}" = set && test "x${with_stlport_arg}" != "xno"; then
       dnl Argument given and not empty.
       stlport_path="${with_stlport_path}"
    else
       dnl No argument, or argument empty.
       stlport_path="/usr/local"
    fi
    dnl We check the headers, then the library.
    saved_CPPFLAGS="${CPPFLAGS}"
    saved_LDFLAGS="${LDFLAGS}"
    CPPFLAGS="${CPPFLAGS} -I${stlport_path}/include/stlport"
    LDFLAGS="${LDFLAGS} -L${stlport_path}/lib"
    AC_CHECK_HEADERS(
       [stl/_config.h],
       [AC_CHECK_LIB(
          [${STLPORT_LIB_NAME}],
          [main],
          [STLPORT_LIBS="-L${stlport_path}/lib -l${STLPORT_LIB_NAME}"],
          [AC_MSG_WARN(STLport library not in ${stlport_path}/lib)
           m4_if([$3], [], [:], [$3])])
        STLPORT_CFLAGS="-I${stlport_path}/include/stlport"
        m4_if([$2], [], [:], [$2])],
       [AC_MSG_WARN(STLport headers not in ${stlport_path}/include/stlport)
        m4_if([$3], [], [:], [$3])])
    CPPFLAGS="${saved_CPPFLAGS}"
    LDFLAGS="${saved_LDFLAGS}"
    dnl
    dnl Substitution
    dnl
    AC_SUBST([STLPORT_CFLAGS])
    AC_SUBST([STLPORT_LIBS])
   ])
dnl End of stlport.m4
