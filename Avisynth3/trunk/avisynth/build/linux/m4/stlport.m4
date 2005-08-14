dnl Configure path for STLport
dnl Vincent Torri 2005-01-06
dnl
dnl AM_CHECK_STLPORT(platform [, ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for STLport 4.6.2 or 5.0, and define STLPORT_LIBS and STLPORT_CFLAGS.
dnl
AC_DEFUN([AM_CHECK_STLPORT],
   [
    dnl
    dnl Get the prefix where STLport is installed.
    dnl
    AC_ARG_WITH(
       [stl-path],
       AC_HELP_STRING(
          [--with-stl-path=PATH (default is /usr/local)],
          [STL Library prefix]),
       [with_stl_arg="yes"],
       [with_stl_arg="no"])
    dnl
    dnl Set STLport 5.0 (cvs for now...) instead of STLport 4.6.2.
    dnl In the future, STLport 5.0 will be required.
    dnl
    AC_ARG_ENABLE(
       [stlport5],
       AC_HELP_STRING(
          [--enable-stlport5],
          [Enable the use of STLport 5.0 (default is 4.6.2)]),
       [if test "${enable_stlport5}" = "no" ; then
           use_stlport5="no"
        else
           if test "${enable_stlport5}" = "yes" ; then
              use_stlport5="yes"
           fi
        fi],
       [use_stlport5="no"])
    dnl
    dnl Get the name of the library with respect
    dnl to the platform and the version of STLport.
    dnl
    case "$1" in
       *bsd* | linux* | irix* | solaris* )
          if test "x${use_stlport5}" = "xyes" ; then
             STLPORT_LIB_NAME="stlportg"
          else
             STLPORT_LIB_NAME="stlport_gcc"
          fi
       ;;
       [[cC]][[yY]][[gG]][[wW]][[iI]][[nN]]*|mingw32*|mks*)
          if test "x${use_stlport5}" = "xyes" ; then
             STLPORT_LIB_NAME="stlportg"
          else
             STLPORT_LIB_NAME="stlport_mingw32_static"
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
    if test "${with_stl_path+set}" = set && test "x${with_stl_arg}" != "xno"; then
       dnl Argument given and not empty.
       stl_path="${with_stl_path}"
    else
       dnl No argument, or argument empty.
       stl_path="/usr/local"
    fi
    dnl We check the headers, then the library.
    saved_CPPFLAGS="${CPPFLAGS}"
    saved_LDFLAGS="${LDFLAGS}"
    CPPFLAGS="${CPPFLAGS} -I${stl_path}/include/stlport"
    LDFLAGS="${LDFLAGS} -L${stl_path}/lib ${STLPORT_LIB_NAME}"
    AC_CHECK_HEADERS(
       [stl/_config.h],
       [AC_CHECK_LIB(
          [${STLPORT_LIB_NAME}],
          [main],
          [STLPORT_LIBS="-L${stl_path}/lib ${STLPORT_LIB_NAME}"],
          [AC_MSG_WARN(STLport library not in ${stl_path}/lib)
           m4_if([$3], [], [:], [$3])])
        STLPORT_CFLAGS="-I${stl_path}/include/stlport"
        m4_if([$2], [], [:], [$2])],
       [AC_MSG_WARN(STLport headers not in ${stl_path}/include/stlport)
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
