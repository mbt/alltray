/*
 * freebsd_process_info.c - Functions to get process information on
 * FreeBSD systems.
 * Copyright (c) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU LGPL v3.0 as published by the Free Software Foundation
 *
 * Note: to use this, -lkvm must be added to the cc command line.
 */
#include <fnctl.h>
#include <kvm.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/user.h>

static process_info_t *freebsd_get_process_info(pid_t pid);

static process_info_t *
freebsd_get_process_info(pid_t pid) {
  kvm_t *kernel;
  process_info_t *process_info = NULL;
  struct kinfo_proc *kernel_proc_info = NULL;
  int infos_returned = 0;
  char *proc_name = NULL;
  int proc_name_len = 0;

  kernel = kvm_open(NULL, "/dev/null", NULL, O_RDONLY, "");
  if(kernel == NULL) {
    fprintf(stderr, "Unable to get kernel object to get process info\n");
    fprintf(stderr, "I don't think I can continue, I am sorry.\n");
    abort();
  }

  kernel_proc_info = kvm_getprocs(kernel, KERN_PROC_PID, (int)pid,
                                  &infos_returned);
  if(infos_returned == 0) {
    fprintf(stderr, "Unable to get info for process with pid %d.\n", pid);
    return(NULL);
  }

  process_info = (process_info_t *)malloc(sizeof(process_info_t));
  if(process_info == NULL) {
    fprintf(stderr, "AllTray has run out of memory.\n");
    abort();
  }

  process_info = memset(process_info, 0, sizeof(process_info_t));
  process_info->pid = kernel_proc_info->ki_pid;
  process_info->ppid = kernel_proc_info->ki_ppid;

  proc_name_len = strlen(kernel_proc_info->ki_paddr->p_comm);
  process_info->name = (char *)malloc(proc_name_len + 1);
  if(process_info->name == NULL) {
    fprintf(stderr, "AllTray has run out of memory.\n");
    abort();
  }
  process_info->name = memset(process_info->name, 0, proc_name_len + 1);
  strncpy(process_info->name, kernel_proc_info->ki_paddr->p_comm, proc_name_len);
  process_info->name[proc_name_len] = '\0';

  return(process_info);
}
