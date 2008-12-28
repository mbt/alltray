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

GdkFilterReturn parent_window_filter (GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data)
{
  XEvent *xev = (XEvent *)xevent;
  XConfigureEvent *xconfigure;
  XVisibilityEvent *xvisibilty;
  
  win_struct *win= (win_struct*) user_data;
  
   
  switch (xev->xany.type) {
    
    case ConfigureNotify:
    
      if (debug) printf ("configure notify\n");  
      
      xconfigure = (XConfigureEvent*) xev;
      
      static gint old_width=0;
      static gint old_height=0;
      
      if (old_width == xconfigure->width && old_height == xconfigure->height)
        break;

      old_width=xconfigure->width;
      old_height=xconfigure->height;
      
      gdk_window_resize (win->child_gdk, old_width, old_height);

    break;
      
    case FocusIn:
       
       if (debug) printf ("focus in event\n");
       
        if (!assert_window (win->child_xlib))
          break;
          
        if (!xlib_window_is_viewable (win->child_xlib))
         break;
    
        XSetInputFocus (win->display, win->child_xlib, RevertToParent, CurrentTime);

     break;
        
     case ClientMessage:

       if (xev->xclient.data.l[0] == wm_delete_window) {
         if (debug) printf ("delete event!\n");
           
         show_hide_window (win, TRUE, FALSE);
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

GdkFilterReturn root_filter_workspace (GdkXEvent *xevent, 
    GdkEvent *event, gpointer user_data)
{
  XEvent *xev = (XEvent *)xevent;
  gint num;
  gboolean state;


  win_struct *win = (win_struct*) user_data;

  //if (debug) printf ("root_filter_workspace event: %s\n", event_names[xev->xany.type]);

  if (xev->xany.type == ClientMessage &&
      xev->xclient.message_type == net_current_desktop) {
      
      if (debug) printf ("workspace switched %d\n", (int) xev->xclient.data.l[0]);  
      
      win->desktop=(gint) xev->xclient.data.l[0];
      num=win->desktop +1;
         
      gint length=win->workspace->len;
      gint i;
      
      if (num > length) {
        for (i=win->workspace->len; i<num; i++) {
          
          gboolean value=FALSE;
          g_array_append_val(win->workspace, value);
        
        }
      } 

      state=g_array_index(win->workspace, gboolean, num -1);

      show_hide_window (win, TRUE, state);

  }

 return GDK_FILTER_CONTINUE;
}


/* XXX do not work ?
void pid_watch (GPid pid, gint status, gpointer data)
{

  printf ("child watch\n");
}
*/


void win_struct_init(win_struct *win)
{

  win->display=GDK_DISPLAY();
  win->root_xlib=GDK_ROOT_WINDOW();
  win->root_gdk=gdk_screen_get_root_window (gdk_screen_get_default());

  win->screen_width=gdk_screen_get_width (gdk_screen_get_default());
  win->screen_height=gdk_screen_get_height (gdk_screen_get_default());
    
  win->parent_gdk=NULL;
  win->parent_xlib=None;

  win->visibility=VisibilityUnobscured;


  win->workspace=g_array_new (FALSE, FALSE, sizeof (gboolean));
  gboolean value=FALSE;
  g_array_append_val (win->workspace, value);
  
 
  win->hide_start=FALSE;
  win->show=FALSE;
  win->command=NULL;
 
  win->child_xlib=None;
     
  win->wm_res_class=NULL;
  win->wm_res_name=NULL;
  win->wm_name=NULL;
  win->pid=(GPid) 0;
  win->manager_window=None;
  win->tray_window=NULL;
  win->fixed=NULL;
  win->image_icon=NULL;
  win->desktop=0;
  win->ignore_splash_window=FALSE;
  win->title=NULL;
  
  win->icon=NULL;
  win->user_icon=NULL;
    
}

void command_line_init (win_struct *win, int argc, char **argv)
{

  gchar *string=NULL;
  gchar *user_icon_path=NULL;

  
   if (!parse_arguments(argc, argv, &string, &user_icon_path, &win->command, &win->show, 
         &win->ignore_splash_window, &win->hide_start)) {

    exit(1);
  }
  
  if (debug) {
    printf ("string: %s\n", string);  printf ("command: %s\n", win->command);
    
    if (win->show) printf ("show=TRUE\n");
    if (win->ignore_splash_window)  printf ("ignore_splash_window=TRUE\n");
    if (win->user_icon) printf ("have user icon\n");
    if (win->hide_start) printf ("hide_start=TRUE\n");
  } 
   
  if (string) {

    if (!parse_string (string, win)) {
      show_help();
      exit(1);
    }
  
    g_free (string);
    
    if (debug) {
      printf ("wm_res_class: %s\n", win->wm_res_class);
      printf ("wm_res_name: %s\n", win->wm_res_name);
      printf ("wm_name: %s\n", win->wm_name);
    }
  
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
   
     if (debug) printf ("no manager window !!!\n");
          
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

  g_free(win->command);
 
  if (debug) printf ("wait for window\n");
  gtk_main();

  if (debug) {
    printf ("found child window: %d\n", (int) win->child_xlib);
    printf ("found child pid: %d\n", (int) win->pid);
  }
  
  gdk_window_remove_filter(win->root_gdk, root_filter_map, (gpointer) win);
}

void free_wm_stuff (win_struct *win)
{

   if (win->wm_res_class)
    g_free(win->wm_res_class);
  if (win->wm_res_name)
    g_free(win->wm_res_name);
  if (win->wm_name)
    g_free(win->wm_name);
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
  
  XWMHints *leader_change;
  leader_change = XGetWMHints(GDK_DISPLAY(),win->child_xlib);
  leader_change->flags = (leader_change->flags | WindowGroupHint);
  leader_change->window_group = GDK_ROOT_WINDOW();
  XSetWMHints(GDK_DISPLAY(),win->child_xlib,leader_change);
          
  classHint=XAllocClassHint();
  classHint->res_name="alltray";
  classHint->res_class="Alltray";
  XSetClassHint(win->display, win->parent_xlib, classHint);
  XFree (classHint);

  wmhints=XAllocWMHints();
  wmhints->input=1;
  wmhints->initial_state=win->show ? NormalState : WithdrawnState;
  wmhints->flags = InputHint | StateHint;
  XSetWMHints(win->display, win->parent_xlib, wmhints);
  XFree(wmhints);
     
  Atom protocols [2];
  
  protocols[0]=wm_delete_window;
  protocols[1]=wm_take_focus;

  XSetWMProtocols (win->display, win->parent_xlib, protocols, 2); 
     
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
      GDK_FOCUS_CHANGE_MASK | GDK_VISIBILITY_NOTIFY_MASK);
  gdk_window_add_filter(win->parent_gdk, parent_window_filter, (gpointer) win); 

  gdk_window_set_events(win->root_gdk,GDK_SUBSTRUCTURE_MASK);
  gdk_window_add_filter(win->root_gdk, root_filter_workspace, (gpointer) win); 


  
  if (win->show)
    show_hide_window (win, FALSE, FALSE);
  
  free_wm_stuff(win);
  
  gtk_main ();

  return 0;
}
