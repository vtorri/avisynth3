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


/* C API include */
#include "define_c.h"
#include "private/videoinfo_c_private.h"
#include "private/colorspace_c_private.h"


/* Destructor */

EXTERN_C void
avs_videoinfo_delete (AVS_VideoInfo *p_vi)
{
  if (!p_vi) return;

  delete p_vi;
}

EXTERN_C AVS_ColorSpace *
avs_videoinfo_colorspace_get (const AVS_VideoInfo *p_vi)
{
  AVS_ColorSpace *p_cs;

  if (!p_vi)
    return NULL;
  
  p_cs = new AVS_ColorSpace;
  if (!p_cs)
    return NULL;

  p_cs->p_cs_ = p_vi->p_vi_->GetColorSpace ();

  return p_cs;
}

// Audio methods

EXTERN_C int
avs_videoinfo_has_video (const AVS_VideoInfo *p_vi)
{
  if (p_vi && p_vi->p_vi_->HasVideo ())
    return 1;
  
  return 0;
}

EXTERN_C int
avs_videoinfo_width_get (const AVS_VideoInfo *p_vi)
{
  if (!p_vi)
    return 0;
  
  return p_vi->p_vi_->GetWidth ();
}

EXTERN_C int
avs_videoinfo_height_get (const AVS_VideoInfo *p_vi)
{
  if (!p_vi)
    return 0;
  
  return p_vi->p_vi_->GetHeight ();
}

EXTERN_C int
avs_videoinfo_framecount_get (const AVS_VideoInfo *p_vi)
{
  if (!p_vi)
    return 0;
  
  return p_vi->p_vi_->GetFrameCount ();
}

EXTERN_C int
avs_videoinfo_fps_numerator_get (const AVS_VideoInfo *p_vi)
{
  if (!p_vi)
    return 0;
  
  return p_vi->p_vi_->GetFPSNumerator ();
}

EXTERN_C int
avs_videoinfo_fps_denominator_get (const AVS_VideoInfo *p_vi)
{
  if (!p_vi)
    return 1;
  
  return p_vi->p_vi_->GetFPSDenominator ();
}

EXTERN_C float
avs_videoinfo_fps_float_get (const AVS_VideoInfo *p_vi)
{
  if (!p_vi)
    return 0.0;
  
  return p_vi->p_vi_->GetFloatFPS ();
}

// Audio methods

EXTERN_C int
avs_videoinfo_has_audio (const AVS_VideoInfo *p_vi)
{
  if (p_vi && p_vi->p_vi_->HasAudio ())
    return 1;
  
  return 0;
}

EXTERN_C int
avs_videoinfo_sample_rate_get (const AVS_VideoInfo *p_vi)
{
  if (!p_vi)
    return 0;
  
  return p_vi->p_vi_->GetSampleRate ();
}

EXTERN_C long long int
avs_videoinfo_sample_count_get (const AVS_VideoInfo *p_vi)
{
  if (!p_vi)
    return 0;
  
  return p_vi->p_vi_->GetSampleCount ();
}

EXTERN_C int
avs_videoinfo_channel_count_get (const AVS_VideoInfo *p_vi)
{
  if (!p_vi)
    return 0;
  
  return p_vi->p_vi_->GetChannelCount ();
}
