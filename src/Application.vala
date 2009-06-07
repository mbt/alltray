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
		private Gee.HashMap<ulong, bool?> _windowVisibilityInfo;
		private Wnck.Application? _wnckApp;
		private AllTray.LocalGtk.StatusIcon _appIcon;
		private Process _process;
		private bool _appVisible;

		public Wnck.Application wnck_app {
			get {
				return(_wnckApp);
			}
		}

		public bool visible {
			get {
				return(_appVisible);
			}

			set {
				if(_appVisible != value) {
					toggle_visibility();
				}
			}
		}

		public Application(Process p) {
			_process = p;
			_windowVisibilityInfo = new Gee.HashMap<ulong, bool?>();
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

		private bool are_we_interested(Wnck.Application app) {
			bool retval = false;
			int app_pid = app.get_pid();
			int desired_pid = (int)_process.get_pid();

			StringBuilder msg = new StringBuilder();
			msg.append_printf("Checking if we are interested in PID %d",
							  app_pid);
			Debug.Notification.emit(Debug.Subsystem.Application,
									Debug.Level.Information,
									msg.str);

			// First, see if we're interested in the app itself.
			retval = (app_pid == desired_pid);

			// If no, see if we are interested in the app's parent.
			if(!retval) {
				ProcessInfo? p = new ProcessInfo(app_pid);
				if(p != null) {
					retval = (p.ppid == desired_pid);

					msg.truncate(0);
					msg.append_printf("ProcessInfo: pid = %d, ppid = %d, "+
									  "name = '%s'",
									  p.pid, p.ppid, p.name);
					Debug.Notification.emit(Debug.Subsystem.Application,
											Debug.Level.Information,
											msg.str);
				} else {
					Debug.Notification.emit(Debug.Subsystem.Application,
											Debug.Level.Information,
											"ProcessInfo was NULL!");
				}
			}

			// XXX: Add any new detection schemes just above this
			// comment.
			return(retval);
		}

		private void maybe_setup(Wnck.Screen scr, Wnck.Application app) {
			bool interested = are_we_interested(app);
			StringBuilder msg = new StringBuilder();
			msg.append_printf("Are we interested in pid %d? %s",
							  app.get_pid(), interested.to_string());
			Debug.Notification.emit(Debug.Subsystem.Application,
									Debug.Level.Information,
									msg.str);
			if(!interested) return;

			// If we make it here, we were interested in the process
			// and are about to handle it.
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
			TrayMenu appMenu = new TrayMenu(_windows);
			appMenu.toggle_app_visibility.connect(() => {
					on_menu_toggle_app();
				});

			appMenu.undock.connect(() => {
					on_menu_undock();
				});

			appMenu.toggle_window_visibility.connect((w) => {
					toggle_window_visibility(w);
				});
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

			if(_windowVisibilityInfo[win.get_xid()] == null) {
				_windowVisibilityInfo[win.get_xid()] = _appVisible;
			}
			
			_appIcon.set_tooltip(sb.str);
		}

		private void create_icon() {
			_appIcon =
				new LocalGtk.StatusIcon.from_pixbuf(_wnckApp.get_mini_icon());

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
			StringBuilder sb = new StringBuilder();
			if((state & Wnck.WindowState.MINIMIZED) != 0)
				sb.append("MINIMIZED ");
			if((state & Wnck.WindowState.MAXIMIZED_HORIZONTALLY) != 0)
				sb.append("MAXIMIZED_HORIZONTALLY ");
			if((state & Wnck.WindowState.MAXIMIZED_VERTICALLY) != 0)
				sb.append("MAXIMIZED_VERTICALLY ");
			if((state & Wnck.WindowState.SHADED) != 0)
				sb.append("SHADED ");
			if((state & Wnck.WindowState.SKIP_PAGER) != 0)
				sb.append("SKIP_PAGER ");
			if((state & Wnck.WindowState.SKIP_TASKLIST) != 0)
				sb.append("SKIP_TASKLIST ");
			if((state & Wnck.WindowState.STICKY) != 0)
				sb.append("STICKY ");
			if((state & Wnck.WindowState.HIDDEN) != 0)
				sb.append("HIDDEN ");
			if((state & Wnck.WindowState.FULLSCREEN) != 0)
				sb.append("FULLSCREEN ");
			if((state & Wnck.WindowState.DEMANDS_ATTENTION) != 0)
				sb.append("DEMANDS_ATTENTION ");
			if((state & Wnck.WindowState.URGENT) != 0)
				sb.append("URGENT ");
			if((state & Wnck.WindowState.ABOVE) != 0)
				sb.append("ABOVE ");
			if((state & Wnck.WindowState.BELOW) != 0)
				sb.append("BELOW ");

			debug_msg(sb.str);
		}

		private void toggle_visibility() {
			_appVisible = !_appVisible;
			if(_appVisible) {
				debug_msg("showing application");
			} else {
				debug_msg("hiding application");
			}

			foreach(Wnck.Window w in _windows) {
				_windowVisibilityInfo[w.get_xid()] = _appVisible;
				set_visibility_for_window(w, _appVisible);
			}
		}

		private void toggle_window_visibility(Wnck.Window w) {
			if(_windowVisibilityInfo[w.get_xid()] == null) {
				_windowVisibilityInfo[w.get_xid()] = true;
			}

			_windowVisibilityInfo[w.get_xid()] = 
			  !_windowVisibilityInfo[w.get_xid()];

			if(_windowVisibilityInfo[w.get_xid()] == true) {
				set_visibility_for_window(w, true);
			} else {
				set_visibility_for_window(w, false);
			}
		}

		private void set_visibility_for_window(Wnck.Window w, bool visible) {
			TimeVal tv = TimeVal();
			tv.get_current_time();

			StringBuilder msg = new StringBuilder();
			msg.append_printf("Setting window 0x%08lx visibility to %s",
							  w.get_xid(), _appVisible.to_string());

			if(visible) {
				w.state_changed -= maintain_hiddenness;
				w.unminimize((uint32)tv.tv_sec);
				w.set_skip_tasklist(false);

				Wnck.Workspace ws = w.get_workspace();
				ws.activate((uint32)tv.tv_sec);
				w.activate((uint32)tv.tv_sec);
			} else {
				w.minimize();
				w.set_skip_tasklist(true);
				w.state_changed += maintain_hiddenness;
			}

			Debug.Notification.emit(Debug.Subsystem.Application,
									Debug.Level.Information,
									msg.str);
		}

		/**********************************************************
		 * Context menu handlers.
		 **********************************************************/
		private void on_menu_toggle_app() {
			if(_appIcon.blinking == true) _appIcon.blinking = false;
			toggle_visibility();			
		}

		private void on_menu_undock() {
			foreach(Wnck.Window w in _windows) {
				set_visibility_for_window(w, true);
			}

			Posix.exit(0);
		}
	}
}
