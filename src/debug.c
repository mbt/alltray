/* -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *
 * debug.c - AllTray runtime debugging support.
 * Copyright © 2009 Michael B. Trausch <mike@trausch.us>
 */
#include <alltray.h>

typedef struct {
  gint flag;
  gchar *env_value;
  gchar *description;
} debug_flag_record;

static debug_flag_record debug_flags[] = {
  { ALLTRAY_DEBUG_CMDLINE, "CMDLINE",
    "Command line parsing debugging messages" },
  { ALLTRAY_DEBUG_X11, "X11", "X11 debugging messages" },
  { ALLTRAY_DEBUG_TRAY, "TRAY", "System tray debugging messages" },
  { ALLTRAY_DEBUG_MISC, "MISC", "Miscellaneous debugging messages" },
  { ALLTRAY_DEBUG_ALL, "ALL", "Enable all debugging messages" },
  { 0, NULL, NULL }
};

static gint enabled_debug_flags = ALLTRAY_DEBUG_NONE;

void
alltray_debug_init() {
  gchar *environment_value;

  environment_value = getenv("ALLTRAY_DEBUG");
  if(!environment_value) return;

  for(debug_flag_record *cur_flag = &debug_flags[0]; cur_flag->flag != 0;
      cur_flag++) {
    if(strstr(cur_flag->env_value, environment_value)) {
      enabled_debug_flags |= cur_flag->flag;
    }
  }
}

gboolean
alltray_debug_enabled(gint desired_flag) {
  return((enabled_debug_flags & desired_flag) != 0);
}

void
alltray_debug_display_opts() {
  g_print("\nFlag name\tDescription\n");
  g_print("\n---------\t-----------\n\n");

  for(debug_flag_record *cur_flag = &debug_flags[0];
      cur_flag->flag != 0;
      cur_flag++) {
    g_print("%s\t\t%s\n", cur_flag->env_value, cur_flag->description);
  }
}
