#include "common.h"

#ifndef __UTILS_H__
#define __UTILS_H__


extern char *event_names[];


void show_pixbuf (GdkPixbuf *buf);
void display_window_id(Display *display, Window window);
void atom_init (void);
void gtk_sleep (gint sec);
gboolean withdrawn (Window window);
gboolean parse_arguments(int argc, char **argv, gchar **icon,
    gchar  **rest, gint *show, gboolean *hide_start,
    gboolean *debug, gboolean *borderless,
    gboolean *large_icons, GArray *command_menu);
gchar *strip_command (win_struct *win);
gboolean window_match (Window window, win_struct *win);
void update_window_icon(win_struct *win);
GdkPixbuf *get_user_icon (gchar *path, gint width, gint height);
GdkPixbuf *get_window_icon (Window xwindow, gint width, gint height);
void update_window_title(win_struct *win);
GPid exec_command (gchar *command);
void show_help(void);
void show_version(void);
GdkPixbuf *get_window_icon (Window xwindow, gint width, gint height);
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
gint get_current_desktop(void);
gint get_number_of_desktops(void);
void update_visibility_state (win_struct *win, gboolean new_state);
void update_taskbar_state (win_struct *win, gboolean new_state);

#endif
