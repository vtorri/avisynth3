dnl Configure path for FFMPEG
dnl Vincent Torri 2005-01-06
dnl
dnl AM_CHECK_FFMEG([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for FFMPEG, and define FFMPEG_LIBS, FFMPEG_CFLAGS, FFMPEG_DEBUG_MODE_CFLAGS and
dnl FFMPEG_SOURCE_FILE_ADD (should be removed later).
dnl
AC_DEFUN([AM_CHECK_FFMPEG],
   [
    dnl
    dnl Enable FFMPEG support (default is yes).
    dnl
    AC_ARG_ENABLE([ffmpeg],
       AC_HELP_STRING(
          [--enable-ffmpeg (default is yes)],
          [Enable FFMPEG support]),
       [if test "${enable_ffmpeg}" = "no" ; then
          ffmpeg_enable="no"
        else
           if test "${enable_ffmpeg}" = "yes" ; then
              ffmpeg_enable="yes"
           fi
        fi],
      [ffmpeg_enable="yes"])
    dnl
    dnl Get the prefix where FFMPEG is installed.
    dnl
    AC_ARG_WITH(
       [ffmpeg-path],
       AC_HELP_STRING(
          [--with-ffmpeg-path=PATH (default is default path search)],
          [FFMPEG Library prefix]),
       [with_ffmpeg_arg="yes"],
       [with_ffmpeg_arg="no"])
    dnl
    dnl Enable FFMPEG debug mode (default is no).
    dnl
    AC_ARG_ENABLE(
       [ffmpeg-debug],
       AC_HELP_STRING(
          [--enable-ffmpeg-debug],
          [Enable debug mode for ffmpeg (default is no)]),
       [if test "${enable_ffmpeg_debug}" = "no" ; then
           ffmpeg_debug_mode="no"
        else
           if test "${enable_ffmpeg_debug}" = "yes" ; then
              ffmpeg_debug_mode="yes"
           fi
        fi],
       [ffmpeg_debug_mode="no"])
    dnl
    dnl Check FFMPEG library.
    dnl
    if test "${with_ffmpeg_path+set}" = set && test "x${with_ffmpeg_arg}" != "xno" ; then
       dnl Argument given and not empty.
       ffmpeg_path="${with_ffmpeg_path}"
       saved_CXXFLAGS="${CXXFLAGS}"
       saved_LDFLAGS="${LDFLAGS}"
       CXXFLAGS="${CXXFLAGS} -I${ffmpeg_path}/include"
       LDFLAGS="${LDFLAGS} -L${ffmpeg_path}/lib/"
       AC_CHECK_HEADERS(
          [ffmpeg/avcodec.h ffmpeg/avformat.h],
          [AC_CHECK_LIB(
              [avcodec],
              [main],
              [AC_CHECK_LIB(
                 [avformat],
                 [main],
                 [],
                 [AC_MSG_ERROR(FFMPEG library not in ${ffmpeg_path}/lib)])],
              [AC_MSG_ERROR(FFMPEG library not in ${ffmpeg_path}/lib)])
           FFMPEG_CFLAGS="-DAVS_HAS_FFMPEGSOURCE -I${ffmpeg_path}/include"
           FFMPEG_LIBS="-L${ffmpeg_path}/lib/ -lavformat -lavcodec -lz"],
          [AC_MSG_ERROR(FFMPEG headers not in ${ffmpeg_path}/include)])
       CXXFLAGS="$saved_CXXFLAGS"
       LDFLAGS="$saved_LDFLAGS"
    else
      dnl No argument, or argument empty. Try to check FFMPEG in /usr/local
      AC_CHECK_HEADERS(
         [ffmpeg/avformat.h],
         [AC_CHECK_HEADERS(
            [ffmpeg/avcodec.h ],
            [AC_CHECK_LIB(
               [avformat],
               [main],
               [AC_CHECK_LIB(
                  [avcodec],
                  [main],
                  [],
                  [AC_MSG_WARN([FFMPEG library missing. Run configure --help])
                   AC_MSG_WARN([to see how to configure the path of FFMPEG])
                   AC_MSG_WARN([FFMPEG support disable])
                   ffmpeg_enable="no"])],
               [AC_MSG_WARN([FFMPEG library missing. Run configure --help])
                AC_MSG_WARN([to see how to configure the path of FFMPEG])
                AC_MSG_WARN([FFMPEG support disable])
                ffmpeg_enable="no"])],
            [AC_MSG_WARN([FFMPEG library missing. Run configure --help])
             AC_MSG_WARN([to see how to configure the path of FFMPEG])
             AC_MSG_WARN([FFMPEG support disable])
             ffmpeg_enable="no"])
          FFMPEG_CFLAGS="-DAVS_HAS_FFMPEGSOURCE"
          FFMPEG_LIBS="-lavformat -lavcodec -lz"],
         [AC_MSG_WARN([FFMPEG library missing. Run configure --help])
          AC_MSG_WARN([to see how to configure the path of FFMPEG])
          AC_MSG_WARN([FFMPEG support disable])
          ffmpeg_enable="no"])
    fi
    if test "x${ffmpeg_enable}" = "xyes" ; then
       m4_if([$1], [], [:], [$1])
    else
       m4_if([$2], [], [:], [$2])
    fi
    dnl
    dnl Check FFMPEG debug mode.
    dnl
    FFMPEG_DEBUG_MODE_CFLAGS=
    if test "x${ffmpeg_enable}" = "xyes" &&  test "x${ffmpeg_debug_mode}" = "xyes" ; then
       FFMPEG_DEBUG_MODE_CFLAGS="-DFF_DEBUG=1"
    fi
    dnl
    dnl Check FFMPEG debug mode.
    dnl FIXME: should be removed later.
    dnl
    FFMPEG_SOURCE_FILE_ADD=
    if test "x${ffmpeg_enable}" = "xyes" ; then
       FFMPEG_SOURCE_FILE_ADD="echo   filters/source/ffmpegsource.cpp >> sources.inc"
    fi
    dnl
    dnl Substitution
    dnl
    AC_SUBST([FFMPEG_CFLAGS])
    AC_SUBST([FFMPEG_LIBS])
    AC_SUBST(FFMPEG_DEBUG_MODE_CFLAGS)
    AC_SUBST(FFMPEG_SOURCE_FILE_ADD)
   ])
dnl End of ffmpeg.m4
