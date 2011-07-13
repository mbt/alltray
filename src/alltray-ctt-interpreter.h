/*
 * alltray-ctt-interpreter.h - Interpreter public interface.
 * Copyright © 2011 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation.
 */
#ifndef __ALLTRAY_CTT_INTERPRETER_H_INCLUDED__
#define __ALLTRAY_CTT_INTERPRETER_H_INCLUDED__

#include <stdbool.h>
#include <X11/Xlib.h>

enum ACI_COMMAND {
  ACI_INVALID = 0,
  ACI_HELLO,
  ACI_ATTACH,
  ACI_DETACH,
  ACI_STATUS,
  ACI_EXIT
};

typedef struct alltray_ctt_command alltray_ctt_command;

alltray_ctt_command *aci_parse_command(char *cmdline, int cmdline_len);
bool aci_interpret_command(Display *dpy, alltray_ctt_command *cmd);

#endif /* __ALLTRAY_CTT_INTERPRETER_H_INCLUDED__ */
