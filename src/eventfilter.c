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
#include "utils.h"
#include "parent.h"
#include "grab.h"
#include "trayicon.h"


GdkFilterReturn event_filter (GdkXEvent *xevent, 
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

      grab_done ();

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


        if (win->xmms || !win->parent_is_visible || win->nomini || win->kde)
          break;

         if (debug) printf ("configure notify\n");  

        
        if (!GDK_IS_DRAWABLE (win->target_our_gdk))
          break;

        gdk_drawable_get_size (win->target_our_gdk, 
               &win->target_our_w, &win->target_our_h);

    break;

  }
  
  return GDK_FILTER_CONTINUE;
}                      
