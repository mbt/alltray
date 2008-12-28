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

#ifndef __GTRAY_H__
#define __GTRAY_H__

#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <gdk/gdk.h>
#include <gdk-pixbuf-xlib/gdk-pixbuf-xlib.h>
#include <stdlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/WinUtil.h>
#include <string.h>
#include <X11/cursorfont.h>
#include <unistd.h>

#define _NET_WM_STATE_REMOVE        0    /* remove/unset property */
#define _NET_WM_STATE_ADD           1    /* add/set property */
#define _NET_WM_STATE_TOGGLE        2    /* toggle property  */  


#define WithdrawnState 0
#define NormalState 1
#define IconicState 3

#define force_show 1
#define force_hide 0
#define force_disabled -1

#define window_is_visible 1
#define window_is_hidden 0

Atom wm_name_atom;
Atom wm_icon_atom;
Atom net_wm_icon;
Atom net_close_window;
Atom wm_delete_window;
Atom wm_take_focus;
Atom net_wm_ping;
Atom net_wm_pid;
Atom net_number_of_desktops;
Atom net_current_desktop;
Atom net_wm_state_skip_pager;
Atom net_wm_state_skip_taskbar;
Atom net_wm_state;
Atom wm_state;
Atom net_wm_state_sticky;
Atom net_wm_desktop;
Atom net_active_window;
Atom net_client_list;
Atom net_wm_window_type;
Atom net_wm_window_type_normal;
Atom selection_atom;
Atom manager_atom;
Atom system_tray_opcode_atom;

Atom gdk_timestamp_prop;

typedef struct _win_struct {

  Display *display;

  gint screen_height;
  gint screen_width;

  GdkWindow *parent_gdk;
  Window parent_xlib;
  
  gboolean parent_is_visible;
  
  gboolean hide_start;
  GdkWindow *fake_desktop;

  GdkWindow *root_gdk;
  Window root_xlib;

  gint parent_window_x;
  gint parent_window_y;

  int visibility;

  GArray *workspace;
  
  gboolean show;
  gchar *command;

  Window child_xlib;
  
  GdkWindow *child_gdk;
 
  GPid pid;

  GdkPixbuf *icon;
  GdkPixbuf *user_icon;
    
  Window manager_window;
  GdkWindow *manager_window_gdk;
  
  GtkWidget *plug;
  Window plug_xlib;

  GtkWidget *fixed;
  GtkWidget *image_icon;
  GtkTooltips *tooltip;
  
  gboolean gtk_tray;
    
  gchar *title;
    
  gint desktop;
    
} win_struct;

typedef struct _wm_state_struct {
  
  gboolean visible;
  gboolean show_in_taskbar;
} wm_state_struct;


GdkFilterReturn parent_window_filter (GdkXEvent *xevent, GdkEvent *event, gpointer user_data);
GdkFilterReturn child_window_filter (GdkXEvent *xevent, GdkEvent *event, gpointer user_data);
GdkFilterReturn root_filter_workspace (GdkXEvent *xevent, GdkEvent *event, gpointer user_data);

extern gboolean debug;

#endif
