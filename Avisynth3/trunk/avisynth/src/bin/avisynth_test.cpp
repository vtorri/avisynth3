// g++ -g -Wall -W `pkg-config --cflags --libs gtk+-2.0 gstreamer-0.8` -I$HOME/local/include/stlport -I$HOME/local/include/boost-1_33 -L../build/linux/build -lavisynth -o test_gtk test_gtk.cpp

//gtk+ include
#include <gtk/gtk.h>

//gstreamer include
#include <gst/gst.h>

//stlport include
#include <string>

//avisynth include
#include "../src/core.h"
#include "../src/core/colorspace/get.h"
#include "../src/filters/convert/torgb32/fromyv12.h"
#include "../src/filters/source/gstreamersource.h"

#define __UNUSED__ __attribute__((unused))

//namespaces
using namespace avs;
using namespace avs::colorspace;
using namespace avs::filters;
using namespace avs::filters::convert;
using namespace avs::filters::convert::torgb32;
using namespace avs::filters::source::gstreamer;

//typedef
typedef boost::shared_ptr<GstreamerSource> PGstreamerSource;

//Global variables
PClip      vclip;
GtkWidget *image;
GtkWidget *status;
GtkWidget *entry;


////////
// GUI
////////

//Callbacks
static void cb_delete (GtkWidget *widget,
		       GdkEvent  *event,
		       gpointer   data);

static void cb_open   (GtkWidget *widget,
		       GdkEvent  *event,
		       gpointer   data);

static void cb_info   (GtkWidget *widget,
		       GdkEvent  *event,
		       gpointer   data);

static void cb_begin  (GtkWidget *widget,
		       GdkEvent  *event,
		       gpointer   data);

static void cb_end    (GtkWidget *widget,
		       GdkEvent  *event,
		       gpointer   data);

static void cb_goto   (GtkWidget *widget,
		       GdkEvent  *event,
		       gpointer   data);

static void cb_about  (GtkWidget *widget,
		       GdkEvent  *event,
		       gpointer   data);

static void cb_fd_response (GtkWidget *widget,
			    gint       arg1,
			    gpointer   data);

static void cb_goto_draw (GtkWidget *widget,
			  GdkEvent  *event,
			  gpointer   data);


//Interface
static void draw_frame (long int frame);

static void create_main_window ();


////////
// Main function
////////
int
main (int argc, char *argv[])
{
  gtk_init (&argc, &argv);
  
  create_main_window ();

  gtk_main ();

  return 1;
}



//Callbacks
static void
cb_delete (GtkWidget *widget __UNUSED__,
	   GdkEvent  *event __UNUSED__,
	   gpointer   data __UNUSED__)
{
  gtk_main_quit ();
}

static void
cb_open (GtkWidget *widget __UNUSED__,
	 GdkEvent  *event __UNUSED__,
	 gpointer   data __UNUSED__)
{
  static GtkWidget *fd = NULL;

  if (fd)
    {
      gdk_window_raise (fd->window);
      return;
    }

  fd = gtk_file_chooser_dialog_new ("Open File",
				    NULL,
				    GTK_FILE_CHOOSER_ACTION_OPEN,
				    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				    GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
				    NULL);
  g_signal_connect (G_OBJECT (fd), "destroy",
		    G_CALLBACK (gtk_widget_destroyed), &fd);
  g_signal_connect (G_OBJECT (fd), "delete_event",
		    G_CALLBACK (gtk_widget_destroy), NULL);
  
  g_signal_connect (G_OBJECT (fd),
		    "response", 
		    G_CALLBACK (cb_fd_response),
		    data);

  gtk_widget_show (fd);
}

static void
cb_info (GtkWidget *widget __UNUSED__,
	 GdkEvent  *event __UNUSED__,
	 gpointer   data __UNUSED__)
{
  static GtkWidget *win_info = NULL;
  GtkWidget *button;
  GtkWidget *frame;
  GtkWidget *table;
  GtkWidget *label;
  CPVideoInfo vi = vclip->GetVideoInfo();

  if (win_info)
    {
      gdk_window_raise (win_info->window);
      return;
    }

  win_info = gtk_dialog_new ();

  button = gtk_dialog_add_button (GTK_DIALOG (win_info),
				  GTK_STOCK_CLOSE,
				  GTK_RESPONSE_CLOSE);
  gtk_window_set_title (GTK_WINDOW (win_info), "About");
  g_signal_connect (G_OBJECT (win_info), "destroy",
		    G_CALLBACK (gtk_widget_destroyed), &win_info);
  g_signal_connect (G_OBJECT (win_info), "delete_event",
		    G_CALLBACK (gtk_widget_destroy), NULL);
  g_signal_connect_swapped (G_OBJECT (button), "clicked",
			    G_CALLBACK (gtk_widget_destroy), win_info);
  
  frame = gtk_frame_new ("Video stream");
  gtk_container_set_border_width (GTK_CONTAINER (frame), 6);
  gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (win_info)->vbox),
			       frame);
  gtk_widget_show (frame);

  table = gtk_table_new (3, 2, FALSE);
  gtk_table_set_homogeneous (GTK_TABLE (table), FALSE);
  gtk_table_set_row_spacings (GTK_TABLE (table), 6);
  gtk_table_set_col_spacings (GTK_TABLE (table), 6);
  gtk_container_set_border_width (GTK_CONTAINER (table), 6);
  gtk_container_add (GTK_CONTAINER (frame), table);
  gtk_widget_show (table);

  label = gtk_label_new ("Size: ");
  gtk_misc_set_alignment (GTK_MISC (label),
			  0.0, 0.5);
  gtk_table_attach_defaults (GTK_TABLE (table),
			     label,
			     0, 1,
			     0, 1);
  gtk_widget_show (label);

  label = gtk_label_new ("Framerate: ");
  gtk_misc_set_alignment (GTK_MISC (label),
			  0.0, 0.5);
  gtk_table_attach_defaults (GTK_TABLE (table),
			     label,
			     0, 1,
			     1, 2);
  gtk_widget_show (label);

  label = gtk_label_new ("Frame count: ");
  gtk_misc_set_alignment (GTK_MISC (label),
			  0.0, 0.5);
  gtk_table_attach_defaults (GTK_TABLE (table),
			     label,
			     0, 1,
			     2, 3);
  gtk_widget_show (label);
  
  if (vi && (vi->HasVideo()))
    {
      char str[4096];
      
      snprintf (str, 4096, "%dx%d",
		vi->GetWidth(),
		vi->GetHeight());
      label = gtk_label_new (str);
      gtk_misc_set_alignment (GTK_MISC (label),
			      0.0, 0.5);
      gtk_table_attach_defaults (GTK_TABLE (table),
				 label,
				 1, 2,
				 0, 1);
      gtk_widget_show (label);
      
      snprintf (str, 4096, "%.3f",
		vi->GetFloatFPS());
      label = gtk_label_new (str);
      gtk_misc_set_alignment (GTK_MISC (label),
			      0.0, 0.5);
      gtk_table_attach_defaults (GTK_TABLE (table),
				 label,
				 1, 2,
				 1, 2);
      gtk_widget_show (label);
      
      snprintf (str, 4096, "%d",
		vi->GetFrameCount());
      label = gtk_label_new (str);
      gtk_misc_set_alignment (GTK_MISC (label),
			      0.0, 0.5);
      gtk_table_attach_defaults (GTK_TABLE (table),
				 label,
				 1, 2,
				 2, 3);
      gtk_widget_show (label);
    }
  
  frame = gtk_frame_new ("Audio stream");
  gtk_container_set_border_width (GTK_CONTAINER (frame), 6);
  gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (win_info)->vbox),
			       frame);
  gtk_widget_show (frame);

  table = gtk_table_new (3, 2, FALSE);
  gtk_table_set_homogeneous (GTK_TABLE (table), FALSE);
  gtk_table_set_row_spacings (GTK_TABLE (table), 6);
  gtk_table_set_col_spacings (GTK_TABLE (table), 6);
  gtk_container_set_border_width (GTK_CONTAINER (table), 6);
  gtk_container_add (GTK_CONTAINER (frame), table);
  gtk_widget_show (table);

  label = gtk_label_new ("Channels: ");
  gtk_misc_set_alignment (GTK_MISC (label),
			  0.0, 0.5);
  gtk_table_attach_defaults (GTK_TABLE (table),
			     label,
			     0, 1,
			     0, 1);
  gtk_widget_show (label);

  label = gtk_label_new ("Samplerate: ");
  gtk_misc_set_alignment (GTK_MISC (label),
			  0.0, 0.5);
  gtk_table_attach_defaults (GTK_TABLE (table),
			     label,
			     0, 1,
			     1, 2);
  gtk_widget_show (label);

  label = gtk_label_new ("Sample count: ");
  gtk_misc_set_alignment (GTK_MISC (label),
			  0.0, 0.5);
  gtk_table_attach_defaults (GTK_TABLE (table),
			     label,
			     0, 1,
			     2, 3);
  gtk_widget_show (label);

  if (vi && (vi->HasAudio()))
    {
      char str[4096];
      
      snprintf (str, 4096, "%d",
		vi->GetChannelCount());
      label = gtk_label_new (str);
      gtk_misc_set_alignment (GTK_MISC (label),
			      0.0, 0.5);
      gtk_table_attach_defaults (GTK_TABLE (table),
				 label,
				 1, 2,
				 0, 1);
      gtk_widget_show (label);
      
      snprintf (str, 4096, "%d",
		vi->GetSampleRate());
      label = gtk_label_new (str);
      gtk_misc_set_alignment (GTK_MISC (label),
			      0.0, 0.5);
      gtk_table_attach_defaults (GTK_TABLE (table),
				 label,
				 1, 2,
				 1, 2);
      gtk_widget_show (label);
      
      snprintf (str, 4096, "%Ld",
		vi->GetSampleCount());
      label = gtk_label_new (str);
      gtk_misc_set_alignment (GTK_MISC (label),
			      0.0, 0.5);
      gtk_table_attach_defaults (GTK_TABLE (table),
				 label,
				 1, 2,
				 2, 3);
      gtk_widget_show (label);
    }

  gtk_widget_show (win_info);
}

static void
cb_begin (GtkWidget *widget __UNUSED__,
	  GdkEvent  *event __UNUSED__,
	  gpointer   data __UNUSED__)
{
  if (vclip)
    draw_frame (0);
}

static void
cb_end (GtkWidget *widget __UNUSED__,
	GdkEvent  *event __UNUSED__,
	gpointer   data __UNUSED__)
{
  if (vclip)
    {
      gint framecount = vclip->GetVideoInfo()->GetFrameCount();
      
      draw_frame (framecount - 1);
    }
}

static void
cb_goto (GtkWidget *widget __UNUSED__,
	 GdkEvent  *event __UNUSED__,
	 gpointer   data __UNUSED__)
{
  static GtkWidget *win_goto = NULL;
  GtkWidget *button;
  GtkWidget *hbox;
  GtkWidget *label;

  if (win_goto)
    {
      gdk_window_raise (win_goto->window);
      return;
    }

  win_goto = gtk_dialog_new ();

  if (vclip)
    {
      gtk_window_set_title (GTK_WINDOW (win_goto), "Go to frame...");
      g_signal_connect (G_OBJECT (win_goto), "destroy",
			G_CALLBACK (gtk_widget_destroyed), &win_goto);
      g_signal_connect (G_OBJECT (win_goto), "delete_event",
			G_CALLBACK (gtk_widget_destroy), NULL);
      
      hbox = gtk_hbox_new (FALSE, 6);
      gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (win_goto)->vbox),
				   hbox);
      gtk_widget_show (hbox);
      
      label = gtk_label_new ("Frame number:");
      gtk_misc_set_alignment (GTK_MISC (label),
			      0.0, 0.5);
      gtk_box_pack_start_defaults (GTK_BOX (hbox),
				   label);
      gtk_widget_show (label);
      
      entry = gtk_entry_new ();
      gtk_box_pack_start_defaults (GTK_BOX (hbox),
				   entry);
      
      button = gtk_dialog_add_button (GTK_DIALOG (win_goto),
				      GTK_STOCK_CANCEL,
				      GTK_RESPONSE_CANCEL);
      g_signal_connect_swapped (G_OBJECT (button), "clicked",
				G_CALLBACK (gtk_widget_destroy), win_goto);
      
      button = gtk_dialog_add_button (GTK_DIALOG (win_goto),
				      GTK_STOCK_OK,
				      GTK_RESPONSE_OK);
      g_signal_connect_swapped (G_OBJECT (button), "clicked",
				G_CALLBACK (cb_goto_draw), win_goto);
      gtk_widget_show (entry);
    }
  else
    {
      gtk_window_set_title (GTK_WINDOW (win_goto), "Go to frame...");
      g_signal_connect (G_OBJECT (win_goto), "destroy",
			G_CALLBACK (gtk_widget_destroyed), &win_goto);
      g_signal_connect (G_OBJECT (win_goto), "delete_event",
			G_CALLBACK (gtk_widget_destroy), NULL);
      
      button = gtk_dialog_add_button (GTK_DIALOG (win_goto),
				      GTK_STOCK_CLOSE,
				      GTK_RESPONSE_CLOSE);
      g_signal_connect_swapped (G_OBJECT (button), "clicked",
				G_CALLBACK (gtk_widget_destroy), win_goto);

      label = gtk_label_new ("No Video");
      gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (win_goto)->vbox),
				   label);
      gtk_widget_show (label);
    }

  gtk_widget_show (win_goto);
}

static void
cb_about (GtkWidget *widget __UNUSED__,
	  GdkEvent  *event __UNUSED__,
	  gpointer   data __UNUSED__)
{
  static GtkWidget *win_about = NULL;
  GtkWidget *button;
  GtkWidget *label;
  GtkWidget *table;

  if (win_about)
    {
      gdk_window_raise (win_about->window);
      return;
    }

  win_about = gtk_dialog_new ();

  button = gtk_dialog_add_button (GTK_DIALOG (win_about),
				  GTK_STOCK_CLOSE,
				  GTK_RESPONSE_CLOSE);
  gtk_window_set_title (GTK_WINDOW (win_about), "About");
  g_signal_connect (G_OBJECT (win_about), "destroy",
		    G_CALLBACK (gtk_widget_destroyed), &win_about);
  g_signal_connect (G_OBJECT (win_about), "delete_event",
		    G_CALLBACK (gtk_widget_destroy), NULL);
  g_signal_connect_swapped (G_OBJECT (button), "clicked",
			    G_CALLBACK (gtk_widget_destroy), win_about);
  
  label = gtk_label_new ("Avisynth 3.0 test application\n");
  gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (win_about)->vbox),
			       label);
  gtk_widget_show (label);

  label = gtk_label_new ("A small application to test");
  gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (win_about)->vbox),
			       label);
  gtk_widget_show (label);

  label = gtk_label_new ("some features of Avisynth 3.0\n");
  gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (win_about)->vbox),
			       label);
  gtk_widget_show (label);

  table = gtk_table_new (3, 2, FALSE);
  gtk_table_set_homogeneous (GTK_TABLE (table), FALSE);
  gtk_box_pack_start_defaults (GTK_BOX (GTK_DIALOG (win_about)->vbox),
			       table);
  gtk_widget_show (table);

  label = gtk_label_new ("Lead developper: ");
  gtk_misc_set_alignment (GTK_MISC (label),
			  0.0, 0.5);
  gtk_table_attach_defaults (GTK_TABLE (table),
			     label,
			     0, 1,
			     0, 1);
  gtk_widget_show (label);
  label = gtk_label_new ("David Pierre (aka Bidoche)");
  gtk_misc_set_alignment (GTK_MISC (label),
			  0.0, 0.5);
  gtk_table_attach_defaults (GTK_TABLE (table),
			     label,
			     1, 2,
			     0, 1);
  gtk_widget_show (label);

  label = gtk_label_new ("Contributors: ");
  gtk_misc_set_alignment (GTK_MISC (label),
			  0.0, 0.5);
  gtk_table_attach_defaults (GTK_TABLE (table),
			     label,
			     0, 1,
			     1, 2);
  gtk_widget_show (label);
  label = gtk_label_new ("Kurosu");
  gtk_misc_set_alignment (GTK_MISC (label),
			  0.0, 0.5);
  gtk_table_attach_defaults (GTK_TABLE (table),
			     label,
			     1, 2,
			     1, 2);
  gtk_widget_show (label);
  label = gtk_label_new ("Vincent Torri (aka d'Oursse aka caro)");
  gtk_misc_set_alignment (GTK_MISC (label),
			  0.0, 0.5);
  gtk_table_attach_defaults (GTK_TABLE (table),
			     label,
			     1, 2,
			     2, 3);
  gtk_widget_show (label);

  gtk_widget_show (win_about);
}

static void cb_fd_response (GtkWidget *widget,
			    gint       arg1,
			    gpointer   data __UNUSED__)
{
  switch (arg1)
    {
    case GTK_RESPONSE_ACCEPT:
      {
	gchar *filename;
	guint  id;

	filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (widget));
	g_print ("accept %s\n", filename);
	gtk_widget_destroy (widget);

	id = gtk_statusbar_get_context_id (GTK_STATUSBAR (status),
                                           "open");
	gtk_statusbar_push (GTK_STATUSBAR (status), id, "Opening file...");

	PEnvironment env = RuntimeEnvironment::Create(10000000);
	vclip = GstreamerSource::Create(filename, 0, 0, env);

	gtk_statusbar_pop (GTK_STATUSBAR (status), id);
	gtk_statusbar_push (GTK_STATUSBAR (status), id, "File loaded...");

	draw_frame (1000);

	break;
      }
    default:
      gtk_widget_destroy (widget);
      break;
    }
}

static void
cb_goto_draw (GtkWidget *widget,
	      GdkEvent  *event __UNUSED__,
	      gpointer   data __UNUSED__)
{
  const gchar *str;
  gint frame;

  str = gtk_entry_get_text (GTK_ENTRY (entry));
  frame = (gint)g_ascii_strtod (str, NULL);

  gtk_widget_destroy (widget);

  draw_frame (frame);
}

//Interface
static void draw_frame (long int frame_nbr)
{
  GdkPixbuf *pixbuf;
  const BYTE *rgb;
  PClip vclip_rgb32 = ToRGB32::Create(vclip);
  CPVideoFrame frame = vclip_rgb32->GetFrame(frame_nbr);
  guint width = frame->GetDimension().GetWidth();
  guint height = frame->GetDimension().GetHeight();

  rgb = frame->ReadFrom('~').ptr;
  
  gtk_widget_set_size_request (image, width, height);
  pixbuf = gdk_pixbuf_new_from_data (rgb,
				     GDK_COLORSPACE_RGB, TRUE,
				     8, width, height, 4 * width,
				     (GdkPixbufDestroyNotify) g_free,
				     (guint8 *)rgb);
  gtk_image_set_from_pixbuf (GTK_IMAGE (image), pixbuf);
  g_object_unref (G_OBJECT (pixbuf));
}

static void create_main_window ()
{
  GtkWidget       *win;
  GtkWidget       *vbox;
  GtkWidget       *menubar;
  GtkWidget       *menu;
  GtkWidget       *item;

  win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (win), "Avisynth 3.0 test application");
  g_signal_connect (G_OBJECT (win), "delete_event",
		    G_CALLBACK (cb_delete), NULL);
  g_signal_connect (G_OBJECT (win), "destroy",
		    G_CALLBACK (cb_delete), NULL);
  gtk_window_set_resizable (GTK_WINDOW (win), FALSE);

  vbox = gtk_vbox_new (FALSE, 6);
  gtk_container_add (GTK_CONTAINER (win), vbox);
  gtk_widget_show (vbox);

  menubar = gtk_menu_bar_new ();
  gtk_box_pack_start_defaults (GTK_BOX (vbox),
			       menubar);
  gtk_widget_show (menubar);
  
  /* Menu bar */
  item = gtk_menu_item_new_with_label ("File");
  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), item);
  gtk_widget_show (item);
  
  /* File menu */
  menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (item), menu);
  gtk_widget_show (menu);
  
  item = gtk_menu_item_new_with_label ("Open");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
  g_signal_connect (G_OBJECT (item), "activate",
		    G_CALLBACK (cb_open), win);
  gtk_widget_show (item);
  
  item = gtk_menu_item_new_with_label ("Info");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
  g_signal_connect (G_OBJECT (item), "activate",
		    G_CALLBACK (cb_info), win);
  gtk_widget_show (item);

  item = gtk_separator_menu_item_new ();
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
  gtk_widget_show (item);
  
  item = gtk_menu_item_new_with_label ("Quit");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
  g_signal_connect (G_OBJECT (item), "activate",
		    G_CALLBACK (cb_delete), NULL);
  gtk_widget_show (item);

  item = gtk_menu_item_new_with_label ("Action");
  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), item);
  gtk_widget_show (item);
  
  /* Action menu */
  menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (item), menu);
  gtk_widget_show (menu);
  
  item = gtk_menu_item_new_with_label ("Begin");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
  g_signal_connect (G_OBJECT (item), "activate",
		    G_CALLBACK (cb_begin), win);
  gtk_widget_show (item);
  
  item = gtk_menu_item_new_with_label ("End");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
  g_signal_connect (G_OBJECT (item), "activate",
		    G_CALLBACK (cb_end), win);
  gtk_widget_show (item);
  
  item = gtk_menu_item_new_with_label ("Go to");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
  g_signal_connect (G_OBJECT (item), "activate",
		    G_CALLBACK (cb_goto), win);
  gtk_widget_show (item);

  item = gtk_menu_item_new_with_label ("Help");
  gtk_menu_item_set_right_justified (GTK_MENU_ITEM (item), TRUE);
  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), item);
  gtk_widget_show (item);
  
  /* Help menu */
  menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (item), menu);
  gtk_widget_show (menu);
  item = gtk_menu_item_new_with_label ("About");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
  g_signal_connect (G_OBJECT (item), "activate",
		    G_CALLBACK (cb_about), win);
  gtk_widget_show (item);

  image = gtk_image_new ();
  gtk_widget_set_size_request (image, 320, 200);
  gtk_box_pack_start_defaults (GTK_BOX (vbox),
			       image);
  gtk_widget_show (image);

  status = gtk_statusbar_new ();
  gtk_statusbar_set_has_resize_grip (GTK_STATUSBAR (status), TRUE);
  gtk_box_pack_start_defaults (GTK_BOX (vbox),
			       status);
  gtk_widget_show (status);

  gtk_widget_show (win);
}
