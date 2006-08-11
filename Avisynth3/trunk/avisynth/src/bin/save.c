#include <stdint.h>
#include <unistd.h>

#include <gtk/gtk.h>

#include <x264.h>
#include <x264_gtk.h>

#include "private.h"
#include "private_x264.h"
#include "status_window.h"
#include "encode.h"
#include "save.h"


static gboolean
_fill_status_window (GIOChannel  *io __UNUSED__,
                     GIOCondition condition __UNUSED__,
                     gpointer     user_data)
{
  gchar             str[128];
  Avs3_Thread_Data *thread_data;
  Avs3_Pipe_Data    pipe_data;
  GIOStatus         status;
  gsize             size;
  gint              eta;
  gdouble           progress;
  gdouble           fps;

  thread_data = (Avs3_Thread_Data *)user_data;
  status = g_io_channel_read_chars (thread_data->io_read,
                                    (gchar *)&pipe_data,
                                    sizeof (Avs3_Pipe_Data),
                                    &size, NULL);
  if (status != G_IO_STATUS_NORMAL) {
    g_print ("Error ! %d %d %d\n", status, sizeof (Avs3_Pipe_Data), size);
    return FALSE;
  }

  g_snprintf (str, 128, "%d/%d", pipe_data.frame, pipe_data.frame_total);
  gtk_entry_set_text (GTK_ENTRY (thread_data->current_video_frame),
                      str);

  g_snprintf (str, 128, "%dKB", pipe_data.file >> 10);
  gtk_entry_set_text (GTK_ENTRY (thread_data->video_data),
                      str);

  fps = pipe_data.elapsed > 0 ? 1000000.0 * (gdouble)pipe_data.frame / (gdouble)pipe_data.elapsed : 0.0;
  g_snprintf (str, 128, "%.2fKB/s (%.2f fps)",
	      (double) pipe_data.file * 8 * thread_data->param->i_fps_num /
	      ((double) thread_data->param->i_fps_den * pipe_data.frame * 1000),
	      fps);
  gtk_entry_set_text (GTK_ENTRY (thread_data->video_rendering_rate),
                      str);

  g_snprintf (str, 128, "%lld:%02lld:%02lld",
	      (pipe_data.elapsed / 1000000) / 3600,
	      ((pipe_data.elapsed / 1000000) / 60) % 60,
	      (pipe_data.elapsed / 1000000) % 60);
  gtk_entry_set_text (GTK_ENTRY (thread_data->time_elapsed),
                      str);

  eta = pipe_data.elapsed * (pipe_data.frame_total - pipe_data.frame) / ((int64_t)pipe_data.frame * 1000000);
  g_snprintf (str, 128, "%d:%02d:%02d", eta / 3600, (eta / 60) % 60, eta % 60);
  gtk_entry_set_text (GTK_ENTRY (thread_data->time_remaining),
                      str);

  progress = (gdouble)pipe_data.frame / (gdouble)pipe_data.frame_total;
  gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (thread_data->progress),
                                 progress);

  g_snprintf (str, 128, "%0.1f%%", 100.0 * progress);
  gtk_progress_bar_set_text (GTK_PROGRESS_BAR (thread_data->progress), str);

  return TRUE;
}

static void
_save_file (Avs3_Encode_Data *encode_data, GtkWidget *dialog)
{
  gint              fds[2];
  x264_param_t     *param;
  X264_Gtk         *x264_gtk;
  Avs3_Thread_Data *thread_data;
  AVS_VideoInfo    *info;
  GtkWidget        *win_status;
  GThread          *thread;
  const gchar      *path_output = NULL;
  const gchar      *filename_output = NULL;
  gchar            *file_output = NULL;
  gchar            *ext;
  gint              container;

  /* input */
  info = avs_clip_videoinfo_get (encode_data->data->clip);
  if (!avs_videoinfo_has_video (info))
    return;

  /* output */
  path_output = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (encode_data->path_output));
  filename_output = gtk_entry_get_text (GTK_ENTRY (encode_data->file_output));
  if (!filename_output ||
      (filename_output[0] == '\0')) {
    _dialog_error (dialog, "Output file name is not set");
    goto free_vi;
  }
  container = gtk_combo_box_get_active (GTK_COMBO_BOX (encode_data->combo));
  switch (container) {
  case 1:
    ext = ".mkv";
    break;
#ifdef MP4_OUTPUT
  case 2:
    ext = ".mp4";
    break;
#endif
  case 0:
  default:
    ext = ".264";
  }
  file_output = g_strconcat (path_output, "/", filename_output, ext, NULL);

  {
    GIOChannel *file;

    file = g_io_channel_new_file (file_output, "r", NULL);
    if (file) {
      GtkWidget   *dialog_overwrite;
      GtkWidget   *eb;
      GtkWidget   *label;
      const gchar *label_text = NULL;
      gint         res;

      dialog_overwrite = gtk_dialog_new_with_buttons ("Existing file",
                                                      GTK_WINDOW (dialog),
                                                      GTK_DIALOG_DESTROY_WITH_PARENT,
                                                      GTK_STOCK_YES,
                                                      GTK_RESPONSE_ACCEPT,
                                                      GTK_STOCK_NO,
                                                      GTK_RESPONSE_REJECT,
                                                      NULL);

      eb = gtk_event_box_new ();
      gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog_overwrite)->vbox), eb);
      gtk_container_set_border_width (GTK_CONTAINER (eb), 6);
      gtk_widget_show (eb);

      label_text = g_strconcat ("Do you want to overwrite file\n",
                                file_output, " ?", NULL);
      label = gtk_label_new (label_text);
      gtk_container_add (GTK_CONTAINER (eb), label);
      gtk_widget_show (label);

      res = gtk_dialog_run (GTK_DIALOG (dialog_overwrite));
      gtk_widget_destroy (dialog_overwrite);
      g_io_channel_unref (file);
      if ((res == GTK_RESPONSE_REJECT) || (res == GTK_RESPONSE_DELETE_EVENT))
        goto free_file_output;
    }
  }

  x264_gtk = x264_gtk_load ();
  param = x264_gtk_param_get (x264_gtk);
  x264_gtk_free (x264_gtk);

  if (pipe (fds) == -1)
    goto free_param;

  param->i_width = avs_videoinfo_width_get (info);
  param->i_height = avs_videoinfo_height_get (info);
  param->i_fps_num = avs_videoinfo_fps_numerator_get (info);
  param->i_fps_den = avs_videoinfo_fps_numerator_get (info);
  param->i_frame_total = avs_videoinfo_framecount_get (info);
  avs_videoinfo_delete (info);

  thread_data = (Avs3_Thread_Data *)g_malloc0 (sizeof (Avs3_Thread_Data));
  thread_data->data = encode_data->data;
  thread_data->param = param;
  thread_data->file_output = g_strdup (file_output);
  thread_data->container = container;
  g_free (file_output);

  thread_data->io_read = g_io_channel_unix_new (fds[0]);
  g_io_channel_set_encoding (thread_data->io_read, NULL, NULL);
  thread_data->io_write = g_io_channel_unix_new (fds[1]);
  g_io_channel_set_encoding (thread_data->io_write, NULL, NULL);

  g_io_add_watch (thread_data->io_read, G_IO_IN,
                  (GIOFunc)_fill_status_window, thread_data);

  win_status = avs3_status_window (thread_data);
  gtk_window_set_transient_for (GTK_WINDOW (win_status), GTK_WINDOW (dialog));
  gtk_window_set_modal (GTK_WINDOW (win_status), TRUE);
  gtk_widget_show (win_status);

  thread = g_thread_create ((GThreadFunc)avs3_encode,
                            thread_data, FALSE, NULL);
  return;

 free_param:
  g_free (param);
 free_file_output:
  g_free (file_output);
 free_vi:
  avs_videoinfo_delete (info);
}

void
avs3_save (Avs3_Data *data)
{
  GtkWidget        *win_save;
  Avs3_Encode_Data *encode_data = NULL;
  gint              res;
  GtkWidget        *table;
  GtkWidget        *label;

  if (!data->clip) {
    _dialog_error (data->window, "No clip avalaible");
    return;
  }


  encode_data = (Avs3_Encode_Data *)g_malloc0 (sizeof (Avs3_Encode_Data));
  if (!encode_data)
    return;

  encode_data->data = data;

  win_save = gtk_dialog_new_with_buttons ("Save File",
                                          GTK_WINDOW (data->window),
                                          GTK_DIALOG_DESTROY_WITH_PARENT,
                                          GTK_STOCK_CANCEL,
                                          GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_EXECUTE,
                                          GTK_RESPONSE_APPLY,
                                          NULL);

  table = gtk_table_new (3, 2, TRUE);
  gtk_table_set_row_spacings (GTK_TABLE (table), 6);
  gtk_table_set_col_spacings (GTK_TABLE (table), 6);
  gtk_container_set_border_width (GTK_CONTAINER (table), 6);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (win_save)->vbox), table,
                      TRUE, TRUE, 0);
  gtk_widget_show (table);

  label = gtk_label_new ("Output path:");
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
  gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 0, 1);
  gtk_widget_show (label);

  encode_data->path_output = gtk_file_chooser_button_new ("Select a path",
                                                          GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (encode_data->path_output),
                                       g_get_home_dir ());
  gtk_table_attach_defaults (GTK_TABLE (table), encode_data->path_output, 1, 2, 0, 1);
  gtk_widget_show (encode_data->path_output);

  label = gtk_label_new ("Output file (without ext.):");
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
  gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 1, 2);
  gtk_widget_show (label);

  encode_data->file_output = gtk_entry_new_with_max_length (4095);
  gtk_table_attach_defaults (GTK_TABLE (table), encode_data->file_output, 1, 2, 1, 2);
  gtk_widget_show (encode_data->file_output);

  label = gtk_label_new ("Container:");
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
  gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 2, 3);
  gtk_widget_show (label);

  encode_data->combo = gtk_combo_box_new_text ();
  gtk_combo_box_append_text (GTK_COMBO_BOX (encode_data->combo),
                             "Raw ES");
  gtk_combo_box_append_text (GTK_COMBO_BOX (encode_data->combo),
                             "Matroska");
#ifdef MP4_OUTPUT
  gtk_combo_box_append_text (GTK_COMBO_BOX (encode_data->combo),
                             "Mp4");
#endif
  gtk_table_attach_defaults (GTK_TABLE (table), encode_data->combo, 1, 2, 2, 3);
  gtk_widget_show (encode_data->combo);

  gtk_combo_box_set_active (GTK_COMBO_BOX (encode_data->combo), 0);

  res = gtk_dialog_run (GTK_DIALOG (win_save));
  switch (res) {
  case GTK_RESPONSE_APPLY:
    _save_file (encode_data, win_save);
    break;
  default:
    break;
  }
  gtk_widget_destroy (win_save);
}
