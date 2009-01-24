/* -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *
 * common.c - Common functions used throughout the program
 * Copyright © 2009 Michael B. Trausch <mike@trausch.us>
 */
#include <alltray.h>

/**
 * Used to indicate to the user that a bug has been detected in the
 * AllTray program somewhere, and provides information on how to
 * submit the bug report.  This function never returns, and will try
 * to dump core if possible so that the user can provide the core dump
 * with their bug report.
 */
void
alltray_common_bug_detected() {
  g_error(
"%s has encountered a programming error (a bug) and cannot\n"
"continue.\n"
"Please file a bug report at %s.\n\n"
"If a core file is generated, please attach that core file, and your copy of\n"
"the program executable, to the bug report (and also tell what operating\n"
"system and hardware platform you are running on); this can help make\n"
"discovering and eliminating the bug easier.\n",
PACKAGE_NAME, PACKAGE_BUGREPORT);
}
