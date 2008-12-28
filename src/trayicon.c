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
 *    Jochen Baier, 2004 (email@Jochen-Baier.de)
 *
 *
 * Based on:
 *
 *    steal-xwin.c (acano@systec.com)
 *    xswallow (Caolan McNamara ?)
 *    kdocker (Girish Ramakrishnan)
 *    libwnck (Havoc Pennington <hp@redhat.com>)
 *    eggtrayicon (Anders Carlsson <andersca@gnu.org>)
 *    dsimple.c ("The Open Group")
 *    .....lot more, THANX !!!
 *    
*/


#include "gtray.h"
#include "utils.h"
#include "trayicon.h"

#define SYSTEM_TRAY_REQUEST_DOCK 0
#define SYSTEM_TRAY_BEGIN_MESSAGE 1
#define SYSTEM_TRAY_CANCEL_MESSAGE 2


GtkWidget *menu = NULL;
GtkWidget *show_item;
GtkWidget *exit_item; 

void tray_done (win_struct *win)
{
  
  gdk_window_remove_filter (win->root_gdk, manager_filter, (gpointer) win);
  gdk_window_remove_filter (win->manager_window_gdk, manager_filter, NULL);

  if (menu)
    gtk_widget_destroy (menu);
  
  if (win->tray_window)
    gtk_widget_destroy (win->tray_window);
}

void exit_call(GtkWidget * button, gpointer user_data)
{

  win_struct *win= (win_struct*) user_data;
 
  destroy_all_and_exit (win, TRUE);

}

void show_hide_call (GtkWidget * button, gpointer user_data)
{

  win_struct *win= (win_struct*) user_data;

  show_hide_window (win, force_disabled);
}

void menu_init (win_struct *win)
{
  
  menu = gtk_menu_new();

  GtkWidget *title = gtk_menu_item_new_with_label("   AllTray");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), title);
  gtk_widget_set_sensitive(title, FALSE);
 
  GtkWidget *separator1 = gtk_menu_item_new();
  gtk_widget_show(separator1);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), separator1);
 
  show_item = gtk_menu_item_new_with_label("Show/Hide");
  g_signal_connect(G_OBJECT(show_item), "activate",
      G_CALLBACK(show_hide_call), (gpointer) win);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), show_item);

  GtkWidget *separator2 = gtk_menu_item_new();
  gtk_widget_show(separator2);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), separator2);
  gtk_widget_set_sensitive(separator2, FALSE);

  exit_item = gtk_menu_item_new_with_label("Exit");
  g_signal_connect(G_OBJECT(exit_item), "activate",
      G_CALLBACK(exit_call), (gpointer) win);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), exit_item);

  gtk_widget_show_all(menu);
}

void tray_update_icon(win_struct *win, GdkPixbuf *icon)
{

  if (GTK_IS_WIDGET (win->image_icon))
    gtk_image_set_from_pixbuf(GTK_IMAGE(win->image_icon), icon);

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
        
     
        if (win->tray_window) {
        
          if (GTK_IS_WIDGET(win->tray_window))
            gtk_widget_destroy (win->tray_window);
          
          win->tray_window=NULL;
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

  /*right click */
  if (event->button == 1) {
  
    show_hide_window (win, force_disabled);
    return TRUE;

  }

  /*left click */
  if (event->button == 3) {
  
  gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, 0,
      gtk_get_current_event_time());
  }

  return TRUE;
}

static GdkFilterReturn
tray_window_filter (GdkXEvent *xevent, GdkEvent *event, gpointer user_data)
{
  
  XEvent *xev = (XEvent *)xevent;
  XConfigureEvent *xconfigure;
  gint height;
  
  if ( !(xev->xany.type == ConfigureNotify) )
   return GDK_FILTER_CONTINUE;
  
  win_struct *win = (win_struct*) user_data;

  xconfigure =(XConfigureEvent*) xev;
  
  if (debug) printf ("xconfigure.height: %d\n", xconfigure->height);

  height=  xconfigure->height;
     
  gtk_fixed_move (GTK_FIXED(win->fixed), win->image_icon, 0, 
     height >=24 ? (height/2) -12 : 0);
   
  
  return GDK_FILTER_CONTINUE;
}

void create_tray_and_dock (win_struct *win)
{

  GtkWidget *image_back;
  GdkPixbuf *background_pixbuf;

  Window tray_win_xlib;
  XWindowAttributes wa;

  GdkGeometry hints;

  gint width;
  gint height;

  if (win->manager_window==None)
    win->manager_window=get_manager_window();
  
  if (win->manager_window==None)
    return;
    
  win->manager_window_gdk=gdk_window_foreign_new(win->manager_window);
  gdk_window_set_events (win->manager_window_gdk, GDK_SUBSTRUCTURE_MASK);
  gdk_window_add_filter (win->manager_window_gdk, manager_filter, (gpointer) win);
     
  if (GTK_IS_WIDGET(win->tray_window)) {
    gtk_widget_destroy (win->tray_window);
    win->tray_window=NULL;
  }

  win->tray_window =gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_decorated (GTK_WINDOW(win->tray_window), FALSE);
  gtk_widget_realize (win->tray_window);

  hints.min_width = 24;
  hints.min_height = 24;
              
  gdk_window_set_geometry_hints (win->tray_window->window,
        &hints, GDK_HINT_MIN_SIZE);

  tray_win_xlib = GDK_WINDOW_XID (win->tray_window->window);
  XSetWindowBackgroundPixmap(GDK_DISPLAY(), tray_win_xlib, ParentRelative);
  if (debug) printf ("send dock request\n");

  dock_window (win->manager_window, tray_win_xlib);
  gtk_sleep(2);
  
  gdk_error_trap_push();
   
  XGetWindowAttributes (GDK_DISPLAY(), tray_win_xlib, &wa);
  
  if (!gdk_error_trap_pop()) {
             
    width=wa.width;
    height=wa.height;

   } else  {
   
    width=24;
    height=24;
      
  }

  background_pixbuf=gdk_pixbuf_xlib_get_from_drawable (NULL, tray_win_xlib, 0, NULL,
    0, 0, 0, 0, width, height);
 
  win->fixed= gtk_fixed_new ();
  gtk_fixed_set_has_window(GTK_FIXED (win->fixed),TRUE);
  gtk_container_add(GTK_CONTAINER(win->tray_window), win->fixed);
  
  image_back=gtk_image_new ();
  gtk_image_set_from_pixbuf(GTK_IMAGE(image_back), background_pixbuf);
  g_object_unref (background_pixbuf);

  gtk_fixed_put (GTK_FIXED (win->fixed), GTK_WIDGET (image_back), 0, 0);
  
  win->image_icon=gtk_image_new ();
  gtk_image_set_from_pixbuf(GTK_IMAGE(win->image_icon), win->icon);
  gtk_fixed_put (GTK_FIXED (win->fixed), GTK_WIDGET (win->image_icon), 0, 
    height >=24 ? (height/2) -12 : 0);
  
  gtk_widget_show (image_back);
  gtk_widget_show (win->image_icon);
  gtk_widget_show(win->fixed);
  gtk_widget_show (win->tray_window);

  g_signal_connect (win->tray_window, "button_press_event",
                    G_CALLBACK (on_icon_window_press_event),
                    (gpointer) win);
                    
  tray_update_tooltip (win);
  
  gdk_window_set_events(win->tray_window->window, GDK_ALL_EVENTS_MASK);
  gdk_window_add_filter(win->tray_window->window, tray_window_filter, (gpointer) win);    
  
   
}

void tray_update_tooltip (win_struct *win)
{

  if (win->tray_window && win->title) {
     if (debug) printf ("update tooltip have tray window\n");
        
      gtk_tooltips_set_tip(GTK_TOOLTIPS(win->tooltip),
          GTK_WIDGET(win->tray_window), win->title, NULL);
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
