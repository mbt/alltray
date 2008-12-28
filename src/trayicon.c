/*
 * GPL Notice:
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Library General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *
 * Name:
 *
 *    alltray
 *
 *
 * Copyright:
 * 
 *    Jochen Baier, 2004, 2005 (email@Jochen-Baier.de)
 *
 *
 * Based on code from:
 *
 *    steal-xwin.c (acano@systec.com)
 *    xswallow (Caolan McNamara ?)
 *    kdocker (Girish Ramakrishnan)
 *    libwnck (Havoc Pennington <hp@redhat.com>)
 *    eggtrayicon (Anders Carlsson <andersca@gnu.org>)
 *    dsimple.c ("The Open Group")
 *    xfwm4 (Olivier Fourdan <fourdan@xfce.org>)
 *    .....lot more, THANX !!!
 *    
*/


#include "common.h"
#include "utils.h"
#include "trayicon.h"

#define SYSTEM_TRAY_REQUEST_DOCK 0

GtkWidget *menu = NULL;
GtkWidget *show_item;
GtkWidget *exit_item; 

void tray_done (win_struct *win)
{
  
  gdk_window_remove_filter (win->root_gdk, manager_filter, (gpointer) win);
  gdk_window_remove_filter (win->manager_window_gdk, manager_filter, NULL);

  if (menu)
    gtk_widget_destroy (menu);
  
  if (win->plug)
    gtk_widget_destroy (win->plug);
  
}

void exit_call(GtkWidget * button, gpointer user_data)
{

  win_struct *win= (win_struct*) user_data;
 
  destroy_all_and_exit (win, TRUE);

}

void show_hide_call (GtkWidget * button, gpointer user_data)
{

  win_struct *win= (win_struct*) user_data;

  show_hide_window (win, force_disabled, FALSE);
}

void command_menu_call (GtkWidget * button, gpointer user_data)
{

  gchar *command= (gchar*) user_data;
  
  if (debug) printf ("command_menu_call: %s\n", command);
  
  exec_command (command);
}

void menu_init (win_struct *win)
{
  
  menu = gtk_menu_new();
  
  command_menu_struct command;
  gint i;
     
  
  GtkWidget *title = gtk_menu_item_new_with_label("   AllTray");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), title);
  gtk_widget_set_sensitive(title, FALSE);
 
  GtkWidget *separator1 = gtk_menu_item_new();
  gtk_widget_show(separator1);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), separator1);
  
  
  if (debug) printf ("command_menu->len: %d\n", win->command_menu->len);
  
  if (win->command_menu->len >0) {
  
    for (i=0; i < win->command_menu->len; i++) {
      
      command=g_array_index (win->command_menu, command_menu_struct, i);
      
      if (debug) printf ("found command.entry: %s\n", command.entry);
      
      GtkWidget *item = gtk_menu_item_new_with_label(command.entry);
      g_signal_connect(G_OBJECT(item), "activate",
        G_CALLBACK(command_menu_call), (gpointer) command.command);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
  
    }
    
    GtkWidget *separator2 = gtk_menu_item_new();
    gtk_widget_show(separator2);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), separator2);
    gtk_widget_set_sensitive(separator2, FALSE);
  
  }
  
 
  show_item = gtk_menu_item_new_with_label("Show/Hide");
  g_signal_connect(G_OBJECT(show_item), "activate",
      G_CALLBACK(show_hide_call), (gpointer) win);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), show_item);

  GtkWidget *separator3 = gtk_menu_item_new();
  gtk_widget_show(separator3);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), separator3);
  gtk_widget_set_sensitive(separator3, FALSE);

  exit_item = gtk_menu_item_new_with_label("Exit");
  g_signal_connect(G_OBJECT(exit_item), "activate",
      G_CALLBACK(exit_call), (gpointer) win);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), exit_item);

  gtk_widget_show_all(menu);
}

void update_tray_icon(win_struct *win)
{
  
  GdkPixbuf *tmp=NULL;
  XWindowAttributes wa;
  gboolean resize=FALSE;
   
  gdk_error_trap_push();
  XGetWindowAttributes (GDK_DISPLAY(), win->plug_xlib, &wa);
  
  if (gdk_error_trap_pop())
    return;
   
  if (debug) printf ("update_tray_icon: real plug size: %dx%d\n", wa.width, wa.height);
    
  if (wa.width <=1 || wa.height <= 1)
    return;

  if ((wa.height > 24 && win->large_icons) || wa.height < 24 ) {
    resize=TRUE;
    if (debug) printf ("resize = TRUE\n");
  }
  
  if (resize) {
    
    if (win->user_icon_path)
      tmp=get_user_icon (win->user_icon_path, wa.height, wa.height);
  
    if (!tmp)
      tmp=get_window_icon (win->child_xlib, wa.height, wa.height);

    if (win->tray_icon)
      g_object_unref(win->tray_icon);
    win->tray_icon=tmp;
    
  }

  if (!resize)
    gtk_fixed_move (GTK_FIXED(win->fixed), win->image_icon, 
    (wa.width - 24) /2, (wa.height -24) /2);
    
  if (GTK_IS_WIDGET (win->image_icon))
    gtk_image_set_from_pixbuf(GTK_IMAGE(win->image_icon), win->tray_icon);

}

void dock_window(Window manager_window, Window window)
{

  XClientMessageEvent ev;
  
  ev.type = ClientMessage;
  ev.window = manager_window;
  ev.message_type =system_tray_opcode_atom;
  ev.format = 32;
  ev.data.l[0] = CurrentTime;
  ev.data.l[1] = SYSTEM_TRAY_REQUEST_DOCK;
  ev.data.l[2] = window;
  ev.data.l[3] =0;
  ev.data.l[4] = 0;

  gdk_error_trap_push ();
    XSendEvent (GDK_DISPLAY(), manager_window, False, NoEventMask, (XEvent *)&ev);
    XSync (GDK_DISPLAY(), False);
  gdk_error_trap_pop ();

}

Window get_manager_window (void)
{

  Window manager_window=None;
 
  gdk_error_trap_push();

   XGrabServer (GDK_DISPLAY());
  
  manager_window = XGetSelectionOwner (GDK_DISPLAY(), selection_atom);
     
  XUngrabServer (GDK_DISPLAY());
  XFlush (GDK_DISPLAY());

  if (gdk_error_trap_pop())
    return None;
    
  display_window_id(GDK_DISPLAY(), manager_window);
    
  return manager_window;
}

GdkFilterReturn
manager_filter (GdkXEvent *xevent, GdkEvent *event, gpointer user_data)
{
  
  XEvent *xev = (XEvent *)xevent;
  XWindowAttributes wa; 

  win_struct *win= (win_struct *) user_data;

  if (xev->xany.type == ClientMessage &&
      xev->xclient.message_type == manager_atom &&
      xev->xclient.data.l[1] == selection_atom)
    {
      
    
    if (debug) printf ("manager: here i am\n");
                          
    create_tray_and_dock(win);
    
    }
  else if (xev->xany.window == win->manager_window)
    {
      if (xev->xany.type == DestroyNotify)
    {
    
       if (debug) printf ("manger destroy notfiy\n");
      
      
      gdk_error_trap_push ();
          
      XGetWindowAttributes (GDK_DISPLAY(), win->manager_window, &wa);
      
     if (gdk_error_trap_pop()) {
     
         gdk_window_remove_filter (win->manager_window_gdk, manager_filter, NULL);
        
     
        if (win->plug) {
        
          if (GTK_IS_WIDGET(win->plug))
            gtk_widget_destroy (win->plug);
          
          win->plug=NULL;
        }
      }
  
    }

    }
  
  return GDK_FILTER_CONTINUE;
}

gboolean on_icon_window_press_event(GtkWidget *widget, GdkEventButton * event,
    gpointer user_data)
{
  
  win_struct *win= (win_struct*) user_data;
  
  if (debug) printf ("icon window press event\n");

  /*right click */
  if (event->button == 1) {
  
    show_hide_window (win, force_disabled, FALSE);
    return TRUE;

  }

  /*left click */
  if (event->button == 3) {
  
  gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, 0,
      gtk_get_current_event_time());
  }

  return TRUE;
}

gboolean icon_window_configure_event (GtkWidget *widget,
  GdkEventConfigure * event, gpointer user_data)
{
 
  win_struct *win = (win_struct*) user_data;

  if (!GTK_IS_WIDGET (win->fixed))
    return FALSE;
  
  if (debug) printf ("icon_window_configure_event: %dx%d\n",
    event->width, event->height);
  
  if (event->width <= 1 || event->height <= 1)
    return FALSE;
  

  update_tray_icon(win);

  return FALSE;
    
}

/*check for gdk_timestamp_prop. if this is set the system*/
/*tray seems to be made with GTK. -> no fake transparency*/
/*tray icon background needed. (Gnome, XFCE)*/

gboolean tray_is_made_with_gtk (Window win)
{
  Atom *list;
  gint nitems;
  gint i;
  
  gboolean status=FALSE;
  gint err;
            
  gdk_error_trap_push();
    
  list=XListProperties(GDK_DISPLAY(), win, &nitems);
  
  err=gdk_error_trap_pop();
  
  if (err==0 && list!=NULL) {
 
    for (i=0; i< nitems; i++) {
      
      if (list[i] == gdk_timestamp_prop) {
        status=TRUE;
        if (debug) printf ("gdk_timestamp_prop found\n");
      }
            
      
    }
        
  }
  
  if (list)
    XFree (list);

  return status;
  
}

void plug_embedded (GtkWidget *widget,
  gpointer user_data)
{
   win_struct *win= (win_struct*) user_data;
  
  if (debug) printf ("plug embedded\n");

  //XMapWindow (GDK_DISPLAY(), win->plug_xlib);
  
  if (!win->gtk_tray)
    gtk_sleep (2000); //XXX better idea ?

  gtk_main_quit ();
  
}

void create_tray_and_dock (win_struct *win)
{

  GtkWidget *image_back=NULL;
  GdkPixbuf *background_pixbuf;
  XWindowAttributes wa;
  GdkGeometry hints;
  gint width, icon_width;
  gint height, icon_height;
  gint x_return=0, y_return=0;
  gint subtract=0;
  
  if (win->manager_window==None)
    win->manager_window=get_manager_window();
  
  if (win->manager_window==None)
    return;
  
  /*too lazy to fix that now*/
  if (!assert_window (win->manager_window)) {
    
    printf ("Alltray: Can not dock!.\nPlease remove system tray applet\n"\
    "and add it again.\n");
    return;
  }

  //display_window_id (GDK_DISPLAY(), win->manager_window);
    
  win->gtk_tray = tray_is_made_with_gtk(win->manager_window);
  
  win->manager_window_gdk=NULL;
  win->manager_window_gdk=gdk_window_foreign_new(win->manager_window);
  
     
  gdk_window_set_events (win->manager_window_gdk, GDK_SUBSTRUCTURE_MASK);
  gdk_window_add_filter (win->manager_window_gdk, manager_filter, (gpointer) win);
  
  if (win->plug && GTK_IS_WIDGET (win->plug)) {
    if (debug) printf ("plug is still alive\n");
    gtk_widget_destroy(win->plug);
  }
  
  win->plug=NULL;
    
  win->plug= gtk_plug_new (0);
  gtk_container_set_border_width (GTK_CONTAINER (win->plug), 0);
  gtk_widget_realize (win->plug);

  hints.min_width = 24;
  hints.min_height = 24;

  gdk_window_set_geometry_hints (win->plug->window,
    &hints, GDK_HINT_MIN_SIZE);

  win->plug_xlib=gtk_plug_get_id (GTK_PLUG(win->plug));
  
  if (debug) printf ("win->plug_xlib: %d\n", (gint) win->plug_xlib);

  if (!win->gtk_tray)
    XSetWindowBackgroundPixmap(GDK_DISPLAY(), win->plug_xlib, ParentRelative);
  
  if (debug) printf ("send dock request\n");
    
  g_signal_connect ((gpointer) win->plug, "embedded",
                    G_CALLBACK (plug_embedded),
                    (gpointer) win);

  dock_window (win->manager_window, win->plug_xlib);
      
  gtk_main ();
   
  gdk_error_trap_push();
   
  XGetWindowAttributes (GDK_DISPLAY(), win->plug_xlib, &wa);
  
  if (!gdk_error_trap_pop()) {
             
    width=wa.width;
    height=wa.height;

   } else  {
   
    width=24;
    height=24;
      
  }
  
  if (debug) printf ("first real tray size: height: %d, width: %d\n", height, width);
    
  if (width <=1 || height <=1) {
    width=24;
    height=24;
  }
  
  if (debug) printf ("fixed tray size: height: %d, width: %d\n", height, width);
 
  win->fixed= gtk_fixed_new ();
  gtk_fixed_set_has_window(GTK_FIXED (win->fixed),TRUE);
  gtk_container_add(GTK_CONTAINER(win->plug), win->fixed);

  if (!win->gtk_tray) {
    
    /*KDE bug: KDE likes to move tray window out of screen if panel*/
    /*is small (< 26 pixel)*/
    if (debug) printf ("wa.x: %d wa.y:%d\n", wa.x, wa.y);
            
    get_window_position (win->plug_xlib, &x_return, &y_return);
    
    if (debug) printf ("x_return: %d y_return: %d\n", x_return, y_return);
    
    if (y_return + height >= win->screen_height -1 ) {
      if (debug) printf ("window out of screen !\n");
      subtract= (y_return + height +2) - (win->screen_height);
      if (debug) printf ("subtract: %d\n", subtract);
      
    }

    background_pixbuf=gdk_pixbuf_xlib_get_from_drawable
      (NULL, win->plug_xlib, 0, NULL, 0, 0, 0, 0, width, height-subtract);
   
    
    image_back=gtk_image_new ();
    gtk_image_set_from_pixbuf(GTK_IMAGE(image_back), background_pixbuf);
    g_object_unref (background_pixbuf);
  
    gtk_fixed_put (GTK_FIXED (win->fixed), GTK_WIDGET (image_back), 0, 0);
        
  }  
  
  win->image_icon=gtk_image_new ();
  
  if (win->tray_icon)
    g_object_unref(win->tray_icon);
  win->tray_icon=NULL;
 
  if (win->large_icons)
   {icon_width=width; icon_height=height; }
  else
   {icon_width=24; icon_height=24;}

  if (win->user_icon_path)
    win->tray_icon=get_user_icon (win->user_icon_path, icon_width, icon_height);
  
  if (!win->tray_icon) {
    win->tray_icon=get_window_icon (win->child_xlib, icon_width, icon_height);
  }

  gtk_image_set_from_pixbuf(GTK_IMAGE(win->image_icon), win->tray_icon);
  
  if ((height > 24 && win->large_icons) || height < 24 )
    gtk_fixed_put (GTK_FIXED (win->fixed), GTK_WIDGET (win->image_icon),
      0, 0);
  else
     gtk_fixed_put (GTK_FIXED(win->fixed), win->image_icon, 
       (width - 24) /2, (height-24) /2);  
  
   
  
  if (!win->gtk_tray)
    gtk_widget_show (image_back);
  gtk_widget_show (win->image_icon);
  gtk_widget_show(win->fixed);
  gtk_widget_show (win->plug);

  g_signal_connect ((gpointer) win->plug, "button_press_event",
                    G_CALLBACK (on_icon_window_press_event),
                    (gpointer) win);
 

  g_signal_connect ((gpointer) win->plug, "configure_event",
                    G_CALLBACK (icon_window_configure_event),
                    (gpointer) win);

  
  tray_update_tooltip (win);

  
  //update_tray_icon (win);
  
}

void tray_update_tooltip (win_struct *win)
{

  if (win->plug && win->title) {
     if (debug) printf ("update tooltip have plug window\n");
        
      gtk_tooltips_set_tip(GTK_TOOLTIPS(win->tooltip),
          GTK_WIDGET(win->plug), win->title, NULL);
    }
  
}

void tray_init (win_struct *win)
{
   
  menu_init(win);

  win->manager_window=get_manager_window();

  win->tooltip = gtk_tooltips_new();
  
  if (win->manager_window) {
          
    if (debug) printf ("have manager window.... now creat_tray.....\n");
    create_tray_and_dock(win);
  } else
  {
    printf ("Alltray: no system tray/notification area found. I will wait..... I have time....\n");
  }
  
  gdk_window_add_filter (win->root_gdk, manager_filter, (gpointer) win);

}
