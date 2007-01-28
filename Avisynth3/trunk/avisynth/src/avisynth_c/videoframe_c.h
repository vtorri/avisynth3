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

#ifndef __VIDEOFRAME_C_H__
#define __VIDEOFRAME_C_H__


/* C API include */
#include "define_c.h"


/**
 * @file videoframe_c.h
 * @brief C interface for accessing the data of frames.
 *
 * C interface for accessing the data of frames. A videoframe is
 * created with the function avs_clip_videoframe_get().
 *
 * You can get the size of the frame with the functions
 * avs_videoframe_width_get() and avs_videoframe_height_get().
 *
 * You can retrieve the data of a frame with the function
 * avs_videoframe_plane_get(). This function takes a char as
 * parameter, which corresponds to the plane that you want to get. If
 * the colorspace is interleaved (RGB24, RGB32 or YUY2), then use the
 * character '~'. If the colorspace is planar (YV12 or YV24), use 'Y',
 * 'U' or 'V' to access respectively to the Y, U and V planes.
 *
 * If you want to blit the frame on your own data, use
 * avs_videoframe_plane_blit().
 *
 * Once you have finished with an #AVS_VideoFrame, you delete it with
 * avs_videoframe_delete().
 */

/**
 * opaque declaration of a videoinfo
 */
typedef struct AVS_VideoFrame_ AVS_VideoFrame;


/** @brief Delete an AVS_VideoFrame.
 *
 * @param p_vf The frame to delete.
 *
 * Delete the frame @p p_vf.
 */
AVS_C_API void avs_videoframe_delete (AVS_VideoFrame *p_vf);


/** @brief Retrieve the width of a frame.
 *
 * @param p_vf The videoframe.
 * @return The width of the frame.
 *
 * Retrieve the width of the frame @p p_vf. If
 * @p p_vf is @c NULL, this function returns @c 0.
 */
AVS_C_API int avs_videoframe_width_get (const AVS_VideoFrame *p_vf);


/** @brief Retrieve the height of a frame.
 *
 * @param p_vf The videoframe.
 * @return The height of the frame.
 *
 * Retrieve the height of the frame @p p_vf. If
 * @p p_vf is @c NULL, this function returns @c 0.
 */
AVS_C_API int avs_videoframe_height_get (const AVS_VideoFrame *p_vf);


/** @brief Retrieve the plane of a frame.
 *
 * @param p_vf The videoframe.
 * @param plane The plane.
 * @return The pitch of the data of the plane.
 *
 * Retrieve the pitch of the frame @p p_vf given by @p plane. For an
 * interleaved colorspace, use '~'. For a planar colorspace, use 'Y',
 * 'U' or 'V' for respctively the planes Y, U and V. If
 * @p p_vf is @c NULL, this function returns @c 0, otherwise, it
 * returns the pitch of the data of the plane.
 */
AVS_C_API int avs_videoframe_pitch_get (const AVS_VideoFrame *p_vf, char plane);


/** @brief Retrieve the plane of a frame.
 *
 * @param p_vf The videoframe.
 * @param plane The plane.
 * @return The pitch of the data of the plane.
 *
 * Retrieve the pad of the frame @p p_vf given by @p plane. For an
 * interleaved colorspace, use '~'. For a planar colorspace, use 'Y',
 * 'U' or 'V' for respctively the planes Y, U and V. If
 * @p p_vf is @c NULL, this function returns @c 0, otherwise, it
 * returns the pad of the data of the plane.
 */
AVS_C_API int avs_videoframe_pad_get (const AVS_VideoFrame *p_vf, char plane);


/** @brief Retrieve the plane of a frame.
 *
 * @param p_vf The videoframe.
 * @param plane The plane.
 * @return The pointer on the data of the plane.
 *
 * Retrieve the plane of the frame @p p_vf given by @p plane. For an
 * interleaved colorspace, use '~'. For a planar colorspace, use 'Y',
 * 'U' or 'V' for respctively the planes Y, U and V. If
 * @p p_vf is @c NULL, this function returns @c NULL, otherwise, it
 * returns a pointer on the data of the plane.
 */
AVS_C_API const unsigned char *avs_videoframe_plane_get (const AVS_VideoFrame *p_vf, char plane);


/** @brief Blit the plane of a frame on some data.
 *
 * @param p_vf The videoframe.
 * @param plane The plane to blit.
 * @param ptr the pointer on which we want to blit.
 * @param pitch The pitch to use.
 *
 * Blit the plane @p plane of the frame @p p_vf on the data specified
 * by @p ptr. The pitch is set by @p pitch. If @p p_vf is @c NULL,
 * this function does nothing.
 */
AVS_C_API void avs_videoframe_plane_blit (const AVS_VideoFrame *p_vf, char plane, unsigned char *ptr, int pitch);


#endif /* __VIDEOFRAME_C_H__ */
