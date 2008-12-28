#include "common.h"

#define GCONF_FILE ".gconf/apps/metacity/general/%gconf.xml"
#define THEME_FILENAME "metacity-theme-1.xml"
#define THEME_SUBDIR "metacity-1"
#define THEME_DIR1 "/usr/share/"
#define THEME_DIR2 "/usr/local/share/"
 
static int depth_gconf = 0;
static int depth= 0;
static gboolean found_theme=FALSE;
static gboolean found_element=FALSE;
static gboolean read_stringvalue=FALSE;
gchar *theme_name=NULL;


static void start_element_handler_gconf (GMarkupParseContext * ctx,
  const gchar * element_name,
  const gchar ** attr_names,
  const gchar ** attr_vals, gpointer user_data, GError ** err) {

  if (debug) printf ("start handler: element_name ist: %s\n", element_name);

  if (strncmp (element_name, "entry", 5) == 0 && depth_gconf == 1){

    found_theme=FALSE;

    int i = 0;
    while (attr_names[i] != NULL) {

      if (strcmp (attr_names[i], "name") == 0) {
      
        if (strcmp (attr_vals[i], "theme") == 0) {
          if (debug) fprintf (stdout, "theme entry found !\n");
          found_theme=TRUE;
          break;
        }
      
      }
      i++;
    }

  }


  if (found_theme && strncmp (element_name, "stringvalue", 11) == 0 && depth_gconf == 2) {
    read_stringvalue=TRUE;
  }

  depth_gconf++;
}

static void end_element_handler_gconf (GMarkupParseContext * ctx,
    const gchar * element_name, gpointer data, GError ** err) {

  read_stringvalue=FALSE;
  depth_gconf--;
}

static void
text_handler_gconf (GMarkupParseContext * ctx,
               const gchar * text, gsize len, gpointer data, GError ** err) {

  if (read_stringvalue && depth_gconf == 3) {
    if (debug) fprintf (stdout, "%s\n", text);
    theme_name=g_strdup(text);
  }

}

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
 
static GMarkupParser gconf_parser = {
         start_element_handler_gconf,
         end_element_handler_gconf,
         text_handler_gconf,
         NULL,
         error_handler
};

static GMarkupParser parser = {
         start_element_handler,
         end_element_handler,
         NULL,
         NULL,
         error_handler
};
 
gboolean parse_theme (win_struct *win) {

  gchar *content=NULL, *gconf_content=NULL;
  gint length, gconf_length;
  GError *err=NULL;
  GMarkupParseContext *ctx, *gconf_ctx;
  gchar *theme_dir, *theme_file;
  gchar *metacity_gconf_file;

  metacity_gconf_file = g_build_filename (g_get_home_dir (), GCONF_FILE,NULL);

  err = NULL;
  if (!g_file_get_contents (metacity_gconf_file, &gconf_content, &gconf_length, &err)) {
    printf( "Failed to read gconf file %s: %s\n",  metacity_gconf_file, err->message);
    g_error_free (err);
    g_free (metacity_gconf_file);
    return FALSE;
  }

  g_free (metacity_gconf_file);

  if (debug) printf ("gconf_content: %s\n", gconf_content);

  err = NULL;
  gconf_ctx = g_markup_parse_context_new (&gconf_parser, 0, NULL, NULL);

  if (!g_markup_parse_context_parse (gconf_ctx, gconf_content, gconf_length, NULL)) {
    g_free (gconf_content);
    g_markup_parse_context_free (gconf_ctx);
    return FALSE;
  }
  
  if (!g_markup_parse_context_end_parse (gconf_ctx, NULL)) {
    g_free (gconf_content);
    g_markup_parse_context_free (gconf_ctx);
    return FALSE;
  }

  g_markup_parse_context_free (gconf_ctx);
  g_free (gconf_content);


  g_assert (theme_name);
  if (debug) printf ("theme name: %s\n", theme_name);


  theme_dir = g_build_filename (g_get_home_dir (), ".themes", theme_name, 
      THEME_SUBDIR, NULL);

  theme_file = g_build_filename (theme_dir, THEME_FILENAME, NULL);
  
  err = NULL;
  if (!g_file_get_contents (theme_file,  &content, &length, &err)) {
    if (debug) printf( "Failed to read theme from file %s: %s\n",  theme_file, err->message);
    g_error_free (err);
    g_free (theme_dir);
    g_free (theme_file);
    theme_file = NULL;
  }

  if (content == NULL) { 
    theme_dir = g_build_filename (THEME_DIR1, "themes", theme_name,
        THEME_SUBDIR, NULL);
    
    theme_file = g_build_filename (theme_dir, THEME_FILENAME, NULL);
    
    err = NULL;
    if (!g_file_get_contents (theme_file, &content, &length, &err)) {
      printf ("Failed to read theme from file %s: %s\n",  theme_file, err->message);
      g_error_free (err);
      g_free (theme_file);
      g_free (theme_dir);
      return FALSE;
    }
  }
  
  g_assert (content);

  g_free (theme_name);
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
