#include "common.h"

#ifndef __SHORTCUT_H__
#define __SHORTCUT_H__


gboolean parse_shortcut (gchar *string, 
  unsigned int *key_return, unsigned int *modifier_return);

void shortcut_init (win_struct *win);
void shortcut_done (win_struct *win);
void check_if_pointer_is_over_button (win_struct *win);

#endif
