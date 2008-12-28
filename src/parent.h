#include "common.h"

#ifndef __PARENT_H__
#define __PARENT_H__


gboolean parse_arguments(int argc, char **argv, gchar **icon,
    gchar  **rest, gint *show, gboolean *debug, gboolean *borderless,
    gboolean *large_icons, GArray *command_menu);
gchar *strip_command (win_struct *win);
void show_help(void);
void show_version(void);
void wait_for_manager(win_struct *win);
gboolean append_command_to_menu(GArray *command_menu,
  gchar *string);

#endif
