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
  DEBUG_TRAY("Initializing the system tray module");

  memset(&internal_state, 0, sizeof(internal_state));
  internal_state.systray_window = alltray_systray_get_window();
  DEBUG_TRAY(g_strdup_printf("Got system tray window: 0x%08x",
                             (guint)internal_state.systray_window));

  internal_state.systray_window_name = alltray_systray_get_window_name();
  DEBUG_TRAY(g_strdup_printf("Got system tray window name: %s",
                             internal_state.systray_window_name));

  if((internal_state.systray_window != 0) &&
     (internal_state.systray_window_name != NULL)) {
    retval = internal_state.initialized = TRUE;
  }

  return(retval);
}

Window
alltray_systray_get_window() {
  // System tray is _NET_SYSTEM_TRAY_Sn where n is the screen number.
  gint screen = alltray_x11_get_default_screen();
  gchar *system_tray_selection = g_strdup_printf("_NET_SYSTEM_TRAY_S%01d",
                                                 screen);
  Window retval = alltray_x11_get_selection_owner(system_tray_selection);
  return(retval);
}

gchar *
alltray_systray_get_window_name() {
  gchar *retval;

  retval = alltray_x11_get_window_name(internal_state.systray_window);
  return(retval);
}

/**
 * Send a message to the system tray icon.
 */
void
alltray_systray_send_message(gchar *msg) {
  #warning Not yet implemented...
  return;
}

/**
 * Check once per second until there is a system tray available.
 */
void
alltray_systray_wait_for_available() {
  while(alltray_systray_get_window() == 0) {
    DEBUG_TRAY("Waiting for a system tray to start...");
    sleep(1);
  }
}
