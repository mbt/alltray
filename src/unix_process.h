/*
 * unix_process.h - Get information on processes for various UNIX systems.
 * Copyright (c) 2009 Michael B. Trausch <mike@trausch.us>
 * Licene: GNU LGPL v3.0 as published by the Free Software Foundation
 */
#ifndef __unix_process_h_included__
#define __unix_process_h_included__

#include <sys/types.h>
#include <unistd.h>

typedef struct {
  pid_t pid;
  pid_t ppid;
  char *name;
} process_info_t;

// Get the PID, PPID, and process name for a given PID
process_info_t *get_process_info(pid_t pid);

#endif /* __unix_process_h_included__ */
