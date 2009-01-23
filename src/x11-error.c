/* -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *
 * x11-error.c - X Window System error handler
 * Copyright © 2009 Michael B. Trausch <mike@trausch.us>
 */
#include <alltray.h>

typedef struct {
  gboolean initialized;
  gboolean error_handler_installed;
  guint error_handler_count;
  GSList *error_list;

  Display *x11_display;
  gint x11_screen_number;
  int (*old_x11_error_handler)(Display *, XErrorEvent *);
} x11_error_internal_state;

static x11_error_internal_state internal_state;

GQuark
alltray_x11_error_quark() {
  return(g_quark_from_static_string("alltray-x11-error-quark"));
}

void
alltray_x11_error_init(Display *disp, gint screen) {
  memset(&internal_state, 0, sizeof(x11_error_internal_state));
  internal_state.x11_display = disp;
  internal_state.x11_screen_number = screen;

  internal_state.initialized = TRUE;
}

/**
 * Install the error handler.  Care must be used in that the error
 * handler should not be called multiple times; that would be
 * indicitive of a programming error in the program.
 *
 * @returns TRUE if the error handler was installed, FALSE if the
 * error handler was already installed and this function was
 * redundantly called.
 */
gboolean
alltray_x11_error_install_handler() {
  if(internal_state.error_handler_installed == TRUE) {
    DEBUG_X11_ERROR("Warning: alltray_x11_error_install_handler() called with "
		    "no effect");
    return(FALSE);
  }

  internal_state.old_x11_error_handler = XSetErrorHandler(x11_error_handler);
  return(TRUE);
}

/**
 * Uninstall the error handler.  Returns the count of errors
 * encountered from X while the handler was installed, and returns a
 * pointer to a GSList which contains the errors themselves.
 */
static gint
alltray_x11_error_uninstall_handler(GSList **error_list) {
  if(internal_state.error_handler_installed == FALSE) {
    // Calling this function without the handler installed is a big bug.
    g_error("%s has encountered a programming error.\n\n"
	    "Please file a bug report at %s and\n"
	    "include the core dump, if possible.\n",
	    PACKAGE_NAME, PACKAGE_BUGREPORT);
    abort();
  }

  gint retval = g_slist_length(internal_state.error_list);
  *error_list = internal_state.error_list;
  internal_state.error_list = NULL;
  internal_state.error_handler_installed == FALSE;

  return(retval);
}

/**
 * The error handler which we will tell X to call upon receipt of an
 * error from the windowing system.
 */
static gint
x11_error_handler(Display *disp, XErrorEvent *err) {
  gchar *error_message = g_malloc(sizeof(char) * 133);
  XGetErrorText(disp, err->error_code, error_message, 132);

  DEBUG_X11_ERROR(g_strdup_printf("Received an error %d, request %d:%d, xid %d",
				  err->error_code, err->request_code,
				  err->minor_code, err->resourceid));
  DEBUG_X11_ERROR(g_strdup_printf("Error text: %s", error_message));

  AllTrayX11Error ape = x11_error_translate_code(err->code);
  GError *new_error = g_error_new_literal(ALLTRAY_X11_ERROR, ape,
					  error_message);
  internal_state.error_list = g_slist_append(internal_state.error_list,
					     (gpointer) new_error);

  /*
   * X Protocol documentation (man 3 XSetErrorHandler) states that the
   * return value from this function is ignored, so we'll just return 0.
   */
  return(0);
}
