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

#include <X11/cursorfont.h>

#include "config.h"
#include "common.h"
#include "utils.h"

Window dialog_xlib;

static gint
expose_handler (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
  gtk_paint_flat_box (widget->style, widget->window,
                      GTK_STATE_NORMAL, GTK_SHADOW_OUT, 
                      NULL, widget, "tooltip",
                      0, 0, -1, -1);

  return FALSE;
}

GtkWidget*
create_dialog (void)
{
  
  GtkWidget *dialog;
  GtkWidget *vbox1;
  GtkWidget *label1;
  GtkWidget *hseparator1;
  GtkWidget *label2;
    
  dialog = gtk_window_new (GTK_WINDOW_POPUP);
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
  gtk_widget_set_app_paintable (dialog, TRUE);
  gtk_window_set_policy (GTK_WINDOW (dialog), FALSE, FALSE, TRUE);
  gtk_widget_set_name (dialog, "alltray-dialog");
  gtk_container_set_border_width (GTK_CONTAINER (dialog), 8);
  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (dialog), vbox1);
  label1 = gtk_label_new ("AllTray");
  gtk_widget_show (label1);
  gtk_box_pack_start (GTK_BOX (vbox1), label1, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label1), GTK_JUSTIFY_CENTER);
  hseparator1 = gtk_hseparator_new ();
  gtk_widget_show (hseparator1);
  gtk_box_pack_start (GTK_BOX (vbox1), hseparator1, TRUE, TRUE, 5);
  label2 = gtk_label_new ("Please click on the window\nyou would like to dock.\n(Cancel with <c>)");
  gtk_widget_show (label2);
  gtk_box_pack_start (GTK_BOX (vbox1), label2, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label2), GTK_JUSTIFY_CENTER);
  
  g_signal_connect ((gpointer) dialog, "expose_event",
                    G_CALLBACK (expose_handler),
                    NULL);

  return dialog;
}

gboolean grab (gpointer user_data)
{

  Cursor cursor;
  int status;
  XEvent event;
  Window target_win = None;
  Window target_win_xmu = None;
  gboolean not_yet_clicked=TRUE;
  gboolean canceled=FALSE;

  win_struct *win = (win_struct*) user_data;

  cursor = XCreateFontCursor(GDK_DISPLAY(), XC_crosshair);

  XGrabKey(GDK_DISPLAY(),
    XKeysymToKeycode(GDK_DISPLAY(), XStringToKeysym("c")),
    AnyModifier, GDK_ROOT_WINDOW(), True, GrabModeAsync,
    GrabModeAsync);

  status = XGrabPointer(GDK_DISPLAY(), GDK_ROOT_WINDOW(), False,
    ButtonPressMask, GrabModeSync,
    GrabModeAsync, GDK_ROOT_WINDOW(), cursor, CurrentTime);

  if (status != GrabSuccess) printf ("Can't grab the mouse\n.");
 
  while (not_yet_clicked && !canceled) {

    XAllowEvents(GDK_DISPLAY(), SyncPointer, CurrentTime);
    XWindowEvent(GDK_DISPLAY(), GDK_ROOT_WINDOW(),
      ButtonPressMask|ButtonReleaseMask|KeyPressMask, &event);
  
    switch (event.type) {
    
      case ButtonPress:
         target_win= event.xbutton.subwindow;
         target_win_xmu=XmuClientWindow (GDK_DISPLAY(), target_win);
      
         //if (debug) printf ("target win: %d\n", target_win);
         //if (debug) printf ("target win xmu : %d\n", target_win_xmu);
        
         if (target_win != None &&
             target_win_xmu != dialog_xlib &&
             target_win != GDK_ROOT_WINDOW() &&
             target_win != target_win_xmu &&
             window_type_is_normal (target_win_xmu))
             not_yet_clicked=FALSE;
      break;
         
      case KeyPress:
        
       if (debug) printf ("key press\n");
       canceled=TRUE;
      
      break;
  
    }
  
  }
  
  XUngrabPointer(GDK_DISPLAY(), CurrentTime);
  XUngrabKey (GDK_DISPLAY(), 
    XKeysymToKeycode(GDK_DISPLAY(), XStringToKeysym("c")),
    AnyModifier, GDK_ROOT_WINDOW());
  XFreeCursor (GDK_DISPLAY(), cursor);

  if (!canceled) {
    win->child_xlib=target_win_xmu;
    gtk_main_quit ();
  } else exit (0);


  return FALSE;
}

void click_mode(win_struct *win)
{

  GtkWidget *dialog;

  dialog=create_dialog();
  gtk_widget_realize (dialog);
  gtk_widget_show (dialog);

  dialog_xlib=GDK_WINDOW_XID(dialog->window);
  
  g_idle_add (grab, win);

  gtk_main ();

  gtk_widget_destroy (dialog);

}
