#include <gtk/gtk.h>

#include "private.h"


void
avs3_save (Avs3_Data *data)
{
  GtkWidget *win_save;
  GtkWidget *table;
  GtkWidget *label;
  GtkWidget *fc_path;
  GtkWidget *fc_file;
  GtkWidget *combo;
  gint       res;

  win_save = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  table = gtk_table_new (3, 2, TRUE);
  gtk_table_set_row_spacings (GTK_TABLE (table), 6);
  gtk_table_set_col_spacings (GTK_TABLE (table), 6);
  gtk_container_set_border_width (GTK_CONTAINER (table), 6);
  gtk_container_add (GTK_CONTAINER (win_save), table);
  gtk_widget_show (table);

  label = gtk_label_new ("Output path:");
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
  gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 0, 1);
  gtk_widget_show (label);

  fc_path = gtk_file_chooser_button_new ("Select a path",
                                         GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (fc_path),
                                       g_get_home_dir ());
  gtk_table_attach_defaults (GTK_TABLE (table), fc_path, 1, 2, 0, 1);
  gtk_widget_show (fc_path);

  label = gtk_label_new ("Output file (without ext.):");
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
  gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 1, 2);
  gtk_widget_show (label);

  fc_file = gtk_entry_new_with_max_length (4095);
  gtk_table_attach_defaults (GTK_TABLE (table), fc_file, 1, 2, 1, 2);
  gtk_widget_show (fc_file);

  label = gtk_label_new ("Container:");
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
  gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 2, 3);
  gtk_widget_show (label);

  combo = gtk_combo_box_new_text ();
  gtk_combo_box_append_text (GTK_COMBO_BOX (combo),
                             "Raw ES");
  gtk_combo_box_append_text (GTK_COMBO_BOX (combo),
                             "Matroska");
#ifdef MP4_OUTPUT
  gtk_combo_box_append_text (GTK_COMBO_BOX (combo),
                             "Mp4");
#endif
  gtk_table_attach_defaults (GTK_TABLE (table), combo, 1, 2, 2, 3);
  gtk_widget_show (combo);

  gtk_combo_box_set_active (GTK_COMBO_BOX (combo), 0);

  gtk_widget_show (win_save);

/*   win_save = gtk_file_chooser_dialog_new ("Save File", */
/*                                           NULL, */
/*                                           GTK_FILE_CHOOSER_ACTION_OPEN, */
/*                                           GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, */
/*                                           GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, */
/*                                           NULL); */
/*   g_signal_connect (G_OBJECT (win_save), "delete_event", */
/* 		    G_CALLBACK (gtk_widget_destroy), NULL); */

/*   res = gtk_dialog_run (GTK_DIALOG (win_save)); */

/*   switch (res) { */
/*   case GTK_RESPONSE_ACCEPT: { */
    
/*     data->filename = g_strdup (gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (win_save))); */
/*     g_print ("accept %s\n", data->filename); */
    
/*     data->id = gtk_statusbar_get_context_id (GTK_STATUSBAR (data->status), */
/*                                        "save"); */
/*     gtk_statusbar_push (GTK_STATUSBAR (data->status), data->id, "Saving file..."); */

/*     _save_script (data); */
/*     break; */
/*   } */
/*   case GTK_RESPONSE_CANCEL: */
/*   default: */
/*     break; */
/*   } */

/*   gtk_widget_destroy (win_save); */
}
