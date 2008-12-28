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

gboolean parse_shortcut (gchar *string, 
    unsigned int *key_return, unsigned int *modifier_return)
{

  gchar *tmp=NULL;
  gchar *colon=NULL;
  gchar *modifier=NULL;


  if (debug) printf ("parse_shortcut\n");
  
  if (string == NULL)
    return FALSE;

  tmp=g_strdup (string);
  
  colon=strstr (tmp,":");

  if (colon) {
    if (debug) printf ("colon: %s\n", colon);

    *key_return =atoi (++colon);
    
    if (*key_return == 0)
      *key_return=XKeysymToKeycode(GDK_DISPLAY(), XStringToKeysym(++colon));

   *(--colon)=0;
   modifier=tmp;
  
   if (debug) printf ("modifier: %s\n", modifier);

   do {
    
    if (!strcmp (modifier, "Shift")) {
      if (debug) printf ("found Shift\n");
      *modifier_return=ShiftMask;
      break;
    }
    
    if (!strcmp (modifier, "Control")) {
      if (debug) printf ("found Control\n");
      *modifier_return=ControlMask;
      break;
    }
  
    if (!strcmp (modifier, "Alt")) {
      if (debug) printf ("found Alt\n");
      *modifier_return=Mod1Mask;
      break;
    }
    
    if (!strcmp (modifier, "Winkey_right")) {
      if (debug) printf ("found Winkey_right\n");
      *modifier_return=Mod4Mask;
      break;
    }   

    if (!strcmp (modifier, "AltGr")) {
      if (debug) printf ("found AltGr\n");
      *modifier_return=Mod5Mask;
      break;
    }       


    printf ("\n\nAllTray: unknown modifier!"\
    " (Only \"Shift\", \"Control\" \"Alt\" or \"AltGr\" is allowed).\n\n");
    
    g_free (tmp);
    return FALSE;

  } while (0);
 
           
  } else {

    *key_return =atoi (tmp);
    
    if (*key_return == 0) 
      *key_return=XKeysymToKeycode(GDK_DISPLAY(), XStringToKeysym(tmp));

    *modifier_return=AnyModifier;
  
  }

  g_free (tmp);

  return TRUE;

}

static GdkFilterReturn
root_filter (GdkXEvent *xevent, 
  GdkEvent *event, gpointer user_data)
{
  
  XEvent *xev = (XEvent *)xevent;
  
  win_struct *win= (win_struct*) user_data;


   switch (xev->type) {

      case KeyRelease:
        
      if (debug) printf ("shortcut key released\n");
        show_hide_window (win, force_disabled, FALSE);

      break;
      
  }

  return GDK_FILTER_CONTINUE;
}

void check_if_pointer_is_over_button (win_struct *win)
{

   gint x,y;


  if (debug) printf ("check if.......\n");

  gdk_window_get_pointer (win->target_our_gdk, &x, &y, NULL);

   if (x >= (win->target_our_w - win->button_width - win->target_right_border) &&
        x < (win->target_our_w - win->target_right_border) && y >  0 ) {
      if (debug) printf ("inside\n");

      XWarpPointer(GDK_DISPLAY(), None, win->target_our_xlib, 0, 0, 0, 0, 
        win->target_our_w - win->button_width - 5, 20);

    }

}                    

void shortcut_init (win_struct *win)
{

  GdkWindow *root_gdk;
  root_gdk=gdk_screen_get_root_window (gdk_screen_get_default());

  gdk_error_trap_push();
  XGrabKey(GDK_DISPLAY(), win->shortcut_key , win->shortcut_modifier,
      GDK_ROOT_WINDOW(), True ,GrabModeAsync,GrabModeAsync);

  if (gdk_error_trap_pop()) {
  
   printf ("\nAllTray: Keyboard shortcut already used by other application."\
    "\nWill not work with AllTray!\n\n");

  } else
  
  gdk_window_add_filter(root_gdk, root_filter, (gpointer) win);

}

void shortcut_done (win_struct *win)
{

  GdkWindow *root_gdk;
  root_gdk=gdk_screen_get_root_window (gdk_screen_get_default());
  
  XUngrabKey (GDK_DISPLAY(), win->shortcut_key, win->shortcut_modifier,
    GDK_ROOT_WINDOW());

  gdk_window_remove_filter(root_gdk, root_filter, (gpointer) win);

}
