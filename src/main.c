/*
 * GPL Notice:
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Library General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *
 * Name:
 *
 *    alltray
 *
 *
 * Copyright:
 *
 *    Jochen Baier, 2004, 2005, 2006 (email@Jochen-Baier.de)
 *
 *
 * Based on code from:
 *
 *    steal-xwin.c (acano@systec.com)
 *    xswallow (Caolan McNamara ?)
 *    kdocker (Girish Ramakrishnan)
 *    libwnck (Havoc Pennington <hp@redhat.com>)
 *    eggtrayicon (Anders Carlsson <andersca@gnu.org>)
 *    dsimple.c ("The Open Group")
 *    xfwm4 (Olivier Fourdan <fourdan@xfce.org>)
 *    .....lot more, THANX !!!
 *
*/


#include "config.h"

#include "common.h"
#include "parent.h"
#include "child.h"
#include "utils.h"
#include "trayicon.h"
#include "clickmode.h"
#include "grab.h"
#include "kde.h"
#include "binreloc.h"
#include "shortcut.h"
#include "eventfilter.h"
#include "gnome_theme.h"

gboolean debug=FALSE;

void win_struct_init(win_struct *win)
{

  win->display=GDK_DISPLAY();
  win->root_xlib=GDK_ROOT_WINDOW();
  win->root_gdk=gdk_screen_get_root_window (gdk_screen_get_default());

  win->parent_is_visible=FALSE;

  win->screen_width=gdk_screen_get_width (gdk_screen_get_default());
  win->screen_height=gdk_screen_get_height (gdk_screen_get_default());

  win->initial_x=0;
  win->initial_y=0;
  win->initial_w=0;
  win->initial_h=0;
  win->geo_bitmask=0;
  win->parent_gdk=NULL;
  win->parent_xlib=None;

  win->libspy_window=None;
  win->libspy_window_gdk=NULL;

  win->visibility=VisibilityUnobscured;

  win->command_menu=g_array_new (FALSE, FALSE, sizeof (command_menu_struct));

  win->show=FALSE;
  win->command=NULL;
  win->command_only=NULL;
  win->child_xlib=None;
  win->borderless=FALSE;
  win->large_icons=FALSE;

  win->parent_pid=getpid();
  win->child_pid=0;
  if (debug) printf ("win->parent_pid: %d\n", win->parent_pid);

  win->manager_window=None;

  win->plug=NULL;
  win->image_icon=NULL;
  win->title=NULL;

  win->window_icon=NULL;
  win->tray_icon=NULL;

  win->user_icon=NULL;
  win->user_icon_path=NULL;

  win->balloon=NULL;
  win->balloon_message_allowed=FALSE;

  win->xmms=FALSE;
  win->xmms_main_window_xlib=None;
  win->xmms_main_window_gdk=NULL;

  win->xmms_playlist_window_xlib=None;
  win->xmms_equalizer_window_xlib=None;

  win->title_time=0;

  win->click_mode=FALSE;

  win->window_manager=NULL;
  win->gnome=FALSE;
  win->kde=FALSE;

  win->no_reparent=FALSE;

  win->target_our_xlib=None;
  win->target_our_gdk=NULL;

  win->target_our_w=0;
  win->target_our_h=0;

  win->target_right_border=0;
  win->target_above_border=0;

  win->normal_map=FALSE;

  win->sticky=FALSE;
  win->skip_tasklist=FALSE;
  win->no_title=FALSE;

  win->kde_close_button_pos =NO_SUCCESS;

  win->shortcut_key=0;
  win->shortcut_modifier=0;

  win->notray = 0;

  win->nomini = 0;


}

void command_line_init (win_struct *win, int argc, char **argv)
{

  gchar *geometry=NULL;
  gboolean configure=FALSE;
  gchar *gnome_terminal_start=NULL;
  gchar *old_command=NULL;
  gchar *tmp_start=NULL;
  gchar *tmp_start_gnome_terminal=NULL;
  gchar *tmp_end=NULL;

  win->window_manager=get_window_manager();

  if (debug) printf ("window manager: %s\n", win->window_manager);

  if (!strcmp(win->window_manager, "Metacity")) {

    #ifndef GCONF_NOT_DISABLED
          printf ("\n\nAlltray: To use Alltray under Gnome, Gconf support should not be disabled.\n"
                  "         But it is.\n\n");
          exit (0);
    #endif

    win->gnome=TRUE;
    win->no_reparent=TRUE;
  }

  if (!strcmp(win->window_manager, "KWin")) {
     win->kde=TRUE;
     win->no_reparent=TRUE;
     win->kde_close_button_pos = kde_get_close_button_positon ();
  }

  g_free (win->window_manager);

  if (argc==1 || (debug=(argc == 2 && !strcmp (argv[1], "-d")))) {

    if (debug) printf ("click_mode\n");
    win->click_mode=TRUE;

    if (win->kde && win->kde_close_button_pos == NO_SUCCESS) {
       if (!kde_show_configure_dialog (win)) {
         printf ("Configuration canceled. \"alltray -conf\" to reshow.\n");
         exit (1);
       }
    }

      return;
  }

  if (!parse_arguments(argc, argv, &win->user_icon_path,
    &win->command, &win->show, &debug, &win->borderless, &win->sticky, &win->skip_tasklist,
    &configure, &win->large_icons, win->command_menu, &win->title_time, &geometry,
    &win->shortcut_key, &win->shortcut_modifier, &win->notray, &win->nomini)) {

    if (win->user_icon_path)
      g_free(win->user_icon_path);
    if (win->command)
      g_free (win->command);

    if (geometry)
      g_free (geometry);


    if (win->command_menu)
      free_command_menu (win->command_menu);

    g_free(win);
    exit(1);
  }

  if (configure && !win->kde) {
    printf ("\n\nAlltray: \"--configure\" option only usefull for the KDE desktop.\n\n");
    exit (0);
  }

  if (win->borderless)
    win->nomini=TRUE;

  if (win->nomini) {

    win->gnome=FALSE;
    win->kde=FALSE;
    win->no_reparent=TRUE;
  }


  if (win->kde) {

    if (win->kde_close_button_pos == NO_SUCCESS || configure) {

     if (!kde_show_configure_dialog (win)) {
    printf ("Configuration canceled. \"alltray -conf\" to reshow.\n");

    if (win->user_icon_path)
      g_free(win->user_icon_path);
    if (win->command)
      g_free (win->command);

    if (geometry)
      g_free (geometry);

    if (win->command_menu)
      free_command_menu (win->command_menu);

     exit (1);
    }

   }

  }


  if (win->user_icon_path)
    win->user_icon=get_user_icon (win->user_icon_path, 30, 30);

  gnome_terminal_start=strstr (win->command, "gnome-terminal");
  if (gnome_terminal_start && !strstr (win->command, "disable-factory")) {

    if (debug) printf ("gnome_terminal_start: <%s>\n", gnome_terminal_start);

    old_command=win->command;
    tmp_end=gnome_terminal_start+14;
    if (debug) printf ("tmp_end: <%s>\n", tmp_end);

    old_command=win->command;
    tmp_end=gnome_terminal_start+14;
    if (debug) printf ("tmp_end: <%s>\n", tmp_end);

    tmp_start=g_strdup (win->command);
    tmp_start_gnome_terminal=strstr (tmp_start, "gnome-terminal");
    *(tmp_start_gnome_terminal + 14)=0;
    if (debug) printf ("tmp_start: <%s>\n", tmp_start);

    win->command=g_strconcat (tmp_start, " --disable-factory", tmp_end, NULL);

    g_free (old_command);
    g_free (tmp_start);

  }

  win->command_only=strip_command(win);

  if (!strcmp (win->command_only, "xmms")) {
    win->xmms=TRUE;
    win->no_reparent=TRUE;
    if (debug) printf ("child is xmms\n");
  }


  if (geometry) {

    win->geo_bitmask=XParseGeometry(geometry, &win->initial_x, &win->initial_y,
              (unsigned int *) &win->initial_w, (unsigned int *) &win->initial_h);

    if (debug) printf ("inital values: x:%d y: %d, w:%d, h:%d\n",
                        win->initial_x, win->initial_y, win->initial_w, win->initial_h);

  }

  if (debug) {
    printf ("command: %s\n", win->command);
    if (win->show) printf ("show=TRUE\n");
    if (win->user_icon) printf ("have user icon\n");
    if (win->borderless) printf ("borderless=TRUE\n");
    printf ("win->title_time: %d\n", win->title_time);
    if (geometry) printf ("geometry: %s\n", geometry);
    if (win->sticky) printf ("sticky=TRUE\n");
    if (win->skip_tasklist) printf ("skip tasklist=TRUE\n");
    if (win->no_title) printf ("keep original title\n");

  }

}

int
main (int argc, char *argv[])
{

  XClassHint *classHint;
  XWMHints *wmhints;
  XWMHints *leader_change;
  XSizeHints sizehints;
  glong supplied_return;
  Atom protocols[3];
  gint err=0;
  gint w,h;

  gtk_init (&argc, &argv);

  #ifdef GCONF_NOT_DISABLED
    gconf_init(argc, argv, NULL);
  #endif

  gdk_pixbuf_xlib_init (GDK_DISPLAY(), DefaultScreen (GDK_DISPLAY()));
  gbr_init (NULL);
  atom_init ();

  win_struct *win = malloc (sizeof (win_struct));

  win_struct_init (win);
  command_line_init (win, argc, argv);


  if (debug) printf ("win->notray: %d\n", win->notray);

  if (!win->click_mode && !win->notray)
    wait_for_manager(win);

  if (!win->click_mode)
    exec_and_wait_for_window(win);
  else
    click_mode(win);

  if (win->xmms) {
    win->xmms_main_window_gdk=gdk_window_foreign_new(win->xmms_main_window_xlib);
    append_command_to_menu(win->command_menu, "Play:xmms -p");
    append_command_to_menu(win->command_menu, "Stop:xmms -s");
    append_command_to_menu(win->command_menu, "Pause:xmms -u");
    append_command_to_menu(win->command_menu, "Next:xmmsnext");
  }

  win->child_gdk=gdk_window_foreign_new(win->child_xlib);

  if (win->borderless) {
      gdk_window_set_decorations(win->child_gdk, 0);
      gtk_sleep (100);
  }


  if (win->no_reparent && win->normal_map && win->geo_bitmask) {

    if (debug) printf ("normap map geo_move\n");

    geo_move (win->child_gdk, win->screen_width, win->screen_height,
        win->initial_x, win->initial_y, win->initial_h, win->initial_w, win->geo_bitmask);
  }


  if (!win->show && ((win->click_mode && win->no_reparent) || win->normal_map))
    show_hide_window (win, force_hide, FALSE);

  if (win->show && win->no_reparent)
    win->parent_is_visible=TRUE;

  if (!win->no_reparent) {

     if (debug) printf ("need to reparent\n");

    win->child_gdk=gdk_window_foreign_new(win->child_xlib);

    gdk_error_trap_push ();
    leader_change = XGetWMHints(GDK_DISPLAY(), win->child_xlib);
    err=gdk_error_trap_pop ();

    if (err==0 && leader_change!=NULL) {

      if (debug) printf ("leader change\n");

      leader_change->flags = (leader_change->flags | WindowGroupHint);
      leader_change->window_group = GDK_ROOT_WINDOW();
      XSetWMHints(GDK_DISPLAY(),win->child_xlib, leader_change);
      XFree(leader_change);
    }

    withdraw_window(win);

    get_child_size (win->child_gdk, &w,&h);

    if (win->initial_w != 0)
      w=win->initial_w;

    if (win->initial_h != 0)
      h=win->initial_h;

    if (win->initial_h || win->initial_w)
      gdk_window_resize (win->child_gdk, w, h);

    win->parent_xlib= XCreateSimpleWindow(win->display, win->root_xlib, win->initial_x,
      win->initial_y, w, h, 0, 0, 0);

    classHint=XAllocClassHint();
    classHint->res_name="alltray";
    classHint->res_class="Alltray";
    XSetClassHint(win->display, win->parent_xlib, classHint);
    XFree (classHint);

    wmhints=XAllocWMHints();
    wmhints->input=False;
    wmhints->initial_state=NormalState;
    wmhints->flags = InputHint | StateHint;
    XSetWMHints(win->display, win->parent_xlib, wmhints);
    XFree (wmhints);

    gdk_error_trap_push();
    XGetWMNormalHints(win->display, win->child_xlib, &sizehints,
        &supplied_return);
    err=gdk_error_trap_pop();

    if (err==0) {

      if (win->initial_x || win->initial_y || win->initial_w || win->initial_h)
        sizehints.flags |= USPosition;

      XSetWMNormalHints(win->display, win->parent_xlib, &sizehints);
    }

    protocols[0]=wm_delete_window;
    protocols[1]=wm_take_focus;
    protocols[2]=net_wm_ping;

    XSetWMProtocols (win->display, win->parent_xlib, protocols, 3);

    XChangeProperty (GDK_DISPLAY(), win->parent_xlib,
      net_wm_pid, XA_CARDINAL, 32, PropModeReplace,
      (guchar *)&win->parent_pid, 1);

    XChangeProperty (GDK_DISPLAY(), win->parent_xlib,
      net_wm_window_type, XA_ATOM, 32, PropModeReplace,
      (guchar *)&net_wm_window_type_normal, 1);

    win->parent_gdk= gdk_window_foreign_new (win->parent_xlib);

    XReparentWindow (GDK_DISPLAY(), win->child_xlib, win->parent_xlib, 0, 0);
    XSync (GDK_DISPLAY(), FALSE);

    XMapWindow (GDK_DISPLAY(), win->child_xlib);
    XSync (GDK_DISPLAY(), FALSE);
  }

  update_window_icon(win);

  if (!win->notray)
    tray_init (win);

  update_window_title(win);

  if (win->skip_tasklist)
   skip_taskbar (win, TRUE);


   if (win->no_reparent) {

    if (win->gnome) {

      if (!parse_theme (win)) {
        printf ("*** parsing theme failed ! oh oh ***\n");
        exit (1);
      }
    }

    if (win->xmms) {
       gdk_window_set_events(win->xmms_main_window_gdk, GDK_VISIBILITY_NOTIFY_MASK | GDK_STRUCTURE_MASK);
       gdk_window_add_filter(win->xmms_main_window_gdk, event_filter, (gpointer) win);
    } else {
      gdk_window_set_events(win->child_gdk, GDK_VISIBILITY_NOTIFY_MASK | GDK_STRUCTURE_MASK);
      gdk_window_add_filter(win->child_gdk, event_filter, (gpointer) win);
    }

    } else {

    gdk_window_set_events(win->child_gdk, GDK_STRUCTURE_MASK);
    gdk_window_add_filter(win->child_gdk, child_window_filter, (gpointer) win);

    gdk_window_set_events(win->parent_gdk, GDK_STRUCTURE_MASK |
      GDK_VISIBILITY_NOTIFY_MASK);
    gdk_window_add_filter(win->parent_gdk, parent_window_filter, (gpointer) win);

  }

  if (win->show && !win->click_mode && !win->normal_map)
    show_hide_window (win, force_show, FALSE);

  if (win->shortcut_key != 0 && win->shortcut_modifier != 0)
    shortcut_init (win);


  gtk_main ();

  return 0;
}
