#include <gtk/gtk.h>

#include "private.h"
#include "preview.h"
#include "open.h"


static gpointer
_open_script (Avs3_Data *data)
{
  AVS_VideoInfo *info;
/*   gpointer ret; */

  if (!data->env)
    data->env = avs_environment_new (10000000);
  data->clip = avs_clip_new_from_file (data->filename, data->env);

  info = avs_clip_videoinfo_get (data->clip);
  if (avs_videoinfo_has_video (info)) {
    data->frames_count = avs_videoinfo_framecount_get (info);
    g_print ("\n\nframe count !! : %d\n", data->frames_count);
    gtk_range_set_range (GTK_RANGE (data->scale),
                         0.0, (gdouble)(data->frames_count - 1));
    avs3_preview_draw (data, data->frame_current);
  }
  avs_videoinfo_delete (info);

  gtk_statusbar_pop (GTK_STATUSBAR (data->status), data->id);
  gtk_statusbar_push (GTK_STATUSBAR (data->status), data->id, "File loaded...");
  g_print ("FRAME : %d\n", data->frame_current);

/*   ret = g_thread_join (data->thread); */
/*   g_thread_exit (ret); */

  return NULL;
}

void
avs3_open (Avs3_Data *data)
{
  GtkWidget *win_open;
  gint       res;

  win_open = gtk_file_chooser_dialog_new ("Open File",
                                          NULL,
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);
  g_signal_connect (G_OBJECT (win_open), "delete_event",
		    G_CALLBACK (gtk_widget_destroy), NULL);

  res = gtk_dialog_run (GTK_DIALOG (win_open));
  switch (res) {
  case GTK_RESPONSE_ACCEPT: {
/*     guint    id; */
    
    data->filename = g_strdup (gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (win_open)));
    g_print ("accept %s\n", data->filename);
    
    data->id = gtk_statusbar_get_context_id (GTK_STATUSBAR (data->status),
                                       "open");
    gtk_statusbar_push (GTK_STATUSBAR (data->status), data->id, "Opening file...");

    _open_script (data);
/*     data->thread = g_thread_create ((GThreadFunc)_open_script, data, FALSE, NULL); */
    break;
  }
  case GTK_RESPONSE_CANCEL:
  default:
    break;
  }

  gtk_widget_destroy (win_open);
}
