#include "common.h"

#ifndef __KDE_H__
#define __KDE_H__

gboolean kde_get_close_button_positon (void);
gboolean kde_show_configure_dialog (win_struct *win);
button kde_get_close_button (Window target, gboolean pos);

#endif
