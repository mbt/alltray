/* -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *
 * wm.c - Window manager functions
 * Copyright © 2009 Michael B. Trausch <mike@trausch.us>
 */
#include <alltray.h>

/*
 * This is a bit hashish, IMHO, and I don't really like it.  There has
 * to be a better, more general way to do this, but I don't really know
 * what that might be at the moment.
 *
 * However, hashish it may be, it is at least done in a single spot.
 */
typedef struct {
  gchar *wm_name;

  gboolean wm_reparents_children;
} wm_properties;

typedef struct {
  gboolean initialized;
  gchar *wm_name;
  Window wm_window;

  wm_properties *wm_properties;
} wm_internal_state;

static wm_internal_state internal_state;
static wm_properties known_wm_props[] = {
  { "compiz", TRUE },

  // "End of list" sentinel
  { NULL, FALSE }
};

static gboolean wm_is_ewmh_compliant(void);
static wm_properties *wm_find_properties(void);
static wm_properties *wm_try_detect_wm_properties(void);
static gboolean wm_try_detect_reparent(void);
static void wm_cleanup(void);

gboolean
alltray_wm_init() {
  gboolean retval = FALSE;
  
  memset(&internal_state, 0, sizeof(internal_state));
  
  internal_state.wm_window = alltray_wm_get_window();
  internal_state.wm_name = alltray_wm_get_name();
  
  if(wm_is_ewmh_compliant()) {
    retval = internal_state.initialized = TRUE;    
  }

  internal_state.wm_properties = wm_find_properties();
  atexit(wm_cleanup);
  return(retval);
}

/**
 * Return the window created by the currently-running window manager.
 *
 * This is also how we detect whether or not the currently-running
 * window manager supports the Extended Window Manager Hints (EMWH)
 * specification.
 *
 * @returns 0 if the currently-running WM does not support EMWH, or the
 * window ID if the currently-running WM supports EMWH and has thus
 * published its window ID.
 */
Window
alltray_wm_get_window() {
  Window root_window = alltray_x11_get_root_window();
  Window retval =
    alltray_x11_get_window_window_property(root_window,
                                           "_NET_SUPPORTING_WM_CHECK");
  return(retval);
}

/**
 * Return the name of the currently-running window manager.
 */
gchar *
alltray_wm_get_name() {
  gchar *retval = NULL;

  if(internal_state.wm_window != 0) {
    retval = alltray_x11_get_window_utf8_property(internal_state.wm_window,
                                                  "_NET_WM_NAME");
  } else {
    // We shouldn't ever reach this point, but just in case...

    g_printerr("%s has encountered an internal error.", PACKAGE_NAME);
    abort();
  }

  return(retval);
}

/**
 * Find the properties of the currently running window manager, if we
 * know them.  Provide a warning if we don't know what the currently
 * running window manager's properties are.
 *
 * @todo Try to discover the important properties of a window manager.
 * We really do not want to return a NULL pointer.
 */
static wm_properties *
wm_find_properties() {
  wm_properties *retval = NULL;

  for(wm_properties *cur = &known_wm_props[0]; cur->wm_name != NULL; cur++) {
    if(!strcmp(cur->wm_name, internal_state.wm_name)) {
      // We've found the properties for the currently running window manager.
      retval = g_memdup(cur, sizeof(wm_properties));
      break;
    }
  }

  if(retval == NULL) {
    g_printerr("I don't know anything about the currently running window\n"
               "manager.  %s will try to detect the properties of\n"
               "your window manager (%s).\n\n", 
               PACKAGE_NAME, internal_state.wm_name);
    g_printerr("However, %s may act strangely (or not work at all).\n"
               "Please file a bug report either way, so I don't say these\n"
               "things in future releases!", PACKAGE_NAME);
    retval = wm_try_detect_wm_properties();
  }

  return(retval);
}

/**
 * Run through a cycle of tests to attempt to detect window manager
 * behavior.  When new properties are added to the wm_properties
 * structure, tests for them should be implemented (see
 * wm_try_detect_reparent()) and then a call to those runtime tests
 * should be inserted into this routine.
 *
 * @returns A newly-allocated wm_properties structure with information
 * about the window manager.
 */
static wm_properties *
wm_try_detect_wm_properties() {
  wm_properties *retval = g_malloc0(sizeof(wm_properties));

  retval->wm_name = g_strdup(internal_state.wm_name);
  retval->wm_reparents_children = wm_try_detect_reparent();

  return(retval);
}

/**
 * Attempt to determine whether or not the currently running window
 * manager is a reparenting window manager.
 *
 * @returns TRUE if the window manager appears to reparent, FALSE if
 * not.
 */
static gboolean
wm_try_detect_reparent() {
  gboolean retval = TRUE;

  Window root_window = alltray_x11_get_root_window();
  Window new_window = alltray_x11_create_window(root_window, 0, 0, 0, 0);
  Window new_window_parent = alltray_x11_get_window_parent(new_window);

  if(new_window_parent == root_window) {
    retval = FALSE;
  }

  return(retval);
}

/**
 * Returns whether or not the window manager is (appears to be) EWMH-compliant.
 *
 * @returns TRUE if we think the window manager is EWMH-compliant, or
 * FALSE if not.
 */
static gboolean
wm_is_ewmh_compliant() {
  gboolean retval = TRUE;

  if(internal_state.wm_window == 0) retval = FALSE;
  if(internal_state.wm_name == NULL) retval = FALSE;

  return(retval);
}

/**
 * Cleans up after this module.
 */
static void
wm_cleanup() {
  g_free(internal_state.wm_name);
  internal_state.wm_name = 0;
}
