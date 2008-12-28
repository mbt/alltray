

#ifndef __SYSTEM_TRAY_H__
#define __SYSTEM_TRAY_H__

#include "gtray.h"
void tray_init(win_struct *win);
void tray_done (win_struct *win);
void tray_update_icon(win_struct *win, GdkPixbuf *icon);
void tray_update_tooltip (win_struct *win);
Window get_manager_window (void);

GdkFilterReturn
  manager_filter (GdkXEvent *xevent, GdkEvent *event, gpointer user_data);
void create_tray_and_dock (win_struct *win);


#endif
