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
#include "inlinepixbufs.h"


#define closebuttonx 264
#define closebuttony 3
#define closebuttonsize 9

static gboolean grapped=TRUE;
static guint tag=0;

gboolean grap_pointer (gpointer user_data)
{
  
  int status;
  XEvent event;
  
  gboolean inside=TRUE;
  gboolean button_pressed=FALSE;
  
  
  win_struct *win= (win_struct*) user_data;

  status = XGrabPointer(GDK_DISPLAY(), win->xmms_main_window_xlib, False,
    ButtonPressMask | PointerMotionMask, GrabModeSync,
    GrabModeAsync, GDK_ROOT_WINDOW(), None, CurrentTime);
  
  if (status != GrabSuccess) {
    printf ("Can't grab the mouse.");
    grapped=FALSE;
    return FALSE;
  }

  while (inside && !button_pressed) {
  
    XAllowEvents(GDK_DISPLAY(), SyncPointer, CurrentTime);
    XWindowEvent(GDK_DISPLAY(), win->xmms_main_window_xlib,
        ButtonPressMask| PointerMotionMask, &event);
    
    
    switch (event.type) {
      
      case ButtonPress:
          if (debug) printf ("button press\n");
          button_pressed=TRUE;
      break;
      
      case MotionNotify:
         if (debug) printf ("motion notify: %d %d\n", event.xmotion.x, event.xmotion.y);
  
        if (event.xmotion.x < closebuttonx ||
            event.xmotion.x >= (closebuttonx + closebuttonsize) ||
            event.xmotion.y < closebuttony ||
           event.xmotion.y >= (closebuttony + closebuttonsize) ) {
          inside=FALSE;
          if (debug) printf ("ungrab...\n");
        }
        
      break;
   }
  
  }
  
  if (button_pressed)
    show_hide_window (win, force_hide, FALSE);
    
  XUngrabPointer(GDK_DISPLAY(), CurrentTime);
  
  
  if (!inside)
    grapped=FALSE;
     
  return FALSE;
}

void inside (win_struct *win)
{
  
  if (grapped)
    return;
  
  grapped=TRUE;
    
  if (debug) printf ("grab...\n");
  
  g_timeout_add (1, grap_pointer, (gpointer) win);

}
 

gboolean timer (gpointer user_data)
{

  win_struct *win= (win_struct*) user_data;
    
  gint x,y;
    
  gdk_window_get_pointer (win->xmms_main_window_gdk, &x, &y,  NULL);
  
  if (x >= closebuttonx &&
      x < (closebuttonx+closebuttonsize) &&
      y >= closebuttony &&
      y < (closebuttony + closebuttonsize)) {

    inside (win);
  }
  
  return TRUE;
}

GdkFilterReturn xmms_main_window_filter (GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data)
{
  XEvent *xev = (XEvent *)xevent;
     
  win_struct *win= (win_struct*) user_data;
  
  switch (xev->xany.type) {
  
    case EnterNotify:
      if (debug) printf ("EnterNotify\n");
      
      if (!tag) {
        grapped=FALSE;
        tag=g_timeout_add (100, timer, (gpointer) win);
      }
    
    break;
    
    case LeaveNotify:
    
      if (debug) printf("LeaveNotify\n");
      
      if (tag) {
        g_source_remove(tag);
        tag=0;
      }
  
    break;
    
    case MapNotify:
    
      if (debug) printf ("xmms main window map notify\n");
      
      update_visibility_state (win, window_is_visible);
    
    break;
    
    case UnmapNotify:
    
      if (debug) printf ("xmms main window unmap notify\n");
      
      update_visibility_state (win, window_is_hidden);
    
    
    break;
      
    case DestroyNotify:
      
      if (debug) printf ("destroy notify\n");
        
      if (!assert_window (win->xmms_main_window_xlib)) {

        XUngrabPointer(GDK_DISPLAY(), CurrentTime);
        
        if (tag)
          g_source_remove(tag);

        destroy_all_and_exit (win, FALSE);
        
      }
      
    break;
      
      case PropertyNotify:
      { 
        XPropertyEvent *xproperty = (XPropertyEvent *) xev;
      
        if (debug) printf ("property notify\n");
      
        if (xproperty->atom == wm_name_atom) {
          update_window_title(win);
          break;
        }
      
      }
    break;

  }
  
  return GDK_FILTER_CONTINUE;
}

void deiconify_xmms_windows (win_struct *win)
{
  
    if (win->xmms_main_window_xlib != None) {
    XWMHints *wm_hints;
    
    gdk_error_trap_push();
              
    wm_hints= XGetWMHints(GDK_DISPLAY(), win->xmms_main_window_xlib);
       
    if (!gdk_error_trap_pop() && wm_hints!=NULL) {
  
      wm_hints->initial_state=NormalState;
      XSetWMHints(GDK_DISPLAY(), win->xmms_main_window_xlib, wm_hints);
      XFree(wm_hints);
    }
    
  }
  
   if (win->xmms_playlist_window_xlib != None) {
    XWMHints *wm_hints;
    
    gdk_error_trap_push();
              
    wm_hints= XGetWMHints(GDK_DISPLAY(), win->xmms_playlist_window_xlib);
       
    if (!gdk_error_trap_pop() && wm_hints!=NULL) {
  
      wm_hints->initial_state=NormalState;
      XSetWMHints(GDK_DISPLAY(), win->xmms_playlist_window_xlib, wm_hints);
      XFree(wm_hints);
    }
    
    XMapWindow (GDK_DISPLAY(), win->xmms_playlist_window_xlib);
    
  }
  
   if (win->xmms_equalizer_window_xlib != None) {
    XWMHints *wm_hints;
    
    gdk_error_trap_push();
              
    wm_hints= XGetWMHints(GDK_DISPLAY(), win->xmms_equalizer_window_xlib);
       
    if (!gdk_error_trap_pop() && wm_hints!=NULL) {
  
      wm_hints->initial_state=NormalState;
      XSetWMHints(GDK_DISPLAY(), win->xmms_equalizer_window_xlib, wm_hints);
      XFree(wm_hints);
    }
    
    XMapWindow (GDK_DISPLAY(), win->xmms_equalizer_window_xlib);
    
  }
 }

void xmms_filter_init (win_struct *win)
{
 
  gdk_window_set_events(win->xmms_main_window_gdk, 
    GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK);
  
  gdk_window_add_filter(win->xmms_main_window_gdk,
    xmms_main_window_filter, (gpointer) win);

}

GdkPixbuf *get_xmms_icon (gint width, gint height)
{

  if (debug) printf ("get_xmms_icon\n");
    
  GdkPixbuf *fallback=NULL;
  GdkPixbuf *fallback_scaled=NULL;
    
  fallback = gdk_pixbuf_new_from_inline (-1, xmms_icon,
                                     FALSE,
                                     NULL);
  
  if (fallback) {
    fallback_scaled=gdk_pixbuf_scale_simple (fallback, 
        width, height, GDK_INTERP_HYPER);
    
    g_object_unref (fallback);
  }
      
  
  g_assert (fallback_scaled);

  return fallback_scaled;

}
