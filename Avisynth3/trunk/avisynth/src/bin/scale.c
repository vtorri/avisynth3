#include <gtk/gtk.h>

#include "private.h"
#include "preview.h"


void
avs3_scale (Avs3_Data *data, GtkWidget *parent, gint frame)
{
  if (!data->clip) {
    _dialog_no_clip (parent);
    return;
  }

  data->frame_current = frame;
  avs3_preview_draw (data, data->frame_current);
}
