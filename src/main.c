/* -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *
 * main.c - AllTray entry point.
 * Copyright © 2009 Michael B. Trausch <mike@trausch.us>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>
#include <libintl.h>
#include <alltray.h>

void
alltray_display_banner() {
  g_print("%s version %s\n", PACKAGE_NAME, PACKAGE_VERSION);
  g_print("  Copyright © 2004-2006 Jochen Baier <email@Jochen-Baier.de>\n");
  g_print("  Copyright © 2009 Michael B. Trausch <mike@trausch.us>\n");
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

static gboolean
output_is_terminal() {
  return(isatty(fileno(stdout)));
}

int
main(int argc, char *argv[]) {
  cmdline_parse(&argc, &argv);

  if((!cmdline_quiet) && output_is_terminal()) alltray_display_banner();
}
