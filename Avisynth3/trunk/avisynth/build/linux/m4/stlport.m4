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
          [--with-stlport-shared-lib-name=name],
          [STLport shared library name]),
       [with_stlport_lib_name_arg="yes"],
       [with_stlport_lib_name_arg="no"])

    dnl
    dnl Name of the stlport shared library with debug symbols option.
    dnl
    AC_ARG_WITH(
       [stlport-lib-debug-name],
       AC_HELP_STRING(
          [--with-stlport-shared-lib-debug-name=name],
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
                STLPORT_LIB_NAME=${with_stlport_lib_name}
             else
                STLPORT_LIB_NAME="libstlportg.5.1.so"
             fi
          else
             if test x"${with_stlport_lib_name_arg}" = x"yes" ; then
                STLPORT_LIB_NAME=${with_stlport_lib_name}
             else
                STLPORT_LIB_NAME="libstlport.5.1.so"
             fi
          fi
       ;;
       [[cC]][[yY]][[gG]][[wW]][[iI]][[nN]]* | mingw32* | mks*)
          if test x"${core_debug_mode}" = x"yes" ; then
             if test x"${with_stlport_lib_debug_name_arg}" = x"yes" ; then
                STLPORT_LIB_NAME=${with_stlport_lib_name}
             else
                STLPORT_LIB_NAME="stlportg.5.1"
             fi
          else
             if test x"${with_stlport_lib_name_arg}" = x"yes" ; then
                STLPORT_LIB_NAME=${with_stlport_lib_name}
             else
                STLPORT_LIB_NAME="stlport.5.1"
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

    dnl We check the headers, then the library.
    stlport_lib_fullname=${stlport_libdir_path}/${STLPORT_LIB_NAME}
    saved_CPPFLAGS="${CPPFLAGS}"
    CPPFLAGS="${CPPFLAGS} -I${stlport_includedir_path}"
    have_stlport_headers="no"
    AC_CHECK_HEADERS(
       [stl/config/user_config.h],
       [STLPORT_CFLAGS="-I${stlport_includedir_path}"
        have_stlport_headers="yes"],
       [AC_MSG_WARN(STLport headers not in ${stlport_includedir_path})
        m4_if([$3], [], [:], [$3])])
    CPPFLAGS="${saved_CPPFLAGS}"

    saved_LDFLAGS="${LDFLAGS}"
    if test "x${have_stlport_headers}" = "xyes" ; then
       case "${target_os}" in
          *bsd* | linux* | irix* | solaris* )
             AC_CHECK_FILE(
                [${stlport_lib_fullname}],
                [STLPORT_LIBS="${stlport_lib_fullname}"
                 m4_if([$2], [], [:], [$2])],
                [AC_MSG_WARN(STLport shared library not in ${stlport_libdir_path})
                 m4_if([$3], [], [:], [$3])])
          ;;
          [[cC]][[yY]][[gG]][[wW]][[iI]][[nN]]* | mingw32* | mks*)
             LDFLAGS="${LDFLAGS} -L${stlport_libdir_path}"
             AC_CHECK_LIB(
                [${STLPORT_LIB_NAME}],
                [main],
                [STLPORT_LIBS="-L${stlport_libdir_path} -l${STLPORT_LIB_NAME}"
                 m4_if([$2], [], [:], [$2])],
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
    fi
    LDFLAGS="${saved_LDFLAGS}"
    dnl
    dnl Substitution
    dnl
    AC_SUBST([STLPORT_CFLAGS])
    AC_SUBST([STLPORT_LIBS])
   ])
dnl End of stlport.m4
