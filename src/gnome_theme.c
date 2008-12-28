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

#include "common.h"

#define GCONF_METACITY_THEME_PATH "/apps/metacity/general/theme"
#define THEME_FILENAME "metacity-theme-1.xml"
#define THEME_SUBDIR "metacity-1"
#define THEME_DIR1 "/usr/share/"
#define THEME_DIR2 "/usr/local/share/"
 
static int depth= 0;
static gboolean found_element=FALSE;
gchar *theme_name=NULL;


static void start_element_handler (GMarkupParseContext * ctx,
  const gchar * element_name,
  const gchar ** attr_names,
  const gchar ** attr_vals, gpointer user_data, GError ** err) {


  win_struct *win= (win_struct*) user_data;


  if (strncmp (element_name, "frame_geometry", 14) == 0 && depth == 1){

    int i = 0;
    while (attr_names[i] != NULL) {

      if (strcmp (attr_names[i], "name") == 0) {
      
        if (strcmp (attr_vals[i], "normal") == 0) {
          if (debug) fprintf (stdout, "normal found !\n");
          found_element=TRUE;
          break;
        }
      
      }
      i++;
    }

  }
  
  if (found_element &&  strncmp (element_name, "distance", 8) == 0 && depth == 2) {
    
    int i = 0;
    while (attr_names[i] != NULL) {

      if (strcmp (attr_names[i], "name") == 0) {
      
        if (strcmp (attr_vals[i], "right_titlebar_edge") == 0) {
        
         if (debug) fprintf (stdout, "right_titlebar_edge: %s !\n", attr_vals[i+1]);

          win->target_right_border=atoi (attr_vals[i+1]);
        
          found_element=FALSE;
          break;
        
        }
      
      }

      i++;
    }

  }
  
  depth++;
}

static void end_element_handler (GMarkupParseContext * ctx,
                      const gchar * element_name, gpointer data, GError ** err) {
  depth--;
}

static void error_handler (GMarkupParseContext * ctx, GError * err, gpointer data) {
         fprintf (stderr, " %s\n", err->message);
}

static GMarkupParser parser = {
         start_element_handler,
         end_element_handler,
         NULL,
         NULL,
         error_handler
};

gchar *get_metacity_theme (win_struct *win) {


  GConfClient *client;
  gchar *theme=NULL;
    
  client = gconf_client_get_default();
  theme= gconf_client_get_string(client, GCONF_METACITY_THEME_PATH, NULL);

  if (debug) printf ("theme is: %s\n", theme);

  g_object_unref(client);

  return theme;

}

gboolean parse_theme (win_struct *win) {

  gchar *content=NULL;
  gsize length=0;
  GError *err=NULL;
  GMarkupParseContext *ctx;
  gchar *theme_dir, *theme_file;
  gchar *metacity_theme=NULL;

  metacity_theme=get_metacity_theme (win);
  if (metacity_theme == NULL) {
     printf ("\n\nAlltray: ups...\n");
  }
  g_assert (metacity_theme);
  
  if (debug) printf ("theme name: %s\n", metacity_theme);

  theme_dir = g_build_filename (g_get_home_dir (), ".themes", metacity_theme, 
      THEME_SUBDIR, NULL);
  theme_file = g_build_filename (theme_dir, THEME_FILENAME, NULL);
  
  err = NULL;
  if (!g_file_get_contents (theme_file, &content, &length, &err)) {
    if (debug) printf( "Failed to read theme from file %s: %s\n",  theme_file, err->message);
    g_error_free (err);
    g_free (theme_dir);
    g_free (theme_file);
 }

  if (content == NULL) {
  
    theme_dir = g_build_filename (THEME_DIR1, "themes", metacity_theme,
      THEME_SUBDIR, NULL);
    
    if (debug) printf ("try location: %s\n", theme_dir);
    
    theme_file = g_build_filename (theme_dir, THEME_FILENAME, NULL);
    
    err = NULL;
    if (!g_file_get_contents (theme_file, &content, &length, &err)) {
    if (debug) printf ("Failed to read theme from file %s: %s\n",  theme_file, err->message);
    g_error_free (err);
    g_free (theme_file);
    g_free (theme_dir);
    return FALSE;
    }
  
  }

  
  g_assert (content);

  g_free (metacity_theme);
  g_free (theme_file);
  g_free (theme_dir);


  err = NULL;
  ctx = g_markup_parse_context_new (&parser, 0, (gpointer) win, NULL);

  if (!g_markup_parse_context_parse (ctx, content, length, NULL)) {
    g_free (content);
    g_markup_parse_context_free (ctx);
    return FALSE;
  }
  
  if (!g_markup_parse_context_end_parse (ctx, NULL)) {
    g_free (content);
    g_markup_parse_context_free (ctx);
    return FALSE;
  }

  g_markup_parse_context_free (ctx);
  g_free (content);

  return TRUE;
}
