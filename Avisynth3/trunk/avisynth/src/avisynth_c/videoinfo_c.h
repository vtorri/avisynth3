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

#ifndef __VIDEOINFO_C_H__
#define __VIDEOINFO_C_H__


/* C API include */
#include "define_c.h"
#include "colorspace_c.h"


/**
 * @file videoinfo_c.h
 * @brief C interface for retrieving informations of a clip.
 *
 * C interface for videoinfos. They are related to the clip they are
 * retrieved from. The clip contains informations about the video
 * stream and the adio stream.
 *
 * For the video stream, you can get the size of the clip, its
 * colorspace, its frame count or framerate. Before checking this,
 * make sure that the clip has a video stream with
 * avs_videoinfo_has_video().
 *
 * For the audio stream, you can get the smaple rate, the sample count
 * and the channel count. As with the video stream, be sure that the
 * clip has an audio stream by using avs_videoinfo_has_audio().
 *
 * Once you have finished with an #AVS_VideoInfo, you delete it with
 * avs_videoinfo_delete().
 */

/**
 * opaque declaration of a videoinfo
 */
typedef struct AVS_VideoInfo_ AVS_VideoInfo;


/** @brief Delete an AVS_VideoInfo.
 *
 * @param p_vi The videoinfo to delete.
 *
 * Delete the videoinfo @p p_vi.
 */
AVS_C_API void avs_videoinfo_delete (AVS_VideoInfo *p_vi);


/** @brief Check wether the clip has a video stream or not.
 *
 * @param p_vi The videoinfo to check.
 * @return @c 1 if the clip has a video stream, @c 0 otherwise.
 *
 * Check wether the clip has a video stream or not. If @p p_vi has
 * informations on video, then the function returns 1, otherwise it
 * returns 0.
 */
AVS_C_API int avs_videoinfo_has_video (const AVS_VideoInfo *p_vi);


/** @brief Retrieve the colorspace of the video.
 *
 * @param p_vi The videoinfo.
 * @return The colorspace of the video stream.
 *
 * Retrieve the colorspace of the video from @p p_vi, as a
 * AVS_ColorSpace, or @c NULL if an error occurred. The returned
 * result must be freed with avs_colorspace_delete().
 */
AVS_C_API AVS_ColorSpace *avs_videoinfo_colorspace_get (const AVS_VideoInfo *p_vi);


/** @brief Retrieve the width of the video.
 *
 * @param p_vi The videoinfo.
 * @return The width of the video.
 *
 * Retrieve the width of the video from the videoinfo @p p_vi. If
 * @p p_vi is @c NULL, this function returns @c 0.
 */
AVS_C_API int avs_videoinfo_width_get (const AVS_VideoInfo *p_vi);


/** @brief Retrieve the height of the video.
 *
 * @param p_vi The videoinfo.
 * @return The height of the video.
 *
 * Retrieve the height of the video from the videoinfo @p p_vi. If
 * @p p_vi is @c NULL, this function returns @c 0.
 */
AVS_C_API int avs_videoinfo_height_get (const AVS_VideoInfo *p_vi);


/** @brief Retrieve the frame count of the video.
 *
 * @param p_vi The videoinfo.
 * @return The frame count of the video.
 *
 * Retrieve the frame count of the video from the videoinfo @p p_vi. If
 * @p p_vi is @c NULL, this function returns @c 0.
 */
AVS_C_API int avs_videoinfo_framecount_get (const AVS_VideoInfo *p_vi);


/** @brief Retrieve the numerator of the frame rate of the video.
 *
 * @param p_vi The videoinfo.
 * @return The numerator of the frame rate of the video.
 *
 * Retrieve the numerator of the frame rate of the video from the
 * videoinfo @p p_vi. If @p p_vi is @c NULL, this function returns
 * @c 0.
 */
AVS_C_API int avs_videoinfo_fps_numerator_get (const AVS_VideoInfo *p_vi);


/** @brief Retrieve the denominator of the frame rate of the video.
 *
 * @param p_vi The videoinfo.
 * @return The denominator of the frame rate of the video.
 *
 * Retrieve the denominator of the frame rate of the video from the
 * videoinfo @p p_vi. If @p p_vi is @c NULL, this function returns
 * @c 1.
 */
AVS_C_API int avs_videoinfo_fps_denominator_get (const AVS_VideoInfo *p_vi);


/** @brief Retrieve the frame rate of the video as a float.
 *
 * @param p_vi The videoinfo.
 * @return The frame rate of the video.
 *
 * Retrieve the frame rate of the video from the videoinfo @p p_vi as a
 * float. If @p p_vi is @c NULL, this function returns @c 0.
 */
AVS_C_API float avs_videoinfo_fps_float_get (const AVS_VideoInfo *p_vi);



/** @brief Check wether the clip has an audio stream or not.
 *
 * @param p_vi The videoinfo to check.
 * @return @c 1 if the clip has a audio stream, @c 0 otherwise.
 *
 * Check wether the clip has an audio stream or not. If @p p_vi has
 * informations on audio, then the function returns 1, otherwise it
 * returns 0.
 */
AVS_C_API int avs_videoinfo_has_audio (const AVS_VideoInfo *p_vi);


/** @brief Retrieve the sample rate of the audio.
 *
 * @param p_vi The videoinfo.
 * @return The sample rate of the audio.
 *
 * Retrieve the sample rate of the audio from the videoinfo @p p_vi.
 * If @p p_vi is @c NULL, this function returns @c 0.
 */
AVS_C_API int avs_videoinfo_sample_rate_get (const AVS_VideoInfo *p_vi);


/** @brief Retrieve the sample count of the audio.
 *
 * @param p_vi The videoinfo.
 * @return The sample count of the audio.
 *
 * Retrieve the sample count of the audio from the videoinfo @p p_vi. If
 * @p p_vi is @c NULL, this function returns @c 0.
 */
AVS_C_API long long int avs_videoinfo_sample_count_get (const AVS_VideoInfo *p_vi);


/** @brief Retrieve the channel count of the audio.
 *
 * @param p_vi The videoinfo.
 * @return The channel count of the audio.
 *
 * Retrieve the channel count of the audio from the videoinfo @p p_vi. If
 * @p p_vi is @c NULL, this function returns @c 0.
 */
AVS_C_API int avs_videoinfo_channel_count_get (const AVS_VideoInfo *p_vi);


#endif /* __VIDEOINFO_C_H__ */
