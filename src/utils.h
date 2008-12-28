#include "common.h"

#ifndef __UTILS_H__
#define __UTILS_H__


extern char *event_names[];


void show_pixbuf (GdkPixbuf *buf);
void display_window_id(Display *display, Window window);
void atom_init (void);
void gtk_sleep (gint sec);
void update_window_icon(win_struct *win);
GdkPixbuf *get_user_icon (gchar *path, gint width, gint height);
GdkPixbuf *get_window_icon (Window xwindow, gint width, gint height);
void update_window_title(win_struct *win);
GPid exec_command (gchar *command);
GdkPixbuf *get_window_icon (Window xwindow, gint width, gint height);
gboolean xlib_window_is_viewable (Window w);
gboolean assert_window (Window window);
Window get_active_window (void);
Window one_under_root (Display *display, Window window);
void get_window_position (Window window, gint *x, gint *y);
void skip_taskbar (win_struct *win, gboolean add);
//void skip_pager (Window window);
gint get_current_desktop(void);
gboolean get_window_list (Window   xwindow, Atom atom,
   Window **windows, int *len);
gint get_pid (Window w);
gboolean window_type_is_normal (Window win);
void sticky (Window window);
void destroy_all_and_exit (win_struct *win,
  gboolean kill_child);
void show_hide_window (win_struct *win, gint force_state,
  gboolean keep_in_taskbar);
gboolean search_gnome_panel (void);
void xprop (Window window);
gchar  *get_window_manager(void);
void free_command_menu (GArray *command_menu);
void geo_move (GdkWindow *window, gint screen_width, gint screen_height, 
  gint x, gint y, gint w, gint h, int mask);

#endif
