/*
 * TrayIcon.vala - Tray icon management
 * Copyright (c) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation
 */
using GLib;

namespace AllTray {
	public errordomain TrayIconError {
		FAILED
	}

	public class TrayIcon : GLib.Object {
		private AllTray.Application _appForIcon;
		private AllTray.Process _processForIcon;
		private Gtk.StatusIcon _statusIcon;

		public TrayIcon(AllTray.Process process,
						AllTray.Application application) {
			_appForIcon = application;
			_processForIcon = process;

			_statusIcon = new Gtk.StatusIcon();
			StringBuilder msg = new StringBuilder();
			msg.append_printf("Created tray icon for PID %d",
							  (int)_processForIcon.get_pid());

			if(_appForIcon.wnck_app == null) {
				// The application isn't ready yet.  Wait for it to
				// become ready and _then_ get the icon setup.
				_statusIcon.set_from_stock(Gtk.STOCK_MISSING_IMAGE);
				GLib.Timeout.add_seconds(1, get_app_icon_retry);
			} else {
				_statusIcon.set_from_pixbuf(_appForIcon.wnck_app.get_icon());
			}

			_statusIcon.visible = true;
			_statusIcon.activate += on_click;

			Debug.Notification.emit(Debug.Subsystem.TrayIcon,
									Debug.Level.Information,
									msg.str);
		}

		public bool get_app_icon_retry() {
			if(_appForIcon.wnck_app == null)
				return(true);

			_statusIcon.set_from_pixbuf(_appForIcon.wnck_app.get_icon());
			return(false);
		}

		public void on_click() {
			_appForIcon.toggle_visibility();
		}
	}
}