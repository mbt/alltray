/*
 * alltray-ctt-helper.c - AllTray Close-To-Tray helper program
 * Copyright © 2011 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <X11/Xlib.h>
#include <X11/xpm.h>
#include <X11/extensions/Xext.h>
#include <X11/extensions/shape.h>
#include <../images/alltray_ctt.xpm>

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
static Window
make_ctt_window(Display *dpy, Window parent) {
  Window ctt_window = create_ctt_window(dpy, parent);

  // XXX: What's the return value for XSelectInput() mean?
  XSelectInput(dpy, ctt_window, ButtonPressMask | ButtonReleaseMask);

  return(ctt_window);
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

  // XXX: At this point we need to enter a loop...
  return(0);
}
