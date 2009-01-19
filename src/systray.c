/* -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *
 * systray.c - System tray functions
 * Copyright © 2009 Michael B. Trausch <mike@trausch.us>
 */
#include <alltray.h>

typedef struct {
  gboolean initialized;
  Window systray_window;
  gchar *systray_window_name;
} systray_internal_state;

static systray_internal_state internal_state;

gboolean
alltray_systray_init() {
  gboolean retval = FALSE;

  memset(&internal_state, 0, sizeof(internal_state));
  internal_state.systray_window = alltray_systray_get_window();

  if((internal_state.systray_window != 0) &&
     (internal_state.systray_window_name != NULL)) {
    retval = internal_state.initialized = TRUE;
  }

  return(retval);
}

Window
alltray_systray_get_window() {
  return(alltray_x11_get_selection_owner("_NET_SYSTEM_TRAY"));
}

gchar *
alltray_systray_get_window_name() {
  return(alltray_x11_get_window_name(internal_state.systray_window));
}
