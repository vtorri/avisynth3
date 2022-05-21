/* Avisynth 3.0 C Interface
 * Copyright 2005 Vincent Torri <vtorri at univ-evry dot fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA, or visit
 * http://www.gnu.org/copyleft/gpl.html .
 *
 * As a special exception, I give you permission to link to the
 * Avisynth C interface with independent modules that communicate with
 * the Avisynth C interface solely through the interfaces defined in
 * avisynth_c.h, regardless of the license terms of these independent
 * modules, and to copy and distribute the resulting combined work
 * under terms of your choice, provided that every copy of the
 * combined work is accompanied by a complete copy of the source code
 * of the Avisynth C interface and Avisynth itself (with the version
 * used to produce the combined work), being distributed under the
 * terms of the GNU General Public License plus this exception.  An
 * independent module is a module which is not derived from or based
 * on Avisynth C Interface, such as 3rd-party filters, import and
 * export plugins, or graphical user interfaces.
 */

#ifndef __AVISYNTH_3_0_C_H__
#define __AVISYNTH_3_0_C_H__


/**
 * @file avisynth_c.h
 * @brief The file that should be included by any project using the C
 * interface of avisynth 3.0.
 *
 * The file that should be included by any project using the C
 * interface of avisynth 3.0. It provides all the necessary headers
 * and includes to work with the C interface. It is discouraged to
 * include each header file individually.
 */

/**
 * @page doc_api_c Avisynth 3.0 C Interface
 * @version 0.1
 * @author Vincent Torri < vtorri at univ-evry dot fr >
 * @date 2006
 *
 * @section summary Summary
 *
 * @li @ref intro
 * @li @ref presentation
 * @li @ref how_to_get_info
 * @li @ref how_to_get_data
 * @li @ref api
 *
 * @section intro Introduction
 *
 * Avisynth 3.0 is a powerful frameserver for Windows and Linux. It
 * aims at editing and processing videos in a non linear manner. It is
 * written in C++, but a C API is provided, for those who do not like
 * this language.
 *
 * This document presents the complete C API and a general
 * presentation on how to use it
 *
 * @section presentation Presentation
 *
 * This API has been used (and actually written) for the test
 * application that is provided with avisynth. It is usually
 * sufficient for the most common use of avisynth when writing a
 * program, that is: reading a script.
 *
 * The general use of this API is:
 * @li You create an environment with avs_environment_new().
 * @li You create a clip from a file or a script (with respectively
 * avs_clip_new_from_file() and avs_clip_new_from_script()).
 * @li You can get informations on the script with
 * avs_clip_videoinfo_get().
 * @li You can get the data of the frames with
 * avs_clip_videoframe_get().
 *
 * It is then easy to create an input for programs like x264 or
 * gstreamer. We now give two usefull examples of the use of this
 * API.
 *
 * @section how_to_get_info How to get informations of a clip
 *
 * As mentioned above, it is sufficient to get an environment, a clip,
 * the @link ::AVS_VideoInfo AVS_VideoInfo @endlink of this clip, and use the API to get inforations
 * about the clip:
 *
 * @code
#include <stdio.h>

int
main (int argc, char *argv[])
{
  Avs_Environment *p_env;
  Avs_Clip        *p_clip;
  AVS_VideoInfo   *p_vi;

  if (argc < 2) {
    printf ("Usage: %s file.avs\n", argv[0]);
    return 0;
  }

  p_env = avs_environment_new (1000000);
  if (!p_env)
    goto quit;

  p_clip = avs_clip_new_from_file (argv[1]);
  if (!p_clip)
    goto free_env;

  p_vi = avs_clip_videoinfo_get (p_clip);
  if (!p_vi)
    goto free_clip;

  if (avs_videoinfo_has_video (p_vi) {
    AVS_ColorSpace *p_csp;

    printf ("size.........: %dx%d\n",
            avs_videoinfo_width_get (p_vi),
            avs_videoinfo_height_get (p_vi));
    printf ("frame count..: %d\n",
            avs_videoinfo_framecount_get (p_vi));
    printf ("frame ratet..: %d/%d\n",
            avs_videoinfo_fps_numerator_get (p_vi),
            avs_videoinfo_fps_denominator_get (p_vi));
    p_csp = avs_videoinfo_colorspace_get (p_vi);
    if (p_csp) {
      printf ("colorspace...: ");
      switch (avs_colorspace_id_get (p_csp)) {
      case I_RGB32:
        printf ("RGB32\n");
        break;
      case I_YV12:
        printf ("YV12\n");
        break;
      default:
        printf ("Other (too lazy to list all of them)\n");
        break;
      }
      avs_colorspace_delete (p_csp);
    }
  }
  else {
    printf ("No video stream\n");
  }

  avs_videoinfo_delete (p_vi);
 free_clip:
  avs_clip_delete (p_clip);
 free_env:
  avs_environment_delete (p_env);
 quit:
  return 0;
}
 * @endcode
 *
 * @section how_to_get_data How to get raw data of a video stream
 *
 * Again, it is sufficient to get an environment, a clip,
 * the @link ::AVS_VideoFrame AVS_VideoFrame @endlink of a plane of this clip, and use the API to get
 * the raw data of the plane. We suppose here that the colorspace of
 * the video stream is RGB32:
 *
 * @code
#include <stdio.h>

int
main (int argc, char *argv[])
{
  Avs_Environment *p_env;
  Avs_Clip        *p_clip;
  AVS_VideoInfo   *p_vi;
  AVS_VideoFrame  *p_vf;

  if (argc < 2) {
    printf ("Usage: %s file.avs\n", argv[0]);
    return 0;
  }

  p_env = avs_environment_new (1000000);
  if (!p_env)
    goto quit;

  p_clip = avs_clip_new_from_file (argv[1]);
  if (!p_clip)
    goto free_env;

  p_vi = avs_clip_videoinfo_get (p_clip);
  if (!p_vi)
    goto free_clip;

  p_vf = avs_clip_videoframe_get (p_clip);
  if (!p_vf)
    goto free_videoinfo;

  if (avs_videoinfo_has_video (p_vi) {
    AVS_ColorSpace *p_csp;
    unsigned char  *data = NULL;

    p_csp = avs_videoinfo_colorspace_get (p_vi);
    if (p_csp) {
      if (avs_colorspace_id_get (p_csp) == I_RGB32) {
        data = avs_videoframe_plane_get ('~');

        avs_colorspace_delete (p_csp);
      }
    }
  }
  else {
    printf ("No video stream\n");
  }

  avs_videoframe_delete (p_vf);
 free_videoinfo:
  avs_videoinfo_delete (p_vi);
 free_clip:
  avs_clip_delete (p_clip);
 free_env:
  avs_environment_delete (p_env);
 quit:
  return 0;
}
 * @endcode
 *
 * @section api API
 *
 * @li @link clip_c.h Clip @endlink
 * @li @link colorspace_c.h Colorspace @endlink
 * @li @link runtime_environment_c.h Environment @endlink
 * @li @link videoframe_c.h VideoFrame @endlink
 * @li @link videoinfo_c.h VideoInfo @endlink
 *
 */


/* Always use C linkage */
#ifdef __cplusplus
extern "C"
{
#endif


#ifndef __AVISYNTH_H__
enum { AVISYNTH_INTERFACE_VERSION = 1 };
#endif


#include "avisynth_c/clip_c.h"
#include "avisynth_c/colorspace_c.h"
#include "avisynth_c/runtime_environment_c.h"
#include "avisynth_c/videoframe_c.h"
#include "avisynth_c/videoinfo_c.h"


#ifdef __cplusplus
}
#endif


#endif /* __AVISYNTH_3_0_C_H__ */
