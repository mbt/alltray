/* -*- mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *
 * process.c - Process spawning/management functions
 * Copyright © 2009 Michael B. Trausch <mike@trausch.us>
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <alltray.h>

static gboolean process_spawn(char *argv[], GPid *pid);
static void process_install_ld_preload(void);
static void process_child_preexec(gpointer *data);
static gchar *process_arg_list_to_string(char *argv[]);
static void process_install_ld_library_path(const gchar *lib_name);
static void process_prepend_to_ld_library_path(const gchar *path);
static gchar *process_find_library(const gchar *name);
static gboolean process_check_for_suid_or_guid(const gchar *cmd,
                                               GError **error_return);

GQuark
alltray_process_error_quark() {
  return(g_quark_from_static_string("alltray-process-error-quark"));
}

gboolean
alltray_process_spawn_new(char *argv[], GPid *child_pid) {
  process_install_ld_preload();
  return(process_spawn(argv, child_pid));
}

static gboolean
process_spawn(char *argv[], GPid *pid) {
  GPid child = 0;
  GError *error = NULL;
  gboolean retval;

  gboolean passes_checks = process_check_for_suid_or_guid(argv[0],
                                                          &error);

  if(!passes_checks) {
    gint error_code = error->code;
    gchar *error_message = g_strdup(error->message);
    g_error_free(error);
    error = NULL;

    switch(error_code) {
    case ALLTRAY_PROCESS_SUID_EXECUTABLE:
      g_printerr("Error: %s is a setuid executable, and as such may not\n"
                 "properly work under %s because the helper library\n"
                 "will not load for it.  If there are problems, you\n"
                 "can make the helper library setuid to the same user as the\n"
                 "executable, which may make this problem go away.\n"
                 "\nNot attempting to execute the program.\n",
                 argv[0], PACKAGE_NAME);
      break;

    case ALLTRAY_PROCESS_SGID_EXECUTABLE:
      g_printerr("Error: %s is a setgid executable, and as such may not\n"
                 "properly work under %s because the helper library\n"
                 "will not load for it.  If there are problems, you\n"
                 "can make the helper library setgid to the same user as the\n"
                 "executable, which may make this problem go away.\n"
                 "\nNot attempting to execute the program.\n",
                 argv[0], PACKAGE_NAME);
      break;

    case ALLTRAY_PROCESS_NOT_FOUND:
      g_printerr("Error: No executable file named %s could be found.\n",
                 argv[0]);
      break;

    default:
      g_error("%s has encountered a programming error.\n\n"
              "Please file a bug report at %s and\n"
              "include the core dump, if possible.\n",
              PACKAGE_NAME, PACKAGE_BUGREPORT);
      abort();
    }

    return(FALSE);
  }

  retval = g_spawn_async(NULL /* Inherit the current working directory */,
                         argv,
                         NULL /* Inherit the current environment */,
                         G_SPAWN_SEARCH_PATH | G_SPAWN_DO_NOT_REAP_CHILD,
                         (GSpawnChildSetupFunc)process_child_preexec,
                         NULL /* User data for process_set_child_environment */,
                         &child,
                         &error);

  if(retval == FALSE) {
    g_printerr("%s failed to spawn the target application:\n", PACKAGE_NAME);
    g_printerr("  Command: %s\n", argv[0]);
    g_printerr("  Arguments: %s\n\n", process_arg_list_to_string(&argv[1]));
    g_printerr("Error returned by g_spawn_async:\n  %s\n",
               error->message);
    g_clear_error(&error);
  }

  *pid = child;
  return(retval);
}

static gboolean
process_check_for_suid_or_guid(const gchar *argv0, GError **error) {
  gchar *full_name = g_find_program_in_path(argv0);
  struct stat program_stat_result;

  memset(&program_stat_result, 0, sizeof(struct stat));

  if(full_name == NULL) {
    *error = g_error_new(ALLTRAY_PROCESS_ERROR,
                         ALLTRAY_PROCESS_NOT_FOUND,
                         "The file %s was not found.", argv0);
    return(FALSE);
  }

  int status = stat(full_name, &program_stat_result);
  if(status == 0) {
    mode_t file_mode = program_stat_result.st_mode;
    if((file_mode & S_ISUID) == S_ISUID) {
      *error = g_error_new(ALLTRAY_PROCESS_ERROR,
                           ALLTRAY_PROCESS_SUID_EXECUTABLE,
                           "The file %s is setuid.", argv0);
      return(FALSE);
    } else if((file_mode & S_ISGID) == S_ISGID) {
      *error = g_error_new(ALLTRAY_PROCESS_ERROR,
                           ALLTRAY_PROCESS_SUID_EXECUTABLE,
                           "The file %s is setgid.", argv0);
      return(FALSE);
    }
  }

  return(TRUE);
}

/**
 * Callback function handling the death of a child process.
 */
void alltray_process_child_exited(GPid which, gint exit_status,
                                  gpointer alltray_main_loop) {
  DEBUG_PROCESS(g_strdup_printf("Child %d exited with status %d",
                                which, exit_status));
  GMainLoop *loop = (GMainLoop *)alltray_main_loop;

  g_main_loop_quit(loop);
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

  if(argv[0] != NULL) {
    for(char *cur_str = argv[i]; argv[i] != NULL; i++) {
      if(i != 0) g_string_append_printf(gs_retval, " ");
      g_string_append_printf(gs_retval, "\"%s\"", argv[i]);
    }
  } else {
    g_string_append_printf(gs_retval, "(none)");
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
  DEBUG_PROCESS("In child process now");
  DEBUG_PROCESS(g_strdup_printf("LD_LIBRARY_PATH: %s",
                                getenv("LD_LIBRARY_PATH")));
  DEBUG_PROCESS(g_strdup_printf("LD_PRELOAD: %s",
                                getenv("LD_PRELOAD")));
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
  process_install_ld_library_path(ALLTRAY_HELPER_LIBRARY);

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

/**
 * Configures LD_LIBRARY_PATH to contain the path to the AllTray helper
 * library, so that ld.so can find it when processing the LD_PRELOAD
 * variable.  This is necessarily hackish to support development and
 * testing in-tree if there is already a version of AllTray installed on
 * the host system.
 */
static void
process_install_ld_library_path(const gchar *lib_name) {
  gboolean found = FALSE;
  gchar *correct_path = process_find_library(lib_name);

  if(correct_path == NULL) {
    g_printerr("Error:  I could not find the helper library, which is\n"
               "required for proper operation.  Am I correctly installed?\n");
    abort();
  }

  process_prepend_to_ld_library_path(correct_path);
}

/**
 * Prepends our custom LD_LIBRARY_PATH value to the environment
 * variable, or sets it if it doesn't already exist.
 */
static void
process_prepend_to_ld_library_path(const gchar *path) {
  gchar *cur_ld_library_path = getenv("LD_LIBRARY_PATH");

  if(cur_ld_library_path == NULL) {
    setenv("LD_LIBRARY_PATH", path, FALSE);
  } else {
    gchar *repl_path = g_strdup_printf("%s:%s", path, cur_ld_library_path);
    setenv("LD_LIBRARY_PATH", repl_path, TRUE);
  }
}

/**
 * Finds the helper library and returns its path if it can be found, so
 * that AllTray can manage client programs.
 */
static gchar *
process_find_library(const gchar *lib_name) {
  gchar *retval = NULL;
  gchar *buf = NULL;
  gboolean found = FALSE;

  gchar *search_path[] = {
    "./../lib/.libs" /* for when running in project_dir/src */,
    ALLTRAY_HELPER_DIRECTORY,
    NULL
  };

  gchar *cur;
  int i;

  for(i = 0, cur = search_path[i];
      cur != NULL; i++, cur = search_path[i]) {
    buf = g_strdup_printf("%s/%s", cur, ALLTRAY_HELPER_LIBRARY);
    DEBUG_PROCESS(g_strdup_printf("Seeking helper lib in %s", cur));

    struct stat stat_result;
    memset(&stat_result, 0, sizeof(struct stat));

    int result = stat(buf, &stat_result);
    if(result == -1) {
      DEBUG_PROCESS(g_strdup_printf("Did not find it in %s", cur));
      continue;
    } else {
      DEBUG_PROCESS(g_strdup_printf("********  Found it in %s", cur));
      found = TRUE;
      retval = g_malloc(PATH_MAX);
      retval = realpath(cur, retval);
      break;
    }
  }

  return(retval);
}
