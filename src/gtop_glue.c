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
#include <glibtop/proclist.h>
#include <glibtop/procstate.h>

int
alltray_get_ppid_for(int pid) {
  glibtop_proc_uid *buffer =
    (glibtop_proc_uid*)g_malloc0(sizeof(glibtop_proc_uid));

  glibtop_get_proc_uid(buffer, (pid_t)pid);
  int retval = buffer->ppid;
  free(buffer);

  return(retval);
}

void
alltray_get_processes_in_pgrp(int pgrp, int **procs, int *procs_len) {
  pid_t *retval = NULL;
  gint64 wanted_pgid = (gint64)pgrp;

  glibtop_proclist *proclist_info =
    (glibtop_proclist *)g_malloc0(sizeof(glibtop_proclist));

  retval = glibtop_get_proclist(proclist_info, GLIBTOP_KERN_PROC_PGRP,
				wanted_pgid);
  *procs_len = proclist_info->number;
  free(proclist_info);

  *procs = retval;
}

/*
 * Return a newly-allocated string that contains the name of the
 * process for the given PID.  This means that the caller owns the
 * variable.
 */
char *
alltray_get_process_name(int pid) {
  char *retval = NULL;
  glibtop_proc_state *glbuf = NULL;

  glbuf = (glibtop_proc_state *)g_malloc0(sizeof(glibtop_proc_state));
  glibtop_get_proc_state(glbuf, pid);

  retval = g_strndup(glbuf->cmd, 40);
  free(glbuf);

  return(retval);
}
