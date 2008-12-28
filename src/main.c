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


#include "common.h"
#include "utils.h"
#include "trayicon.h"


gboolean debug=FALSE;


static GdkFilterReturn
root_filter_manager_window (GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data)
{
  
  XEvent *xev = (XEvent *)xevent;
  
  //win_struct *win= (win_struct *) user_data;
  
  if (debug) printf ("root_filter_manager_window event\n");
  
  if (xev->xany.type == ClientMessage &&
    xev->xclient.message_type == manager_atom &&
    xev->xclient.data.l[1] == selection_atom) {
  
    display_window_id (GDK_DISPLAY(), xev->xclient.window);
  
    if (debug) printf ("manager: here i am\n");
  
    gtk_main_quit ();

  }
  
  return GDK_FILTER_CONTINUE;
}

static GdkFilterReturn root_filter_map (GdkXEvent *xevent, 
    GdkEvent *event, gpointer user_data)
{
  XEvent *xev = (XEvent *)xevent;
  XMapEvent *xmap;
  Window window=None;

  win_struct *win = (win_struct*) user_data;

 //if (debug) printf ("root_filter_map event: %s\n", event_names[xev->xany.type]);
   
  if (xev->xany.type == MapNotify) {
  
   if (debug) printf ("map notify\n");
       
    xmap=(XMapEvent*) xev;
    window=XmuClientWindow(GDK_DISPLAY(), xmap->window);
   
    //display_window_id (GDK_DISPLAY(), window);
       
    if (window != None) {
      if (window_match (window, win)) {
        win->child_xlib=window;
        gtk_main_quit ();
      }
    }   
 
  }
  
  return GDK_FILTER_CONTINUE;
}

GdkFilterReturn root_filter_workspace (GdkXEvent *xevent, 
    GdkEvent *event, gpointer user_data)
{
  XEvent *xev = (XEvent *)xevent;
  gint num;
  gint current_desk=0;
  
  win_struct *win = (win_struct*) user_data;

  //if (debug) printf ("root_filter_workspace event: %s\n", event_names[xev->xany.type]);

  if (xev->xany.type == PropertyNotify &&
     xev->xproperty.atom == net_current_desktop) {
       
     current_desk=get_current_desktop ();
     if (debug) printf ("workspace switched %d\n", current_desk);

      win->desktop=current_desk;
      num=win->desktop +1;
      
      gint length=win->workspace->len;
      gint i;
      
      if (num > length) {
        for (i=win->workspace->len; i<num; i++) {
          
          wm_state_struct new_state;
          new_state.visible=FALSE;
          new_state.show_in_taskbar=FALSE;
          g_array_append_val(win->workspace, new_state);
        
        }
      } 
  
      wm_state_struct current_struct;
                  
      current_struct=g_array_index(win->workspace, wm_state_struct, num -1);
      
      if (debug) {
      
        if (current_struct.visible)
          printf ("new workspace: window will be visible\n");
        else
          printf ("new workspace: window will be hidden\n");
  
        if (current_struct.show_in_taskbar)
          printf ("new workspace: show in taskbar\n");
        else
          printf ("new workspace: not in taskbar\n");
      
      }
      
      
      show_hide_window (win, current_struct.visible,
        current_struct.show_in_taskbar);
 

  }

 return GDK_FILTER_CONTINUE;
}

GdkFilterReturn parent_window_filter (GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data)
{
  XEvent *xev = (XEvent *)xevent;
  XConfigureEvent *xconfigure;
  XVisibilityEvent *xvisibilty;
  XConfigureRequestEvent *xconfigurerequest;
  
  gint return_type=GDK_FILTER_CONTINUE;
    
  win_struct *win= (win_struct*) user_data;

  switch (xev->xany.type) {
    
    
    case MapNotify:
      
     if (debug) printf ("map notify\n");
    
     update_visibility_state (win, window_is_visible);
    
    break;
    
    case UnmapNotify:
      
     if (debug) printf ("unmap notify\n");
    
     update_visibility_state (win, window_is_hidden);
       
    
    break;
     
    case ConfigureNotify:
    
      //if (debug) printf ("configure notify\n");  
      
      xconfigure = (XConfigureEvent*) xev;
      
      static gint old_width=0;
      static gint old_height=0;
      
      if (old_width == xconfigure->width && old_height == xconfigure->height)
        break;

      old_width=xconfigure->width;
      old_height=xconfigure->height;
      
      gdk_window_resize (win->child_gdk, old_width, old_height);

    break;
      
    case ClientMessage:

      if (xev->xclient.data.l[0] == wm_delete_window) {
        if (debug) printf ("delete event!\n");
        
        show_hide_window (win, force_hide, FALSE);
        break;
      }
      
      if (xev->xclient.data.l[0] == wm_take_focus) {
        if (debug) printf ("wm take focus !!!\n");
          
        if (!assert_window(win->child_xlib)) {
          if (debug) printf ("can not set focus to child ! assert(window) failed\n");
          break;
        }
        
        while (!xlib_window_is_viewable (win->child_xlib))
          gtk_sleep (10);
                
        XSetInputFocus (win->display, win->child_xlib, 
          RevertToParent, xev->xclient.data.l[1]);
        
        break;
      }   
      
      if (xev->xclient.data.l[0] == net_wm_ping) {
        if (debug) printf ("net wm ping!\n");
        
        XEvent xe = *xev;
        
        xe.xclient.window = win->root_xlib;
        XSendEvent (win->display, win->root_xlib, False, 
          SubstructureRedirectMask | SubstructureNotifyMask, &xe);
        
        break;
      }   
     
      break;
     
      case VisibilityNotify:
        
        xvisibilty = (XVisibilityEvent*) xev;
      
        win->visibility=xvisibilty->state;
              
        if (debug) printf ("visibility notify state: %d\n", win->visibility);
      break;

        
      /*bad child wanted to move inside parent -> deny*/
      case ConfigureRequest:
        
      xconfigurerequest = (XConfigureRequestEvent*) xev;
              
      if (debug) printf ("child configure request\n");

      if (xconfigurerequest->x !=0  || xconfigurerequest->y != 0) {
        
        if (debug) printf ("deny configure request\n");
        return_type=GDK_FILTER_REMOVE;
      }
      
      break;
      
      
  }
  
   return return_type;
}

GdkFilterReturn child_window_filter (GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data)
{
  XEvent *xev = (XEvent *)xevent;
   
  win_struct *win= (win_struct*) user_data;
  
  //if (debug) printf ("child window our event: %s\n", event_names[xev->xany.type]);
  
  switch (xev->xany.type) {
    
    case DestroyNotify:
      if (debug) printf ("destroy notify\n");
        
      if (!assert_window (win->child_xlib))
        destroy_all_and_exit (win, FALSE);
    break;
    
    case PropertyNotify:
    { 
      XPropertyEvent *xproperty = (XPropertyEvent *) xev;
    
      if (debug) printf ("property notify\n");
    
      if (xproperty->atom == wm_name_atom) {
        update_window_title(win);
        break;
      }
    
      if (xproperty->atom == net_wm_icon  || 
          xproperty->atom == wm_icon_atom ) {
        update_window_icon(win);
        update_tray_icon(win);
      } 
    
    }
    break;
    
    case ConfigureNotify:
    {
      XConfigureEvent *xconfigure = (XConfigureEvent*) xev;
      
      if (debug) printf ("child configure event: x: %d, y: %d\n",
        xconfigure->x, xconfigure->y);
      
      if (xconfigure->x >=1 || xconfigure->y >=1) {
        
        /*deny move inside parent in the future*/
        XSelectInput (GDK_DISPLAY(),win->parent_xlib,
          SubstructureRedirectMask | StructureNotifyMask |
          VisibilityChangeMask);
        
        gdk_window_move (win->child_gdk, 0, 0);
      }
      

    }
    break;
            
  
  }
  
  return GDK_FILTER_CONTINUE;
}

GdkFilterReturn child_window_filter_wm_state(GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data)
{
  XEvent *xev = (XEvent *)xevent;
   
  win_struct *win= (win_struct*) user_data;
 
 
  switch (xev->xany.type) {
  
    case PropertyNotify:
    { 
        XPropertyEvent *xproperty = (XPropertyEvent *) xev;
        
        if (debug) printf ("property notify\n");
 
        if (xproperty->atom == wm_state) {
          if (debug) printf ("wm state changed\n");
          
           if (withdrawn (win->child_xlib))
            gtk_main_quit ();
        }
 
    }
    break;
  
  }
  
  return GDK_FILTER_CONTINUE;

}

void win_struct_init(win_struct *win)
{

  win->display=GDK_DISPLAY();
  win->root_xlib=GDK_ROOT_WINDOW();
  win->root_gdk=gdk_screen_get_root_window (gdk_screen_get_default());
      
  win->parent_is_visible=FALSE;
   
  win->screen_width=gdk_screen_get_width (gdk_screen_get_default());
  win->screen_height=gdk_screen_get_height (gdk_screen_get_default());
    
  win->parent_gdk=NULL;
  win->parent_xlib=None;

  win->visibility=VisibilityUnobscured;
  
  win->workspace=g_array_new (FALSE, FALSE, sizeof (wm_state_struct));
   
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

  win->hide_start=FALSE;
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
  
  win->fake_desktop=NULL;

}

void command_line_init (win_struct *win, int argc, char **argv)
{

  if (!parse_arguments(argc, argv, &win->user_icon_path,
    &win->command, &win->show, &win->hide_start,
    &debug, &win->borderless, &win->large_icons)) {
      
    exit(1);
  }
  
  if (debug) {
    printf ("command: %s\n", win->command);
    if (win->show) printf ("show=TRUE\n");
    if (win->user_icon) printf ("have user icon\n");
    if (win->hide_start) printf ("hide_start=TRUE\n");
    if (win->borderless) printf ("borderless=TRUE\n");
  }

  win->command_only=strip_command(win->command);

  if (win->user_icon_path)
    win->user_icon=get_user_icon (win->user_icon_path, 30, 30);


}

void wait_for_manager(win_struct *win)
{

  win->manager_window=get_manager_window();

  if (win->manager_window == None) {
   
     printf ("\nAlltray: no system tray/notification area found.\n"\
             "I will wait..... I have time....\n\n"\
             "In the meantime you may add a system tray applet\n"\
             "to the panel.\n");
               
     gdk_window_add_filter(win->root_gdk, root_filter_manager_window, (gpointer) win);
     gtk_main ();
     gdk_window_remove_filter(win->root_gdk, root_filter_manager_window, (gpointer) win);
   }
 
  else { if (debug) printf ("HAVE MANAGER WINDOW\n");};
}

void exec_and_wait_for_window(win_struct *win)
{

  gdk_window_set_events(win->root_gdk, GDK_SUBSTRUCTURE_MASK);
  gdk_window_add_filter(win->root_gdk, root_filter_map, (gpointer) win); 
  
  if (debug) printf ("execute program: %s\n", win->command);
  if (!(win->child_pid=exec_command (win->command))) {
  
    if (debug) printf ("execute failed\n");
    exit (1);
  }

  if (debug) printf ("wait for window\n");
  gtk_main();
   
  
  if (debug) {
    printf ("found child window: %d\n", (int) win->child_xlib);
    printf ("child have pid: %d\n", (int) win->child_pid);
  }
  
  gdk_window_remove_filter(win->root_gdk, root_filter_map, (gpointer) win);
  
}

void withdraw_window(win_struct *win)
{
  gdk_window_set_events(win->child_gdk,GDK_STRUCTURE_MASK);
  gdk_window_add_filter(win->child_gdk, child_window_filter_wm_state, (gpointer) win);   
  
  XWithdrawWindow (GDK_DISPLAY (), win->child_xlib, XDefaultScreen (GDK_DISPLAY()));
  XSync (GDK_DISPLAY(), False);
 
  if (debug) printf ("wait for withdrawn\n");
  gtk_main();
  gdk_window_remove_filter(win->child_gdk, child_window_filter_wm_state, (gpointer) win);   
  if (debug) printf ("withdrawn --> ok\n");
  
}

void create_fake_desktop (win_struct *win)
{
 
  GdkWindowAttr attributes;
  GdkGeometry hints;
  gint attributes_mask;

  GdkPixbuf *background;
  GdkPixmap *pixmap_return;
  GdkBitmap *mask_return;
  
  if (debug) printf ("create fake desktop\n");

  attributes.x=0;
  attributes.y=0;
  attributes.width = win->screen_width;
  attributes.height = win->screen_height;
  attributes.window_type = GDK_WINDOW_TOPLEVEL;
  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes_mask = GDK_WA_X | GDK_WA_Y;
  
  win->fake_desktop=gdk_window_new(NULL, &attributes, attributes_mask);
 
  hints.min_width = win->screen_width;
  hints.min_height = win->screen_height;
  
  gdk_window_set_geometry_hints (win->fake_desktop,
    &hints, GDK_HINT_MIN_SIZE);
   
  gdk_window_set_decorations (win->fake_desktop, 0);
  gdk_window_fullscreen(win->fake_desktop);

  background=gdk_pixbuf_xlib_get_from_drawable (NULL, GDK_ROOT_WINDOW(),
      0, NULL, 0, 0, 0, 0, win->screen_width, win->screen_height);
          
  gdk_pixbuf_render_pixmap_and_mask (background, &pixmap_return,
    &mask_return, 255);
    
  g_object_unref ( (gpointer) background);
              
  gdk_window_set_back_pixmap (win->fake_desktop, pixmap_return, FALSE);
  gdk_window_show (win->fake_desktop);

  sleep (1);
  
  gdk_window_set_keep_above (win->fake_desktop, TRUE);
  gdk_window_set_keep_below (win->fake_desktop, FALSE);
  
}

void get_child_size (GdkWindow *child_gdk, 
    gint *w_return, gint *h_return)
{
  
  gint w=0, h=0, last_w=-1, last_h=-1;

  do {
  
    if (debug) printf ("child size loop\n");
    
    last_w=w;
    last_h=h;
    
    gdk_window_get_geometry (child_gdk, 
        NULL, NULL, &w, &h, NULL);
    
    if (last_w == w && last_h == h)
      break;
    
    gtk_sleep (300);
  
  }while (1);

  *w_return=w;
  *h_return=h;
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
 
  if (win->hide_start)
    create_fake_desktop(win);
  
  exec_and_wait_for_window(win);

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
    
  win->parent_xlib= XCreateSimpleWindow(win->display, win->root_xlib, 0, 0, 
        w, h, 0, 0, 0);

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
    XSetWMNormalHints(win->display, win->parent_xlib, &sizehints);
  }

  protocols[0]=wm_delete_window;
  protocols[1]=wm_take_focus;
  protocols[2]=net_wm_ping;
  
  XSetWMProtocols (win->display, win->parent_xlib, protocols, 3); 

  win->parent_gdk= gdk_window_foreign_new (win->parent_xlib);
  
  if (win->borderless)
    gdk_window_set_decorations (win->parent_gdk, 0);
     
  XReparentWindow (GDK_DISPLAY(), win->child_xlib, win->parent_xlib, 0, 0);
  XSync (GDK_DISPLAY(), FALSE);

  XMapWindow (GDK_DISPLAY(), win->child_xlib);
  XSync (GDK_DISPLAY(), FALSE);

  update_window_icon(win);
 
  if (win->hide_start) {
    gdk_window_set_keep_above (win->fake_desktop, FALSE);
    gdk_window_set_keep_below (win->fake_desktop, TRUE); 
    gdk_window_destroy(win->fake_desktop);
  }
    
  tray_init (win);
  
  update_window_title(win);

  gdk_window_set_events(win->child_gdk, GDK_STRUCTURE_MASK);
  gdk_window_add_filter(win->child_gdk, child_window_filter, (gpointer) win); 

  gdk_window_set_events(win->parent_gdk, GDK_STRUCTURE_MASK | 
    GDK_VISIBILITY_NOTIFY_MASK);
  gdk_window_add_filter(win->parent_gdk, parent_window_filter, (gpointer) win); 

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
