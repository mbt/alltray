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

typedef struct {
  unsigned char x11_code;
  AllTrayX11Error alltray_x11_code;
} x11_error_transtbl_row;

// Map X11 errors to internal errors that we handle.
static x11_error_transtbl_row x11_error_transtbl[] = {
  { BadRequest, ALLTRAY_X11_ERROR_BAD_REQUEST },
  { BadValue, ALLTRAY_X11_ERROR_BAD_VALUE },
  { BadWindow, ALLTRAY_X11_ERROR_BAD_WINDOW },
  { BadPixmap, ALLTRAY_X11_ERROR_BAD_PIXMAP },
  { BadAtom, ALLTRAY_X11_ERROR_BAD_ATOM },
  { BadCursor, ALLTRAY_X11_ERROR_BAD_CURSOR },
  { BadFont, ALLTRAY_X11_ERROR_BAD_FONT },
  { BadMatch, ALLTRAY_X11_ERROR_BAD_MATCH },
  { BadDrawable, ALLTRAY_X11_ERROR_BAD_DRAWABLE },
  { BadAccess, ALLTRAY_X11_ERROR_BAD_ACCESS },
  { BadAlloc, ALLTRAY_X11_ERROR_BAD_ALLOC },
  { BadColor, ALLTRAY_X11_ERROR_BAD_COLOR },
  { BadLength, ALLTRAY_x11_ERROR_BAD_LENGTH },
  { BadImplementation, ALLTRAY_X11_ERROR_BAD_IMPLEMENTATION },
  { 0, 0 }
};

static x11_error_internal_state internal_state;

static gint x11_error_handler(Display *d, XErrorEvent *evt);
static AllTrayX11Error x11_error_translate_code(unsigned char x11_error_code);

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
void
alltray_x11_error_install_handler() {
  if(internal_state.error_handler_installed) alltray_common_bug_detected();

  // Ensure that we won't catch any previously-generated errors.
  XFlush(internal_state.x11_display);

  internal_state.old_x11_error_handler = XSetErrorHandler(x11_error_handler);
  DEBUG_X11_ERROR("Installed X11 error handler");
}

/**
 * Uninstall the error handler.  Returns the count of errors
 * encountered from X while the handler was installed, and returns a
 * pointer to a GSList which contains the errors themselves.
 */
gint
alltray_x11_error_uninstall_handler(GSList **error_list) {
  if(!internal_state.error_handler_installed) alltray_common_bug_detected();

  XFlush(internal_state.x11_display);

  gint retval = g_slist_length(internal_state.error_list);
  *error_list = internal_state.error_list;
  internal_state.error_list = NULL;

  XSetErrorHandler(internal_state.old_x11_error_handler);
  internal_state.old_x11_error_handler = NULL;
  internal_state.error_handler_installed == FALSE;

  DEBUG_X11_ERROR(g_strdup_printf("Uninstalled X11 error handler (%d errors "
                                  "caught)", retval));
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
				  err->minor_code, (guint32)err->resourceid));
  DEBUG_X11_ERROR(g_strdup_printf("Error text: %s", error_message));

  AllTrayX11Error ape = x11_error_translate_code(err->error_code);
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

/**
 * Translate an X11 error code into an error code that we understand
 * internally and use within our error reporting system.
 */
static AllTrayX11Error
x11_error_translate_code(unsigned char x11_error_code) {
  AllTrayX11Error retval = 0;
  x11_error_transtbl_row *cur;

  for(cur = &x11_error_transtbl[0]; cur->x11_code != 0; cur++) {
    if(x11_error_code == cur->x11_code) retval = cur->alltray_x11_code;
  }

  if((x11_error_code >= FirstExtensionError) &&
     (x11_error_code <= LastExtensionError) && (retval == 0)) {
    retval = ALLTRAY_X11_ERROR_EXTENSION_FAILED;
  } else {
    retval = ALLTRAY_X11_ERROR_FAILED;
  }

  return(retval);
}
