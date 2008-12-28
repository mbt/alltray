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
#include "utils.h"
#include "trayicon.h"

void update_visibility_state (win_struct *win, gboolean new_state)
{
   wm_state_struct *old_state;
 
  if (debug) printf ("update_visibility_state workspace->len: %d\n", 
    win->workspace->len);

  gint current_max_workspace=win->workspace->len-1;
  gint i;
        
  if (win->desktop > current_max_workspace) {
    for (i=current_max_workspace; i<win->desktop; i++) {
      
      wm_state_struct new_state;
      new_state.visible=FALSE;
      new_state.show_in_taskbar=FALSE;
      g_array_append_val(win->workspace, new_state);
    
    }
  }

  old_state=&g_array_index (win->workspace, wm_state_struct, win->desktop);
    (*old_state).visible=new_state;

  if (debug) printf ("update_visibility_state to: %d\n", (*old_state).visible);
  
  win->parent_is_visible=new_state;
    
}

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

gboolean parse_arguments(int argc, char **argv, gchar **icon,
    gchar  **rest, gboolean *show, gboolean *debug, gboolean *borderless,
    gboolean *large_icons, GArray *command_menu, gint *title_time, gchar **geometry)
{
  int i;
  char rest_buf[4096]="";
  int x, y, w, h;
  
  if (argc == 1) {
    show_help();
    return FALSE;
  }  
  
  for (i = 1; i < argc; i++) {
  
    do {
    
      if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")) {
        show_help();
        return FALSE;
      }
      
      if (!strcmp(argv[i], "--version") || !strcmp(argv[i], "-v")) {
        show_version();
        return FALSE;
      }
        
      if (!strcmp(argv[i], "--show") || !strcmp(argv[i], "-s")) {
        *show=TRUE;
        break;
      }  
     
      if (!strcmp(argv[i], "--borderless") || !strcmp(argv[i], "-x")) {
        *borderless=TRUE;
        break;
      }
      
      if (!strcmp(argv[i], "--large_icons") || !strcmp(argv[i], "-l")) {
        *large_icons=TRUE;
        break;
      }  
    
         
      if (!strcmp(argv[i], "--icon") || !strcmp(argv[i], "-i")) {
        if ((i+1) ==  argc) {
          show_help();
          return FALSE;
        }
      
        *icon=g_strdup (argv[i+1]);
        i++;
        break;
      }
      
     if (!strcmp(argv[i], "--geometry") || !strcmp(argv[i], "-g")) {
        if ((i+1) ==  argc) {
          show_help();
          return FALSE;
        }
      
        *geometry=g_strdup (argv[i+1]);
        
        if (XParseGeometry(*geometry, &x, &y, &w, &h) == 0) {
          show_help();
          return FALSE;
        }
        
        i++;
        break;
     } 
      
     if (!strcmp(argv[i], "--title") || !strcmp(argv[i], "-t")) {
      if ((i+1) ==  argc) {
        show_help();
        return FALSE;
      }
    
      *title_time=atoi (argv[i+1]);
      
      if (*title_time == 0) {
        show_help ();
        return FALSE;
      }
      
      i++;
      break;
    }
      
    if (!strcmp(argv[i], "--menu") || !strcmp(argv[i], "-m")) {
      if ((i+1) ==  argc) {
        show_help();
        return FALSE;
      }
      
      if (!append_command_to_menu(command_menu, argv[i+1])) {
        printf ("\nAllTray: \"%s\" is not a valid menu entry !\n"\
        "         Syntax: -m \"menu text:command\"\n", argv[i+1]);
        return FALSE;
      }
                      
      i++;
      break;
    }

    if (!strcmp(argv[i], "--debug") || !strcmp(argv[i], "-d")) {
      *debug=TRUE;
      break;
    }  

    if (g_str_has_prefix (argv[i],"-")) {
      printf ("\nAlltray: Unknown option '%s'\n\n", argv[i]);
      return FALSE;
    }

    if (strlen (rest_buf) + strlen (argv[i]) >= 4096) {
      printf ("Alltray: Command Buffer too small (; [max 4096 letters]\n");
      return FALSE;
    }
            
    if (strlen (rest_buf) > 0)
      strcat (rest_buf, " ");
      
    strcat (rest_buf, argv[i]);
      
    }while (0);
  
  }
  
  if (strlen (rest_buf) == 0) {
    show_help();
    return FALSE;
  }
  
  
  *rest=g_strdup (rest_buf);

  return TRUE;
}

gboolean append_command_to_menu(GArray *command_menu, gchar *string)
{
  
  command_menu_struct new;
  
  new.entry=NULL;
  new.command=NULL;
    
  gchar *tmp=NULL;
  gchar *command=NULL;
  
  tmp=g_strdup (string);
  
  if (!tmp)
    return FALSE;
        
  command = g_strrstr (tmp,":");
  
  if (debug) printf ("command: %s\n", command);
    
  if (!command) {
    g_free (tmp);
    return FALSE;
  }
  
  new.command=g_strdup(++command);
  
  if (strlen (new.command) == 0) {
    g_free (tmp);
    g_free (new.command);
    return FALSE;
  }
   
  if (debug) printf ("new.command: %s\n", new.command);
  
  *(--command)=0;
  
  if (strlen (tmp) == 0) {
    g_free (tmp);
    g_free (new.command);
    return FALSE;
  }
  
  new.entry=tmp;
    
  if (debug) printf ("new.entry: %s\n", new.entry);
    
  g_array_append_val(command_menu, new);
  
  return TRUE;
}

gchar *strip_command (win_struct *win)
{

  gchar *command_copy=NULL;
  gchar *space=NULL;
  gchar *last_slash=NULL;
  gchar *return_value=NULL;
        
  command_copy=g_strdup (win->command);

  space=g_strstr_len (command_copy, 
      strlen (command_copy) *sizeof (gchar), " ");
  
  if (space)
    *space=0;
  
  if (debug) printf ("command without args: %s\n", command_copy);
 
  return_value=command_copy;
    
  last_slash = g_strrstr (command_copy, "/");
  
  if (last_slash) {
    
    
    if (g_file_test (command_copy, G_FILE_TEST_EXISTS)) {
     return_value = g_strdup (++last_slash);
     if (debug) printf ("\"%s\" exists\n", command_copy);
     g_free (command_copy);  
    } else {
      
      printf ("AllTray: Command: \"%s\" do not exist !\n", command_copy);
    
      if (win->user_icon_path)
        g_free (win->user_icon_path);
      if (command_copy)
        g_free(command_copy);
      if (win->command)
        g_free (win->command);
      if (win->command_menu)
        g_array_free (win->command_menu, TRUE);
      g_array_free (win->workspace, FALSE);
      g_free(win);
      exit (1);
    }
  
  }

  if (debug) printf ("command stripped: %s\n", return_value);
  
  return return_value;
}

void show_help(void)
{
  printf ("\nAllTray Version %s\n\n" \
  
             "Dock any program into the system tray.\n\n"  \

             "usage: alltray [options] [\"] <program_name> [program parameter] [\"]\n\n" \
             " where options include:\n"\
             "   --help; -h:  print this message\n"\
             "   --version; -v: print version\n"\
             "   --debug; -d: show debug messages\n"\
             "   --show; -s:  do not hide window after start\n"\
             "   --icon; -i  <path to png>: use this icon\n"\
             "   --large_icons; -l: allow large icons (> 24x24)\n"\
             "   --borderless; -x: remove border, title, frame... from parent\n"\
             "   --menu; -m: \"menu text:command\": add entry to popdown menu\n" \
             "   --title; -t <sec>: show title change for <sec> seconds\n"\
             "   --geometry; -g [<width>x<height>][+<x>+<y>]: initial position/geo\n\n"\
             "usage: alltray\n\n"\
             " Click-Mode: Click on the window you would like to dock.\n"\
             " (Abort with <c>)\n\n"

  , VERSION);

}

void show_version (void)
{
  printf ("\nAlltray version %s\n\n", VERSION);
}
