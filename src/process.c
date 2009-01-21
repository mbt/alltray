/* -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *
 * process.c - Process spawning/management functions
 * Copyright © 2009 Michael B. Trausch <mike@trausch.us>
 */
#include <alltray.h>

static gboolean process_spawn(char *argv[], GPid *pid);
static void process_install_ld_preload(void);
static void process_child_preexec(gpointer *data);
static gchar *process_arg_list_to_string(char *argv[]);

gboolean
alltray_process_spawn_new(char *argv[], GPid *child_pid) {
  process_install_ld_preload();
  return(process_spawn(argv, child_pid));
}

static gboolean
process_spawn(char *argv[], GPid *pid) {
  GPid child = 0;
  GError *error;
  gboolean retval;

  retval = g_spawn_async(NULL /* Inherit the current working directory */,
                         argv,
                         NULL /* Inherit the current environment */,
                         G_SPAWN_SEARCH_PATH,
                         (GSpawnChildSetupFunc)process_child_preexec,
                         NULL /* User data for process_set_child_environment */,
                         &child,
                         &error);

  if(retval == FALSE) {
    g_printerr("%s failed to spawn the target application:\n"
               "Command: %s\n",
               "Arguments: %s\n", PACKAGE_NAME, argv[1],
               process_arg_list_to_string(&argv[2]));
    g_printerr("Error returned by g_spawn_async:\n  %s\n",
               error->message);
    g_clear_error(&error);
  }

  *pid = child;
  return(retval);
}

/**
 * Converts a a NULL-terminated argument list into a command line, using
 * quote characters around each separate token so that humans can
 * understand what happened when they look at it.
 */
static gchar *
process_arg_list_to_string(char *argv[]) {
  GString *gs_retval;
  gchar *retval;

  gs_retval = g_string_new("");
  int i = 0;

  for(char *cur_str = argv[i]; argv[i] != NULL; i++) {
    if(i != 0) g_string_append_printf(gs_retval, " ");
    g_string_append_printf(gs_retval, "\"%s\"", argv[i]);
  }

  retval = g_string_free(gs_retval, FALSE);
  return(retval);
}

/**
 * Perform setup tasks which may be required within the child process.
 *
 * To be specific, this function is called sometime after we fork() and
 * sometime before we perform one of the exec() functions (which one is
 * actually determined by g_spawn_async(), not by us).
 */
static void
process_child_preexec(gpointer *data) {
  return;
}

/**
 * Installs (or updates) the LD_PRELOAD environment variable.
 *
 * This is required so that we can manage the client application
 * successfully.  This means, however, is that AllTray will _not_ work
 * when the user is starting a setuid or setgid program, or the user is
 * running on an operating system which does not support LD_PRELOAD (at
 * least HP-UX, possibly others).
 *
 * It would be nice if we could find a way to do this that could be done
 * (portably) on all systems without the use of LD_PRELOAD and with the
 * ability to control the application, even if setuid/setgid.  Actually,
 * there may be.
 *
 * What if AllTray eventually provided a proxy (at the X protocol level)
 * between a child application and the X11 server, kind of like SSH
 * does?  That is an interesting idea, though it may dramatically
 * increase the resource usage of AllTray.  It wouldn't make a bad
 * experimental branch of AllTray, though, if someone wanted to do that.
 */
static void
process_install_ld_preload() {
  gchar *env_ld_preload = getenv("LD_PRELOAD");
  gchar *new_env_value;

  if(env_ld_preload == NULL) {
    new_env_value = g_strdup_printf("%s", ALLTRAY_HELPER_LIBRARY);
  } else {
    new_env_value = g_strdup_printf("%s %s", env_ld_preload,
                                    ALLTRAY_HELPER_LIBRARY);
  }

  int status = setenv("LD_PRELOAD", new_env_value, TRUE);
  if(status == -1) {
    g_printerr("Error:  Out of environment space.\n"
               "This should never happen, bailing out.\n");
    abort();
  }
}
