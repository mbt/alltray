/* -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *
 * cmdline.c - Parse the command line.
 * Copyright © 2009 Michael B. Trausch <mike@trausch.us>
 */
#include <glib.h>
#include <alltray.h>

gboolean cmdline_debug_enabled;
gboolean cmdline_initially_show_window;
gboolean cmdline_hide_taskbar;
gboolean cmdline_modify_window_title;
gboolean cmdline_sticky_windows;
gboolean cmdline_list_debug_opts;
gboolean cmdline_quiet;
gboolean cmdline_show_version;
gboolean cmdline_show_ext_version;
guint cmdline_display_title_changes_delay;
gchar *cmdline_x11_display;
gboolean cmdline_wait_for_wm;
gboolean cmdline_wait_for_systray;

/*
 * The following flag is set when we receive the --test-mode option.
 * The --test-mode option is *NOT* intended for use by regular users,
 * and so it is omitted from the manpage as well as the --help output.
 *
 * In test mode, AllTray will not spawn a new application; instead, it
 * will create a tray icon and submit it to the system tray manager, and
 * pretend that the tray belongs to a fictitious application which does
 * nothing.
 *
 * Do not depend on the existence of test mode in any way, it may get
 * violently yanked away from the sources someday.
 */
gboolean cmdline_test_mode;

// Module-local data.
static GOptionEntry cmdline_entries[] = {
  { "show", 's', 0, G_OPTION_ARG_NONE, &cmdline_initially_show_window,
    "Initially show the target window", NULL },
  { "sticky", 'S', 0, G_OPTION_ARG_NONE, &cmdline_sticky_windows,
    "Make managed windows \"sticky\"", NULL },
  { "hide-from-taskbar", 'h', 0, G_OPTION_ARG_NONE, &cmdline_hide_taskbar,
    "Hide managed windows from the taskbar", NULL },
  { "display-title-change", 'd', 0, G_OPTION_ARG_INT,
    &cmdline_display_title_changes_delay,
    "Display title changes for SEC seconds", "SEC" },
  { "modify-title", 'm', 0, G_OPTION_ARG_NONE, &cmdline_modify_window_title,
    "Add \"(AllTray)\" to managed window titles", NULL },

  { "quiet", 'q', 0, G_OPTION_ARG_NONE, &cmdline_quiet,
    "Do not display verbose messages", NULL },
  { "list-debug-opts", 'L', 0, G_OPTION_ARG_NONE,
    &cmdline_list_debug_opts,
    "List debugging options and exit", NULL },
  { "debug", 'D', 0, G_OPTION_ARG_NONE, &cmdline_debug_enabled,
    "Enable debugging messages", NULL },
  { "version", 'v', 0, G_OPTION_ARG_NONE, &cmdline_show_version,
    "Display version info and exit", NULL },
  { "extended-version", 'V', 0, G_OPTION_ARG_NONE, &cmdline_show_ext_version,
    "Display extended version info and exit", NULL },

  { "display", '\0', 0, G_OPTION_ARG_STRING, &cmdline_x11_display,
    "Specify an X11 display to use", "DISP" },
  { "wait-for-wm", '\0', 0, G_OPTION_ARG_NONE, &cmdline_wait_for_wm,
    "Wait for a window manager to load", NULL },
  { "wait-for-systray", '\0', 0, G_OPTION_ARG_NONE, &cmdline_wait_for_systray,
    "Wait for a system tray to load", NULL },
  { "test-mode", '\0', G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_NONE, 
    &cmdline_test_mode, "Jump directly to a testing mode", NULL },

  { NULL, '\0', 0, 0, NULL, NULL, NULL }
};

static gchar *cmdline_help = ""
  "Start PROGRAM (with optional arguments provided in ARGS) and dock it to\n"
  "the system tray.  If ARGS contains options starting with - or --, then\n"
  "the PROGRAM must have the \"--\" parameter proceeding it, after any\n"
  "AllTray options.\n\n"
  "If no PROGRAM is specified, AllTray starts in \"click mode\" and will dock\n"
  "an already-running application.";

/**
 * Parse the command line.
 *
 * The parameters provided are passed by reference.  They are updated in that
 * known options will be removed from the command line as they are encountered
 * and handled.
 *
 * If the -v (--version) or -L (--list-debug-opts) options are
 * specified, this function DOES NOT RETURN and will exit the program
 * immediately after processing those options.
 */
void cmdline_parse(int *argc, char ***argv) {
  GError *error = NULL;
  GOptionContext *context;

  context = g_option_context_new("[--] [PROGRAM] [ARGS]");

  g_option_context_set_summary(context, cmdline_help);
  g_option_context_add_main_entries(context, cmdline_entries, PACKAGE);

  if(!g_option_context_parse(context, argc, argv, &error)) {
    g_print("error: %s\n", error->message);
    exit(ALLTRAY_EXIT_INVALID_ARGS);
  }

  if(cmdline_show_ext_version) {
    main_display_banner();
    main_display_extended_banner();
    exit(ALLTRAY_EXIT_SUCCESS);
  }

  if(cmdline_show_version) {
    main_display_banner();
    exit(ALLTRAY_EXIT_SUCCESS);
  }

  if(cmdline_list_debug_opts) {
    alltray_debug_display_opts();
    exit(ALLTRAY_EXIT_SUCCESS);
  }

  return;
}
