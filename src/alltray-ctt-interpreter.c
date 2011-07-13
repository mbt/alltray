/*
 * alltray-ctt-interpreter.c - AllTray Close-To-Tray command interpreter
 * Copyright © 2011 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <alltray-ctt-interpreter.h>
#include <alltray-ctt-helper.h>
#include <alltray-ctt-windowlist.h>
#include <config.h>

struct alltray_ctt_command {
  enum ACI_COMMAND cmd;
  uint32_t argc;
  char **argv;
};

static char *delimiter = " \t\n";

static uint32_t
aci_get_word_count(char *cmdline) {
  char *strtok_saveptr = NULL;
  char *token = NULL;
  uint32_t i = 0;
  char *parseStr = strdup(cmdline);

  for(i = 1, parseStr;; i++, parseStr = NULL) {
    token = strtok_r(parseStr, delimiter, &strtok_saveptr);
    if(token == NULL) {
      i--;
      break;
    }
  }

  free(parseStr);
  return(i);
}

/**
 * Parse the supplied command.
 *
 * Essentially, we break things apart into words; the first word is
 * the command name, the second word and subsequent words (if present)
 * are the command's parameters.
 *
 * Returns NULL if there was no command, or the (opaque) command data
 * structure.
 */
struct alltray_ctt_command *
aci_parse_command(char *cmdline, int cmdline_len) {
  char *strtok_saveptr = NULL;
  char *token = NULL;

  struct alltray_ctt_command *retval =
    malloc(sizeof(struct alltray_ctt_command));
  if(retval == NULL) {
    abort();
  } else {
    memset(retval, 0, sizeof(struct alltray_ctt_command));
  }

  retval->argc = aci_get_word_count(cmdline);
  retval->argv = calloc(retval->argc, sizeof(char *));

  if(retval->argv == NULL) {
    abort();
  }

  char *parseStr = strdup(cmdline);
  int i = 0;
  for(i = 0;; i++, parseStr = NULL) {
    token = strtok_r(parseStr, delimiter, &strtok_saveptr);

    if(token == NULL) break;

    if(i == 0) {
      if(strcasecmp(token, "HELLO") == 0) {
	retval->cmd = ACI_HELLO;
      } else if(strcasecmp(token, "ATTACH") == 0) {
	retval->cmd = ACI_ATTACH;
      } else if(strcasecmp(token, "DETACH") == 0) {
	retval->cmd = ACI_DETACH;
      } else if(strcasecmp(token, "STATUS") == 0) {
	retval->cmd = ACI_STATUS;
      } else if(strcasecmp(token, "EXIT") == 0) {
	retval->cmd = ACI_EXIT;
      } else {
	retval->cmd = ACI_INVALID;
      }
    }

    /*
     * TODO: find out if this is necessary.
     *
     * The documentation does not specify whether the return value of
     * strtok_r() is a malloc()'d value or not.  I am guessing that it
     * _is_, and if so then we will leak memory because we're copying
     * it without freeing it.  However, I want to be sure that this is
     * the case first.
     */
    retval->argv[i] = strdup(token);
  }

  free(parseStr);
  return(retval);
}

static bool
aci_command_invalid(Display *dpy, struct alltray_ctt_command *cmd) {
  printf("NAK - INVALID COMMAND: %s\n", cmd->argv[0]);
  return(true);
}

static bool
aci_command_hello(Display *dpy, struct alltray_ctt_command *cmd) {
  printf("ACK - %s CTT HELPER VERSION %s\n", PACKAGE_NAME, PACKAGE_VERSION);
  return(true);
}

static bool
aci_command_attach(Display *dpy, struct alltray_ctt_command *cmd) {
  int i = 0;
  int attached = 0;
  int our_window = 0;

  for(i = 1; i < cmd->argc; i++) {
    int parent_window = strtol(cmd->argv[i], NULL, 10);
    our_window = ctt_make_window(dpy, parent_window);
    alltray_ctt_windowlist_add(dpy, our_window, parent_window);
    attached++;
  }

  printf("ACK - CTT ATTACHED %d WINDOW(S)\n", attached);
  return(true);
}

static bool
aci_command_detach(Display *dpy, struct alltray_ctt_command *cmd) {
  int i = 0;
  int detached = 0;
  int window = 0;

  for(i = 1; i < cmd->argc; i++) {
    window = strtol(cmd->argv[i], NULL, 10);

    ctt_destroy_window(dpy, window);
    alltray_ctt_windowlist_del(window);

    detached++;
  }

  printf("ACK - CTT DETACHED %d WINDOWS\n", detached);
  return(true);
}

static bool
aci_command_status(Display *dpy, struct alltray_ctt_command *cmd) {
  int count = alltray_ctt_windowlist_len();

  if(count == 0) {
    printf("ACK - NO WINDOWS MANAGED\n");
  } else {
    int window_count = 0;
    Window *window_list = NULL;
    alltray_ctt_windowlist_get_all_parents(&window_count, &window_list);

    int i = 0;
    printf("ACK - WINDOW LIST:");
    for(i = 0; i < window_count; i++)
      printf(" %ld", window_list[i]);

    printf("\n");

    /*
     * XXX: We didn't (directly) allocate this, but this is the only
     * reasonable place to free it.
     */
    free(window_list);
  }

  return(true);
}

static bool
aci_command_exit(Display *dpy, struct alltray_ctt_command *cmd) {
  printf("ACK - %s CTT HELPER EXITING...\n", PACKAGE_NAME);
  exit(1);
}

bool
aci_interpret_command(Display *dpy, struct alltray_ctt_command *cmd) {
  switch(cmd->cmd) {
  case ACI_INVALID:
    return(aci_command_invalid(dpy, cmd));
  case ACI_HELLO:
    return(aci_command_hello(dpy, cmd));
  case ACI_ATTACH:
    return(aci_command_attach(dpy, cmd));
  case ACI_DETACH:
    return(aci_command_detach(dpy, cmd));
  case ACI_STATUS:
    return(aci_command_status(dpy, cmd));
  case ACI_EXIT:
    return(aci_command_exit(dpy, cmd));
  default:
    return(aci_command_invalid(dpy, cmd));
  }
}
