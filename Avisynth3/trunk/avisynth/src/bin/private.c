#include <gtk/gtk.h>

#include "private.h"


void
_data_delete (Avs3_Data *data)
{
  if (data->filename)
    g_free (data->filename);
  if (data->clip)
    avs_clip_delete (data->clip);
  if (data->env)
    avs_environment_delete (data->env);

  g_free (data);
}

void
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
