/* Avisynth 3.0 C Interface
 * Copyright 2005-2006 Vincent Torri <vtorri at univ-evry dot fr>
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

#include <gtk/gtk.h>

#include "private.h"
#include "preview.h"


void
avs3_preview_draw (Avs3_Data *data, gint frame_nbr)
{
  GdkPixbuf           *pixbuf;
  AVS_Clip            *clip_rgb32;
  AVS_VideoInfo       *info;
  AVS_ColorSpace      *csp;
  AVS_VideoFrame      *frame;
  AVS_ColorSpace_Id    csp_id;
  guint                width;
  guint                height;
  const unsigned char *rgb;

  guchar *buffer;
  guchar *position;
  AVS_SampleType type;
  gint nbytes;

  info = avs_clip_videoinfo_get (data->clip);
  csp = avs_videoinfo_colorspace_get (info);
  csp_id = avs_colorspace_id_get (csp);

  type = avs_videoinfo_sample_type_get (info);
  switch (type) {
  case SAMPLE_INT8:
    nbytes = 1;
    break;
  case SAMPLE_INT16:
    nbytes = 2;
    break;
  case SAMPLE_INT24:
    nbytes = 3;
    break;
  case SAMPLE_INT32:
    nbytes = 4;
    break;
  case SAMPLE_FLOAT:
    nbytes = sizeof (float);
    break;
  }

  avs_colorspace_delete (csp);
  avs_videoinfo_delete (info);

  if (csp_id == I_RGB32)
    clip_rgb32 = data->clip;
  else
    clip_rgb32 = avs_clip_new_to_rgb32 (data->clip);

  frame = avs_clip_videoframe_get (clip_rgb32, frame_nbr);

  {

    g_print (" ** audio type   : %d\n", type);
    g_print (" ** audio buffer : %d\n", 150 * nbytes);
    buffer = (guchar *)g_malloc (sizeof (guchar) * 150 * nbytes);
/*     position = avs_clip_audio_get(clip_rgb32, */
/*                                   buffer, */
/*                                   (frame_nbr * avs_videoinfo_sample_rate_get (info) * avs_videoinfo_fps_denominator_get (info)) / avs_videoinfo_fps_numerator_get (info), */
/*                                   150); */
    position = avs_clip_audio_get(clip_rgb32,
                                  buffer,
                                  0,
                                  150);
    g_free (buffer);
  }

  width = avs_videoframe_width_get (frame);
  height = avs_videoframe_height_get (frame);
  rgb = avs_videoframe_plane_get (frame, '~');
  g_print ("size (preview) : %dx%d  %d\n", width, height, sizeof (*rgb));

  gtk_widget_set_size_request (data->preview, (gint)width, (gint)height);
  pixbuf = gdk_pixbuf_new_from_data (rgb,
				     GDK_COLORSPACE_RGB, TRUE,
				     8, (gint)width, (gint)height, 4 * (gint)width,
				     (GdkPixbufDestroyNotify) g_free,
				     NULL);
  gtk_image_set_from_pixbuf (GTK_IMAGE (data->preview), pixbuf);

  avs_videoframe_delete (frame);
  if (csp_id != I_RGB32)
    avs_clip_delete (clip_rgb32);
}
