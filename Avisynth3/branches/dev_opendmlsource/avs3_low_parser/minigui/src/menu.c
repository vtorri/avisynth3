#include <stdint.h>

#include <gtk/gtk.h>

#include <x264.h>
#include <x264_gtk.h>

#include "private.h"
#include "open.h"
#include "save.h"
#include "info.h"
#include "edit.h"
#include "about.h"
#include "menu.h"


static void _open_cb (GtkMenuItem *menuitem,
                      gpointer     user_data);
static void _save_cb (GtkMenuItem *menuitem,
                      gpointer     user_data);
static void _info_cb (GtkMenuItem *menuitem,
                      gpointer     user_data);
static void _quit_cb (GtkMenuItem *menuitem,
                      gpointer     user_data);
static void _edit_begin_cb (GtkMenuItem *menuitem,
                      gpointer     user_data);
static void _edit_end_cb (GtkMenuItem *menuitem,
                      gpointer     user_data);
static void _edit_goto_cb (GtkMenuItem *menuitem,
                      gpointer     user_data);
static void _about_window_cb (GtkMenuItem *menuitem,
                              gpointer     user_data);
static void _x264_config_cb (GtkMenuItem *menuitem,
                             gpointer     user_data);


GtkWidget *
avs3_menu (Avs3_Data *data)
{
  GtkWidget       *menubar;
  GtkWidget       *menu;
  GtkWidget       *item;

  menubar = gtk_menu_bar_new ();

  /* File menu */
  item = gtk_menu_item_new_with_label ("File");
  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), item);
  gtk_widget_show (item);

  menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (item), menu);
  gtk_widget_show (menu);
  
  /* File menu items */
  item = gtk_image_menu_item_new_from_stock (GTK_STOCK_OPEN, NULL);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
  g_signal_connect (G_OBJECT (item), "activate",
		    G_CALLBACK (_open_cb), data);
  gtk_widget_show (item);
  
  item = gtk_image_menu_item_new_from_stock (GTK_STOCK_SAVE, NULL);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
  g_signal_connect (G_OBJECT (item), "activate",
		    G_CALLBACK (_save_cb), data);
  gtk_widget_show (item);
  
  item = gtk_image_menu_item_new_from_stock (GTK_STOCK_INFO, NULL);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
  g_signal_connect (G_OBJECT (item), "activate",
		    G_CALLBACK (_info_cb), data);
  gtk_widget_show (item);

  item = gtk_separator_menu_item_new ();
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
  gtk_widget_show (item);
  
  item = gtk_image_menu_item_new_from_stock (GTK_STOCK_QUIT, NULL);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
  g_signal_connect (G_OBJECT (item), "activate",
		    G_CALLBACK (_quit_cb), data);
  gtk_widget_show (item);

  /* Edition menu */
  item = gtk_menu_item_new_with_label ("Edit");
  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), item);
  gtk_widget_show (item);

  menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (item), menu);
  gtk_widget_show (menu);

  /* Edition menu items */
  item = gtk_menu_item_new_with_label ("Begin");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
  g_signal_connect (G_OBJECT (item), "activate",
		    G_CALLBACK (_edit_begin_cb), data);
  gtk_widget_show (item);
  
  item = gtk_menu_item_new_with_label ("End");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
  g_signal_connect (G_OBJECT (item), "activate",
		    G_CALLBACK (_edit_end_cb), data);
  gtk_widget_show (item);
  
  item = gtk_menu_item_new_with_label ("Go to");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
  g_signal_connect (G_OBJECT (item), "activate",
		    G_CALLBACK (_edit_goto_cb), data);
  gtk_widget_show (item);

  /* Video menu */
  item = gtk_menu_item_new_with_label ("Video");
  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), item);
  gtk_widget_show (item);
  
  menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (item), menu);
  gtk_widget_show (menu);

  /* Video menu items */  
  item = gtk_menu_item_new_with_label ("Compression");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
  g_signal_connect (G_OBJECT (item), "activate",
		    G_CALLBACK (_x264_config_cb), data);
  gtk_widget_show (item);
  
  /* Help menu */
  item = gtk_menu_item_new_with_label ("Help");
  gtk_menu_item_set_right_justified (GTK_MENU_ITEM (item), TRUE);
  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), item);
  gtk_widget_show (item);
  
  /* Help menu items */
  menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (item), menu);
  gtk_widget_show (menu);
  item = gtk_menu_item_new_with_label ("About");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
  g_signal_connect (G_OBJECT (item), "activate",
		    G_CALLBACK (_about_window_cb), NULL);
  gtk_widget_show (item);

  return menubar;
}

static void
_open_cb (GtkMenuItem *menuitem,
          gpointer     user_data)
{
  avs3_open ((Avs3_Data *)user_data);
}

static void
_save_cb (GtkMenuItem *menuitem,
          gpointer     user_data)
{
  avs3_save ((Avs3_Data *)user_data);
}

static void
_info_cb (GtkMenuItem *menuitem,
          gpointer     user_data)
{
  avs3_info_plugins ((Avs3_Data *)user_data);
}

static void
_quit_cb (GtkMenuItem *menuitem,
          gpointer     user_data)
{
  gtk_main_quit ();
  
  _data_delete ((Avs3_Data *)user_data);
}

static void
_edit_begin_cb (GtkMenuItem *menuitem,
                gpointer     user_data)
{
  avs3_edit_begin ((Avs3_Data *)user_data, NULL);
}

static void
_edit_end_cb (GtkMenuItem *menuitem,
              gpointer     user_data)
{
  avs3_edit_end ((Avs3_Data *)user_data, NULL);
}

static void
_edit_goto_cb (GtkMenuItem *menuitem,
               gpointer     user_data)
{
  avs3_edit_goto ((Avs3_Data *)user_data, NULL);
}

static void
_about_window_cb (GtkMenuItem *menuitem,
                  gpointer     user_data)
{
  avs3_about ();
}

static void
_x264_config_cb (GtkMenuItem *menuitem,
                 gpointer     user_data)
{
  Avs3_Data *data;
  GtkWidget *window;

  data = (Avs3_Data *)user_data;
  window = x264_gtk_window_create (GTK_WIDGET (data->window));
  x264_gtk_shutdown (window);
}
