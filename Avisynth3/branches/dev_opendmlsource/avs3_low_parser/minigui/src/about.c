#include <gtk/gtk.h>

void
avs3_about (void)
{
  GtkWidget *win_about;
  const gchar *authors[3] = {
    "David Pierre",
    "Vincent Torri",
    NULL
  };

  win_about = gtk_about_dialog_new ();
  gtk_show_about_dialog (NULL,
                         "authors", authors,
                         "copyright", "Copyright Vincent Torri 2006",
                         "license", "GPL",
                         "name", "Avisynth 3.0\nTest Application",
                         "version", "0.1",
                         "website", "http://avisynth3.unite-video.com",
                         NULL);
}
