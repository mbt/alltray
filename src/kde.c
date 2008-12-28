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
#include "parent.h"
#include "utils.h"

#include <X11/cursorfont.h>
#include <fcntl.h>
#include <sys/stat.h>

GtkWidget *hint_label;
GtkWidget *cancel_label;

Window get_kwin_window (Window target)
{

  Window our;
  Window parent=None;
  Window root_return=None;
  Window *our_kids;
  gint our_nkids;
  gint err;
  gint i;
  XClassHint class_hints;
  int result;
  Window kwin_window=None;

  our= one_under_root (GDK_DISPLAY(), target);
 
  gdk_error_trap_push();
  if (!XQueryTree (GDK_DISPLAY (), our, &root_return, &parent, &our_kids, (unsigned int *) &our_nkids))
     {exit (1); }
  gdk_error_trap_pop();


  for (i=0; i < our_nkids; i++)  {

    if (debug) printf ("our kid %d: %d\n", i, (gint) our_kids[i]);
    
    gdk_error_trap_push();
    result=XGetClassHint(GDK_DISPLAY(), our_kids[i], &class_hints);
    err=gdk_error_trap_pop();
    
    if (err || result==0 ) {
      printf ("ERROR get class hints\n"); 
      continue;
    } else {
    
      if (debug) printf ("our_kids: res_class: %s  res_name %s   \n",
      class_hints.res_class, class_hints.res_name);
      
      if (!strcmp (class_hints.res_class, "Kwin")) {
      
        kwin_window = our_kids[i];
        XFree (class_hints.res_class);
        XFree (class_hints.res_name);
        break;
      
      }

      XFree (class_hints.res_class);
      XFree (class_hints.res_name);
    
    }


  }

  if (our_kids) XFree ((char *)our_kids);

  return kwin_window;
}

void free_button_list (GList *buttons)
{

  GList *l;

   l=buttons;
  
  while (l != NULL) {
    button *value= (button*) l->data;
    g_free (value);
    l=l->next;
  }

  g_list_free (buttons);
}

GList *get_button_list (Window kwin_window)
{

  Window *kwin_kids, root_return, parent;
  gint kwin_nkids;
  GList *buttons=NULL;
  gint i;
  GList *l;

  gdk_error_trap_push();
   if (!XQueryTree (GDK_DISPLAY (), kwin_window, &root_return, &parent, &kwin_kids, (unsigned int *) &kwin_nkids))
     { exit (1); }
  gdk_error_trap_pop();


  for (i=0; i < kwin_nkids; i++) {
    if (debug) printf ("kwin_kid[%d]: %d        ", i, (gint) kwin_kids[i]);
  
    XWindowAttributes w;
  
    gdk_error_trap_push();
    XGetWindowAttributes (GDK_DISPLAY (), kwin_kids[i], &w);
    if (gdk_error_trap_pop())
      continue;

    if (debug) printf ("x: %d; y: %d width: %d \n", w.x, w.y, w.width);
  
    button *new =g_new (button, 1);
  
    new->window_xlib = kwin_kids[i];
    new->window_gdk =gdk_window_foreign_new (new->window_xlib);
    new->start_x = w.x;
    new->start_y = w.y;
    new->width=w.width;
    new->height=w.height;
  
    buttons= g_list_append (buttons, (gpointer) new);
  }

  if (kwin_kids) XFree ((char *)kwin_kids);
  
  l=buttons;
  
  while (l != NULL) {
  
    button *value= (button*) l->data;
    
    if (debug) printf ("button window: %d\n", (gint) value->window_xlib);
    
    l=l->next;
  
  }

  return buttons;

}

gint button_num_by_x (GList *buttons, gint x, gint y)
{

  gboolean valid=FALSE;
  gint num=1;
  GList *l;

  l=buttons;
  
  while (l != NULL) {
  
    button *value= (button*) l->data;

  //  printf ("start_x: %d, end_x: %d, start_y: %d, end_y: %d\n", 
   //     value->start_x, value->end_x,value->start_y,value->end_y);

    if (x >= value->start_x && x <=  (value->start_x + value->width -1) &&
      y >= value->start_y && y <= (value->start_y + value->height -1)) {
      if (debug) printf ("valid\n");
      valid=TRUE;
      break;
    }
    
    num++;
    l=l->next;
  
  }
  
  if (valid)
    return num;
  else
   return 0;

}

gboolean notabutton_func (gpointer user_data)
{

  gtk_label_set_text (GTK_LABEL(hint_label), " >not a button<");
  gtk_sleep (1000);
  gtk_label_set_text (GTK_LABEL(hint_label), "Waiting for click...");

  return FALSE;
}

gint get_button_number (Window target, GList *buttons)
{
  
  int status;
  XEvent event;
  gboolean button_pressed=FALSE;
  gboolean canceled =FALSE;
  gint num=0;

  Cursor cursor = XCreateFontCursor(GDK_DISPLAY(), XC_crosshair);

  status = XGrabPointer(GDK_DISPLAY(), target, False, ButtonReleaseMask |
    ButtonPressMask, GrabModeSync, GrabModeAsync, target, cursor, CurrentTime);
  
  if (status != GrabSuccess) {
    exit (1);
  }

  while (!button_pressed && !canceled) {
  
    XAllowEvents(GDK_DISPLAY(), SyncPointer, CurrentTime);
      XWindowEvent(GDK_DISPLAY(), target,
      ButtonPressMask | ButtonReleaseMask, &event);
    
    switch (event.type) {
    
      case ButtonRelease:
  
        num=button_num_by_x (buttons, event.xbutton.x, event.xbutton.y);
        if (debug) printf ("num: %d\n", num);
            
        if (num != 0)
          button_pressed=TRUE;
        else {
          if (debug) printf ("canceld=TRUE\n");
          canceled=TRUE;
        }
         
      break;
     
        
       case ButtonPress:
         
        /*if (debug) printf ("button press\n");
        num=button_num_by_x (buttons, event.xbutton.x, event.xbutton.y);
        if (debug) printf ("num: %d\n", num);
        
        if (num == 0)
          g_timeout_add (1, notabutton_func, NULL);*/
  
       break;

   }
  
  // while (gtk_events_pending ())
  //   gtk_main_iteration ();
  
  }

  XUngrabPointer(GDK_DISPLAY(), CurrentTime);
  XFreeCursor (GDK_DISPLAY(), cursor);

  gtk_label_set_markup (GTK_LABEL(cancel_label), "<span size=\"small\"> </span>");

  if (!canceled) {
    gtk_label_set_text (GTK_LABEL(hint_label), "Thanks. Configuration completed.");
    gtk_sleep  (4000);
  }
    
  else {
    gtk_label_set_text (GTK_LABEL(hint_label), "Configuration canceled.");
    gtk_sleep (2000);
  }

  return num;
}

gboolean write_string_to_file (char *str, char* file)
{

  mode_t mode = S_IRUSR | S_IWUSR;
  ssize_t size;
  int fd;

  umask(0);

  size = strlen(str);

  fd = open(file, O_WRONLY | O_TRUNC | O_CREAT, mode);

  if(fd == -1) {
      perror("error with open()");
      return FALSE;
  }

  if(write(fd, str, size) != size) {
      perror("error with write()");
      return FALSE;
  }

  close (fd);

  return TRUE;

}

gchar *
alltray_user_dir(void) 
{

  gchar *user_dir=NULL;
  user_dir=g_strconcat (g_get_home_dir (), G_DIR_SEPARATOR_S, ".alltray", NULL);

  return user_dir;
}

GtkWidget*
create_configure_window (void)
{
  GtkWidget *configure_window;
  GtkWidget *vbox1;
  GtkWidget *label3;
  GtkWidget *hseparator1;
  GtkWidget *vbox2;

  configure_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (configure_window), "AllTray - Configuration");
  gtk_window_set_position (GTK_WINDOW (configure_window), GTK_WIN_POS_CENTER);
  gtk_window_set_resizable (GTK_WINDOW (configure_window), FALSE);
  //gtk_window_set_skip_taskbar_hint (GTK_WINDOW (configure_window), TRUE);
  gtk_window_set_skip_pager_hint (GTK_WINDOW (configure_window), TRUE);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (configure_window), vbox1);
  gtk_container_set_border_width (GTK_CONTAINER (vbox1), 20);

  label3 = gtk_label_new ( "<span size=\"large\">Welcome to AllTray Configuration utility </span>\n\nAllTray need to know which of the Window - Frame buttons\nis the \"Close\" button (the one with the \"X\").\n\t\n<span foreground=\"red\">To configure AllTay please click on the close button\n in the top Frame of this window. </span>\n\nYou need to do this only after each theme change !\nYou can start this wizard manualy with: \"alltray --configure\"\n");
  gtk_widget_show (label3);
  gtk_box_pack_start (GTK_BOX (vbox1), label3, FALSE, FALSE, 0);
  gtk_label_set_use_markup (GTK_LABEL (label3), TRUE);
  gtk_label_set_justify (GTK_LABEL (label3), GTK_JUSTIFY_CENTER);

  hseparator1 = gtk_hseparator_new ();
  gtk_widget_show (hseparator1);
  gtk_box_pack_start (GTK_BOX (vbox1), hseparator1, TRUE, TRUE, 7);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox2);
  gtk_box_pack_start (GTK_BOX (vbox1), vbox2, FALSE, FALSE, 0);

  hint_label = gtk_label_new ("Waiting for click...");
  gtk_widget_show (hint_label);
  gtk_box_pack_start (GTK_BOX (vbox2), hint_label, FALSE, FALSE, 0);

  cancel_label = gtk_label_new ("<span size=\"small\"> (To cancel click somewhere else) </span>");
  gtk_widget_show (cancel_label);
  gtk_box_pack_start (GTK_BOX (vbox2), cancel_label, FALSE, FALSE, 0);
  gtk_label_set_use_markup (GTK_LABEL (cancel_label), TRUE);

  return configure_window;
}

void save_close_button_position (gboolean pos)
{

  gchar *user_dir=NULL;
  gchar *filename=NULL;
  FILE *file;

  user_dir=alltray_user_dir();
  if (debug) printf ("user_dir: %s\n", user_dir);

  file = fopen(user_dir, "r");

  if(!file)
    mkdir(user_dir, S_IRUSR | S_IWUSR | S_IXUSR);
  else
    fclose(file);
  
  filename = g_build_filename(user_dir, "prefs", NULL);
  
  if (debug) printf ("filnename: %s\n", filename);
  
  if (pos == LEFT)
    write_string_to_file ("left", filename);
  else
    write_string_to_file ("right", filename);

  g_free (user_dir);
  g_free (filename);

}

gboolean kde_get_close_button_positon (void)
{

  gchar *user_dir=NULL;
  gchar *content;
  gchar *filename;
  gboolean return_value=NO_SUCCESS;

  user_dir=alltray_user_dir();
  if (debug) printf ("user_dir: %s\n", user_dir);

  filename = g_build_filename(user_dir, "prefs", NULL);

  g_free (user_dir);

  if (!g_file_get_contents (filename, &content, NULL, NULL))
    return NO_SUCCESS;

  g_free (filename);

  if (debug) printf ("prefs content: %s\n", content);

  if (!strcmp (content, "left"))
    return_value=LEFT;

  if (!strcmp (content, "right"))
    return_value=RIGHT;

  g_free (content);

  return return_value;

}

gboolean kde_show_configure_dialog (win_struct *win)
{

  GtkWidget *window;
  GdkWindow  *target_gdk;
  Window target;
  Window kwin_window;
  gint num;
  GList *buttons;

  window=create_configure_window();
  gtk_window_set_keep_above (GTK_WINDOW (window), TRUE); //kwin bug
  gtk_widget_realize (window);
  gtk_widget_show (window);

  //while (gtk_events_pending ())
  // gtk_main_iteration ();

  gtk_sleep (500);

  target_gdk = window->window;
  target=GDK_WINDOW_XID(target_gdk);

  //printf ("target id is: %d\n", target);
  
  kwin_window= get_kwin_window (target);
  //printf ("kwin_window: %d\n", kwin_window);
  
  buttons=get_button_list (kwin_window);
  num= get_button_number (kwin_window, buttons);
  
  if (debug) printf ("return num is: %d\n", num);

  free_button_list (buttons);

  gtk_widget_destroy (window);

  if (num == 0)
    return FALSE;

  if (num ==1)  {
    save_close_button_position (LEFT);
    win->kde_close_button_pos=LEFT;
  } else {
    save_close_button_position (RIGHT);
    win->kde_close_button_pos=RIGHT;
  }

return TRUE;

}

button kde_get_close_button (Window target, gboolean pos)
{

  GList *buttons;
  Window kwin_window= get_kwin_window (target);;

  buttons=get_button_list (kwin_window);

  if (pos ==RIGHT) 
      buttons=g_list_last (buttons);
  else
    buttons=g_list_first (buttons);

  button value=  *((button*) buttons->data);

  free_button_list (buttons);
  
  return value;

}
