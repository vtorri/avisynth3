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
    dnl Get the name of the library with respect
    dnl to the platform and the version of Boost.
    dnl
    case "$2" in
       *bsd* | linux* | irix* | solaris* )
          BOOST_LIB_NAME="boost_thread-gcc-mt-s-$1"
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
    dnl We check the headers, then the library.
    saved_CPPFLAGS="${CPPFLAGS}"
    saved_LDFLAGS="${LDFLAGS}"
    CPPFLAGS="${CPPFLAGS} -I${boost_path}/include/boost-$1"
    AC_CHECK_HEADER(
       [boost/config.hpp],
       [
        case "$2" in
           *bsd* | linux* | irix* | solaris* )
              BOOST_LIB_NAME="boost_thread-gcc-mt-$1"
              LDFLAGS="${LDFLAGS} -L${boost_path}/lib/}"
              AC_CHECK_LIB(
                 [${BOOST_LIB_NAME}],
                 [main],
                 [BOOST_LIBS="-L${boost_path}/lib -l${BOOST_LIB_NAME}"],
                 [AC_MSG_WARN(Boost library not in ${boost_path}/lib)
                  m4_if([$4], [], [:], [$4])])

           ;;
           [[cC]][[yY]][[gG]][[wW]][[iI]][[nN]]*|mingw32*|mks*)
              BOOST_LIB_NAME="boost_thread-mgw-mt-$1.dll"
              LDFLAGS="${LDFLAGS} ${boost_path}/lib/${BOOST_LIB_NAME}"
              AC_CHECK_FILE(
                 [${boost_path}/lib/${BOOST_LIB_NAME}],
                 [BOOST_LIBS="${boost_path}/lib/${BOOST_LIB_NAME}"],
                 [AC_MSG_WARN(Boost library not in ${boost_path}/lib)
                  m4_if([$4], [], [:], [$4])])
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
        BOOST_CFLAGS="-I${boost_path}/include/boost-$1"
        m4_if([$3], [], [:], [$3])],
       [AC_MSG_WARN(Boost headers not in ${boost_path}/include/boost-$1)
        m4_if([$4], [], [:], [$4])])
    CPPFLAGS="${saved_CPPFLAGS}"
    LDFLAGS="${saved_LDFLAGS}"
    dnl
    dnl Substitution
    dnl
    AC_SUBST([BOOST_CFLAGS])
    AC_SUBST([BOOST_LIBS])
   ])
dnl End of boost.m4
