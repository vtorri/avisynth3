dnl Configure path for Boost
dnl Vincent Torri 2005-01-06
dnl
dnl AM_CHECK_BOOST(version, platform [, ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
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
          [--with-boost-path=PATH (default is /usr/local)],
          [BOOST Library prefix]),
       [with_boost_arg="yes"],
       [with_boost_arg="no"])

    dnl
    dnl Get the include directory where Boost headers are installed.
    dnl
    AC_ARG_WITH(
       [boost-includedir-path],
       AC_HELP_STRING(
          [--with-boost-includedir-path=PATH (default is /usr/local/include/boost-1_33)],
          [BOOST headers directory. It must contain the boost/ directory]),
       [with_boost_includedir_arg="yes"],
       [with_boost_includedir_arg="no"])

    dnl
    dnl Get the library directory where Boost libraries are installed.
    dnl
    AC_ARG_WITH(
       [boost-libdir-path],
       AC_HELP_STRING(
          [--with-boost-libdir-path=PATH (default is /usr/local/lib)],
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
    case "$2" in
       *bsd* | linux* | irix* | solaris* )
          if test x"${core_debug_mode}" = x"yes" ; then
             if test x"${with_boost_lib_debug_name_arg}" = x"yes" ; then
                BOOST_LIB_NAME=${with_boost_lib_name}
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
       boost_path="/usr/local"
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

    dnl We check the headers.
    have_boost_headers="no"
    saved_CPPFLAGS="${CPPFLAGS}"
    CPPFLAGS="${CPPFLAGS} -I${boost_includedir_path}"

    AC_CHECK_HEADERS(
       [boost/shared_ptr.hpp boost/thread/mutex.hpp boost/circular_buffer.hpp boost/spirit/phoenix/functions.hpp],
       [
        BOOST_CFLAGS="-I${boost_includedir_path}"
        have_boost_headers="yes"
       ],
       [
        AC_MSG_WARN(Boost headers not in ${boost_includedir_path})
        m4_if([$4], [], [:], [$4])
       ]
    )
    CPPFLAGS="${saved_CPPFLAGS}"
    
    dnl We check the library if headers are not missing.
    if test "x${have_boost_headers}" = "xyes" ; then
       saved_LDFLAGS="${LDFLAGS}"
       case "$2" in
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
                 AC_MSG_WARN(Boost library not in ${boost_libdir_path})
                 m4_if([$4], [], [:], [$4])
                ]
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
                 AC_MSG_WARN(Boost library not in ${boost_libdir_path})
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
