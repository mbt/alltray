/*
 * alltray-ctt-windowlist.h - Window list header
 * Copyright © 2011 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation
 */
#ifndef __ALLTRAY_CTT_WINDOWLIST_H_INCLUDED__
#define __ALLTRAY_CTT_WINDOWLIST_H_INCLUDED__

bool alltray_ctt_windowlist_add(Display *dpy, Window ctt, Window parent);
bool alltray_ctt_windowlist_del(Window parent);
int alltray_ctt_windowlist_len();

#endif /* __ALLTRAY_CTT_WINDOWLIST_H_INCLUDED__ */
