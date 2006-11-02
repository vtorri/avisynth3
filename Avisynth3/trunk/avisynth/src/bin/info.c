#include <gtk/gtk.h>

#include <gst/gst.h>

#include "private.h"
#include "info.h"


static gchar *
_get_time_string (gdouble nbr_seconds)
{
  gchar str[128];
  gint milli;
  gint s;
  gint m;
  gint h;

  g_print ("temps : %f\n", nbr_seconds);

  s = (gint)nbr_seconds;
  milli = (gint)((nbr_seconds - (gdouble)s) * 1000.0);
  if (s < 60)
    {
      g_snprintf (str, 128, "%d.%d", s, milli);

      return g_strdup (str);
    }
  else
    {
      m = s / 60;
      s = s % 60;
      if (m < 60)
        {
          g_snprintf (str, 128, "%d:%d.%d", m, s, milli);

          return g_strdup (str);
        }
      else
        {
          h = m / 60;
          m = m % 60;
          g_snprintf (str, 128, "%d:%d:%d.%d", h, m, s, milli);

          return g_strdup (str);
        }
    }
}

void
avs3_file_info (Avs3_Data *data)
{
  GtkWidget *win_info;

  if (data->clip) {
    AVS_VideoInfo *info;
    GtkWidget     *frame;
    GtkWidget     *label;

    info = avs_clip_videoinfo_get (data->clip);

    win_info = gtk_dialog_new_with_buttons ("Info",
                                            NULL,
                                            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                            GTK_STOCK_CLOSE,
                                            GTK_RESPONSE_CLOSE,
                                            NULL);

    /* video */
    frame = gtk_frame_new ("Video stream");
    gtk_container_set_border_width (GTK_CONTAINER (frame), 6);
    gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (win_info)->vbox),
                                 frame);
    gtk_widget_show (frame);

    if (avs_videoinfo_has_video (info)) {
      gchar      str[4096];
      GtkWidget *table;
      gchar     *str_time;

      table = gtk_table_new (4, 2, FALSE);
      gtk_table_set_homogeneous (GTK_TABLE (table), FALSE);
      gtk_table_set_row_spacings (GTK_TABLE (table), 6);
      gtk_table_set_col_spacings (GTK_TABLE (table), 6);
      gtk_container_set_border_width (GTK_CONTAINER (table), 6);
      gtk_container_add (GTK_CONTAINER (frame), table);
      gtk_widget_show (table);

      label = gtk_label_new ("Size: ");
      gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
      gtk_table_attach_defaults (GTK_TABLE (table),
                                 label,
                                 0, 1,
                                 0, 1);
      gtk_widget_show (label);

      label = gtk_label_new ("Framerate: ");
      gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
      gtk_table_attach_defaults (GTK_TABLE (table),
                                 label,
                                 0, 1,
                                 1, 2);
      gtk_widget_show (label);

      label = gtk_label_new ("Frame count: ");
      gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
      gtk_table_attach_defaults (GTK_TABLE (table),
                                 label,
                                 0, 1,
                                 2, 3);
      gtk_widget_show (label);

      label = gtk_label_new ("Time: ");
      gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
      gtk_table_attach_defaults (GTK_TABLE (table),
                                 label,
                                 0, 1,
                                 3, 4);
      gtk_widget_show (label);

      g_snprintf (str, 4096, "%dx%d",
		  avs_videoinfo_width_get (info),
		  avs_videoinfo_height_get (info));
      label = gtk_label_new (str);
      gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
      gtk_table_attach_defaults (GTK_TABLE (table),
                                 label,
                                 1, 2,
                                 0, 1);
      gtk_widget_show (label);

      g_snprintf (str, 4096, "%d/%d (%.3f)",
		  avs_videoinfo_fps_numerator_get (info),
		  avs_videoinfo_fps_denominator_get (info),
		  avs_videoinfo_fps_float_get (info));
      label = gtk_label_new (str);
      gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
      gtk_table_attach_defaults (GTK_TABLE (table),
                                 label,
                                 1, 2,
                                 1, 2);
      gtk_widget_show (label);

      g_snprintf (str, 4096, "%d",
		  avs_videoinfo_framecount_get (info));
      label = gtk_label_new (str);
      gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
      gtk_table_attach_defaults (GTK_TABLE (table),
                                 label,
                                 1, 2,
                                 2, 3);
      gtk_widget_show (label);

      str_time = _get_time_string ((gdouble)(avs_videoinfo_framecount_get (info) * avs_videoinfo_fps_denominator_get (info)) / avs_videoinfo_fps_numerator_get (info));
      label = gtk_label_new (str_time);
      gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
      gtk_table_attach_defaults (GTK_TABLE (table),
                                 label,
                                 1, 2,
                                 3, 4);
      gtk_widget_show (label);
      g_free (str_time);
    }
    else {
      GtkWidget *misc;

      misc = gtk_event_box_new ();
      gtk_container_set_border_width (GTK_CONTAINER (misc), 6);
      gtk_container_add (GTK_CONTAINER (frame), misc);
      gtk_widget_show (misc);

      label = gtk_label_new ("No video stream");
      gtk_container_set_border_width (GTK_CONTAINER (label), 6);
      gtk_container_add (GTK_CONTAINER (misc), label);
      gtk_widget_show (label);
    }

    /* audio */
    frame = gtk_frame_new ("Audio stream");
    gtk_container_set_border_width (GTK_CONTAINER (frame), 6);
    gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (win_info)->vbox),
                                 frame);
    gtk_widget_show (frame);

    if (avs_videoinfo_has_audio (info)) {
      gchar      str[4096];
      GtkWidget *table;

      table = gtk_table_new (3, 2, FALSE);
      gtk_table_set_homogeneous (GTK_TABLE (table), FALSE);
      gtk_table_set_row_spacings (GTK_TABLE (table), 6);
      gtk_table_set_col_spacings (GTK_TABLE (table), 6);
      gtk_container_set_border_width (GTK_CONTAINER (table), 6);
      gtk_container_add (GTK_CONTAINER (frame), table);
      gtk_widget_show (table);

      label = gtk_label_new ("Channels: ");
      gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
      gtk_table_attach_defaults (GTK_TABLE (table),
                                 label,
                                 0, 1,
                                 0, 1);
      gtk_widget_show (label);

      label = gtk_label_new ("Samplerate: ");
      gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
      gtk_table_attach_defaults (GTK_TABLE (table),
                                 label,
                                 0, 1,
                                 1, 2);
      gtk_widget_show (label);

      label = gtk_label_new ("Sample count: ");
      gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
      gtk_table_attach_defaults (GTK_TABLE (table),
                                 label,
                                 0, 1,
                                 2, 3);
      gtk_widget_show (label);

      g_snprintf (str, 4096, "%d",
                avs_videoinfo_channel_count_get (info));
      label = gtk_label_new (str);
      gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
      gtk_table_attach_defaults (GTK_TABLE (table),
                                 label,
                                 1, 2,
                                 0, 1);
      gtk_widget_show (label);

      g_snprintf (str, 4096, "%d",
                avs_videoinfo_sample_rate_get (info));
      label = gtk_label_new (str);
      gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
      gtk_table_attach_defaults (GTK_TABLE (table),
                                 label,
                                 1, 2,
                                 1, 2);
      gtk_widget_show (label);

      g_snprintf (str, 4096, "%lld",
                avs_videoinfo_sample_count_get (info));
      label = gtk_label_new (str);
      gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
      gtk_table_attach_defaults (GTK_TABLE (table),
                                 label,
                                 1, 2,
                                 2, 3);
      gtk_widget_show (label);
    }
    else {
      GtkWidget *misc;

      misc = gtk_event_box_new ();
      gtk_container_set_border_width (GTK_CONTAINER (misc), 6);
      gtk_container_add (GTK_CONTAINER (frame), misc);
      gtk_widget_show (misc);

      label = gtk_label_new ("No audio stream");
      gtk_container_add (GTK_CONTAINER (misc), label);
      gtk_widget_show (label);
    }
  }
  else {
    win_info = gtk_message_dialog_new (GTK_WINDOW (data->window),
                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                       GTK_MESSAGE_ERROR,
                                       GTK_BUTTONS_CLOSE,
                                       "No clip avalaible");
  }
  gtk_dialog_run (GTK_DIALOG (win_info));
  gtk_widget_destroy (win_info);
}

void
avs3_plugins_info (Avs3_Data *data __UNUSED__)
{
  GtkWidget  *win_info;
  GtkWidget  *frame;
  GtkWidget  *hbox;
  GtkWidget  *vbox;
  GtkWidget  *check;
  GstElement *elt;

  win_info = gtk_dialog_new_with_buttons ("Info",
                                          NULL,
                                          GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                          GTK_STOCK_CLOSE,
                                          GTK_RESPONSE_CLOSE,
                                          NULL);

  hbox = gtk_hbox_new (TRUE, 6);
  gtk_widget_set_sensitive (hbox, TRUE);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 6);
  gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (win_info)->vbox),
                               hbox);
  gtk_widget_show (hbox);

  /* demuxers */
  frame = gtk_frame_new ("Demuxers");
  gtk_box_pack_start (GTK_BOX (hbox), frame, TRUE, TRUE, 6);
  gtk_widget_show (frame);

  vbox = gtk_vbox_new (TRUE, 6);
  gtk_widget_set_sensitive (vbox, FALSE);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 6);
  gtk_container_add (GTK_CONTAINER (frame), vbox);
  gtk_widget_show (vbox);

  check = gtk_check_button_new_with_label ("Asf");
  gtk_box_pack_start (GTK_BOX (vbox), check, TRUE, TRUE, 0);
  if ((elt = gst_element_factory_make ("asfdemux", NULL))) {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), TRUE);
    gst_object_unref (elt);
  }
  gtk_widget_show (check);

  check = gtk_check_button_new_with_label ("Avi");
  gtk_box_pack_start (GTK_BOX (vbox), check, TRUE, TRUE, 0);
  if ((elt = gst_element_factory_make ("avidemux", NULL))) {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), TRUE);
    gst_object_unref (elt);
  }
  gtk_widget_show (check);

  check = gtk_check_button_new_with_label ("DV");
  gtk_box_pack_start (GTK_BOX (vbox), check, TRUE, TRUE, 0);
  if ((elt = gst_element_factory_make ("dvdemux", NULL))) {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), TRUE);
    gst_object_unref (elt);
  }
  gtk_widget_show (check);

  check = gtk_check_button_new_with_label ("DVD");
  gtk_box_pack_start (GTK_BOX (vbox), check, TRUE, TRUE, 0);
  if ((elt = gst_element_factory_make ("dvddemux", NULL))) {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), TRUE);
    gst_object_unref (elt);
  }
  gtk_widget_show (check);

  check = gtk_check_button_new_with_label ("Matroska");
  gtk_box_pack_start (GTK_BOX (vbox), check, TRUE, TRUE, 0);
  if ((elt = gst_element_factory_make ("matroskademux", NULL))) {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), TRUE);
    gst_object_unref (elt);
  }
  gtk_widget_show (check);

  check = gtk_check_button_new_with_label ("Mp4 / QuickTime");
  gtk_box_pack_start (GTK_BOX (vbox), check, TRUE, TRUE, 0);
  if ((elt = gst_element_factory_make ("qtdemux", NULL))) {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), TRUE);
    gst_object_unref (elt);
  }
  gtk_widget_show (check);

  check = gtk_check_button_new_with_label ("Mpeg Stream");
  gtk_box_pack_start (GTK_BOX (vbox), check, TRUE, TRUE, 0);
  if ((elt = gst_element_factory_make ("mpegdemux", NULL))) {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), TRUE);
    gst_object_unref (elt);
  }
  gtk_widget_show (check);

  check = gtk_check_button_new_with_label ("Ogg");
  gtk_box_pack_start (GTK_BOX (vbox), check, TRUE, TRUE, 0);
  if ((elt = gst_element_factory_make ("oggdemux", NULL))) {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), TRUE);
    gst_object_unref (elt);
  }
  gtk_widget_show (check);

  check = gtk_check_button_new_with_label ("Real Media");
  gtk_box_pack_start (GTK_BOX (vbox), check, TRUE, TRUE, 0);
  if ((elt = gst_element_factory_make ("rmdemux", NULL))) {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), TRUE);
    gst_object_unref (elt);
  }
  gtk_widget_show (check);

  /* decoders */
  frame = gtk_frame_new ("Decoders");
  gtk_box_pack_start (GTK_BOX (hbox), frame, TRUE, TRUE, 6);
  gtk_widget_show (frame);

  vbox = gtk_vbox_new (TRUE, 6);
  gtk_widget_set_sensitive (vbox, FALSE);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 6);
  gtk_container_add (GTK_CONTAINER (frame), vbox);
  gtk_widget_show (vbox);

  check = gtk_check_button_new_with_label ("Divx");
  gtk_box_pack_start (GTK_BOX (vbox), check, TRUE, TRUE, 0);
  if ((elt = gst_element_factory_make ("divxdemux", NULL))) {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), TRUE);
    gst_object_unref (elt);
  }
  gtk_widget_show (check);

  check = gtk_check_button_new_with_label ("Dv");
  gtk_box_pack_start (GTK_BOX (vbox), check, TRUE, TRUE, 0);
  if ((elt = gst_element_factory_make ("dvdec", NULL))) {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), TRUE);
    gst_object_unref (elt);
  }
  gtk_widget_show (check);

  check = gtk_check_button_new_with_label ("Theora");
  gtk_box_pack_start (GTK_BOX (vbox), check, TRUE, TRUE, 0);
  if ((elt = gst_element_factory_make ("theoradec", NULL))) {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), TRUE);
    gst_object_unref (elt);
  }
  gtk_widget_show (check);

  check = gtk_check_button_new_with_label ("Xvid");
  gtk_box_pack_start (GTK_BOX (vbox), check, TRUE, TRUE, 0);
  if ((elt = gst_element_factory_make ("xviddec", NULL))) {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), TRUE);
    gst_object_unref (elt);
  }
  gtk_widget_show (check);

  check = gtk_check_button_new_with_label ("H264");
  gtk_box_pack_start (GTK_BOX (vbox), check, TRUE, TRUE, 0);
  if ((elt = gst_element_factory_make ("ffdec_h264", NULL))) {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), TRUE);
    gst_object_unref (elt);
  }
  gtk_widget_show (check);

  gtk_widget_show (win_info);

  gtk_dialog_run (GTK_DIALOG (win_info));
  gtk_widget_destroy (win_info);
}
