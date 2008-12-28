#ifndef __COMMON_H__
#define __COMMON_H__

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
#include <stdlib.h>

pid_t getpgid(pid_t pid);

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
Atom net_wm_name;
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
Atom net_wm_window_type;
Atom net_wm_window_type_normal;
Atom selection_atom;
Atom manager_atom;
Atom system_tray_opcode_atom;
Atom net_client_list_stacking;
Atom net_client_list;
Atom gdk_timestamp_prop;
Atom utf8_string;
Atom net_wm_visible_name;

typedef struct _balloon_struct {
  
  GtkWidget *window;
  GdkGC *gc;
  
  gint x;
  gint y;
    
  gint w;
  gint h;
  
  gboolean tail_on_top;
  gint dock_x;
  gint dock_y;
  gint dock_w;
  gint dock_h;
  
  gint tail_x;
  
  gulong handler_id;
  gint timeout;
  
  guint start_delay;
  
} balloon_struct;


typedef struct _win_struct {

  Display *display;

  gint screen_height;
  gint screen_width;

  GdkWindow *parent_gdk;
  Window parent_xlib;
  
  gboolean parent_is_visible;

  gboolean large_icons;
    
  gboolean borderless;
  
  GdkWindow *root_gdk;
  Window root_xlib;

  gint parent_window_x;
  gint parent_window_y;
  
  gint initial_x;
  gint initial_y;
  gint initial_w;
  gint initial_h;

  int visibility;

  GArray *workspace;
  GArray *command_menu;
      
  gboolean show;
  gchar *command;
  gchar *command_only;
  
  Window child_xlib;
  
  GdkWindow *child_gdk;
 
  gint parent_pid; 
  gint child_pid;
  
  
  Window libspy_window;
  GdkWindow *libspy_window_gdk;
  
  GdkPixbuf *window_icon;
  GdkPixbuf *tray_icon;
  
  GdkPixbuf *user_icon;
  gchar *user_icon_path;
      
  Window manager_window;
  GdkWindow *manager_window_gdk;
  
  GtkWidget *plug;
  Window plug_xlib;

  GtkWidget *fixed;
  GtkWidget *image_icon;
  
  balloon_struct *balloon;
  
  gboolean gtk_tray;
    
  gchar *title;
    
  gint desktop;
  
  gboolean gnome_panel_found;
  
  gboolean xmms;
  Window xmms_main_window_xlib;
  GdkWindow *xmms_main_window_gdk;
  
  
  Window xmms_playlist_window_xlib;
  Window xmms_equalizer_window_xlib;
  
  gint title_time;
  gboolean balloon_message_allowed;
  
  gboolean click_mode;

} win_struct;

typedef struct _wm_state_struct {
  
  gboolean visible;
  gboolean show_in_taskbar;
} wm_state_struct;

typedef struct _command_menu_struct {
  gchar *entry;
  gchar *command;
} command_menu_struct;

GdkFilterReturn parent_window_filter (GdkXEvent *xevent, GdkEvent *event, gpointer user_data);
GdkFilterReturn child_window_filter (GdkXEvent *xevent, GdkEvent *event, gpointer user_data);
GdkFilterReturn root_filter_workspace (GdkXEvent *xevent, GdkEvent *event, gpointer user_data);

extern gboolean debug;

#endif
