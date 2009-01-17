/* -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *
 * alltray.h - Project-wide include file.
 * Copyright © 2009 Michael B. Trausch <mike@trausch.us>
 */
#ifndef __ALLTRAY_H_INCLUDED__
#define __ALLTRAY_H_INCLUDED__

#include <glib.h>
#include <config.h>

// Exit codes.
const int ALLTRAY_EXIT_SUCCESS = 0;
const int ALLTRAY_EXIT_INVALID_ARGS = 1;

// Prototypes for cmdline.c
void cmdline_parse(int *argc, char *argv[]);

// Prototypes for debug.c
void alltray_debug_init();
gboolean alltray_debug_enabled(gint desired_flag);

// Debug constants and macros
const int ALLTRAY_DEBUG_NONE = 0x0000;
const int ALLTRAY_DEBUG_CMDLINE = 0x0001;
const int ALLTRAY_DEBUG_X11 = 0x0002;
const int ALLTRAY_DEBUG_TRAY = 0x0004;
const int ALLTRAY_DEBUG_MISC = 0x0008;

const int ALLTRAY_DEBUG_ALL =
  (ALLTRAY_DEBUG_CMDLINE | ALLTRAY_DEBUG_X11 | ALLTRAY_DEBUG_TRAY |
   ALLTRAY_DEBUG_MISC);

#ifndef ALLTRAY_DISABLE_DEBUG

#define DEBUG_CMDLINE(msg) \
  if(alltray_debug_enabled(ALLTRAY_DEBUG_CMDLINE)) { \
    g_print("[Debug:CMDLINE] (%s:%i) %s\n", __FILE__, __LINE__, msg); \
  }

#define DEBUG_X11(msg) \
  if(alltray_debug_enabled(ALLTRAY_DEBUG_X11)) { \
    g_print("[Debug:X11] (%s:%i) %s\n", __FILE__, __LINE__, msg); \
  }

#define DEBUG_TRAY(msg) \
  if(alltray_debug_enabled(ALLTRAY_DEBUG_TRAY)) { \
    g_print("[Debug:TRAY] (%s:%i) %s\n", __FILE__, __LINE__, msg); \
  }

#define DEBUG_MISC(msg) \
  if(alltray_debug_enabled(ALLTRAY_DEBUG_MISC)) { \
    g_print("[Debug:MISC] (%s:%i) %s\n", __FILE__, __LINE__, msg); \
  }

#else /* !ALLTRAY_DISABLE_DEBUG */

#define DEBUG_CMDLINE(msg) NULL
#define DEBUG_X11(msg) NULL
#define DEBUG_TRAY(msg) NULL
#define DEBUG_MISC(msg) NULL

#endif /* !ALLTRAY_DISABLE_DEBUG */

#endif /* !__ALLTRAY_H_INCLUDED__ */
