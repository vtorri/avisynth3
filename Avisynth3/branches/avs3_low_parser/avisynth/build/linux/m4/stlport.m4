dnl Configure path for STLport
dnl Vincent Torri 2004-2006
dnl
dnl AM_CHECK_STLPORT(prefix [, ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
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
          [--with-stlport-path=PATH],
          [STLport Library prefix]),
       [with_stlport_arg="yes"],
       [with_stlport_arg="no"])

    dnl
    dnl Get the include directory where STLport headers are installed.
    dnl
    AC_ARG_WITH(
       [stlport-includedir-path],
       AC_HELP_STRING(
          [--with-stlport-includedir-path=PATH],
          [STLport headers directory. It must contain the stlport/ directory]),
       [with_stlport_includedir_arg="yes"],
       [with_stlport_includedir_arg="no"])

    dnl
    dnl Get the library directory where STLport libraries are installed.
    dnl
    AC_ARG_WITH(
       [stlport-libdir-path],
       AC_HELP_STRING(
          [--with-stlport-libdir-path=PATH],
          [STLport libraries directory]),
       [with_stlport_libdir_arg="yes"],
       [with_stlport_libdir_arg="no"])

    dnl
    dnl Name of the stlport shared library option.
    dnl
    AC_ARG_WITH(
       [stlport-lib-name],
       AC_HELP_STRING(
          [--with-stlport-lib-name=name],
          [STLport shared library name]),
       [with_stlport_lib_name_arg="yes"],
       [with_stlport_lib_name_arg="no"])

    dnl
    dnl Name of the stlport shared library with debug symbols option.
    dnl
    AC_ARG_WITH(
       [stlport-lib-debug-name],
       AC_HELP_STRING(
          [--with-stlport-lib-debug-name=name],
          [STLport shared library name with debug symbols]),
       [with_stlport_lib_debug_name_arg="yes"],
       [with_stlport_lib_debug_name_arg="no"])

    dnl
    dnl Get the name of the library with respect
    dnl to the platform and the version of STLport.
    dnl
    case "${target_os}" in
       *bsd* | linux* | irix* | solaris* )
          if test x"${core_debug_mode}" = x"yes" ; then
             if test x"${with_stlport_lib_debug_name_arg}" = x"yes" ; then
                STLPORT_LIB_NAME=${with_stlport_lib_debug_name}
             else
                STLPORT_LIB_NAME="libstlportg.so"
             fi
          else
             if test x"${with_stlport_lib_name_arg}" = x"yes" ; then
                STLPORT_LIB_NAME=${with_stlport_lib_name}
             else
                STLPORT_LIB_NAME="libstlport.so"
             fi
          fi
       ;;
       [[cC]][[yY]][[gG]][[wW]][[iI]][[nN]]* | mingw32* | mks*)
          if test x"${core_debug_mode}" = x"yes" ; then
             if test x"${with_stlport_lib_debug_name_arg}" = x"yes" ; then
                STLPORT_LIB_NAME=${with_stlport_lib_debug_name}
             else
                STLPORT_LIB_NAME="stlportg
             fi
          else
             if test x"${with_stlport_lib_name_arg}" = x"yes" ; then
                STLPORT_LIB_NAME=${with_stlport_lib_name}
             else
                STLPORT_LIB_NAME="stlport
             fi
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
       stlport_path="$1"
    fi

    dnl We set the include directory to use.
    if test "${with_stlport_includedir_path+set}" = set && test "x${with_stlport_includedir_arg}" != "xno"; then
       dnl Argument given and not empty.
       stlport_includedir_path="${with_stlport_includedir_path}"
    else
       dnl No argument, or argument empty.
       stlport_includedir_path="${stlport_path}/include/stlport"
    fi

    dnl We set the lib directory to use.
    if test "${with_stlport_libdir_path+set}" = set && test "x${with_stlport_libdir_arg}" != "xno"; then
       dnl Argument given and not empty.
       stlport_libdir_path="${with_stlport_libdir_path}"
    else
       dnl No argument, or argument empty.
       stlport_libdir_path="${stlport_path}/lib"
    fi

    dnl We check the header files
    have_stlport_headers="no"
    saved_CPPFLAGS="${CPPFLAGS}"
    CPPFLAGS="${CPPFLAGS} -I${stlport_includedir_path}"

    AC_MSG_CHECKING([for STLport header files in ${stlport_includedir_path}])
    AC_COMPILE_IFELSE(
       [
        AC_LANG_PROGRAM(
           [[
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <hash_map>
#include <iosfwd>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>
           ]])
       ],
       [
        AC_MSG_RESULT([yes])
        STLPORT_CFLAGS="-I${stlport_includedir_path}"
        have_stlport_headers="yes"
       ],
       [
        AC_MSG_RESULT([no])
        AC_MSG_WARN(STLport header files not in ${stlport_includedir_path})
        m4_if([$3], [], [:], [$3])
       ]
    )
    CPPFLAGS="${saved_CPPFLAGS}"
    
    dnl We check the library if header files are not missing.
    if test "x${have_stlport_headers}" = "xyes" ; then
       saved_LDFLAGS="${LDFLAGS}"
       case "${target_os}" in
          *bsd* | linux* | irix* | solaris* )
             stlport_lib_fullname=${stlport_libdir_path}/${STLPORT_LIB_NAME}
             AC_CHECK_FILE(
                [${stlport_lib_fullname}],
                [
                 STLPORT_LIBS="${stlport_lib_fullname}"
                 m4_if([$2], [], [:], [$2])
                ],
                [
                 AC_MSG_WARN(STLport shared library not in ${stlport_libdir_path})
                 m4_if([$3], [], [:], [$3])
                ]
             )
          ;;
          [[cC]][[yY]][[gG]][[wW]][[iI]][[nN]]* | mingw32* | mks*)
             LDFLAGS="${LDFLAGS} -L${stlport_libdir_path}"
             AC_CHECK_LIB(
                [${STLPORT_LIB_NAME}],
                [main],
                [
                 STLPORT_LIBS="-L${stlport_libdir_path} -l${STLPORT_LIB_NAME}"
                 m4_if([$2], [], [:], [$2])
                ],
                [
                 AC_MSG_WARN(STLport static library not in ${stlport_libdir_path})
                 m4_if([$3], [], [:], [$3])
                ],
                [-mthreads]
             )
          ;;
          darwin*|raphsody*)
          ;;
          beos)
          ;;
          *)
          ;;
       esac
       LDFLAGS="${saved_LDFLAGS}"
    fi

    dnl
    dnl Substitution
    dnl
    AC_SUBST([STLPORT_CFLAGS])
    AC_SUBST([STLPORT_LIBS])
   ])
dnl End of stlport.m4
