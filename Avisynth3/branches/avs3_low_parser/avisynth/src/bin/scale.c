#include <gtk/gtk.h>

#include "private.h"
#include "preview.h"
#include "scale.h"


void
avs3_scale (Avs3_Data *data, GtkWidget *parent, gint frame)
{
  AVS_VideoInfo *info;

  if (!data->clip) {
    _dialog_error (parent, "No video clip avalaible");
    return;
  }

  info = avs_clip_videoinfo_get (data->clip);
  if (!avs_videoinfo_has_video (info)) {
    _dialog_error (parent, "No video in clip");
    goto free_videoinfo;
  }

  data->frame_current = frame;
  avs3_preview_draw (data, data->frame_current);

 free_videoinfo:
  avs_videoinfo_delete (info);
}
