#include <gtk/gtk.h>

#include "private.h"

void
avs3_preview_draw (Avs3_Data *data, gint frame_nbr)
{
  GdkPixbuf           *pixbuf;
  AVS_Clip            *clip_rgb32;
  AVS_VideoFrame      *frame;
  guint                width;
  guint                height;
  const unsigned char *rgb;

  clip_rgb32 = avs_clip_new_to_rgb32 (data->clip);
  frame = avs_clip_videoframe_get (clip_rgb32, frame_nbr);
  width = avs_videoframe_width_get (frame);
  height = avs_videoframe_height_get (frame);
  rgb = avs_videoframe_plane_get (frame, '~');
  g_print ("size (preview) : %dx%d  %d\n", width, height, sizeof (*rgb));

  gtk_widget_set_size_request (data->preview, width, height);
  g_print ("apres resize...\n");
  pixbuf = gdk_pixbuf_new_from_data (rgb,
				     GDK_COLORSPACE_RGB, TRUE,
				     8, width, height, 4 * width,
				     (GdkPixbufDestroyNotify) g_free,
				     NULL);
  gtk_image_set_from_pixbuf (GTK_IMAGE (data->preview), pixbuf);

  g_print ("apres affichage...\n");
  avs_videoframe_delete (frame);
  avs_clip_delete (clip_rgb32);
  g_print ("apres liberation...\n");
}
