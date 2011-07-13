/*
 * alltray-ctt-helper.c - AllTray Close-To-Tray helper program
 * Copyright © 2011 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <X11/Xlib.h>
#include <X11/xpm.h>
#include <X11/extensions/Xext.h>
#include <X11/extensions/shape.h>
#include <../images/alltray_ctt.xpm>
#include <alltray-ctt-interpreter.h>

struct alltray_ctt_window_list_node {
  Display *dpy;
  Window window;
  Window parent;
  struct alltray_ctt_window_list_node *next;
  struct alltray_ctt_window_list_node *prev;
};

static struct alltray_ctt_window_list_node *wl_first_node = NULL;

static struct alltray_ctt_window_list_node *
ctt_node_create() {
  if(wl_first_node == NULL) {
    wl_first_node = calloc(1, sizeof(struct alltray_ctt_window_list_node));
    if(wl_first_node == NULL) abort();
    return(wl_first_node);
  }

  struct alltray_ctt_window_list_node *prev = NULL;
  struct alltray_ctt_window_list_node *cur = NULL;

  for(prev = wl_first_node, cur = prev->next; cur != NULL; prev = cur, cur = prev->next);
  prev->next = calloc(1, sizeof(struct alltray_ctt_window_list_node));
  if(prev->next == NULL) abort();
  prev->next->prev = prev;
  return(prev->next);
}

static void
ctt_node_delete(struct alltray_ctt_window_list_node *node) {
  if(node->next != NULL) {
    if(node->prev != NULL) {
      node->prev->next = node->next;
      node->next->prev = node->prev;
    } else {
      // first node.
      node->next->prev = NULL;
    }

  } else {
    // last node.  Might also be the first node.
    if(node->prev != NULL) {
      node->prev->next = NULL;
    }
  }

  free(node);
  return;
}

static struct alltray_ctt_window_list_node *
ctt_node_find(Window ctt_window_id) {
  struct alltray_ctt_window_list_node *prev, *cur;
  prev = NULL;
  for(cur = wl_first_node; cur != NULL; cur = cur->next)
    if(cur->window == ctt_window_id) return(cur);

  return(NULL);
}

static struct alltray_ctt_window_list_node *
ctt_node_find_by_parent(Window parent_window_id) {
  struct alltray_ctt_window_list_node *prev, *cur;
  prev = NULL;
  for(cur = wl_first_node; cur != NULL; cur = cur->next)
    if(cur->parent == parent_window_id) return(cur);

  return(NULL);
}

bool
alltray_ctt_helper_add_window(Display *dpy, Window ctt_window, Window ctt_parent) {
  struct alltray_ctt_window_list_node *new_node = ctt_node_create();
  new_node->dpy = dpy;
  new_node->window = ctt_window;
  new_node->parent = ctt_parent;
}

bool
alltray_ctt_helper_del_window(Window parent_window) {
  struct alltray_ctt_window_list_node *node =
    ctt_node_find_by_parent(parent_window);

  if(node != NULL) {
    ctt_node_delete(node);
  }
}

Window
alltray_ctt_get_ctt_for_parent(Window parent) {
  struct alltray_ctt_window_list_node *node =
    ctt_node_find_by_parent(parent);

  if(node != NULL)
    return(node->window);

  return(0);
}

static void
set_ctt_window_shape_and_background(Display *dpy, Window ctt_window,
				    unsigned long ctt_attribute_mask,
				    XSetWindowAttributes *ctt_attributes) {
  Pixmap ctt_xpm, ctt_shape;
  XpmCreatePixmapFromData(dpy, ctt_window, atcttxpm, &ctt_xpm, &ctt_shape,
			  NULL /* Attributes; IIRC, we don't need this. */);
  ctt_attribute_mask |= CWBackPixmap;
  ctt_attributes->background_pixmap = ctt_xpm;
  XChangeWindowAttributes(dpy, ctt_window, ctt_attribute_mask, ctt_attributes);
  XShapeCombineMask(dpy, ctt_window, ShapeBounding, 0, 0, ctt_shape, ShapeSet);
}

static Window
create_ctt_window(Display *dpy, Window parent) {
  XWindowAttributes *parent_attributes = NULL;
  XSetWindowAttributes *ctt_attributes = NULL;
  Window ctt_window = 0;

  parent_attributes = calloc(1, sizeof(XWindowAttributes));
  ctt_attributes = calloc(1, sizeof(XSetWindowAttributes));

  Status s = XGetWindowAttributes(dpy, parent, parent_attributes);
  if(s == 0) {
    fprintf(stderr, "The call to XGetWindowAttributes failed!\n");
    return(0);
  }

  int ctt_xy_size = 16;
  int ctt_xpos = parent_attributes->width - ctt_xy_size;
  int ctt_ypos = 0;
  int ctt_border = 0;
  int ctt_depth = CopyFromParent;
  int ctt_class = CopyFromParent;
  Visual *ctt_visual = (Visual *)CopyFromParent;
  unsigned long ctt_attribute_mask = CWWinGravity;

  ctt_attributes->win_gravity = NorthEastGravity;
  ctt_window = XCreateWindow(dpy, parent, ctt_xpos, ctt_ypos,
			     ctt_xy_size, ctt_xy_size, ctt_border,
			     ctt_depth, ctt_class, ctt_visual,
			     ctt_attribute_mask, ctt_attributes);

  set_ctt_window_shape_and_background(dpy, ctt_window, ctt_attribute_mask,
				      ctt_attributes);
  return(ctt_window);
}

/**
 * Make the CTT window.
 *
 * Manufactures the window (button, really, but in X11, _everything_
 * is a window) that reacts to the user's click to close the parent to
 * the tray.  Returns the X11 window ID as the result.
 */
Window
ctt_make_window(Display *dpy, Window parent) {
  Window ctt_window = create_ctt_window(dpy, parent);

  // XXX: What's the return value for XSelectInput() mean?
  XSelectInput(dpy, ctt_window, ButtonPressMask | ButtonReleaseMask);
  XMapRaised(dpy, ctt_window);

  return(ctt_window);
}

static void
handle_x11_event(Display *dpy) {
  fprintf(stderr, "STUB: handle_x11_event\n");
  XEvent *event = calloc(1, sizeof(XEvent));
  XNextEvent(dpy, event);
  free(event);
}

static void
handle_alltray_event(Display *dpy) {
  fprintf(stderr, "STUB: handle_alltray_event\n");

  // Stub read: throw the bytes away.
  char *buf = malloc(4096);
  if(buf == NULL) abort();

  ssize_t bytes_read = read(STDIN_FILENO, buf, 4096);

  alltray_ctt_command *cmd = aci_parse_command(buf, bytes_read);
  if(cmd != NULL) {
    aci_interpret_command(dpy, cmd);
  } else {
    printf("NAK - COMMAND NOT PROCESSED\n");
  }

  free(buf);
}

/**
 * The main event loop for the program.
 */
static int
event_loop(Display *dpy) {
  fd_set read_list;
  fd_set err_list;

  while(1) {
    FD_ZERO(&read_list);
    FD_ZERO(&err_list);
    int xlib_fileno = ConnectionNumber(dpy);
    int master_list[2] = { STDIN_FILENO, xlib_fileno };
    int i = 0;

    for(i = 0; i < 2; i++) {
      FD_SET(master_list[i], &read_list);
      FD_SET(master_list[i], &err_list);
    }

    // Clear the local Xlib event queue
    int pending = XPending(dpy);
    if(pending > 0)
      for(i = 0; i < pending; i++)
	handle_x11_event(dpy);

    int status = select(master_list[1] + 1,
			&read_list, NULL, &err_list, NULL);
    if(status == -1) {
      fprintf(stderr, "select() failed (%d): %s\n", errno, strerror(errno));
      return(1); // XXX: Breaks the infinite loop.
    } else {
      if(FD_ISSET(STDIN_FILENO, &read_list)) {
	handle_alltray_event(dpy);
      } else if(FD_ISSET(xlib_fileno, &read_list)) {
	handle_x11_event(dpy);
      } else if(FD_ISSET(STDIN_FILENO, &err_list)) {
	fprintf(stderr, "STDIN has error\n");
      } else if(FD_ISSET(xlib_fileno, &err_list)) {
	fprintf(stderr, "xlib_fileno has error\n");
      } else {
	fprintf(stderr, "Michael Trausch doesn't understand select().\n");
	return(-1);
      }
    }
  }
}

/**
 * Program entry point.
 *
 * Note that standard input comes from the AllTray process, and
 * standard output goes to the AllTray process.  Therefore, stdin and
 * stdout are communication channels with AllTray.
 */
int
main(int argc, char *argv[]) {
  Display *dpy = XOpenDisplay(NULL);
  if(dpy == NULL) {
    printf("ERROR: XOpenDisplay() failed\n");
    exit(1);
  }

  int shape_ver_major = 0;
  int shape_ver_minor = 0;
  Bool shape_supported = XShapeQueryVersion(dpy, &shape_ver_major,
					    &shape_ver_minor);
  if(shape_supported == False) {
    // TODO: Fall back to uglier behavior instead of aborting.
    printf("ERROR: X11 SHAPE extension is unsupported\n");
    exit(1);
  }

  return(event_loop(dpy));
}
