/* -*- mode: vala; c-basic-offset: 2; tab-width: 8; -*-
 * GtkStatusIcon.vala - GTK+ status icon widget and menu
 * Copyright (c) 2011 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation
 */
using GLib;
using Gtk;

namespace AllTray {
  public errordomain GtkStatusIconError {
    FAILED
  }

  public class GtkStatusIcon : StatusIcon {
    private AllTray.Application _app;

    public GtkStatusIcon(Gdk.Pixbuf pixbuf, string tooltip) {
      this.set_from_pixbuf(pixbuf);
      this.set_tooltip(tooltip);
    }

    public void set_tooltip(string tooltip) {
      base.set_tooltip_markup("<b>AllTray:</b> %s".printf(tooltip));
    }
  }
}