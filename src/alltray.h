/* -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *
 * alltray.h - Project-wide include file
 * Copyright © 2009 Michael B. Trausch <mike@trausch.us>
 */
#ifndef __ALLTRAY_H_INCLUDED__
#define __ALLTRAY_H_INCLUDED__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <glib.h>
#include <glib-object.h>
#include <config.h>

// Exit codes.
#define ALLTRAY_EXIT_SUCCESS  0
#define ALLTRAY_EXIT_INVALID_ARGS 1
#define ALLTRAY_EXIT_X11_ERROR 2
#define ALLTRAY_EXIT_WM_ERROR 3
#define ALLTRAY_EXIT_SYSTRAY_ERROR 4
#define ALLTRAY_EXIT_SPAWN_ERROR 5

// This is a stop-gap until the remainder of the program is (re)written.
#define ALLTRAY_EXIT_NOT_IMPLEMENTED 126

// Prototypes for cmdline.c
void cmdline_parse(int *argc, char ***argv);

// Prototypes for debug.c
void alltray_debug_init(void);
gboolean alltray_debug_enabled(gint desired_flag);
void alltray_debug_display_opts(void);

// Data for global.c
extern gboolean cmdline_debug_enabled;
extern gboolean cmdline_initially_show_window;
extern gboolean cmdline_hide_taskbar;
extern gboolean cmdline_modify_window_title;
extern gboolean cmdline_sticky_windows;
extern gboolean cmdline_list_debug_opts;
extern gboolean cmdline_quiet;
extern gboolean cmdline_show_version;
extern gboolean cmdline_show_ext_version;
extern guint cmdline_display_title_changes_delay;
extern gchar *cmdline_x11_display;
extern gboolean cmdline_wait_for_wm;
extern gboolean cmdline_wait_for_systray;
extern gboolean cmdline_test_mode;

// Prototypes for main.c
int main(int argc, char *argv[]);
void main_display_banner(void);
void main_display_extended_banner(void);

// Data & Prototypes for process.c
typedef struct {
  GPid child_pid;

  int window_count;
  GList *window_list;
} alltray_instance_process_info;

typedef enum {
  ALLTRAY_PROCESS_SUID_EXECUTABLE,
  ALLTRAY_PROCESS_SGID_EXECUTABLE,
  ALLTRAY_PROCESS_NOT_FOUND,
  ALLTRAY_PROCESs_PERMISSION_DENIED
} AllTrayProcessError;

#define ALLTRAY_PROCESS_ERROR alltray_process_error_quark()

gboolean alltray_process_spawn_new(char *argv[], GPid *pid) WARN_UNUSED_RETVAL;
void alltray_process_child_exited(GPid which, gint exit_status,
                                  gpointer main_loop);
GQuark alltray_process_error_quark(void);

// Prototypes for systray.c
gboolean alltray_systray_init(void);
Window alltray_systray_get_window(void);
gchar *alltray_systray_get_window_name(void);
void alltray_systray_wait_for_available(void);

// Prototypes for wm.c
gboolean alltray_wm_init(void);
Window alltray_wm_get_window(void);
gchar * alltray_wm_get_name(void);
gboolean alltray_wm_get_process_info(GPid *wm_pid, gchar **wm_host);
void alltray_wm_wait_for_available(void);

// Prototypes for x11.c
gboolean alltray_x11_init(const gchar *display_name);
Atom alltray_x11_get_atom(const gchar *atom_name);
gchar *alltray_x11_get_window_name(Window win);
GList *alltray_x11_get_window_text_property(Window win, const gchar *prop_name);
gchar *alltray_x11_get_window_utf8_property(Window win, const gchar *prop_name);
Window alltray_x11_get_root_window(void);
Window alltray_x11_get_window_window_property(Window win, 
                                              const gchar *prop_name);
gint alltray_x11_get_window_cardinal_property(Window win,
                                              const gchar *prop_name);
gchar *alltray_x11_get_window_string_property(Window win,
                                              const gchar *prop_name);
gchar *alltray_x11_get_atom_name(Atom atom);
Window alltray_x11_create_window(Window parent, int location_x, int location_y,
                                 int width, int height);
Window alltray_x11_get_window_parent(Window win);
Window alltray_x11_get_selection_owner(const gchar *selection_name);
gint alltray_x11_get_default_screen(void);

// Data & Prototypes for x11-error.c
typedef enum {
  ALLTRAY_X11_ERROR_BAD_ACCESS,
  ALLTRAY_X11_ERROR_BAD_ALLOC,
  ALLTRAY_X11_ERROR_BAD_ATOM,
  ALLTRAY_X11_ERROR_BAD_COLOR,
  ALLTRAY_X11_ERROR_BAD_CURSOR,
  ALLTRAY_X11_ERROR_BAD_FONT,
  ALLTRAY_X11_ERROR_BAD_GC,
  ALLTRAY_X11_ERROR_BAD_ID_CHOICE,
  ALLTRAY_X11_ERROR_BAD_IMPLEMENTATION,
  ALLTRAY_X11_ERROR_BAD_LENGTH,
  ALLTRAY_X11_ERROR_BAD_MATCH,
  ALLTRAY_X11_ERROR_BAD_NAME,
  ALLTRAY_X11_ERROR_BAD_PIXMAP,
  ALLTRAY_X11_ERROR_BAD_REQUEST,
  ALLTRAY_X11_ERROR_BAD_VALUE,
  ALLTRAY_X11_ERROR_BAD_WINDOW,
  ALLTRAY_X11_ERROR_FAILED
} AllTrayX11Error;

#define ALLTRAY_X11_ERROR alltray_x11_error_quark();

void alltray_x11_error_init(Display *disp, gint screen_number);
gboolean alltray_x11_error_install_handler(void);
GSList *alltray_x11_error_uninstall_handler(void);

// Debug constants and macros
#define ALLTRAY_DEBUG_NONE 0x0000
#define ALLTRAY_DEBUG_CMDLINE 0x0001
#define ALLTRAY_DEBUG_X11 0x0002
#define ALLTRAY_DEBUG_TRAY 0x0004
#define ALLTRAY_DEBUG_WM 0x0008
#define ALLTRAY_DEBUG_PROCESS 0x0010
#define ALLTRAY_DEBUG_X11_ERROR 0x0020
#define ALLTRAY_DEBUG_MISC 0x00F0

#define ALLTRAY_DEBUG_ALL ((ALLTRAY_DEBUG_CMDLINE | ALLTRAY_DEBUG_X11 | \
                            ALLTRAY_DEBUG_TRAY | ALLTRAY_DEBUG_WM | \
                            ALLTRAY_DEBUG_PROCESS | ALLTRAY_DEBUG_MISC | \
                            ALLTRAY_DEBUG_X11_ERROR))

#ifndef ALLTRAY_DISABLE_DEBUG

#define DEBUG_CMDLINE(msg) \
  if(alltray_debug_enabled(ALLTRAY_DEBUG_CMDLINE)) { \
    g_print("[Debug:CMDLINE] (%s:%i) %s\n", __FILE__, __LINE__, msg); \
  }

#define DEBUG_X11(msg) \
  if(alltray_debug_enabled(ALLTRAY_DEBUG_X11)) { \
    g_print("[Debug:X11] (%s:%i) %s\n", __FILE__, __LINE__, msg); \
  }

#define DEBUG_X11_ERROR(msg) \
  if(alltray_debug_enabled(ALLTRAY_DEBUG_X11_ERROR)) { \
    g_print("[Debug:X11_ERROR] (%s:%i) %s\n", __FILE__, __LINE__, msg); \
  }

#define DEBUG_TRAY(msg) \
  if(alltray_debug_enabled(ALLTRAY_DEBUG_TRAY)) { \
    g_print("[Debug:TRAY] (%s:%i) %s\n", __FILE__, __LINE__, msg); \
  }

#define DEBUG_WM(msg) \
  if(alltray_debug_enabled(ALLTRAY_DEBUG_WM)) { \
    g_print("[Debug:WM] (%s:%i) %s\n", __FILE__, __LINE__, msg); \
  }

#define DEBUG_MISC(msg) \
  if(alltray_debug_enabled(ALLTRAY_DEBUG_MISC)) { \
    g_print("[Debug:MISC] (%s:%i) %s\n", __FILE__, __LINE__, msg); \
  }

#define DEBUG_PROCESS(msg) \
  if(alltray_debug_enabled(ALLTRAY_DEBUG_PROCESS)) { \
    g_print("[Debug:PROCESS] (%s:%i) %s\n", __FILE__, __LINE__, msg); \
  }

#else /* !ALLTRAY_DISABLE_DEBUG */

#define DEBUG_CMDLINE(msg) NULL
#define DEBUG_X11(msg) NULL
#define DEBUG_X11_ERROR(msg) NULL
#define DEBUG_TRAY(msg) NULL
#define DEBUG_MISC(msg) NULL
#define DEBUG_PROCESS(msg) NULL

#endif /* !ALLTRAY_DISABLE_DEBUG */

// Other constants
#define ALLTRAY_HELPER_LIBRARY "alltray-helper.so"

#endif /* !__ALLTRAY_H_INCLUDED__ */
