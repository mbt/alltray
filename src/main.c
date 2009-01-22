/* -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *
 * main.c - AllTray entry point.
 * Copyright © 2009 Michael B. Trausch <mike@trausch.us>
 */
#include <alltray.h>

void
main_display_banner() {
  g_print("%s version %s\n", PACKAGE_NAME, PACKAGE_VERSION);
  g_print("  Copyright (c) 2004-2006 Jochen Baier <email@Jochen-Baier.de>\n");
  g_print("  Copyright (c) 2009 Michael B. Trausch <mike@trausch.us>\n");
  g_print("Bug reports: %s\n", PACKAGE_BUGREPORT);

  if(strstr(VERSION, "dev") != NULL) {
    g_print("\nThis is a development version of the software.\n");
    g_print("It may not function correctly or at all.\n\n");
  }

  if(strstr(VERSION, "beta") != NULL) {
    g_print("\nThis is a beta version of the software.\n");
    g_print("Please report any bugs you find in your testing.\n\n");
  }

  if(strstr(VERSION, "rc") != NULL) {
    g_print("\nThis is a release candidate version of the software.\n");
    g_print("Please report any bugs you find in your testing.\n\n");
  }
}

void
main_display_extended_banner() {
  g_print("Compilation Environment\n"
          "=======================\n\n");
  g_print("CFLAGS:\t\t%s\n", ALLTRAY_COMPILE_FLAGS);
  g_print("Compiler:\t%s\n", ALLTRAY_COMPILE_COMPILER);
  g_print("Build kernel:\t%s\n", ALLTRAY_COMPILE_OS);
  g_print(" ... release:\t%s\n", ALLTRAY_COMPILE_OS_REL);
  g_print(" ... version:\t%s\n", ALLTRAY_COMPILE_OS_VER);
  g_print(" ... hostname:\t%s\n", ALLTRAY_COMPILE_OS_HOST);
  g_print("Build UTC:\t%s\n", ALLTRAY_COMPILE_BUILD_DATE);
  g_print("Build EST5EDT:\t%s\n", ALLTRAY_COMPILE_BUILD_DATE_EUS);
}

static gboolean
output_is_terminal(void) {
  return(isatty(fileno(stdout)));
}

static int
main_spawn_mode(int argc, char *argv[], GMainLoop *loop) {
  GPid child_pid;
  gboolean success;
  int retval = 0;

  success = alltray_process_spawn_new(argv, &child_pid);

  if(G_LIKELY(success)) {
    g_child_watch_add(child_pid, alltray_process_child_exited, loop);
    g_main_loop_run(loop);
  } else {
    retval = ALLTRAY_EXIT_SPAWN_ERROR;
  }

  return(retval);
}

static int
main_click_mode(void) {
  g_printerr("Click Mode is not yet implemented.\n");
  return(ALLTRAY_EXIT_NOT_IMPLEMENTED);
}

static int
main_required_init() {
  int retval = 0;

  if(!alltray_x11_init(cmdline_x11_display)) {
    g_printerr("Error: Unable to initialize the X11 module.\n"
               "Is the DISPLAY variable correctly set?\n");
    retval = ALLTRAY_EXIT_X11_ERROR;
  } else {
    DEBUG_X11("X11 initialized");
  }

  if(!alltray_wm_init() && !retval) {
    if(G_LIKELY(cmdline_wait_for_wm == FALSE)) {
      g_printerr("Error: Unable to initialize the window manager module.\n"
                 "Are you running an EWMH-compliant window manager?\n");
      retval = ALLTRAY_EXIT_WM_ERROR;
    } else {
      alltray_wm_wait_for_available();
      alltray_wm_init();
    }
  } else {
    DEBUG_WM("WM initialized");
  }

  if(!alltray_systray_init() && !retval) {
    if(G_LIKELY(cmdline_wait_for_systray == FALSE)) {
      g_printerr("Error: Unable to initialize the system tray module.\n"
                 "Are you running a system tray manager?\n");
      retval = ALLTRAY_EXIT_SYSTRAY_ERROR;
    } else {
      alltray_systray_wait_for_available();
      alltray_systray_init();
    }
  } else {
    DEBUG_TRAY("TRAY initialized");
  }

  return(retval);
}

int
main(int argc, char *argv[]) {
  int retval = 0;
  GMainLoop *alltray_main_loop;

  g_type_init();
  alltray_main_loop = g_main_loop_new(NULL, FALSE);

  cmdline_parse(&argc, &argv);
  if((!cmdline_quiet) && output_is_terminal()) main_display_banner();
  if(cmdline_debug_enabled) alltray_debug_init();

  retval = main_required_init();

  // Only try to do anything if we've been successful thus far.
  if(retval == 0) {
    if(argc > 1) {
      int passed_argv_start = 1;
      int passed_argc = (argc - 1);

      if(strcmp(argv[1], "--") == 0) {
        passed_argc--;
        passed_argv_start++;
      }

      retval = main_spawn_mode(passed_argc, &argv[passed_argv_start],
                               alltray_main_loop);
    } else {
      retval = main_click_mode();
    }
  }

  return(retval);
}
