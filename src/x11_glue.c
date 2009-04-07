/*
 * x11_glue.c - X11 "glue" code for AllTray
 * Copyright (c) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Fondation
 */
#include <glib.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <libwnck/libwnck.h>
#include "x11_glue.h"

static gboolean wm_state_set (Window window);

// Nearly directly from wnckprop.c in libwnck.
WnckWindow *
alltray_find_managed_window(Window window, GdkDisplay *gdk_display) {
  Window root;
  Window parent;
  Window *kids = NULL;
  WnckWindow *retval;
  guint nkids;
  int result;

  if(wm_state_set(window)) {
    return(wnck_window_get(window));
  }

  gdk_error_trap_push();
  result = XQueryTree(gdk_display, window, &root, &parent, &kids, &nkids);
  if(gdk_error_trap_pop() || !result) {
    return NULL;
  }

  retval = NULL;
  for(int i = 0; i < nkids; i++) {
    if(wm_state_set(kids[i])) {
      retval = wnck_window_get(kids[i]);
      break;
    }

    retval = alltray_find_managed_window(kids[i]);
    if(retval != NULL) break;
  }

  if(kids) XFree(kids);
  return(retval);
}

// Same as last function.
static gboolean
wm_state_set (Window window)
{
  Atom    wm_state;
  gulong  nitems;
  gulong  bytes_after;
  gulong *prop;
  Atom    ret_type = None;
  int     ret_format;
  int     err, result;

  wm_state = gdk_x11_get_xatom_by_name ("WM_STATE");

  gdk_error_trap_push ();
  result = XGetWindowProperty (gdk_display,
                               window,
                               wm_state,
                               0, G_MAXLONG,
                               False, wm_state, &ret_type, &ret_format, &nitems,
                               &bytes_after, (gpointer) &prop);
  err = gdk_error_trap_pop ();
  if (err != Success ||
      result != Success)
    return FALSE;

  XFree (prop);

  if (ret_type != wm_state)
    return FALSE;

  return TRUE;
}

/// UNUSED?
static Atom _net_wm_pid;
static gulong find_window(GPid pid, Display *d, Window w);

gulong
alltray_get_x11_window_from_pid(GPid pid, Display *d) {
  Window root_win = XDefaultRootWindow(d);
  _net_wm_pid = XInternAtom(d, "_NET_WM_PID", True);

  if(_net_wm_pid == None) {
    // Error: this atom isn't defined?!  What are they _doing_?
    g_critical("AllTray can't find _NET_WM_PID!");
    return(0);
  }

  return(find_window(pid, d, root_win));
}

static gulong
get_window_leader(Display *d, Window w) {
  Atom type;
  int format;
  int status;
  gulong nItems;
  gulong bytesAfter;
  guchar *prop_ret = NULL;
  Atom wm_client_leader = XInternAtom(d, "WM_CLIENT_LEADER", True);
  gulong retval = 0;

  status =
    XGetWindowProperty(d, w, wm_client_leader, 0, 1, False, XA_WINDOW,
		       &type, &format, &nItems, &bytesAfter, &prop_ret);

  if(status == Success) {
    if(prop_ret != 0) {
      retval = *((unsigned long *)prop_ret);
      return(retval);
    }
  } else {
    return(0);
  }
}

static gulong
find_window(GPid pid, Display *d, Window w) {
  Atom type;
  int format;
  int status;
  gulong nItems;
  gulong bytesAfter;
  guchar *prop_ret = NULL;
  Atom _net_wm_pid = XInternAtom(d, "_NET_WM_PID", True);

  status =
    XGetWindowProperty(d, w, _net_wm_pid, 0, 1, False, XA_CARDINAL,
		       &type, &format, &nItems, &bytesAfter, &prop_ret);

  if(status == Success) {
    if(prop_ret != 0) {
      GPid returned_pid = *((unsigned long *)prop_ret);
      if(returned_pid == pid) return(w);
    }
  } else {
    return(0);
  }

  /*
   * If we are still here, we need to continue searching...
   */
  Window root, parent;
  Window *child;
  guint child_count;

  if(XQueryTree(d, w, &root, &parent, &child, &child_count) != 0) {
    for(guint i = 0; i < child_count; i++) {
      guint retval = find_window(pid, d, child[i]);
      if(retval != 0) {
	retval = get_window_leader(d, retval);
	return(get_window_leader(d, retval));
      }
    }
  }

  return(0);
}
