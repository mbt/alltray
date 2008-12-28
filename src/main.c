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
 *    Jochen Baier, 2004 (email@Jochen-Baier.de)
 *
 *
 * Based on:
 *
 *    steal-xwin.c (acano@systec.com)
 *    xswallow (Caolan McNamara ?)
 *    kdocker (Girish Ramakrishnan)
 *    libwnck (Havoc Pennington <hp@redhat.com>)
 *    eggtrayicon (Anders Carlsson <andersca@gnu.org>)
 *    dsimple.c ("The Open Group")
 *    .....lot more, THANX !!!
 *    
*/


#include "gtray.h"
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

  win_struct *win = (win_struct*) user_data;

  //if (debug) printf ("root_filter_workspace event: %s\n", event_names[xev->xany.type]);

  if (xev->xany.type == ClientMessage &&
      xev->xclient.message_type == net_current_desktop) {
      
      if (debug) printf ("workspace switched %d\n", (int) xev->xclient.data.l[0]);
        
       /*if an other program get the focus on the same desktop,*/
       /*we get this message too*/
      if (win->desktop == (gint) xev->xclient.data.l[0]) 
        return GDK_FILTER_CONTINUE;  

      win->desktop=(gint) xev->xclient.data.l[0];
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

  }
  
   return GDK_FILTER_CONTINUE;
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
  set_current_desktop (&win->desktop);
  
  if (debug) printf ("current desktop: %d\n", win->desktop);

  win->hide_start=FALSE;
  win->show=FALSE;
  win->command=NULL;
  win->child_xlib=None;
  win->pid=(GPid) 0;
  win->manager_window=None;
  
  win->plug=NULL;
  win->fixed=NULL;
  win->image_icon=NULL;
  win->title=NULL;
  
  win->icon=NULL;
  win->user_icon=NULL;
  
  win->fake_desktop=NULL;

}

void command_line_init (win_struct *win, int argc, char **argv)
{
  
  gchar *user_icon_path=NULL;
  
  if (!parse_arguments(argc, argv, &user_icon_path,
    &win->command, &win->show, &win->hide_start)) {
      
    exit(1);
  }
  
  if (debug) {
    printf ("command: %s\n", win->command);
    
    if (win->show) printf ("show=TRUE\n");
    if (win->user_icon) printf ("have user icon\n");
    if (win->hide_start) printf ("hide_start=TRUE\n");
  } 
  
  if (user_icon_path) {
  
    if (debug) printf ("icon_user_path: %s\n", user_icon_path);
    
    if (g_file_test (user_icon_path, G_FILE_TEST_EXISTS)) {
    
       GError *error=NULL;
 
       win->user_icon=gdk_pixbuf_new_from_file_at_size (user_icon_path, 24, 24, &error);
           
       if (!win->user_icon)
        printf ("%s\n", error->message);
            
    
    } else {
      
      printf ("Alltray: Icon file %s do not exist !\n", user_icon_path);
    }
  
    g_free (user_icon_path);
  
  }

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
  
  if (debug) printf ("execute program\n");
  if (!(win->pid=exec_command (win->command))) {
  
    if (debug) printf ("execute failed\n");
    exit (1);
  }

  if (debug) printf ("wait for window\n");
  gtk_main();
   
  
  if (debug) {
    printf ("found child window: %d\n", (int) win->child_xlib);
    printf ("found child pid: %d\n", (int) win->pid);
  }
  
  gdk_window_remove_filter(win->root_gdk, root_filter_map, (gpointer) win);
  g_free(win->command);
  
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

int
main (int argc, char *argv[])
{

  gint child_window_w;
  gint child_window_h;
  gint child_window_x;
  gint child_window_y;

  XClassHint *classHint;
  XWMHints *wmhints;
  XWMHints *leader_change;
  XSizeHints sizehints;
  glong supplied_return;
  Atom protocols[3];
  gint err=0;
     
  
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
  
  gdk_window_get_geometry (win->child_gdk, 
      &child_window_x, &child_window_y, &child_window_w, &child_window_h, NULL);
   
  withdraw_window(win);
    
  win->parent_xlib= XCreateSimpleWindow(win->display, win->root_xlib, 0, 0, 
        child_window_w, child_window_h, 0, 0, 0);
  
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
    
          
  classHint=XAllocClassHint();
  classHint->res_name="alltray";
  classHint->res_class="Alltray";
  XSetClassHint(win->display, win->parent_xlib, classHint);
  XFree (classHint);

  wmhints=XAllocWMHints();
  wmhints->input=1;
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
  
  gtk_main ();

  return 0;
}
