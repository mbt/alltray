#include "common.h"

#ifndef __XMMS_H__
#define __XMMS_H__

void xmms_filter_init (win_struct *win);
void deiconify_xmms_windows (win_struct *win);
GdkFilterReturn xmms_main_window_filter (GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data);
GdkPixbuf *get_xmms_icon (gint width, gint height);

#endif
