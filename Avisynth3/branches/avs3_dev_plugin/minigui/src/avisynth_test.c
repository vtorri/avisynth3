#include <gtk/gtk.h>

#include "main_window.h"


int
main (int argc, char *argv[])
{
  if (!g_thread_supported ())
    g_thread_init (NULL);
  gtk_init (&argc, &argv);

  avs3_main_window ();

  gtk_main ();

  return 0;  
}
