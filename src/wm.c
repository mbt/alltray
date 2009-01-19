/* -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *
 * wm.c - Window manager functions
 * Copyright © 2009 Michael B. Trausch <mike@trausch.us>
 */
#include <alltray.h>

typedef struct {
  gboolean initialized;
  gchar *wm_name;
  Window wm_window;
} wm_internal_state;

static wm_internal_state internal_state;
static gboolean wm_is_ewmh_compliant(void);
static void wm_cleanup(void);

gboolean
alltray_wm_init() {
  gboolean retval = FALSE;
  
  memset(&internal_state, 0, sizeof(internal_state));
  
  internal_state.wm_window = alltray_wm_get_window();
  internal_state.wm_name = alltray_wm_get_name();
  
  if(wm_is_ewmh_compliant()) {
    retval = internal_state.initialized = TRUE;    
  }

  atexit(wm_cleanup);
  return(retval);
}

/**
 * Return the window created by the currently-running window manager.
 *
 * This is also how we detect whether or not the currently-running
 * window manager supports the Extended Window Manager Hints (EMWH)
 * specification.
 *
 * @returns 0 if the currently-running WM does not support EMWH, or the
 * window ID if the currently-running WM supports EMWH and has thus
 * published its window ID.
 */
Window
alltray_wm_get_window() {
  Window root_window = alltray_x11_get_root_window();
  Window retval =
    alltray_x11_get_window_window_property(root_window,
                                           "_NET_SUPPORTING_WM_CHECK");
  return(retval);
}

/**
 * Return the name of the currently-running window manager.
 */
gchar *
alltray_wm_get_name() {
  gchar *retval = NULL;

  if(internal_state.wm_window != 0) {
    retval = alltray_x11_get_window_utf8_property(internal_state.wm_window,
                                                  "_NET_WM_NAME");
  } else {
    // We shouldn't ever reach this point, but just in case...

    g_printerr(g_strdup_printf("%s has encountered an internal error.",
                               PACKAGE_NAME));
    abort();
  }

  return(retval);
}

/**
 * Returns whether or not the window manager is (appears to be) EWMH-compliant.
 *
 * @returns TRUE if we think the window manager is EWMH-compliant, or
 * FALSE if not.
 */
static gboolean
wm_is_ewmh_compliant() {
  gboolean retval = TRUE;

  if(internal_state.wm_window == 0) retval = FALSE;
  if(internal_state.wm_name == NULL) retval = FALSE;

  return(retval);
}

/**
 * Cleans up after this module.
 */
static void
wm_cleanup() {
  g_free(internal_state.wm_name);
  internal_state.wm_name = 0;

  return(internal_state.initialized = FALSE);
}
