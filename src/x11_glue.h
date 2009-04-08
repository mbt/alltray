/*
 * x11_glue.h
 */
#ifndef __x11_glue_included__
#define __x11_glue_included__
#include <glib.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <libwnck/libwnck.h>

gulong alltray_get_x11_window_from_pid(GPid pid, Display *d);
WnckWindow *alltray_find_managed_window(Window window, GdkDisplay *gdk_display);

#endif /* __x11_glue_included__ */
