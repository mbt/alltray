#include "common.h"

#ifndef __GRAB_H__
#define __GRAB_H__

void grab_done (void);
void grab_init (win_struct *win);

GdkFilterReturn motion_filter_gnome (GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data);
GdkFilterReturn motion_filter_xmms (GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data);
GdkFilterReturn target_filter (GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data);


#endif
