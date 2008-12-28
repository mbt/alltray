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
#include "clientwin.h"

GtkWidget *dialog;  
Window dialog_xlib;
GtkWidget *label3;    

static gint
expose_handler (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
  gtk_paint_flat_box (widget->style, widget->window,
                      GTK_STATE_NORMAL, GTK_SHADOW_OUT, 
                      NULL, widget, "tooltip",
                      0, 0, -1, -1);

  gdk_draw_rectangle (widget->window, widget->style->black_gc, 
    0,  label3->allocation.x+4,  label3->allocation.y+2,  
    label3->allocation.width-8, label3->allocation.height-3);

  return FALSE;
}

GtkWidget*
create_dialog (void)
{
  GtkWidget *window1;
  GtkWidget *vbox1;
  GtkWidget *label1;
  GtkWidget *hseparator1;
  GtkWidget *label2;
 

  window1 = gtk_window_new (GTK_WINDOW_POPUP);
  gtk_container_set_border_width (GTK_CONTAINER (window1), 8);
  gtk_window_set_title (GTK_WINDOW (window1), "alltray-clickmode");
  gtk_window_set_position (GTK_WINDOW (window1), GTK_WIN_POS_CENTER);
  gtk_window_set_resizable (GTK_WINDOW (window1), FALSE);
  gtk_window_set_decorated (GTK_WINDOW (window1), FALSE);
  gtk_widget_set_app_paintable (window1, TRUE); 
  gtk_window_set_policy (GTK_WINDOW (window1), FALSE, FALSE, TRUE); 
 
  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (window1), vbox1);

  label1 = gtk_label_new ("AllTray");
  gtk_widget_show (label1);
  gtk_box_pack_start (GTK_BOX (vbox1), label1, FALSE, FALSE, 0);
  gtk_misc_set_padding (GTK_MISC (label1), 0, 1);

  hseparator1 = gtk_hseparator_new ();
  gtk_widget_show (hseparator1);
  gtk_box_pack_start (GTK_BOX (vbox1), hseparator1, TRUE, TRUE, 0);

  label2 = gtk_label_new ("Please click on the window\nyou would like to dock.");
  gtk_widget_show (label2);
  gtk_box_pack_start (GTK_BOX (vbox1), label2, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label2), GTK_JUSTIFY_CENTER);
  gtk_misc_set_padding (GTK_MISC (label2), 0, 2);

  label3 = gtk_label_new ("Cancel");
  gtk_widget_show (label3);
  gtk_box_pack_start (GTK_BOX (vbox1), label3, FALSE, FALSE, 0);
  gtk_misc_set_padding (GTK_MISC (label3), 0, 6);


  g_signal_connect ((gpointer) window1, "expose_event",
                    G_CALLBACK (expose_handler),
                    NULL);      
  

  return window1;
}                                


static GdkFilterReturn root_filter (GdkXEvent *xevent, 
    GdkEvent *event2, gpointer user_data)
{
  XEvent *event = (XEvent *)xevent;
  Window target_win = None;
  Window target_win_xmu = None; 

  win_struct *win= (win_struct*) user_data; 

  switch (event->type) {
   
    case ButtonPress:  

      target_win= event->xbutton.subwindow;
      target_win_xmu=ClientWindow (target_win);
      
      //if (debug) printf ("target win: %d\n", target_win);
      //if (debug) printf ("target win xmu : %d\n", target_win_xmu);
        
        if (target_win == dialog_xlib || (

             target_win != None &&
             target_win != GDK_ROOT_WINDOW() &&
             target_win != target_win_xmu &&
             window_type_is_normal (target_win_xmu))) {

              gdk_pointer_ungrab  (GDK_CURRENT_TIME); 
              gdk_window_remove_filter (gdk_get_default_root_window (), root_filter, NULL);
              gtk_widget_destroy(dialog); 

            if (target_win == dialog_xlib) {
              if (debug) printf ("cancle");
              
              exit (0);
            } else  {
              if (debug) printf ("found alltrayable window");
              win->child_xlib=target_win_xmu;
              gtk_main_quit (); 
            }
  
          }

    break;

  }

  return GDK_FILTER_CONTINUE;

}

void new_grab (win_struct *win)
{

  GdkCursor *cursor = gdk_cursor_new (GDK_CROSSHAIR);
    gdk_pointer_grab (gdk_get_default_root_window() , FALSE, GDK_BUTTON_PRESS_MASK, 
      gdk_get_default_root_window(), cursor, GDK_CURRENT_TIME);
       
  gdk_cursor_unref (cursor);
  gdk_window_add_filter (gdk_get_default_root_window(), root_filter, win);            

}

void click_mode(win_struct *win)
{

  dialog=create_dialog();
  gtk_widget_show (dialog);

  dialog_xlib=GDK_WINDOW_XID(dialog->window);

  new_grab(win);

  gtk_main ();

}
