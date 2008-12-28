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
 *    kdocker (Girish Ramakrishnan)
 *    libwnck, eggtrayicon (Havoc Pennington <hp@redhat.com>)
 *    dsimple.c ("The Open Group")
 *    .....lot more, THANX !!!
 *    
*/


#include "gtray.h"
#include "utils.h"
#include "trayicon.h"


gboolean debug=FALSE;


static GdkFilterReturn
root_filter_manager_window (GdkXEvent *xevent, GdkEvent *event, gpointer user_data)
{
  
  XEvent *xev = (XEvent *)xevent;
  XWindowAttributes wa; 

  win_struct *win= (win_struct *) user_data;

  if (debug) printf ("root_filter_manager_window event\n");

  if (xev->xany.type == ClientMessage &&
      xev->xclient.message_type == manager_atom &&
      xev->xclient.data.l[1] == selection_atom)
    {
    
    display_window_id (GDK_DISPLAY(), xev->xclient.window);
      
    
    if (debug) printf ("manager: here i am\n");
      
      gtk_main_quit ();
                        
  }
 
  return GDK_FILTER_CONTINUE;
}

static GdkFilterReturn root_filter_map (GdkXEvent *xevent, 
    GdkEvent *event, gpointer user_data)
{
  XEvent *xev = (XEvent *)xevent;
  XMapEvent *xmap;
  Window window=None;

  win_struct *win = (win_struct*) user_data;

 //if (debug) printf ("root_filter_map event: %s\n", event_names[xev->xany.type]);
   
  if (xev->xany.type == MapNotify) {
  
   if (debug) printf ("map notify\n");
       
    xmap=(XMapEvent*) xev;
    window=XmuClientWindow(GDK_DISPLAY(), xmap->window);
   
    //display_window_id (GDK_DISPLAY(), window);
       
    if (window != None) {
      if (window_match (window, win)) {
        win->xlib=window;
        gtk_main_quit ();
      }
    }   
 
  }
  
  return GDK_FILTER_CONTINUE;
}

static  GdkFilterReturn root_filter_workspace (GdkXEvent *xevent, 
    GdkEvent *event, gpointer user_data)
{
  XEvent *xev = (XEvent *)xevent;
  XMapEvent *xmap;
  Window window=None;

  win_struct *win = (win_struct*) user_data;

  //if (debug) printf ("root_filter_workspace event: %s\n", event_names[xev->xany.type]);

  if (xev->xany.type == ClientMessage &&
      xev->xclient.message_type == net_current_desktop) {
  
    if (debug) printf ("workspace switched\n");
      
    win->last_desktop=get_current_desktop();
    
    gtk_window_get_position(GTK_WINDOW(win->parent_window), 
        &win->parent_window_x, &win->parent_window_y);
    
    gtk_widget_hide (win->parent_window);
  
  }

 return GDK_FILTER_CONTINUE;
}

GdkFilterReturn child_window_filter_our(GdkXEvent *xevent, GdkEvent *event, gpointer user_data)
{
  XEvent *xev = (XEvent *)xevent;
  XExposeEvent *xexpose;
  XVisibilityEvent *xvisibility;
  XConfigureEvent *xconfigure;
  
  win_struct *win= (win_struct*) user_data;
  
  //if (debug) printf ("child window our event: %s\n", event_names[xev->xany.type]);
  
  switch (xev->xany.type) {
    
    case UnmapNotify: {
    
      if (debug) printf ("unmap notify\n");
    
      if (not_reparent (win->xlib_our))
        gtk_socket_add_id (GTK_SOCKET (win->sock), win->xlib_our);
      }
      
    break;

    case DestroyNotify:
      if (debug) printf ("destroy notify\n");
     	 destroy_all_and_exit (win, FALSE);
    break;
    
   case PropertyNotify:
    { 
      XPropertyEvent *xproperty = (XPropertyEvent *) xev;
      
      if (debug) printf ("property notify\n");
            
      if (xproperty->atom == wm_name_atom) {
        update_window_title(win);
      break;
      }
      
      if (xproperty->atom == net_wm_icon  || 
          xproperty->atom == wm_icon_atom ) {
        update_window_icon(win);
    } 
    
    }
    
  }

  return GDK_FILTER_CONTINUE;
}

GdkFilterReturn child_window_filter (GdkXEvent *xevent, GdkEvent *event, gpointer user_data)
{
  XEvent *xev = (XEvent *)xevent;
  XExposeEvent *xexpose;
  XVisibilityEvent *xvisibility;
  XConfigureEvent *xconfigure;
  
  win_struct *win= (win_struct*) user_data;
  
  //if (debug) printf ("child window event: %s\n", event_names[xev->xany.type]);
  
  switch (xev->xany.type) {
  
    case PropertyNotify:
    { 
      XPropertyEvent *xproperty = (XPropertyEvent *) xev;
    
     if (debug) printf ("property notify\n");
      
      if (xproperty->atom == wm_name_atom) {
        update_window_title(win);
      break;
      }
      
      if (xproperty->atom == net_wm_icon  || 
          xproperty->atom == wm_icon_atom ) {
        update_window_icon(win);
    }
    
    break;
    
    }
  
  }

  return GDK_FILTER_CONTINUE;
}

gboolean on_parent_window_delete_event (GtkWidget       *widget,
                                        GdkEvent *event,
                                        gpointer         user_data)
{

  win_struct *win= (win_struct*) user_data;
 
  if (debug) printf ("delete event\n");
  gtk_window_get_position(GTK_WINDOW(widget), &win->parent_window_x, &win->parent_window_y);
  gtk_widget_hide (widget);
  return TRUE;

}

gboolean on_parent_window_focus_in_event (GtkWidget       *widget,
                                        GdkEvent *event,
                                        gpointer         user_data)
{
  
  win_struct *win= (win_struct*) user_data;
  
  if (debug) printf ("focus event\n");
  
  if (!assert_window (win->xlib))
    return FALSE;
  
  if (!xlib_window_is_viewable (win->xlib))
    return FALSE;
    
  XSetInputFocus (GDK_DISPLAY(), win->xlib, RevertToPointerRoot, CurrentTime);
 
  return FALSE;
}

gboolean on_parent_window_enter_notify_event(GtkWidget       *widget,
                                        GdkEvent *event,
                                        gpointer         user_data)
{
  
  win_struct *win= (win_struct*) user_data;

  if (debug) printf ("enter notify\n");

  if (!assert_window (win->xlib))
    return FALSE;
  
  if (!xlib_window_is_viewable (win->xlib))
    return FALSE;
   
  XSetInputFocus (GDK_DISPLAY(), win->xlib,  RevertToPointerRoot, CurrentTime);
 
  return FALSE;
}

/*for KDE*/
gboolean on_parent_window_configure_event             (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{

  static gint old_width=0;
  static gint old_height=0;

  if (old_width == event->width && old_height == event->height)
    return FALSE;
  
  win_struct *win= (win_struct*) user_data;
  
  old_width=event->width;
  old_height=event->height;
    
  gdk_window_resize (win->gdk, old_width, old_height);

  return FALSE;
}


/* XXX do not work ?
void pid_watch (GPid pid, gint status, gpointer data)
{

  printf ("child watch\n");
}
*/


int
main (int argc, char *argv[])
{

  gint child_window_w;
  gint child_window_h;
  gint child_window_x;
  gint child_window_y;
  
  gchar *string=NULL;
   
  gchar *wm_strings;
  gchar *command=NULL;
  gint show=0;
    
  GdkWindow *root_window=NULL;


  gtk_init (&argc, &argv);
  gdk_pixbuf_xlib_init (GDK_DISPLAY(), DefaultScreen (GDK_DISPLAY()));
  atom_init ();
 
  win_struct *win = malloc (sizeof (win_struct));

  win->parent_window=NULL;
  win->parent_window_x=100;
  win->parent_window_y=100;
  
  win->xlib=None;
  win->xlib_our=None;
     
  win->wm_res_class=NULL;
  win->wm_res_name=NULL;
  win->wm_name=NULL;
  win->pid=(GPid) 0;
  win->manager_window=None;
  win->tray_window=NULL;
  win->last_desktop=0;
  win->ignore_splash_window=FALSE;
  win->title=NULL;
        

  if (!parse_arguments(argc, argv, &string, &command, &show, &win->ignore_splash_window)) {
    return 0;
  }
  
  if (debug) {
    printf ("string: %s\n", string);
    printf ("command: %s\n", command);
    printf ("show: %d\n", show);
  
    if (win->ignore_splash_window)
      printf ("ignore splash window\n");
  } 
   
  if (string) {

    if (!parse_string (string, win)) {
      show_help();
      return 0;
    }
  
    g_free (string);
    
    if (debug) {
      printf ("wm_res_class: %s\n", win->wm_res_class);
      printf ("wm_res_name: %s\n", win->wm_res_name);
      printf ("wm_name: %s\n", win->wm_name);
    }
  
  }
  
  
  win->parent_window= gtk_window_new (GTK_WINDOW_TOPLEVEL);
  win->sock = gtk_socket_new ();
  gtk_container_add (GTK_CONTAINER (win->parent_window), win->sock);

  root_window = gdk_screen_get_root_window (gdk_screen_get_default());

  win->manager_window=get_manager_window();

  if (win->manager_window == None) {
   
     if (debug) printf ("no manager window !!!\n");
          
     gdk_window_add_filter(root_window, root_filter_manager_window, (gpointer) win);
     gtk_main ();
     gdk_window_remove_filter(root_window, root_filter_manager_window, (gpointer) win);
   }
 
 else { if (debug) printf ("HAVE MANAGER WINDOW\n");};
 
  gdk_window_set_events(root_window, GDK_SUBSTRUCTURE_MASK);
  gdk_window_add_filter(root_window, root_filter_map, (gpointer) win); 
  
  if (debug) printf ("execute program\n");
  if (!(win->pid=exec_command (command))) {
  
    if (debug) printf ("execute failed\n");
    return 0;
  }

  if (debug)
    printf ("win->pid: %d\n", win->pid);
      

  g_free(command);

  //g_child_watch_add (win->pid, pid_watch, (gpointer) NULL);

  if (debug) printf ("wait for window\n");
  gtk_main();

  if (debug) printf ("found window: %d\n", win->xlib);
  
  gdk_window_remove_filter(root_window, root_filter_map, (gpointer) win);
  
  if (win->wm_res_class)
    g_free(win->wm_res_class);
  if (win->wm_res_name)
    g_free(win->wm_res_name);
  if (win->wm_name)
    g_free(win->wm_name);
   
  gdk_window_set_events(root_window,GDK_SUBSTRUCTURE_MASK );
  gdk_window_add_filter(root_window, root_filter_workspace, (gpointer) win); 
  
  
  if (debug) printf ("wait vor viewable\n");
  
  while (!xlib_window_is_viewable(win->xlib))
    {usleep (100000);} 
  
  if (debug) printf ("viewable\n");

  win->gdk=gdk_window_foreign_new(win->xlib);
    
  gdk_window_stick(win->gdk);
  gtk_window_stick(GTK_WINDOW(win->parent_window));
  gdk_window_set_skip_taskbar_hint (win->gdk, TRUE);
  gdk_window_set_skip_pager_hint (win->gdk, TRUE);
  
  while (window_has_deco (win->gdk)) {
    gdk_window_set_decorations(win->gdk, 0);
    usleep (10000);
  }

  if (debug) printf ("vor geometry\n");
  
  gdk_window_get_geometry (win->gdk, 
      &child_window_x, &child_window_y, &child_window_w, &child_window_h, NULL);
  gtk_window_set_default_size(GTK_WINDOW (win->parent_window), 
     child_window_w, child_window_h);

  if (debug) printf ("nach  geometry und size\n");
  
  win->xlib_our=one_under_root (GDK_DISPLAY(), win->xlib);
 
  if (debug) printf ("win->xlib_our: %d\n", win->xlib_our);  
    
  win->gdk_our=gdk_window_foreign_new(win->xlib_our);


  gtk_socket_add_id (GTK_SOCKET (win->sock),  win->xlib_our);

  win->icon= get_window_icon (win->xlib);
  
  if (win->icon)
    gtk_window_set_icon (GTK_WINDOW (win->parent_window), win->icon);
  
  tray_init (win);
  
  gtk_widget_show (win->sock);
    
  if (show)
    gtk_widget_show (win->parent_window);

  
   if (debug) printf ("reparent necessary ?\n");
         
  /*reparent again if something goes wrong*/  
  while (not_reparent (win->xlib_our)) {
    usleep (10000);  
    gtk_socket_add_id (GTK_SOCKET (win->sock),  win->xlib_our);
  }
  
  gdk_window_set_events(win->gdk_our, GDK_SUBSTRUCTURE_MASK);
  gdk_window_add_filter(win->gdk_our, child_window_filter_our, (gpointer) win); 


  gdk_window_set_events(win->gdk, GDK_SUBSTRUCTURE_MASK);
  gdk_window_add_filter(win->gdk, child_window_filter, (gpointer) win); 

  
  g_signal_connect ((gpointer) win->parent_window, "delete_event",
                    G_CALLBACK (on_parent_window_delete_event),
                    (gpointer) win);

  g_signal_connect ((gpointer) win->parent_window, "focus_in_event",
                    G_CALLBACK (on_parent_window_focus_in_event),
                    (gpointer) win);

  g_signal_connect ((gpointer) win->parent_window, "enter_notify_event",
                    G_CALLBACK (on_parent_window_enter_notify_event),
                    (gpointer) win);

  g_signal_connect ((gpointer) win->parent_window, "configure_event",
                    G_CALLBACK (on_parent_window_configure_event),
                    (gpointer) win);

  update_window_title(win);

  if (show) 
    XSetInputFocus (GDK_DISPLAY(), win->xlib, RevertToPointerRoot, CurrentTime);
  
 
  gtk_main ();
  return 0;
}
