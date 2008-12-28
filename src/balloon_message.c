#include "config.h"
#include "common.h"
#include "utils.h"

#define tailwidth 10
#define border 10
#define radius 3
#define pad 5
#define inner_padding 10

void destroy_balloon (win_struct *win)
{
  
  if (win->balloon == NULL)
    return;
  
  if (win->balloon->start_delay)
    gtk_timeout_remove (win->balloon->start_delay);
  
  if (win->balloon->handler_id)
    g_signal_handler_disconnect ((gpointer) win->plug, win->balloon->handler_id);
  
  gtk_widget_destroy (win->balloon->window);
  g_free (win->balloon);
  win->balloon=NULL;
  
}

gboolean leave_handler (GtkWidget *widget, GdkEventCrossing *event,
               gpointer user_data)

{
 
  win_struct *win= (win_struct*) user_data;
 
  if (debug) printf ("leave handler\n");
  
  destroy_balloon (win);
  
  return FALSE;
}

gboolean timeout_function (gpointer user_data)
{
  
  win_struct *win= (win_struct*) user_data;

  if (debug) printf ("balloon timeout_function");
  
  if (win->balloon == NULL)
    return FALSE;
  
  destroy_balloon (win);
  
  return FALSE;
}

gboolean expose_handler (GtkWidget       *widget,
                GdkEventExpose  *event,
                gpointer         user_data)

{
  
  if (debug) printf ("balloon expose\n");
  
  balloon_struct *balloon= (balloon_struct*) user_data;
  
  gint w=balloon->w;
  gint h=balloon->h;
 
  gint tail_x=balloon->tail_x;
  
  if (balloon->tail_on_top) {
    
    gdk_draw_line (balloon->window->window, balloon->gc, border+radius, border, tail_x-tailwidth/2, border);
    gdk_draw_line (balloon->window->window, balloon->gc , tail_x+tailwidth/2, border, w-border-radius, border);
    gdk_draw_line (balloon->window->window, balloon->gc, border+radius, h-border, w-border, h-border);
    gdk_draw_line (balloon->window->window, balloon->gc, border+radius+1, h-border+1, w-border+1, h-border+1);
    gdk_draw_line (balloon->window->window, balloon->gc, tail_x-tailwidth/2, border, tail_x+tailwidth/2, 0); 
    gdk_draw_line (balloon->window->window, balloon->gc, tail_x+tailwidth/2, 0, tail_x+tailwidth/2, border);
    gdk_draw_line (balloon->window->window, balloon->gc, tail_x+tailwidth/2+1, 2, tail_x+tailwidth/2+1, border);

  } else {

    gdk_draw_line (balloon->window->window, balloon->gc, border+radius, h-border, tail_x-tailwidth/2, h-border);
    gdk_draw_line (balloon->window->window, balloon->gc, border+radius+1, h-border+1, tail_x-tailwidth/2, h-border+1); 
    gdk_draw_line (balloon->window->window, balloon->gc, tail_x+tailwidth/2, h-border, w-border, h-border); 
    gdk_draw_line (balloon->window->window, balloon->gc, tail_x+tailwidth/2, h-border+1, w-border+1, h-border+1); 
    gdk_draw_line (balloon->window->window, balloon->gc, border+radius, border, w-border-radius, border);
    gdk_draw_line (balloon->window->window, balloon->gc, tail_x-tailwidth/2, h-border, tail_x-tailwidth/2, h); 
    gdk_draw_line (balloon->window->window, balloon->gc, tail_x-tailwidth/2, h, tail_x+tailwidth/2, h-border); 
    gdk_gc_set_line_attributes (balloon->gc, 2, GDK_LINE_SOLID, GDK_CAP_ROUND, GDK_JOIN_ROUND);  
    gdk_draw_line (balloon->window->window, balloon->gc, tail_x-tailwidth/2+1, h, tail_x+tailwidth/2, h-border+1);
    gdk_gc_set_line_attributes (balloon->gc, 1, GDK_LINE_SOLID, GDK_CAP_ROUND, GDK_JOIN_ROUND); 
  }
 
  
  gdk_draw_line (balloon->window->window, balloon->gc, border, border+radius, border+radius, border);
  gdk_draw_line (balloon->window->window, balloon->gc, w-border-radius, border, w-border, border+radius);
  gdk_draw_line (balloon->window->window, balloon->gc, border, h-border-radius, border+radius, h-border);
  gdk_draw_line (balloon->window->window, balloon->gc, w-border-radius+1, h-border, w-border, h-border-radius+1);
  gdk_draw_line (balloon->window->window, balloon->gc, border, border+radius, border, h-border-radius);
  gdk_draw_line (balloon->window->window, balloon->gc, w-border, border+radius, w-border, h-border);
  gdk_draw_line (balloon->window->window, balloon->gc, w-border+1, border+radius+1, w-border+1, h-border);

  return FALSE;
 
}

gboolean one_moment (gpointer user_data)
{
  
  win_struct *win= (win_struct*) user_data;
  
  if (win->balloon == NULL)
    return FALSE;
  
  gtk_widget_show (win->balloon->window);


  return FALSE;
}

void show_balloon (win_struct *win, gchar *message, gint timeout)
{
  
  GtkWidget *label;
  GdkGC *shape_gc;
  GdkPixmap *xpm;
  GdkPoint *points;
  XWindowAttributes wa;
  gint tail_x;
  gint w;
  gint h;
 
  if (!win->balloon_message_allowed)
    return;
  
  if (win->balloon)
    return;
  
  if (!win->title)
    return;
  
  if (debug) printf ("show_balloon\n");
  
  balloon_struct *balloon = (balloon_struct*) malloc (sizeof (balloon_struct));
  
  win->balloon=balloon;
  balloon->handler_id=0;
  balloon->timeout=timeout;
  balloon->start_delay=0;
 
  balloon->window = gtk_window_new (GTK_WINDOW_POPUP);
  gtk_widget_set_app_paintable (balloon->window, TRUE);
  gtk_window_set_policy (GTK_WINDOW (balloon->window), FALSE, FALSE, TRUE);
  gtk_widget_set_name (balloon->window, "alltray-balloon_message");
  gtk_container_set_border_width (GTK_CONTAINER (balloon->window), border+inner_padding);
  
  gtk_widget_realize (balloon->window);
  
  balloon->gc= gdk_gc_new(balloon->window->window);
  gdk_gc_set_foreground(balloon->gc, &balloon->window->style->black);
  
  gdk_error_trap_push();
   
  XGetWindowAttributes (GDK_DISPLAY(), win->plug_xlib, &wa);
  
  if (!gdk_error_trap_pop()) {
    
    balloon->dock_w=wa.width;
    balloon->dock_h=wa.height;
    
    if (debug) printf ("wa.x: %d, wa.y %d\n", wa.x, wa.y);
  }
  
  Window junkwin;
      
  XTranslateCoordinates (GDK_DISPLAY(),win->plug_xlib, GDK_ROOT_WINDOW(), 
    -wa.border_width,-wa.border_width, &balloon->dock_x, &balloon->dock_y, &junkwin);

  if (debug) printf ("balloon->dock_x: %d, balloon->dock_y %d\n", balloon->dock_x, balloon->dock_y);
  
  
  label = gtk_label_new (NULL);
  gtk_label_set_line_wrap (GTK_LABEL (label), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label), 0.5, 0.5);
  gtk_widget_show (label);
  
  gtk_container_add (GTK_CONTAINER (balloon->window), label);
  gtk_label_set_markup (GTK_LABEL (label), message);

  gtk_window_get_size (GTK_WINDOW (balloon->window), &balloon->w, &balloon->h);
  
  w=balloon->w;
  h=balloon->h;
      
  if (debug) printf ("balloon->w: %d, balloon->h: %d\n", balloon->w, balloon->h);
  
  tail_x=balloon->tail_x=(balloon->w/5)*4;
  
  if (debug) printf ("tail_x: %d\n", tail_x);
  
  balloon->x=(balloon->dock_x + balloon->dock_w/2- tail_x);
  
  if (balloon->dock_y < balloon->dock_h + pad) {
    
    if (debug) printf ("on top\n");
    balloon->tail_on_top=TRUE;
    balloon->y=balloon->dock_y + balloon->dock_h + pad;
    
    balloon->x-=tailwidth/2;
    
  } else {
    
    balloon->tail_on_top=FALSE;
    if (debug) printf ("nicht on top\n");
    balloon->y=balloon->dock_y - balloon->h -pad;
  }

  do {
  
    if ((balloon->x + (balloon->w) >= win->screen_width)) {
      
      if (debug) printf ("balloon right out\n");
      
      balloon->x=win->screen_width - balloon->w + border -5;
      tail_x=balloon->tail_x=balloon->w - border - radius - tailwidth -2;
      break;
    }
    
    if (balloon->x < 0) {
      balloon->x=0;
      tail_x=balloon->tail_x=border + radius + tailwidth +2;
      break;
    }
    
  } while (0);
 
  
  if (debug) printf ("balloon->x %d, balloon->y: %d\n", balloon->x, balloon->y);
  
  xpm = gdk_pixmap_new ( NULL, balloon->w, balloon->h, 1);
  shape_gc = gdk_gc_new(xpm);
  gdk_gc_set_foreground(shape_gc, &balloon->window->style->black);
  gdk_draw_rectangle(xpm, shape_gc, TRUE, 0, 0, balloon->w, balloon->h);
  gdk_gc_set_foreground(shape_gc, &balloon->window->style->white);

  
  points=g_new(GdkPoint , 7);
  
  points[0].x=border;
  points[0].y=border+radius;
  points[1].x=border+radius;
  points[1].y=border;
  points[2].x=w-border-radius;
  points[2].y=border;
  points[3].x=w-border+2;
  points[3].y=border+radius+1;
  points[4].x=w-border+2;
  points[4].y=h-border+2;
  points[5].x=border+radius+1;
  points[5].y=h-border+2;
  points[6].x=border;
  points[6].y=h-border-radius;

  gdk_draw_polygon (xpm, shape_gc, TRUE, points, 7);
  g_free (points);

  gdk_gc_set_foreground(shape_gc, &balloon->window->style->black);
  gdk_draw_point (xpm, shape_gc, w-border+1, h-border+1);
  gdk_gc_set_foreground(shape_gc, &balloon->window->style->white);

  if (balloon->tail_on_top) {
  
    points=g_new(GdkPoint , 4);
    
    points[0].x=tail_x-tailwidth/2;
    points[0].y=border;
    points[1].x=tail_x+tailwidth/2;
    points[1].y=0;
    points[2].x=tail_x+tailwidth/2+1;
    points[2].y=1;  
    points[3].x=tail_x+tailwidth/2+2;
    points[3].y=border;
    
    gdk_draw_polygon (xpm, shape_gc, TRUE, points, 4);
    g_free (points);
  
  } else {
    
    points=g_new(GdkPoint , 4);
    
    points[0].x=tail_x-tailwidth/2;
    points[0].y=h-border+1;
    points[1].x=tail_x-tailwidth/2;
    points[1].y=h;
    points[2].x=tail_x-tailwidth/2+2;
    points[2].y=h;  
    points[3].x=tail_x+tailwidth/2+1;
    points[3].y=h-border+2;
    
    gdk_draw_polygon (xpm, shape_gc, TRUE, points, 4);
    g_free (points);
  
  }

  gtk_widget_shape_combine_mask(balloon->window, xpm, 0, 0 );

  gtk_window_move (GTK_WINDOW (balloon->window), balloon->x, balloon->y);
 
  if (win->balloon->timeout == 0) {
    
    win->balloon->start_delay=gtk_timeout_add (500, one_moment,
      (gpointer) win);
   
  } else {
    
    gtk_widget_show (win->balloon->window);

    win->balloon->start_delay=gtk_timeout_add (win->balloon->timeout * 1000,
      timeout_function, (gpointer) win);
    
  }
  
  g_signal_connect ((gpointer) balloon->window, "expose_event",
                    G_CALLBACK (expose_handler), (gpointer) balloon);
  
  win->balloon->handler_id=g_signal_connect ((gpointer) win->plug,
    "leave_notify_event", G_CALLBACK (leave_handler), (gpointer) win);
  

}
