#ifndef __BALLOON_MESSAGE_H__
#define __BALLOON_MESSAGE_H__


void show_balloon (win_struct *win, gchar *message, gint timeout);
void destroy_balloon (win_struct *win);

#endif
