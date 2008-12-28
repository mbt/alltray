#include "common.h"

#ifndef __GNOME_H__
#define __GNOME_H__

void grab_filter_init (win_struct *win);
GdkFilterReturn motion_filter_gnome (GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data);
GdkFilterReturn motion_filter_xmms (GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data);
GdkFilterReturn target_filter (GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data);


#endif
