/*
 * child.c: ???
 * Copyright (c) 2004--2006 Jochen Baier <email@Jochen-Baier.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License Version
 * 3, as published by the Free Software Foundation.  See the
 * COPYING.GPL3 file included with this distribution for information
 * on the license terms which apply to this file.
 *
 * TODO:  Need to check sources for the ability to relicense under gpl3.
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
 */

#include "config.h"
#include "common.h"
#include "utils.h"
#include "trayicon.h"
#include "clientwin.h"

#define PRELOAD_LIB "/liballtray.so.0.0.0"

gboolean window_match (Window window, win_struct *win)
{
  
  gboolean match=FALSE;
  
  XClassHint class_hints;
  class_hints.res_name = NULL;
  class_hints.res_class = NULL;

  gint result=0;
  gint err=0;
  gchar *res_name_down=NULL;
  gchar *command_down=NULL;
  
  gint pid, pgid;
  
  XWindowAttributes attr;
  
  if (debug)  {
    printf ("filter mapped window: ");
    display_window_id (GDK_DISPLAY(), window);
  }
    
  /*filter code is from xfwm4*/
  do {
    
    if (window == None) {
      if (debug) printf ("window == None -> skip\n");
      break;
    }

    gdk_error_trap_push();
    result=XGetWindowAttributes (GDK_DISPLAY(), window, &attr);
    
    if (gdk_error_trap_pop() || result == 0) {
      if (debug) printf ("Cannot get window attributes -> skip");
      break;
    }

    if (attr.override_redirect) {
      if (debug) printf ("override redirect -> skip\n");
      break;
    }
    
    //xmms
    if (attr.width <=10 || attr.height <=10) {
      if (debug) printf ("window too small -> skip\n");
      break;
    }

    //splash windows...
    if (!window_type_is_normal(window)) {
      if (debug) printf ("window typ is not normal -> skip\n!");
      break;
    }

    if (debug) printf ("window got through first filter\n");
    
    pid=get_pid (window);
    
    if (pid !=0) {
    
      if (debug) printf("PID found: yes\n");
      
      if (pid == win->child_pid) {
        if (debug) printf ("PID match: yes\n");
        match=TRUE;
        break;
      }

      if (debug) printf ("PID match: no\n");
      
      pgid=(gint) getpgid((pid_t) pid);
      
      if (debug) printf ("process group id: %d\n", pgid);
      
      if (pgid == win->parent_pid) {
        if (debug) printf ("process id match: yes\n");
        match=TRUE;
        break;
      }
      
      if (debug) printf ("process id match: no\n");
    
    }
     
    if (debug) printf ("PID found: no\n");
     
    gdk_error_trap_push();
    result=XGetClassHint(GDK_DISPLAY(), window, &class_hints);
    err=gdk_error_trap_pop();
                    
    if (err || result==0 || class_hints.res_name == NULL) {
      if (debug) printf ("ERROR get class hints\n"); 
      break;
    }
        
    if (debug) printf ("found strings: res_class: %s  res_name %s   \n",
      class_hints.res_class, class_hints.res_name);
      
    res_name_down= g_ascii_strdown (class_hints.res_name, 
      strlen (class_hints.res_name) *sizeof (gchar));
        
    command_down= g_ascii_strdown (win->command_only,
      strlen (win->command) *sizeof (gchar));
       
    if (debug) printf ("res name down: %s\n", res_name_down);
    if (debug) printf ("command down: %s\n", command_down);

    if (g_strstr_len(res_name_down, 
      strlen (res_name_down) * sizeof (gchar), command_down)){
    
      if (debug) printf ("wm_class match: yes\n");
      
      match=TRUE;
      break;
    
    }
    
  } while (0);
  
 
  if (class_hints.res_class)
    XFree (class_hints.res_class);
  if (class_hints.res_name)
    XFree (class_hints.res_name);
  if (res_name_down)
   g_free (res_name_down);
  if (command_down)
   g_free (command_down);
  
  return match;
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
    window=ClientWindow(xmap->window);
   
    //display_window_id (GDK_DISPLAY(), window);
       
    if (window != None) {
      if (window_match (window, win)) {
        win->child_xlib=window;
        win->normal_map=TRUE;
        gtk_main_quit ();
      }
    }   
 
  }
  
  return GDK_FILTER_CONTINUE;
}

static GdkFilterReturn
liballtraynomap_filter(GdkXEvent *xevent, GdkEvent *event, gpointer user_data)
{
  
  XEvent *xev = (XEvent *)xevent;
  win_struct *win= (win_struct *) user_data;
  int result;
  XClassHint class_hints;
    
  if (debug) printf ("liballtraynomap_filter event: %s\n", event_names[xev->xany.type]);
    
  if (xev->xany.type == ClientMessage &&
      xev->xclient.message_type == alltray_found_window) {
        
    if (debug) printf ("got window from lib: %d\n", (gint) xev->xclient.data.l[0]);
        
        
    if (win->xmms) {
    
      Window window=xev->xclient.data.l[0];
      
      gdk_error_trap_push();
      result=XGetClassHint(GDK_DISPLAY(), window, &class_hints);
      gint err=gdk_error_trap_pop();
      
      if (err || result==0 || class_hints.res_name == NULL) {
        if (debug) printf ("ERROR get class hints\n"); 
        exit (1);
      }
      
      if (debug) printf ("found strings: res_class: %s  res_name %s   \n",
          class_hints.res_class, class_hints.res_name);
      
      do {
      
        if (!strcmp(class_hints.res_name,"XMMS_Player")) {
          win->xmms_main_window_xlib=window;
          gtk_main_quit();
          //break;
        }
        
        if (!strcmp(class_hints.res_name,"XMMS_Playlist")) {
          win->xmms_playlist_window_xlib=window;
          break;
        }
        
        if (!strcmp(class_hints.res_name,"XMMS_Equalizer")) {
          win->xmms_equalizer_window_xlib=window;
          break;
        }
      
      } while (0);
      
      
      XFree (class_hints.res_class);
      XFree (class_hints.res_name);
    } else {
      win->child_xlib=(gint) xev->xclient.data.l[0];
      gtk_main_quit();
    }
    
    
  }

  return GDK_FILTER_CONTINUE;
}

void set_env_stuff (gpointer user_data)
{

  gchar *path_to_lib=NULL;
  gchar *preload_string=NULL;
  gchar *old_preload=NULL;
  gchar *spy_id_string=NULL;

  
  win_struct *win= (win_struct*) user_data;
 
  if (debug) {
    path_to_lib=g_strdup ("/usr/lib");
  } else {
    #ifdef ENABLE_BINRELOC
    if (debug) printf ("binreloc is working\n");
    path_to_lib = gbr_find_lib_dir (NULL);
    if (path_to_lib ==NULL) {
      g_warning ("probs with binreloc. i will use /usr/lib/liballtray.so.0.0.0 just for fun.");
      path_to_lib=g_strdup ("/usr/lib");
    }
    #else
    if (debug) printf ("binreloc is not working\n");
    path_to_lib=g_strdup (PACKAGE_LIB_DIR);
    #endif
  }
 

  if (debug) printf ("lib is here: %s\n", path_to_lib);
  
  old_preload = getenv("LD_PRELOAD");

  if (debug) printf ("old preload: %s\n", old_preload);

  if (old_preload && *old_preload != '\0' ) {
  
    if (debug) printf ("have old preload\n");
    
    preload_string = g_strconcat ("LD_PRELOAD=",
       old_preload, " ", path_to_lib, PRELOAD_LIB, NULL);
    
    setenv ("OLD_PRELOAD", old_preload, 1);
    
  } else {
    preload_string = g_strconcat ("LD_PRELOAD=",
        path_to_lib, PRELOAD_LIB, NULL);
  }

  if (debug) printf ("preload string: %s\n", preload_string);
  putenv(preload_string);

  
  if (win->libspy_window) {
    spy_id_string = g_strdup_printf ("%d", (int) win->libspy_window);
    setenv ("ALLTRAY_SPY_WINDOW", spy_id_string, 1);
  }
  
  if (win->xmms) {
    setenv ("ALLTRAY_XMMS", "YES",1);
  }

  g_free (path_to_lib);

}

GPid exec_child (win_struct *win)
{

  gchar **child_vector=NULL;
  gint num;
  GError *error=NULL;
  GPid pid=0;
  gboolean success_parse;
  gboolean success_spawn;

  success_parse=g_shell_parse_argv (win->command, &num, 
    &child_vector, NULL);
    
  if (success_parse) {
    success_spawn=g_spawn_async (NULL,child_vector, NULL, G_SPAWN_SEARCH_PATH,
    set_env_stuff, (gpointer) win, &pid, &error);
  
    if (!success_spawn) {
      printf ("AllTray: %s\n", error->message);
    }
    
    g_strfreev (child_vector);
  }
   
  return pid;

}

void exec_and_wait_for_window(win_struct *win)
{
  
  win->libspy_window= XCreateSimpleWindow(GDK_DISPLAY(), GDK_ROOT_WINDOW(), 0, 0, 
      1, 1, 0, 0, 0);

  if (debug) printf ("win->libsyp_window id: %d\n", (int) win->libspy_window);
              
  win->libspy_window_gdk=gdk_window_foreign_new (win->libspy_window);

  gdk_window_add_filter(win->libspy_window_gdk, liballtraynomap_filter,
    (gpointer) win); 
   
    
  gdk_window_set_events(win->root_gdk, GDK_SUBSTRUCTURE_MASK);
  gdk_window_add_filter(win->root_gdk, root_filter_map, (gpointer) win);
    
  /*aterm is too fast*/
  gtk_sleep (100);
    
  if (debug) printf ("execute program: %s\n", win->command);
  if (!(win->child_pid=exec_child (win))) {
  
    if (debug) printf ("execute failed\n");
      
    if (win->user_icon_path)
      g_free(win->user_icon_path);
    
    if (win->command)
      g_free (win->command);
    
    g_free (win->command_only);
        
    if (win->command_menu)
      free_command_menu (win->command_menu);
    
    if (win->user_icon)
      g_object_unref (win->user_icon);

    gdk_window_remove_filter(win->libspy_window_gdk, 
      liballtraynomap_filter, (gpointer) NULL);
    XDestroyWindow (GDK_DISPLAY(), win->libspy_window);

    
    g_free(win);
    

    exit (0);
    
  }

  if (debug) printf ("wait for window\n");
  gtk_main();
  
  gdk_window_remove_filter(win->root_gdk, root_filter_map, (gpointer) win);

  if (!win->xmms) {
    gdk_window_remove_filter(win->libspy_window_gdk, 
      liballtraynomap_filter, (gpointer) NULL);
    XDestroyWindow (GDK_DISPLAY(), win->libspy_window);
  }

  if (debug) {
    
     if (!win->xmms)
      printf ("found child window: %d\n", (int) win->child_xlib);
    else
      printf ("found xmms main window: %d\n", (int) win->xmms_main_window_xlib);
    
    printf ("child have pid: %d\n", (int) win->child_pid);
  }

}

gboolean withdrawn (Window window)
{
  unsigned char *data;
  unsigned long nitems;
  unsigned long leftover;
  Atom actual_type;
  int actual_format;
  int status;
  gint err;
  
  gboolean return_value=FALSE;
  
  gdk_error_trap_push();
    
  status = XGetWindowProperty (GDK_DISPLAY(), window,
    wm_state, 0L, 1, False, wm_state, &actual_type, &actual_format,
    &nitems, &leftover, &data);
  
  err=gdk_error_trap_pop();
  
  if (err!=0 || status != Success)
    return FALSE;
  
  if ((actual_type == wm_state) && (nitems == 1) && data) {
  
    gint state = *(gint *) data;
    if (state == WithdrawnState)
      return_value=TRUE;
    XFree (data);
    return return_value;
  } 

  if (actual_type == None)
    return TRUE;
  
  return FALSE;
}

GdkFilterReturn child_window_filter_wm_state(GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data)
{
  XEvent *xev = (XEvent *)xevent;
   
  win_struct *win= (win_struct*) user_data;
 
 
  switch (xev->xany.type) {
  
    case PropertyNotify:
    { 
        XPropertyEvent *xproperty = (XPropertyEvent *) xev;
        
        if (debug) printf ("property notify\n");
 
        if (xproperty->atom == wm_state) {
          if (debug) printf ("wm state changed\n");
          
           if (withdrawn (win->child_xlib))
            gtk_main_quit ();
        }
 
    }
    break;
  
  }
  
  return GDK_FILTER_CONTINUE;

}

void withdraw_window(win_struct *win)
{

  if (withdrawn (win->child_xlib))
    return;
 
  gdk_window_set_events(win->child_gdk,GDK_STRUCTURE_MASK);
  gdk_window_add_filter(win->child_gdk, child_window_filter_wm_state, (gpointer) win);   
  
  XWithdrawWindow (GDK_DISPLAY (), win->child_xlib, XDefaultScreen (GDK_DISPLAY()));
  XSync (GDK_DISPLAY(), False);
 
  if (debug) printf ("wait for withdrawn\n");
  gtk_main();
  gdk_window_remove_filter(win->child_gdk, child_window_filter_wm_state, (gpointer) win);   
  if (debug) printf ("withdrawn --> ok\n");
  
}

void get_child_size (GdkWindow *child_gdk, 
    gint *w_return, gint *h_return)
{
  
  gint w=0, h=0, last_w=-1, last_h=-1;

  do {
  
    if (debug) printf ("child size loop\n");
    
    last_w=w;
    last_h=h;
    
    gdk_window_get_geometry (child_gdk, 
        NULL, NULL, &w, &h, NULL);
    
    if (last_w == w && last_h == h)
      break;
    
    gtk_sleep (300);
  
  }while (1);

  *w_return=w;
  *h_return=h;
}

GdkFilterReturn child_window_filter (GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data)
{
  XEvent *xev = (XEvent *)xevent;
   
  win_struct *win= (win_struct*) user_data;
  
  //if (debug) printf ("child window our event: %s\n", event_names[xev->xany.type]);
  
  switch (xev->xany.type) {
    
    case DestroyNotify:
      if (debug) printf ("destroy notify\n");
        
      if (!assert_window (win->child_xlib))
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
        update_tray_icon(win);
      } 
    
    }
    break;
    
    case ConfigureNotify:
    {
      XConfigureEvent *xconfigure = (XConfigureEvent*) xev;
      
      if (debug) printf ("child configure event: x: %d, y: %d\n",
        xconfigure->x, xconfigure->y);
      
      if (xconfigure->x >=1 || xconfigure->y >=1) {
        
        /*deny move inside parent in the future*/
        XSelectInput (GDK_DISPLAY(),win->parent_xlib,
          SubstructureRedirectMask | StructureNotifyMask |
          VisibilityChangeMask);
        
        gdk_window_move (win->child_gdk, 0, 0);
      }
      

    }
    break;
            
  
  }
  
  return GDK_FILTER_CONTINUE;
}
