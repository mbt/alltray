/* -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *
 * x11.c - X Window System functions
 * Copyright © 2009 Michael B. Trausch <mike@trausch.us>
 */
#include <alltray.h>

typedef struct {
  gboolean initialized;
  Display *x11_display;
  int x11_default_screen;
} x11_internal_state;

typedef struct {
  gchar *name;
  Atom interned_atom;
} x11_atom_node;

static x11_internal_state internal_state;
static GList *interned_atoms = NULL;

static void x11_clear_atom_list(void);
static void x11_local_atoms_init(void);
static void x11_free_dynamic_storage(gpointer data, gpointer ignored);
static gboolean x11_atom_compare(gconstpointer this, gconstpointer that);
static void x11_cleanup(void);

static gchar *atoms[] = {
  "CARDINAL",
  "MANAGER",
  "UTF8_STRING",
  "WINDOW",

  "WM_CLIENT_MACHINE",
  "WM_DELETE_WINDOW",
  "WM_ICON",
  "WM_NAME",
  "WM_STATE",
  "WM_TAKE_FOCUS",

  "_NET_ACTIVE_WINDOW",
  "_NET_CLIENT_LIST",
  "_NET_CLIENT_LIST_STACKING",
  "_NET_CLOSE_WINDOW",
  "_NET_CURRENT_DESKTOP",
  "_NET_NUMBER_OF_DESKTOPS",
  "_NET_SUPPORTING_WM_CHECK",

  "_NET_WM_DESKTOP",
  "_NET_WM_ICON",
  "_NET_WM_NAME",
  "_NET_WM_PID",
  "_NET_WM_PING",
  "_NET_WM_STATE",
  "_NET_WM_STATE_SKIP_PAGER",
  "_NET_WM_STATE_SKIP_TASKBAR",
  "_NET_WM_STATE_STICKY",
  "_NET_WM_VISIBLE_NAME",
  "_NET_WM_WINDOW_TYPE",
  "_NET_WM_WINDOW_TYPE_NORMAL",

  "_NET_SYSTEM_TRAY_OPCODE",

  "_ALLTRAY_MANAGED",
  "_ALLTRAY_MANAGER_PID",
  NULL
};

/**
 * Initializes the X11 module, opening a connection to an X11 server.
 *
 * @returns TRUE on success (X was present and connected to), FALSE on
 * failure.
 */
gboolean
alltray_x11_init(const gchar *display_name) {
  gboolean retval = FALSE;
  memset(&internal_state, 0, sizeof(internal_state));

  DEBUG_X11("Attempting to initialize the display");
  internal_state.x11_display = XOpenDisplay(display_name);

  if(internal_state.x11_display) {
    DEBUG_X11("Succeeded in initializing the display");
    internal_state.x11_default_screen =
      XDefaultScreen(internal_state.x11_display);

    x11_local_atoms_init();

    retval = internal_state.initialized = TRUE;
  }

  alltray_x11_error_init(internal_state.x11_display,
                         internal_state.x11_default_screen);

  atexit(x11_cleanup);
  return(retval);
}

/**
 * Fetches an atom from the list; if it does not exist, it interns it
 * and appends it to the list, and then fetches it.  If this function
 * is called prior to alltray_x11_init(), it will call
 * alltray_common_bug_detected() (which does not return) as there is
 * an obvious program bug present.
 *
 * @returns An interned X11 atom.
 */
Atom
alltray_x11_get_atom(const gchar *atom_name) {
  if(!internal_state.x11_display) alltray_common_bug_detected();

  Atom retval;
  x11_atom_node *data = g_malloc(sizeof(x11_atom_node));
  data->name = g_strdup(atom_name);
  GList *list_node = g_list_find_custom(interned_atoms, data, x11_atom_compare);
  int error_count;
  GSList *error_list = NULL;

  if(!list_node) {
    // Need to create the atom and add it to the list.
    alltray_x11_error_install_handler();
    data->interned_atom = XInternAtom(internal_state.x11_display,
                                      atom_name, False);
    error_count = alltray_x11_error_uninstall_handler(&error_data);

    if(error_count == 0) {
      interned_atoms = g_list_insert_sorted(interned_atoms, data, 
                                            x11_atom_compare);
      retval = data->interned_atom;
    } else {
      // We encountered error interning the atom.  We may not be able
      // to continue.

      for(GSList *err_item = error_data;
          err_item != NULL;
          err_item = err_item->next) {
        GError *err = (GError *)err_item->data;

        switch((AllTrayX11Error)err->code) {
        case ALLTRAY_X11_ERROR_BAD_ALLOC:
          g_printerr("Received a BadAlloc error in response to an XInternAtom "
                     "request.\n"
                     "This means the X server probably ran out of memory.\n"
                     "You may need to adjust your X server settings, add more "
                     "memory which X can\n"
                     "use, or there may be a bug in your X11 server.\n\n"
                     "%s cannot continue.\n", PACKAGE_NAME);
          exit(ALLTRAY_EXIT_X11_ERROR);
          break;

        default:
          alltray_common_bug_detected();
          break;
        }
      }
    }
  } else {
    // We already have the atom, we can free our newly-allocated structure
    // and just return the one we already have in memory.
    g_free(data->name);
    g_free(data);

    retval = ((x11_atom_node *)list_node->data)->interned_atom;
  }

  return(retval);
}

/**
 * Get the X11 root window.
 */
Window
alltray_x11_get_root_window() {
  Window retval = XRootWindow(internal_state.x11_display,
                              internal_state.x11_default_screen);
  return(retval);
}

/**
 * Get the X11 default screen number.
 */
gint
alltray_x11_get_default_screen() {
  return(internal_state.x11_default_screen);
}

/**
 * Create a new X11 window in a simple fashion.
 *
 * @returns The Window ID of the newly-created X11 Window.
 */
Window
alltray_x11_create_window(Window parent, int location_x, int location_y,
                          int width, int height) {
  Window retval =
    XCreateSimpleWindow(internal_state.x11_display, parent,
                        location_x, location_y, width, height, 0, 0, 0);

  return(retval);
}

Window
alltray_x11_get_window_parent(Window win) {
  Window root_return, parent_return;
  Window *children_return;
  guint number_of_children;

  XQueryTree(internal_state.x11_display, win, &root_return, &parent_return,
             &children_return, &number_of_children);

  if(number_of_children > 0) {
    XFree(children_return);
  }

  return(parent_return);
}

/**
 * [Convenience function] -- Returns the name of a given X11 Window.
 */
gchar *
alltray_x11_get_window_name(Window win) {
  gchar *retval;

  retval = alltray_x11_get_window_utf8_property(win, "_NET_WM_VISIBLE_NAME");
  if(!retval) {
    DEBUG_X11("_NET_WM_VISIBLE_NAME had no value, trying next method...");
    retval = alltray_x11_get_window_utf8_property(win, "_NET_WM_NAME");
  }

  if(!retval) {
    DEBUG_X11("_NET_WM_NAME had no value, trying next method...");
    GList *text_property_name = NULL;
    text_property_name = alltray_x11_get_window_text_property(win, "WM_NAME");

    if(!text_property_name) {
      DEBUG_X11("WM_NAME had no value, program didn't set a name!");
      retval = g_strdup("(unknown window name)");
    } else {
      retval = g_strdup(((gchar *)g_list_first(text_property_name)->data));
    }
  }

  return(retval);
}

/**
 * Returns a cardinal property, cast to a gint.
 */
gint
alltray_x11_get_window_cardinal_property(Window win, const gchar *prop_name) {
  union {
    gint8 card8;
    gint16 card16;
    gint32 card32;
  } cardinal_property;

  gint retval, x11_status;
  Atom prop_atom = alltray_x11_get_atom(prop_name);
  Atom req_type = alltray_x11_get_atom("CARDINAL");
  Atom actual_type_return;
  gint actual_format_return;
  gulong num_items_return, bytes_after_return;
  guchar *property_return;

  memset(&cardinal_property, 0, sizeof(cardinal_property));
  x11_status = XGetWindowProperty(internal_state.x11_display, win,
                                  prop_atom, 0, G_MAXLONG, False,
                                  req_type, &actual_type_return,
                                  &actual_format_return, &num_items_return,
                                  &bytes_after_return, &property_return);

  switch(actual_format_return) {
  case 8:
    cardinal_property.card8 = *((gint8 *)property_return);
    retval = (gint)cardinal_property.card8;
    break;
  case 16:
    cardinal_property.card16 = *((gint16 *)property_return);
    retval = (gint)cardinal_property.card16;
    break;
  case 32:
    cardinal_property.card32 = *((gint32 *)property_return);
    retval = (gint)cardinal_property.card32;
    break;
  default:
    g_printerr("%s has encountered an internal error.", PACKAGE_NAME);
    abort();
  }

  XFree(property_return);
  return(retval);
}

/**
 * [Convenience function] -- Returns the Window of a given property on
 * an X11 Window.
 */
Window
alltray_x11_get_window_window_property(Window win, const gchar *prop_name) {
  Window retval;

  gint x11_status = 0;
  Atom prop_atom = alltray_x11_get_atom(prop_name);
  Atom req_type = alltray_x11_get_atom("WINDOW");
  Atom actual_type_return;
  gint actual_format_return;
  gulong num_items_return, bytes_after_return;
  guchar *property_return;

  x11_status = XGetWindowProperty(internal_state.x11_display, win,
                                  prop_atom, 0, G_MAXLONG, False,
                                  req_type, &actual_type_return,
                                  &actual_format_return, &num_items_return,
                                  &bytes_after_return, &property_return);
  retval = *((long *)property_return);
  XFree(property_return);

  return(retval);
}

/**
 * Returns a list of strings associated with a text property for the
 * specified X11 Window.
 */
GList *
alltray_x11_get_window_text_property(Window win, const gchar *prop_name) {
  XTextProperty ret_text_property;
  Atom prop_atom;
  Status x11_status;
  GList *retval = NULL;

  prop_atom = alltray_x11_get_atom(prop_name);
  x11_status = XGetTextProperty(internal_state.x11_display,
                                win, &ret_text_property, prop_atom);
  if(!x11_status) {
    g_critical("X11 returned an error while getting text properties");
  } else {
    gchar **text_list;
    int text_list_count;

    x11_status = XTextPropertyToStringList(&ret_text_property,
                                           &text_list, &text_list_count);
    for(int i = 0; i < text_list_count; i++) {
      retval = g_list_append(retval, g_strdup(text_list[i]));
    }

    XFreeStringList(text_list);
  }

  return(retval);
}

/**
 * Returns the owner Window of an X11 selection.
 */
Window
alltray_x11_get_selection_owner(const gchar *selection_name) {
  Atom selection_atom = alltray_x11_get_atom(selection_name);
  return(XGetSelectionOwner(internal_state.x11_display, selection_atom));
}

/**
 * Returns a string property associated with an X11 Window.
 */
gchar *
alltray_x11_get_window_string_property(Window win, const gchar *prop_name) {
  int x11_status = 0;
  Atom prop_atom = alltray_x11_get_atom(prop_name);
  Atom req_type = alltray_x11_get_atom("STRING");
  Atom actual_type_return;
  int actual_format_return;
  unsigned long num_items_return, bytes_after_return;
  unsigned char *property_return;

  gchar *retval = NULL;
  x11_status = XGetWindowProperty(internal_state.x11_display, win, prop_atom,
                                  0, G_MAXLONG, False, req_type,
                                  &actual_type_return,
                                  &actual_format_return,
                                  &num_items_return,
                                  &bytes_after_return, &property_return);

  if((actual_type_return != req_type) &&
     (property_return != NULL)) {
    DEBUG_X11(g_strdup_printf("Wanted %s, got %s",
                              alltray_x11_get_atom_name(prop_atom),
                              alltray_x11_get_atom_name(actual_type_return)));
    retval = g_strdup((gchar *)property_return);
  } else if(property_return != NULL) {
    retval = g_strdup((gchar *)property_return);
  } else {
    retval = NULL;
  }

  if(property_return != NULL) XFree(property_return);
  return(retval);
}

/**
 * Converts an atom into a string.
 */
gchar *
alltray_x11_get_atom_name(Atom atom) {
  return(XGetAtomName(internal_state.x11_display, atom));
}

/**
 * Returns a UTF-8 string property associated with an X11 Window.
 */
gchar *
alltray_x11_get_window_utf8_property(Window win, const gchar *prop_name) {
  int x11_status = 0;
  Atom prop_atom = alltray_x11_get_atom(prop_name);
  Atom req_type = alltray_x11_get_atom("UTF8_STRING");
  Atom actual_type_return;
  int actual_format_return;
  unsigned long num_items_return;
  unsigned long bytes_after_return;
  unsigned char *property_return;

  gchar *retval = NULL;

  x11_status = XGetWindowProperty(internal_state.x11_display, win, prop_atom,
                                  0, G_MAXINT32, False, req_type,
                                  &actual_type_return,
                                  &actual_format_return,
                                  &num_items_return,
                                  &bytes_after_return,
                                  &property_return);

  if(actual_type_return == None) {
    retval = NULL;
  } else if(actual_type_return != req_type) {
    retval = NULL;
  } else if(actual_format_return != 8) {
    retval = NULL;
  } else {
    retval = g_strdup((char *)property_return);
    XFree(property_return);
  }

  return(retval);
}

/**
 * Clears the list of atoms, freeing all storage used by the list.
 */
static void
x11_clear_atom_list() {
  g_list_foreach(interned_atoms, x11_free_dynamic_storage, NULL);
  g_list_free(interned_atoms);
}

/**
 * Frees the dynamic storage in an interned atom.
 */
static void
x11_free_dynamic_storage(gpointer node,
                         gpointer ignored __attribute__((unused))) {
  x11_atom_node *data = (x11_atom_node *)node;
  g_free(data->name);
  g_free(data);
}

/** 
 * Initializes the list of "common" atoms.
 *
 * This function is used during the initialization of this module so as
 * to avoid any unnecessary round-trips with the X Window System server
 * after startup.  The list of atoms initialized comes from the static
 * atoms array; to add more atoms to the list, add them there.
 */
static void
x11_local_atoms_init() {
  Atom throwaway;
  int i;
  gchar *atom_name;

  for(i = 0, atom_name = atoms[i];
      atom_name != NULL; i++, atom_name = atoms[i]) {
    throwaway = alltray_x11_get_atom(atom_name);
  }

  DEBUG_X11("Atoms initialized");
}

/**
 * Compare the first half of two x11_atom_node structures, the name.
 *
 * This function is used as a callback for the g_list_find_custom() and
 * g_list_insert_sorted() functions from GLib.
 */
static gboolean
x11_atom_compare(gconstpointer a, gconstpointer b) {
  x11_atom_node *this = (x11_atom_node *)a;
  x11_atom_node *that = (x11_atom_node *)b;

  return(strcmp(this->name, that->name));
}

/**
 * Performs cleanup for the module when it is no longer going to be used.
 *
 * This function closes the connection to the X11 server that was
 * connected to in alltray_x11_init() and cleans up any data that we
 * have lingering around.  This function is intended to be used as an
 * atexit() callback, though it could conceivably be called manually.
 */
static void
x11_cleanup() {
  if(!internal_state.initialized) {
    DEBUG_X11("Warning: alltray_x11_cleanup() called, "
              "but X11 module not initialized!");
    return;
  }

  internal_state.x11_default_screen = 0;
  x11_clear_atom_list();
  XCloseDisplay(internal_state.x11_display);

  internal_state.initialized = FALSE;
  return;
}
