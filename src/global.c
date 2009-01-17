/* -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *
 * global.c - AllTray program global data.
 * Copyright © 2009 Michael B. Trausch <mike@trausch.us>
 */
#include <glib.h>
#include <alltray.h>

gboolean cmdline_debug_enabled = FALSE;
gboolean cmdline_initially_show_window = FALSE;
gboolean cmdline_hide_taskbar = FALSE;
gboolean cmdline_modify_window_title = FALSE;
gboolean cmdline_sticky_windows = FALSE;
gboolean cmdline_list_debug_opts = FALSE;
gboolean cmdline_quiet = FALSE;
gboolean cmdline_show_version = FALSE;

guint cmdline_display_title_changes_delay = 0;

gchar *cmdline_x11_display = NULL;
