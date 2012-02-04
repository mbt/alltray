/* -*- mode: vala; c-basic-offset: 2; tab-width: 8; -*-
 * About.vala - AllTray about dialog
 * Copyright (c) 2011 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation
 */
using GLib;
using Gtk;

namespace AllTray {
  public class AboutDialog : Gtk.AboutDialog {
    public AboutDialog() {
      this.program_name = "AllTray";

      this.version = Build.PACKAGE_VERSION;

      this.website = "http://alltray.trausch.us/";
      this.copyright =
	_("Copyright © %s").printf(Build.ALLTRAY_COPYRIGHT_YEARS);
      this.comments = _("Dock applications in the system tray.");
      this.license = Build.ALLTRAY_LICENSE;
      this.response.connect(() => { this.destroy(); });
      this.show_all();
    }
  }
}