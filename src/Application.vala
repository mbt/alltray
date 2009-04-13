/*
 * Application.vala - Application-based window management for AllTray
 * Copyright (c) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation
 */
using GLib;

namespace AllTray {
	public errordomain ApplicationError {
		FAILED
	}

	public class Application : GLib.Object {
		private unowned List<Wnck.Window> _windows;
		private Wnck.Application? _wnckApp;
		private AllTray.LocalGtk.StatusIcon _appIcon;
		// private Sexy.Tooltip _appIconTooltip;
		private Process _process;
		private bool _appVisible;

		public Wnck.Application wnck_app {
			get {
				return(_wnckApp);
			}
		}

		public Application(Process p) {
			_process = p;
			Debug.Notification.emit(Debug.Subsystem.Application,
									Debug.Level.Information,
									"Hey hey, creating a new Application");

			/*
			 * Register interest in learning of new applications on
			 * the display, and then we'll be able to perform the rest
			 * of the setup after we get notification of the
			 * application.
			 */
			Program.WnckScreen.application_opened += maybe_setup;
			Program.WnckScreen.application_closed += bye_wnck_app;

			// If we got early notifications, let's work through them.
			if(Program.WnckEarlyApps != null) {
				foreach(Wnck.Application app in Program.WnckEarlyApps) {
					maybe_setup(Program.WnckScreen, app);
				}
			}
		}

		private void bye_wnck_app(Wnck.Screen scr, Wnck.Application app) {
			if(app == _wnckApp) {
				Debug.Notification.emit(Debug.Subsystem.Application,
										Debug.Level.Information,
										"WHOA - The app went away‽  "+
										"Looking for it to come back...");

				scr.window_opened -= maybe_update_window_count;
				scr.window_closed -= maybe_update_window_count;

				Program.WnckScreen.application_opened += maybe_setup;
			}
		}

		private void maybe_setup(Wnck.Screen scr, Wnck.Application app) {
			StringBuilder msg = new StringBuilder();
			msg.append_printf("maybe_setup called: %d == %d? %s",
							  app.get_pid(), (int)_process.get_pid(),
							  (app.get_pid() ==
							   (int)_process.get_pid()).to_string());

			Debug.Notification.emit(Debug.Subsystem.Application,
									Debug.Level.Information,
									msg.str);

			if(app.get_pid() != (int)_process.get_pid()) return;

			scr.application_opened -= maybe_setup;

			_wnckApp = app;
			_appVisible = true;
			_windows = _wnckApp.get_windows();

			scr.window_opened += maybe_update_window_count;
			scr.window_closed += maybe_update_window_count;

			if(_appIcon == null) create_icon();
			_appIcon.visible = true;
			_appIcon.popup_menu += display_menu;

			// Force an update to catch circumstances where the app
			// disappears and reappears (e.g., The GIMP).
			update_icon_image(_wnckApp);
		}

		private void display_menu(uint button, uint activate_time) {
			Debug.Notification.emit(Debug.Subsystem.Application,
									Debug.Level.Information,
									"Requesting menu!");
			Gtk.Menu appMenu = new Gtk.Menu();

			Gtk.MenuItem mnuToggle =
				new Gtk.MenuItem.with_label("Toggle Visibility");
			Gtk.MenuItem mnuUndock =
				new Gtk.MenuItem.with_label("Undock");
			Gtk.MenuItem mnuSeparator0 =
				new Gtk.SeparatorMenuItem();
			Gtk.MenuItem mnuAbout =
				new Gtk.MenuItem.with_label("About AllTray");

			// Append, wire and show the menu items.
			appMenu.append(mnuToggle);
			mnuToggle.activate += on_menu_toggle_activate;
			mnuToggle.show();

			appMenu.append(mnuUndock);
			mnuUndock.activate += on_menu_undock;
			mnuUndock.show();

			appMenu.append(mnuSeparator0);
			mnuSeparator0.show();

			appMenu.append(mnuAbout);
			mnuAbout.activate += on_menu_about;
			mnuAbout.show();

			appMenu.popup(null, null, null, button, activate_time);
		}

		private void maybe_update_window_count(Wnck.Screen scr,
											   Wnck.Window win) {
			StringBuilder sb = new StringBuilder();
			_windows = _wnckApp.get_windows();
			int wincount = _wnckApp.get_n_windows();
			string plural = (wincount != 1 ? "s" : "");

			sb.append_printf("%s - %d window%s", _wnckApp.get_name(),
							 _wnckApp.get_n_windows(), plural);
			
			_appIcon.set_tooltip(sb.str);
		}

		private void create_icon() {
			_appIcon = new LocalGtk.StatusIcon.from_pixbuf(_wnckApp.get_icon());

			_appIcon.set_tooltip(_wnckApp.get_name());

			_appIcon.activate += on_icon_click;
			_wnckApp.icon_changed += update_icon_image;
			_wnckApp.name_changed += update_icon_name;
		}

		private void update_icon_image(Wnck.Application app) {
			unowned Gdk.Pixbuf new_icon = app.get_icon();

			_appIcon.set_from_pixbuf(new_icon);

			if(new_icon == null) {
				_appIcon.visible = false;
				return;
			}
		}

		private void update_icon_name(Wnck.Application app) {
			_appIcon.set_tooltip(app.get_name());
		}

		private void on_icon_click(LocalGtk.StatusIcon icon) {
			if(icon.blinking == true) icon.blinking = false;
			toggle_visibility();
		}

		/*
		 * This looks like it works.  However, the user may notice the
		 * window trying to appear for a moment before we catch it...
		 *
		 * TODO: Find a way to intercept the window activation and
		 * prevent that from happening at all, and instead setting the
		 * DEMANDS_ATTENTION window manager hint, and generally blink
		 * the icon on DEMANDS_ATTENTION instead.
		 */
		private void maintain_hiddenness(Wnck.Window win,
										 Wnck.WindowState changed_bits,
										 Wnck.WindowState new_state) {
			if(_appVisible) return;

			debug_msg("*** WINDOW CHANGED BITMASK");
			debug_display_windowstate(changed_bits);
			debug_msg("*** WINDOW NEW STATE");
			debug_display_windowstate(new_state);

			if((new_state & Wnck.WindowState.MINIMIZED) == 0) {
				win.minimize();
				_appIcon.blinking = true;
			}
		}

		private void debug_msg(string str) {
			Debug.Notification.emit(Debug.Subsystem.Application,
									Debug.Level.Information,
									str);
		}

		private void debug_display_windowstate(Wnck.WindowState state) {
			if((state & Wnck.WindowState.MINIMIZED) != 0)
				debug_msg("MINIMIZED");
			if((state & Wnck.WindowState.MAXIMIZED_HORIZONTALLY) != 0)
				debug_msg("MAXIMIZED_HORIZONTALLY");
			if((state & Wnck.WindowState.MAXIMIZED_VERTICALLY) != 0)
				debug_msg("MAXIMIZED_VERTICALLY");
			if((state & Wnck.WindowState.SHADED) != 0)
				debug_msg("SHADED");
			if((state & Wnck.WindowState.SKIP_PAGER) != 0)
				debug_msg("SKIP_PAGER");
			if((state & Wnck.WindowState.SKIP_TASKLIST) != 0)
				debug_msg("SKIP_TASKLIST");
			if((state & Wnck.WindowState.STICKY) != 0)
				debug_msg("STICKY");
			if((state & Wnck.WindowState.HIDDEN) != 0)
				debug_msg("HIDDEN");
			if((state & Wnck.WindowState.FULLSCREEN) != 0)
				debug_msg("FULLSCREEN");
			if((state & Wnck.WindowState.DEMANDS_ATTENTION) != 0)
				debug_msg("DEMANDS_ATTENTION");
			if((state & Wnck.WindowState.URGENT) != 0)
				debug_msg("URGENT");
			if((state & Wnck.WindowState.ABOVE) != 0)
				debug_msg("ABOVE");
			if((state & Wnck.WindowState.BELOW) != 0)
				debug_msg("BELOW");
		}

		private void toggle_visibility() {
			_appVisible = !_appVisible;

			StringBuilder msg = new StringBuilder();

			foreach(Wnck.Window w in _windows) {
				msg.truncate(0);
				msg.append_printf("Setting window 0x%08lx visibility to %s",
								  w.get_xid(), _appVisible.to_string());

				if(_appVisible) {
					TimeVal tv = TimeVal();
					tv.get_current_time();

					w.unminimize((uint32)tv.tv_sec);
					w.set_skip_tasklist(false);

					w.state_changed -= maintain_hiddenness;
				} else {
					w.minimize();
					w.set_skip_tasklist(true);

					w.state_changed += maintain_hiddenness;
				}
				Debug.Notification.emit(Debug.Subsystem.Application,
										Debug.Level.Information,
										msg.str);
			}
		}

		/**********************************************************
		 * Context menu handlers.
		 **********************************************************/
		private void on_menu_toggle_activate(Gtk.MenuItem item) {
			if(_appIcon.blinking == true) _appIcon.blinking = false;
			toggle_visibility();			
		}

		private void on_menu_about() {
			// Display an about dialog
			Gtk.AboutDialog about = new Gtk.AboutDialog();
			about.program_name = "AllTray";
			about.version = Build.PACKAGE_VERSION;
			about.website = "http://alltray.trausch.us/";
			about.copyright = "Copyright © "+Build.ALLTRAY_COPYRIGHT_YEARS;
			about.comments = "Dock applications in the system tray.";

			about.show_all();
		}

		private void on_menu_undock() {
			Posix.exit(0);
		}
	}
}