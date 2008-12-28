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
