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

      if((Build.PACKAGE_VERSION.chr(-1, '+') != null) &&
	 (Build.ALLTRAY_BZR_BUILD == "TRUE")) {
	this.version =
	  _("%s\nBranch: %s, r%s\n%s").printf(Build.PACKAGE_VERSION,
					      Build.ALLTRAY_BZR_BRANCH,
					      Build.ALLTRAY_BZR_REVISION,
					      Build.ALLTRAY_BZR_REVID);
      } else {
	this.version = Build.PACKAGE_VERSION;
      }

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