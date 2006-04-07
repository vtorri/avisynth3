#include <gtk/gtk.h>

#include "private.h"
#include "preview.h"


static void
_dialog_no_clip (GtkWidget *parent)
{
  GtkWidget *dialog;

  dialog = gtk_message_dialog_new (GTK_WINDOW (parent),
                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                   GTK_MESSAGE_ERROR,
                                   GTK_BUTTONS_CLOSE,
                                   "No video clip avalaible");
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

void
avs3_edit_begin (Avs3_Data *data, GtkWidget *parent)
{
  if (!data->clip) {
    _dialog_no_clip (parent);
    return;
  }

  data->frame_current = 0;
  avs3_preview_draw (data, data->frame_current);
}

void
avs3_edit_end (Avs3_Data *data, GtkWidget *parent)
{
  AVS_VideoInfo *info;

  if (!data->clip) {
    _dialog_no_clip (parent);
    return;
  }

  info = avs_clip_videoinfo_get (data->clip);
  data->frame_current = avs_videoinfo_framecount_get (info) - 1;
  avs_videoinfo_delete (info);
  avs3_preview_draw (data, data->frame_current);
}

void
avs3_edit_goto (Avs3_Data *data, GtkWidget *parent)
{
  GtkWidget *win_goto;
  GtkWidget *hbox;
  GtkWidget *label;
  GtkWidget *entry;
  gint       res;

  if (!data->clip) {
    _dialog_no_clip (parent);
    return;
  }

  win_goto = gtk_dialog_new_with_buttons ("Go to frame...",
                                          GTK_WINDOW (parent),
                                          GTK_DIALOG_DESTROY_WITH_PARENT,
                                          GTK_STOCK_CANCEL,
                                          GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OK,
                                          GTK_RESPONSE_OK,
                                          NULL);

  hbox = gtk_hbox_new (FALSE, 6);
  gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (win_goto)->vbox),
                               hbox);
  gtk_widget_show (hbox);
  
  label = gtk_label_new ("Frame number:");
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);
  
  entry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (hbox), entry, FALSE, FALSE, 0);
  gtk_widget_show (entry);

  res = gtk_dialog_run (GTK_DIALOG (win_goto));

  switch (res) {
  case GTK_RESPONSE_OK: {
    const gchar *str;

    str = gtk_entry_get_text (GTK_ENTRY (entry));
    data->frame_current = (gint)g_ascii_strtod (str, NULL);
    avs3_preview_draw (data, data->frame_current);
    break;
  }
  case GTK_RESPONSE_CANCEL:
  default:
    break;
  }

  gtk_widget_destroy (win_goto);
}
