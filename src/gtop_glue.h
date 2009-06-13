/*
 * gtop_glue.h - Use the libgtop library, but I am too lazy/annoyed to try
 * to figure out how the ^#!! I'm supposed to bind it to Vala.
 * Copyright (C) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU LGPL 3.0 as published by the Free Software Foundation.
 */
#ifndef __gtop_glue_h_included__
#define __gtop_glue_h_included__

int alltray_get_ppid_for(int pid);
void alltray_get_processes_in_pgrp(int pgrp, int **procs, int *procs_len);
char *alltray_get_process_name(int pid);

#endif /* __gtop_glue_h_included__ */
