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
 *    Jochen Baier, 2004, 2005 (email@Jochen-Baier.de)
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
#include "xmms.h"

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
  win->parent_gdk=NULL;
  win->parent_xlib=None;
  
  win->libspy_window=None;
  win->libspy_window_gdk=NULL;

  win->visibility=VisibilityUnobscured;
  
  win->workspace=g_array_new (FALSE, FALSE, sizeof (wm_state_struct));
  win->command_menu=g_array_new (FALSE, FALSE, sizeof (command_menu_struct));
   
  gint desktops=get_number_of_desktops();
  gint i;
  
  if (debug) printf ("number of desktops: %d\n", desktops);
      
  for (i=0; i<desktops; i++) {
    wm_state_struct new_state;
    new_state.visible=FALSE;
    new_state.show_in_taskbar=FALSE;
    g_array_append_val(win->workspace, new_state);
  }
   
  win->desktop=0;
  win->desktop=get_current_desktop();
  
  if (debug) printf ("init: current desktop: %d\n", win->desktop);

  win->show=FALSE;
  win->command=NULL;
  win->child_xlib=None;
  win->borderless=FALSE;
  win->large_icons=FALSE;
      
  win->parent_pid=getpid();
  win->child_pid=0;
  if (debug) printf ("win->parent_pid: %d\n", win->parent_pid);
   
  win->manager_window=None;
  
  win->plug=NULL;
  win->fixed=NULL;
  win->image_icon=NULL;
  win->title=NULL;
  
  win->window_icon=NULL;
  win->tray_icon=NULL;
  
  win->user_icon=NULL;
  win->user_icon_path=NULL;

  win->gnome_panel_found=FALSE;
  
  win->balloon=NULL;
  win->balloon_message_allowed=FALSE;

  win->xmms=FALSE;
  win->xmms_main_window_xlib=None;
  win->xmms_main_window_gdk=NULL;
  
  win->xmms_playlist_window_xlib=None;
  win->xmms_equalizer_window_xlib=None;
  
  win->title_time=0;
 
}

void command_line_init (win_struct *win, int argc, char **argv)
{
  
  gchar *geometry=NULL;

  if (!parse_arguments(argc, argv, &win->user_icon_path,
    &win->command, &win->show, &debug, &win->borderless,
    &win->large_icons, win->command_menu, &win->title_time, &geometry)) {
    
    if (win->user_icon_path)
      g_free(win->user_icon_path);
    if (win->command)
      g_free (win->command);
    
    if (geometry)
      g_free (geometry);
    
    if (win->workspace)
      g_array_free (win->workspace, FALSE);
    
    if (win->command_menu)
      g_array_free (win->command_menu, TRUE);
    g_free(win);
    exit(1);
  }
  
  if (win->user_icon_path)
    win->user_icon=get_user_icon (win->user_icon_path, 30, 30);

  win->command_only=strip_command(win);
  
  if (!strcmp (win->command_only, "xmms")) {
    win->xmms=TRUE;
    if (debug) printf ("child is xmms\n");
  }
  
  
  if (geometry) {
    
    XParseGeometry(geometry, &win->initial_x, &win->initial_y,
              &win->initial_w, &win->initial_h);
    
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
  gdk_pixbuf_xlib_init (GDK_DISPLAY(), DefaultScreen (GDK_DISPLAY()));
  atom_init ();
     
  win_struct *win = malloc (sizeof (win_struct));

  win_struct_init (win);
  command_line_init (win, argc, argv);

  wait_for_manager(win);
  
  win->gnome_panel_found=search_gnome_panel();
  
  exec_and_wait_for_window(win);
  
  if (win->xmms) {
    win->xmms_main_window_gdk=gdk_window_foreign_new(win->xmms_main_window_xlib);
    append_command_to_menu(win->command_menu, "Play:xmms -p");
    append_command_to_menu(win->command_menu, "Pause:xmms -u");
    append_command_to_menu(win->command_menu, "Next:xmmsnext");
  }

  if (!win->xmms) {
 
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
    
    if (win->borderless)
      gdk_window_set_decorations (win->parent_gdk, 0);
       
    XReparentWindow (GDK_DISPLAY(), win->child_xlib, win->parent_xlib, 0, 0);
    XSync (GDK_DISPLAY(), FALSE);
  
    XMapWindow (GDK_DISPLAY(), win->child_xlib);
    XSync (GDK_DISPLAY(), FALSE);
  }
  
  update_window_icon(win);

  tray_init (win);
  update_window_title(win);
  
  if (!win->xmms) {
    
    gdk_window_set_events(win->child_gdk, GDK_STRUCTURE_MASK);
    gdk_window_add_filter(win->child_gdk, child_window_filter, (gpointer) win); 
  
    gdk_window_set_events(win->parent_gdk, GDK_STRUCTURE_MASK | 
      GDK_VISIBILITY_NOTIFY_MASK);
    gdk_window_add_filter(win->parent_gdk, parent_window_filter, (gpointer) win); 
  
    
  } else {
    
    xmms_filter_init (win);
    
  }

  gdk_window_set_events(win->root_gdk,GDK_SUBSTRUCTURE_MASK);
  gdk_window_add_filter(win->root_gdk, root_filter_workspace, (gpointer) win); 
  
  if (win->show)
    show_hide_window (win, force_show, FALSE);

  if (win->command)
    g_free(win->command);
  if (win->command_only)
    g_free(win->command_only);
    
  gtk_main ();

  return 0;
}
