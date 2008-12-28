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

#include <X11/cursorfont.h>

#include "config.h"
#include "common.h"
#include "parent.h"
#include "utils.h"
#include "trayicon.h"
#include "inlinepixbufs.h"
#include "gnome_theme.h"
#include "kde.h"


#define closebuttonx 264
#define closebuttony 3
#define closebuttonsize 9

static gboolean grabed=FALSE;

gboolean grap_pointer_ (gpointer user_data)
{
  
  int status;
  XEvent event;
  
  gboolean inside=TRUE;
  gboolean button_pressed=FALSE;
  Window grap_window;
  Cursor cursor;
  
  win_struct *win= (win_struct*) user_data;

  cursor = XCreateFontCursor(GDK_DISPLAY(), XC_left_ptr);

  if (win->xmms)
      grap_window=win->xmms_main_window_xlib;
  else
    grap_window= win->target_our_xlib;

  status = XGrabPointer(GDK_DISPLAY(), grap_window, False,
    ButtonReleaseMask | PointerMotionMask, GrabModeSync,
    GrabModeAsync, GDK_ROOT_WINDOW(), cursor, CurrentTime);
  
  if (status != GrabSuccess) {
    printf ("Can't grab the mouse.");
    grabed=FALSE;
    XFreeCursor (GDK_DISPLAY(), cursor);
    return FALSE;
  }

  while (inside && !button_pressed) {
  
    XAllowEvents(GDK_DISPLAY(), SyncPointer, CurrentTime);
    XWindowEvent(GDK_DISPLAY(), grap_window,
        ButtonReleaseMask| PointerMotionMask, &event);
    
    switch (event.type) {
      
      case ButtonRelease:
        if (debug) printf ("button release\n");
        button_pressed=TRUE;
      break;
      
      case MotionNotify:
        if (debug) printf ("grab: motion notify: %d %d\n", event.xmotion.x, event.xmotion.y);
        
        if (win->xmms) {
        
          if (event.xmotion.x < closebuttonx ||
            event.xmotion.x >= (closebuttonx + closebuttonsize) ||
            event.xmotion.y < closebuttony ||
            event.xmotion.y >= (closebuttony + closebuttonsize) ) {
              inside=FALSE;
             if (debug) printf ("ungrab...\n");
          }
        
        } else {
        
          if (event.xmotion.x < (win->target_our_w - win->button_width - win->target_right_border) ||
              event.xmotion.x > (win->target_our_w - win->target_right_border) ||
              event.xmotion.y <= 0 ||
              event.xmotion.y > win->button_height) {
                 inside=FALSE;
                if (debug) printf ("ungrab...\n");
           }   
        
        }

      break;
   }
  
  }

  XUngrabPointer(GDK_DISPLAY(), CurrentTime);
  
  if (button_pressed)
    show_hide_window (win, force_hide, FALSE);
  else
    grabed=FALSE;

  return FALSE;
}

static void inside (win_struct *win)
{
  
  if (grabed) {
    if (debug) printf ("allready grabed\n");
    return;
  }
  
  grabed=TRUE;
    
  if (debug) printf ("grab...\n");

  g_timeout_add (1, grap_pointer_, (gpointer) win);

}

GdkFilterReturn motion_filter_gnome (GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data)
{
  XEvent *xev = (XEvent *)xevent;
  static gint x,y;

  win_struct *win= (win_struct*) user_data;
  
  switch (xev->xany.type) {

    case MotionNotify:
    
     if (debug) printf ("motion notify: %d %d\n", xev->xmotion.x, xev->xmotion.y);

     x= xev->xmotion.x;
     y= xev->xmotion.y;

    if ( y > win->button_height)
      break;

    if (x >= (win->target_our_w - win->button_width - win->target_right_border) &&
        x < (win->target_our_w - win->target_right_border) && y >  0 ) {
      if (debug) printf ("inside\n");
      inside (win);
    } else if (debug) printf ("outside\n");
    
      
    break;
  }
  
  return GDK_FILTER_CONTINUE;
}

GdkFilterReturn motion_filter_xmms (GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data)
{
  XEvent *xev = (XEvent *)xevent;
  static gint x,y;

  win_struct *win= (win_struct*) user_data;
  
  switch (xev->xany.type) {

    case MotionNotify:
    
      if (debug) printf ("motion notify: %d %d\n", xev->xmotion.x, xev->xmotion.y);
   
       x= xev->xmotion.x;
       y= xev->xmotion.y;
      
      if (x >= closebuttonx &&
          x < (closebuttonx+closebuttonsize) &&
          y >= closebuttony &&
          y < (closebuttony + closebuttonsize)) {
      
          inside (win);
      } else if (debug) printf ("outside\n");

  break;
 }
  
  return GDK_FILTER_CONTINUE;
}

gboolean kde_close_window_grap (gpointer user_data)
{
  
  int status;
  XEvent event;
  
  gboolean inside=TRUE;
  gboolean button_pressed=FALSE;
  
  
  win_struct *win= (win_struct*) user_data;

  status = XGrabPointer(GDK_DISPLAY(), win->kde_close_button.window_xlib, False,
    ButtonReleaseMask | PointerMotionMask, GrabModeSync,
    GrabModeAsync, GDK_ROOT_WINDOW(), None, CurrentTime);
  
  if (status != GrabSuccess) {
    printf ("Can't grab the mouse.");
    return FALSE;
  }

  while (inside && !button_pressed) {
  
    XAllowEvents(GDK_DISPLAY(), SyncPointer, CurrentTime);
    XWindowEvent(GDK_DISPLAY(), win->kde_close_button.window_xlib,
        ButtonReleaseMask| PointerMotionMask, &event);
    
    
    switch (event.type) {
      
      case ButtonRelease:
          if (debug) printf ("button release\n");
          button_pressed=TRUE;
      break;
      
      case MotionNotify:
         if (debug) printf ("motion notify: %d %d\n", event.xmotion.x, event.xmotion.y);
  
        if (event.xmotion.x < 0 ||
            event.xmotion.x >= win->kde_close_button.width ||
            event.xmotion.y < 0 ||
           event.xmotion.y >= win->kde_close_button.height ) {
           inside=FALSE;
          if (debug) printf ("outside...\n");
        }
        
      break;
   }
  
  }

  XUngrabPointer(GDK_DISPLAY(), CurrentTime);
  
  if (button_pressed)
    show_hide_window (win, force_hide, FALSE);


  return FALSE;

}

GdkFilterReturn kde_close_window_filter (GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data)
{
  XEvent *xev = (XEvent *)xevent;
     
  win_struct *win= (win_struct*) user_data;
  
  switch (xev->xany.type) {
  
    case EnterNotify:
      if (debug) printf ("EnterNotify\n");
      
      g_timeout_add (1, kde_close_window_grap, (gpointer) win);
    
    break;

  }

  return GDK_FILTER_CONTINUE;

}

void grab_init (win_struct *win)
{

  XWindowAttributes win_attributes;


  do {
  
    if (win->xmms) {
    
      gdk_window_set_events(win->xmms_main_window_gdk, GDK_POINTER_MOTION_MASK);
      gdk_window_add_filter(win->xmms_main_window_gdk, motion_filter_xmms, (gpointer) win);
      break;
    
    }
    
    if (win->gnome) {
    
      win->target_our_xlib=one_under_root (GDK_DISPLAY(), win->child_xlib);
      win->target_our_gdk=gdk_window_foreign_new (win->target_our_xlib);
      
      gdk_drawable_get_size (win->target_our_gdk, &win->target_our_w, &win->target_our_h);
      
      if (debug)
      printf ("target_our_w: %d, target_our_h: %d\n", win->target_our_w, win->target_our_h);
      
      gdk_error_trap_push();
      if (!XGetWindowAttributes(GDK_DISPLAY(), win->child_xlib, &win_attributes))
      printf("Can't get window attributes.");
      gdk_error_trap_pop();
      
      if (debug) printf("  Relative upper-left Y:  %d\n",win_attributes.y);
      
      win->button_width=win->button_height=win_attributes.y;
      
      gdk_window_set_events(win->target_our_gdk, GDK_POINTER_MOTION_MASK);
      gdk_window_add_filter(win->target_our_gdk, motion_filter_gnome, (gpointer) win);
      
      break;
    
    }
    
    if (win->kde) {

      win->kde_close_button= kde_get_close_button (win->child_xlib, win->kde_close_button_pos);
      gdk_window_set_events(win->kde_close_button.window_gdk, GDK_ENTER_NOTIFY_MASK );
      gdk_window_add_filter(win->kde_close_button.window_gdk, kde_close_window_filter, (gpointer) win);
      break;
    }
  
  } until;

}

GdkFilterReturn target_filter (GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data)
{

  XEvent *xev = (XEvent *)xevent;

  static gboolean first_map =TRUE;

  win_struct *win= (win_struct*) user_data;
  
  switch (xev->xany.type) {
   
    case MapNotify:
    
      if (debug) printf ("gnome map notify\n");
     
      update_visibility_state (win, window_is_visible);

      if (first_map && !win->borderless) {
        grab_init(win);
        first_map=FALSE;
      }

      grabed=FALSE;

    break;
    
    case UnmapNotify:
    
      if (debug) printf ("gnome unmap notify\n");
      update_visibility_state (win, window_is_hidden);
    
    break;
      
    case DestroyNotify:
      
      if (debug) printf ("gnome destroy notify\n");

      if (win->xmms && assert_window (win->xmms_main_window_xlib))
        break;
      else if (assert_window (win->child_xlib))
        break;
       
        XUngrabPointer(GDK_DISPLAY(), CurrentTime);
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
        update_tray_icon(win);
      } 

    }
    break;
 
    case VisibilityNotify:
    {
        
        XVisibilityEvent *xvisibilty = (XVisibilityEvent*) xev;
        win->visibility=xvisibilty->state;
        if (debug) printf ("visibility notify state: %d\n", win->visibility);

    }
    break;

    case ConfigureNotify:

        if (debug) printf ("configure notify\n");

        if (win->xmms || !win->parent_is_visible || win->borderless || win->kde)
          break;
        
        if (!GDK_IS_DRAWABLE (win->target_our_gdk))
          break;

        gdk_drawable_get_size (win->target_our_gdk, 
               &win->target_our_w, &win->target_our_h);

    break;

  }
  
  return GDK_FILTER_CONTINUE;
}

void grab_filter_init (win_struct *win)
{

  if (win->gnome) {

    if (!parse_theme (win)) {
      printf ("*** parsing theme failed ! oh oh ***\n");
      exit (1);
    }
   
  }

  if (win->xmms) {
    gdk_window_set_events(win->xmms_main_window_gdk, GDK_VISIBILITY_NOTIFY_MASK | GDK_STRUCTURE_MASK);
    gdk_window_add_filter(win->xmms_main_window_gdk, target_filter, (gpointer) win);
  } else {

   gdk_window_set_events(win->child_gdk, GDK_VISIBILITY_NOTIFY_MASK | GDK_STRUCTURE_MASK);
   gdk_window_add_filter(win->child_gdk, target_filter, (gpointer) win);
  }


}
