/* -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *
 * x11.c - X Window System functions
 * Copyright © 2009 Michael B. Trausch <mike@trausch.us>
 */
#include <alltray.h>

typedef struct {
  gboolean initialized;
  Display *x11_display;
  Screen *x11_default_screen;
} x11_internal_state;

typedef struct {
  gchar *name;
  Atom interned_atom;
} x11_atom_node;

static x11_internal_state internal_state = { FALSE, NULL, NULL };
static GList *interned_atoms = NULL;

static void clear_atom_list(void);
static void local_atoms_init(void);
static void free_dynamic_storage(gpointer data, gpointer ignored);
static gboolean atom_compare(gconstpointer this, gconstpointer that);

static gchar *atoms[] = {
  "WINDOW",
  "UTF8_STRING",
  "MANAGER",

  "WM_DELETE_WINDOW",
  "WM_ICON",
  "WM_NAME",
  "WM_STATE",
  "WM_TAKE_FOCUS",

  "_NET_ACTIVE_WINDOW",
  "_NET_CLIENT_LIST_STACKING",
  "_NET_CLIENT_LIST",
  "_NET_CLOSE_WINDOW",
  "_NET_CURRENT_DESKTOP",
  "_NET_NUMBER_OF_DESKTOPS",
  "_NET_SUPPORTING_WM_CHECK",
  "_NET_WM_ICON",
  "_NET_WM_NAME",
  "_NET_WM_PING",
  "_NET_WM_PID",
  "_NET_WM_STATE",
  "_NET_WM_STATE_SKIP_PAGER",
  "_NET_WM_STATE_SKIP_TASKBAR",
  "_NET_WM_STATE_STICKY",
  "_NET_WM_DESKTOP",
  "_NET_WM_WINDOW_TYPE",
  "_NET_WM_WINDOW_TYPE_NORMAL",
  "_NET_WM_VISIBLE_NAME",

  "_NET_SYSTEM_TRAY",
  "_NET_SYSTEM_TRAY_OPCODE",

  "_ALLTRAY_MANAGED",
  "_ALLTRAY_MANAGER_PID",
  NULL
};

/**
 * Initializes the X11 module, opening a connection to an X11 server.
 *
 * @returns TRUE on success (X was present and connected to), FALSE on failure.
 */
gboolean
alltray_x11_init(const gchar *display_name) {
  gboolean retval = FALSE;

  DEBUG_X11("Attempting to initialize the display");
  internal_state.x11_display = XOpenDisplay(display_name);

  if(internal_state.x11_display) {
    DEBUG_X11("Succeeded in initializing the display");
    internal_state.x11_default_screen =
      XDefaultScreenOfDisplay(internal_state.x11_display);
    local_atoms_init();

    retval = internal_state.initialized = TRUE;
  }

  return(retval);
}

/**
 * Performs cleanup for the module when it is no longer going to be used.
 *
 * This function closes the connection to the X11 server that was
 * connected to in alltray_x11_init() and cleans up any data that we
 * have lingering around.  This function is intended to be used as an
 * atexit() callback, though it could conceivably be called manually.
 */
gboolean
alltray_x11_cleanup() {
  if(!internal_state.initialized) {
    DEBUG_X11("Warning: alltray_x11_cleanup() called, "
              "but X11 module not init'd!");
    return(FALSE);
  }

  internal_state.x11_default_screen = NULL;
  clear_atom_list();
  XCloseDisplay(internal_state.x11_display);

  internal_state.initialized = FALSE;
  return(TRUE);
}

/**
 * Fetches an atom from the list; if it does not exist, it interns it
 * and appends it to the list, and then fetches it.
 *
 * @returns An interned X11 atom.
 */
Atom
alltray_x11_get_atom(const gchar *atom_name) {
  Atom retval;
  x11_atom_node *data = g_malloc(sizeof(x11_atom_node));
  data->name = g_strdup(atom_name);
  GList *list_node = g_list_find_custom(interned_atoms, data, atom_compare);

  if(!list_node) {
    // Need to create the atom and add it to the list.
    data->interned_atom = XInternAtom(internal_state.x11_display,
                                      atom_name, False);
    interned_atoms = g_list_insert_sorted(interned_atoms, data, atom_compare);
    retval = data->interned_atom;
  } else {
    g_free(data->name);
    g_free(data);

    retval = ((x11_atom_node *)list_node->data)->interned_atom;
  }

  return(retval);
}

/**
 * Clears the list of atoms, freeing all storage used by the list.
 */
static void
clear_atom_list() {
  g_list_foreach(interned_atoms, free_dynamic_storage, NULL);
  g_list_free(interned_atoms);
}

/**
 * Frees the dynamic storage in an interned atom.
 */
static void
free_dynamic_storage(gpointer node, gpointer ignored) {
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
local_atoms_init() {
  Atom throwaway;

  DEBUG_X11("Attempting to initialize local atoms");
  gchar *atom_name;

  for(int i = 0, atom_name = atoms[i];
      atom_name != NULL; i++, atom_name = atoms[i]) {
    DEBUG_X11(g_strdup_printf("Initializing atom %s", atom_name));
    throwaway = alltray_x11_get_atom(atom_name);
  }
}

/**
 * Compare the first half of two x11_atom_node structures, the name.
 *
 * This function is used as a callback for the g_list_find_custom() and
 * g_list_insert_sorted() functions from GLib.
 */
static gboolean
atom_compare(gconstpointer a, gconstpointer b) {
  x11_atom_node *this = (x11_atom_node *)a;
  x11_atom_node *that = (x11_atom_node *)b;

  return(strcmp(this->name, that->name));
}
