/* -*- mode: vala; c-basic-offset: 2; tab-width: 8; -*-
 * alltray.vapi - Imports information into Vala from the build system.
 */
[CCode(cheader_filename = "config.h",
       cprefix="", lower_case_cprefix = "")]
namespace AllTray.Build {
  public const string PACKAGE;
  public const string PACKAGE_VERSION;

  public const string GETTEXT_PACKAGE;
  public const string LOCALE_DIR;

  public const string ALLTRAY_COPYRIGHT_YEARS;
  public const string ALLTRAY_LICENSE;

  public const string ALLTRAY_BZR_BUILD;
  public const string ALLTRAY_BZR_BRANCH;
  public const string ALLTRAY_BZR_REVID;
  public const string ALLTRAY_BZR_REVISION;

  public const string ALLTRAY_C_COMPILER;
  public const string ALLTRAY_COMPILE_FLAGS;
  public const string ALLTRAY_COMPILE_OS;
  public const string ALLTRAY_COMPILE_OS_REL;
  public const string ALLTRAY_COMPILE_OS_VER;
  public const string ALLTRAY_COMPILE_OS_HOST;
  public const string ALLTRAY_COMPILE_BUILD_DATE;
  public const string ALLTRAY_COMPILE_BUILD_DATE_EUS;

  public const string ALLTRAY_VALA_COMPILER;

  public const string ALLTRAY_CONFIGURE_FLAGS;
}

[CCode(cprefix = "", lower_case_cprefix = "")]
namespace AllTray.Private.GLib {
  [CCode(cheader_filename = "glib.h,glib/gi18n-lib.h")]
  public static weak string nggettext(string sing, string plural, int count);
}

[CCode(cprefix="Gtk", lower_case_cprefix="gtk_")]
namespace AllTray.LocalGtk {
  [CCode (cheader_filename = "gtk/gtk.h")]
  public class StatusIcon : GLib.Object {
    [CCode (has_construct_function = false)]
    public StatusIcon.from_file (string filename);
    [CCode (has_construct_function = false)]
    public StatusIcon.from_gicon (GLib.Icon icon);
    [CCode (has_construct_function = false)]
    public StatusIcon.from_icon_name (string icon_name);
    [CCode (has_construct_function = false)]
    public StatusIcon.from_pixbuf (Gdk.Pixbuf pixbuf);
    [CCode (has_construct_function = false)]
    public StatusIcon.from_stock (string stock_id);
    public bool get_blinking ();
    public bool get_geometry (out unowned Gdk.Screen screen, out Gdk.Rectangle area, out Gtk.Orientation orientation);
    public unowned GLib.Icon get_gicon ();
    public bool get_has_tooltip ();
    public unowned string get_icon_name ();
    public unowned Gdk.Pixbuf get_pixbuf ();
    public unowned Gdk.Screen get_screen ();
    public int get_size ();
    public unowned string get_stock ();
    public Gtk.ImageType get_storage_type ();
    public unowned string get_tooltip_markup ();
    public unowned string get_tooltip_text ();
    public bool get_visible ();
    public uint32 get_x11_window_id ();
    public bool is_embedded ();
    [CCode (has_construct_function = false)]
    public StatusIcon ();
    [CCode (instance_pos = -1)]
    public void position_menu (Gtk.Menu menu, out int x, out int y, out bool push_in);
    public void set_blinking (bool blinking);
    public void set_from_file (string filename);
    public void set_from_gicon (GLib.Icon icon);
    public void set_from_icon_name (string icon_name);
    public void set_from_pixbuf (Gdk.Pixbuf pixbuf);
    public void set_from_stock (string stock_id);
    public void set_has_tooltip (bool has_tooltip);
    public void set_screen (Gdk.Screen screen);
    public void set_tooltip (string tooltip_text);
    public void set_tooltip_markup (string markup);
    public void set_tooltip_text (string text);
    public void set_visible (bool visible);
    public bool blinking { get; set; }
    [NoAccessorMethod]
    public bool embedded { get; }
    [NoAccessorMethod]
    public string file { set; }
    [NoAccessorMethod]
    public GLib.Icon gicon { owned get; set; }
    public bool has_tooltip { get; set; }
    [NoAccessorMethod]
    public string icon_name { owned get; set; }
    [NoAccessorMethod]
    public Gtk.Orientation orientation { get; }
    [NoAccessorMethod]
    public Gdk.Pixbuf pixbuf { owned get; set; }
    public Gdk.Screen screen { get; set; }
    public int size { get; }
    [NoAccessorMethod]
    public string stock { owned get; set; }
    public Gtk.ImageType storage_type { get; }
    public string tooltip_markup { get; set; }
    public string tooltip_text { get; set; }
    public bool visible { get; set; }
    public virtual signal void activate ();
    public virtual signal bool button_press_event (Gdk.Event event);
    public virtual signal bool button_release_event (Gdk.Event event);
    public virtual signal void popup_menu (uint button, uint activate_time);
    public virtual signal bool query_tooltip (int x, int y, bool keyboard_mode, Gtk.Tooltip tooltip);
    public virtual signal bool scroll_event (Gdk.Event event);
    public virtual signal bool size_changed (int size);
  }
}
