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
		private Wnck.Application? _wnckApp;
		private Process _process;
		private bool _appVisible;
		private ulong _xWin;

		public Wnck.Application wnck_app {
			get {
				return(_wnckApp);
			}
		}

		public Application(Process p) {
			_process = p;
			_xWin = get_xwin_from_pid(p.get_pid());

			_wnckApp = Wnck.Application.get(_xWin);
			if(_wnckApp == null) {
				Debug.Notification.emit(Debug.Subsystem.Process,
										Debug.Level.Information,
										"No App yet. Waiting for one.");
				GLib.Timeout.add_seconds(1, get_wapp);
			}

			_appVisible = true;
		}

		public bool get_wapp() {
			_wnckApp = Wnck.Application.get(_xWin);
			if(_wnckApp == null) {
				Debug.Notification.emit(Debug.Subsystem.Process,
										Debug.Level.Information,
										"No App yet. Waiting for one.");
				GLib.Timeout.add_seconds(1, get_wapp);
				return(true);
			} else {
				Debug.Notification.emit(Debug.Subsystem.Process,
										Debug.Level.Information,
										"Got the app.");
				return(false);
			}			
		}

		public void toggle_visibility() {
			_appVisible = !_appVisible;

			unowned List<Wnck.Window> windows = _wnckApp.get_windows();
			StringBuilder msg = new StringBuilder();

			foreach(Wnck.Window w in windows) {
				msg.truncate(0);
				msg.append_printf("Setting window 0x%08lx visibility to %s",
								  w.get_xid(), _appVisible.to_string());
				Debug.Notification.emit(Debug.Subsystem.Application,
										Debug.Level.Information,
										msg.str);
			}
		}

		private ulong get_xwin_from_pid(Pid pid) {
			void* Display =
				Gdk.x11_display_get_xdisplay(Gdk.Display.get_default());

			ulong xid = c_xwin_from_pid(pid, Display);
			
			while(xid == 0) {
				xid = c_xwin_from_pid(pid, Display);
			}

			return(xid);
		}
	}
}