dnl Configure path for Boost
dnl Vincent Torri 2004-2006
dnl
dnl AM_CHECK_BOOST(version, prefix [, ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for BOOST, and define BOOST_LIBS and BOOST_CFLAGS.
dnl
AC_DEFUN([AM_CHECK_BOOST],
   [
    dnl
    dnl Get the prefix where Boost is installed.
    dnl
    AC_ARG_WITH(
       [boost-path],
       AC_HELP_STRING(
          [--with-boost-path=PATH],
          [BOOST Library prefix]),
       [with_boost_arg="yes"],
       [with_boost_arg="no"])

    dnl
    dnl Get the include directory where Boost headers are installed.
    dnl
    AC_ARG_WITH(
       [boost-includedir-path],
       AC_HELP_STRING(
          [--with-boost-includedir-path=PATH],
          [BOOST headers directory. It must contain the boost/ directory]),
       [with_boost_includedir_arg="yes"],
       [with_boost_includedir_arg="no"])

    dnl
    dnl Get the library directory where Boost libraries are installed.
    dnl
    AC_ARG_WITH(
       [boost-libdir-path],
       AC_HELP_STRING(
          [--with-boost-libdir-path=PATH],
          [BOOST libraries directory]),
       [with_boost_libdir_arg="yes"],
       [with_boost_libdir_arg="no"])

    dnl
    dnl Name of the thread library option.
    dnl
    AC_ARG_WITH(
       [boost-lib-name],
       AC_HELP_STRING(
          [--with-boost-lib-name=name],
          [BOOST thread library name]),
       [with_boost_lib_name_arg="yes"],
       [with_boost_lib_name_arg="no"])

    dnl
    dnl Name of the thread library with debug symbols option.
    dnl
    AC_ARG_WITH(
       [boost-lib-debug-name],
       AC_HELP_STRING(
          [--with-boost-lib-debug-name=name],
          [BOOST thread library name with debug symbols]),
       [with_boost_lib_debug_name_arg="yes"],
       [with_boost_lib_debug_name_arg="no"])

    dnl
    dnl Get the name of the library with respect
    dnl to the platform and the version of Boost.
    dnl
    case "${target_os}" in
       *bsd* | linux* | irix* | solaris* )
          if test x"${core_debug_mode}" = x"yes" ; then
             if test x"${with_boost_lib_debug_name_arg}" = x"yes" ; then
                BOOST_LIB_NAME=${with_boost_lib_debug_name}
             else
                BOOST_LIB_NAME="boost_thread-gcc-mt-d-$1"
             fi
          else
             if test x"${with_boost_lib_name_arg}" = x"yes" ; then
                BOOST_LIB_NAME=${with_boost_lib_name}
             else
                BOOST_LIB_NAME="boost_thread-gcc-mt-$1"
             fi
          fi
          
       ;;
       [[cC]][[yY]][[gG]][[wW]][[iI]][[nN]]*|mingw32*|mks*)
          BOOST_LIB_NAME="boost_thread-mgw-mt-$1.dll"
       ;;
       darwin*|raphsody*)
          BOOST_LIB_NAME=""
       ;;
       beos)
          BOOST_LIB_NAME=""
       ;;
       *)
          BOOST_LIB_NAME=""
       ;;
    esac

    dnl
    dnl Check Boost library.
    dnl

    dnl We set the path to use.
    if test "${with_boost_path+set}" = set && test "x${with_boost_arg}" != "xno"; then
       dnl Argument given and not empty.
       boost_path="${with_boost_path}"
    else
       dnl No argument, or argument empty.
       boost_path="$2"
    fi

    dnl We set the include directory to use.
    if test "${with_boost_includedir_path+set}" = set && test "x${with_boost_includedir_arg}" != "xno"; then
       dnl Argument given and not empty.
       boost_includedir_path="${with_boost_includedir_path}"
    else
       dnl No argument, or argument empty.
       boost_includedir_path="${boost_path}/include/boost-$1"
    fi

    dnl We set the lib directory to use.
    if test "${with_boost_libdir_path+set}" = set && test "x${with_boost_libdir_arg}" != "xno"; then
       dnl Argument given and not empty.
       boost_libdir_path="${with_boost_libdir_path}"
    else
       dnl No argument, or argument empty.
       boost_libdir_path="${boost_path}/lib"
    fi

    dnl We check the header files.
    have_boost_headers="no"
    saved_CPPFLAGS="${CPPFLAGS}"
    CPPFLAGS="${CPPFLAGS} -I${boost_includedir_path}"

    AC_MSG_CHECKING([for Boost header files in ${boost_includedir_path}])
    AC_COMPILE_IFELSE(
       [
        AC_LANG_PROGRAM(
           [[
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/spirit/core.hpp>
#include <boost/spirit/phoenix/functions.hpp>
           ]])
       ],
       [
        AC_MSG_RESULT([yes])
        BOOST_CFLAGS="-I${boost_includedir_path}"
        have_boost_headers="yes"
       ],
       [
        AC_MSG_RESULT([no])
        AC_MSG_WARN(Boost header files not in ${boost_includedir_path})
        m4_if([$4], [], [:], [$4])
       ]
    )
    CPPFLAGS="${saved_CPPFLAGS}"
    
    dnl We check the library if header files are not missing.
    if test "x${have_boost_headers}" = "xyes" ; then
       saved_LDFLAGS="${LDFLAGS}"
       case "${target_os}" in
          *bsd* | linux* | irix* | solaris* )
             LDFLAGS="${LDFLAGS} -L${boost_libdir_path}"
             AC_CHECK_LIB(
                [${BOOST_LIB_NAME}],
                [main],
                [
                 BOOST_LIBS="-L${boost_libdir_path} -l${BOOST_LIB_NAME}"
                 m4_if([$3], [], [:], [$3])
                ],
                [
                 AC_MSG_WARN(Boost library ($1) not in ${boost_libdir_path})
                 m4_if([$4], [], [:], [$4])
                ],
                [-pthread]
             )
          ;;
          [[cC]][[yY]][[gG]][[wW]][[iI]][[nN]]*|mingw32*|mks*)
             LDFLAGS="${LDFLAGS} ${boost_libdir_path}/${BOOST_LIB_NAME}"
             AC_CHECK_FILE(
                [${boost_libdir_path}/${BOOST_LIB_NAME}],
                [
                 BOOST_LIBS="${boost_libdir_path}/${BOOST_LIB_NAME}"
                 m4_if([$3], [], [:], [$3])
                ],
                [
                 AC_MSG_WARN(Boost library ($1) not in ${boost_libdir_path})
                 m4_if([$4], [], [:], [$4])
                ]
             )
          ;;
          darwin*|raphsody*)
             BOOST_LIB=""
          ;;
          beos)
             BOOST_LIB=""
          ;;
          *)
             BOOST_LIB=""
          ;;
       esac
       LDFLAGS="${saved_LDFLAGS}"
    fi

    dnl
    dnl Substitution
    dnl
    AC_SUBST([BOOST_CFLAGS])
    AC_SUBST([BOOST_LIBS])
   ])
dnl End of boost.m4
