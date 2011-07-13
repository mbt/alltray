/*
 * alltray-ctt-helper.h - Helper public interface.
 * Copyright © 2011 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation.
 */
#ifndef __ALLTRAY_CTT_HELPER_H_INCLUDED__
#define __ALLTRAY_CTT_HELPER_H_INCLUDED__

#include <X11/Xlib.h>

Window ctt_make_window(Display *dpy, Window parent);

#endif /* __ALLTRAY_CTT_HELPER_H_INCLUDED__ */
