/*
 * TrayMenu.vala - Tray Menu for a tray icon.
 * Copyright (C) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation.
 */
using GLib;

namespace AllTray {
	public class TrayMenu : Gtk.Menu {
		private Gtk.MenuItem _toggleVis;
		private Gtk.MenuItem _undock;
		private Gtk.MenuItem _separator;
		private Gtk.MenuItem _about;
		private Gtk.Menu _mnuWindowList;
		private Gtk.MenuItem _allWindows;

		public signal void toggle_app_visibility();
		public signal void toggle_window_visibility(Wnck.Window w);
		public signal void undock();

		public TrayMenu(List<Wnck.Window> winList) {
			_toggleVis = new Gtk.MenuItem.with_label("Toggle Visibility");
			append(_toggleVis);
			_toggleVis.show();

			_undock = new Gtk.MenuItem.with_label("Undock");
			_undock.activate.connect(() => undock());
			append(_undock);
			_undock.show();

			_separator = new Gtk.MenuItem();
			append(_separator);
			_separator.show();

			_about = new Gtk.MenuItem.with_label("About AllTray…");
			_about.activate.connect(show_about);
			append(_about);
			_about.show();

			_mnuWindowList = new Gtk.Menu();
			_allWindows = new Gtk.MenuItem.with_label("All Windows");
			_allWindows.activate.connect(() => {
					toggle_app_visibility();
				});

			Gtk.MenuItem sep = new Gtk.MenuItem();
			foreach(Wnck.Window w in winList) {
				Gtk.MenuItem newItem = 
					new Gtk.MenuItem.with_label(w.get_name());
				newItem.set_data("target", w);
				newItem.activate.connect((item) => {
						Wnck.Window win = (Wnck.Window)item.get_data("target");
						toggle_window_visibility(win);
					});
				_mnuWindowList.append(newItem);
				newItem.show();
			}

			_mnuWindowList.append(sep);
			sep.show();

			_mnuWindowList.append(_allWindows);
			_allWindows.show();

			_toggleVis.set_submenu(_mnuWindowList);
			_mnuWindowList.show();

			this.show();
			this.show_all();
		}

		private void show_about() {
			Gtk.AboutDialog about = new Gtk.AboutDialog();
			about.program_name = "AllTray";
			about.version = Build.PACKAGE_VERSION;
			about.website = "http://alltray.trausch.us/";
			about.copyright = "Copyright © "+Build.ALLTRAY_COPYRIGHT_YEARS;
			about.comments = "Dock applications in the system tray.";
			about.license = Build.ALLTRAY_LICENSE;

			about.response += dialog_destroy;

			about.show_all();
		}

		private void dialog_destroy(Gtk.AboutDialog which, int resp_id) {
			which.destroy();
		}
	}
}
