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

#ifndef __UTILS_H__
#define __UTILS_H__


extern char *event_names[];


void show_pixbuf (GdkPixbuf *buf);
void display_window_id(Display *display, Window window);
void atom_init (void);
void gtk_sleep (gint sec);
gboolean withdrawn (Window window);
gboolean parse_arguments(int argc, char **argv, gchar **icon,
    gchar  **rest, gint *show, gboolean *hide_start);
gboolean window_match (Window window, win_struct *win);
void update_window_icon(win_struct *win);
void update_window_title(win_struct *win);
GPid exec_command (gchar *command);
void show_help(void);
void show_version(void);
GdkPixbuf *get_window_icon (Window xwindow);
gboolean xlib_window_is_viewable (Window w);
gboolean not_reparent (Window window);
gboolean assert_window (Window window);
Window get_active_window (void);
gboolean window_has_deco (GdkWindow *win);
void  destroy_all_and_exit (win_struct *win, gboolean kill_child);
Window one_under_root (Display *display, Window window);
void get_window_position (Window window, gint *x, gint *y);
void show_hide_window (win_struct *win, gint force_state,
  gboolean keep_in_taskbar);
void skip_taskbar (win_struct *win, gboolean add);
void set_current_desktop (gint *var);
gint get_number_of_desktops(void);
void update_visibility_state (win_struct *win, gboolean new_state);
void update_taskbar_state (win_struct *win, gboolean new_state);

#endif
