#include "common.h"

#ifndef __CHILD_H__
#define __CHILD_H__


void exec_and_wait_for_window(win_struct *win);
void withdraw_window(win_struct *win);
void get_child_size (GdkWindow *child_gdk, 
    gint *w_return, gint *h_return);
    
GdkFilterReturn child_window_filter (GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data);

#endif
