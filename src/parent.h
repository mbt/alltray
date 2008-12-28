#include "common.h"

#ifndef __PARENT_H__
#define __PARENT_H__

gboolean parse_arguments(int argc, char **argv, gchar **icon,
    gchar  **rest, gboolean *show, gboolean *debug, gboolean *borderless, gboolean *sticky,
    gboolean *skip_tasklist, gboolean *no_title, gboolean *configure, gboolean *large_icons, 
    GArray *command_menu, gint *title_time, gchar **geometry,
    unsigned int *shortcut_key, unsigned int *shortcut_modifier, gboolean *notray, gboolean *nomini);


gchar *strip_command (win_struct *win);
void show_help(void);
void show_version(void);
void wait_for_manager(win_struct *win);
gboolean append_command_to_menu(GArray *command_menu,
  gchar *string);
  
void update_visibility_state (win_struct *win, gboolean new_state);

#endif
