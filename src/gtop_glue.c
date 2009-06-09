/*
 * gtop_glue.c - Use the libgtop library, but I am too lazy/annoyed to try
 * to figure out how the ^#!! I'm supposed to bind it to Vala.
 * Copyright (C) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU LGPL 3.0 as published by the Free Software Foundation.
 */
#include <sys/types.h>
#include <unistd.h>
#include <glib.h>
#include <glibtop.h>
#include <glibtop/procuid.h>

int
alltray_get_ppid_for(int pid) {
  glibtop_proc_uid *buffer =
    (glibtop_proc_uid*)g_malloc0(sizeof(glibtop_proc_uid));

  glibtop_get_proc_uid(buffer, (pid_t)pid);
  int retval = buffer->ppid;
  free(buffer);

  return(retval);
}
