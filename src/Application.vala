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
		private Wnck.Application _wnckApp;
		private Process _process;

		public Application(Process p) {
			ulong xWin;

			_process = p;
			xWin = get_xwin_from_pid(p.pid);
		}

		private ulong get_xwin_from_pid(Pid pid) {
			void* Display =
				Gdk.x11_display_get_xdisplay(Gdk.Display.get_default());

			return(c_xwin_from_pid(pid, Display));
		}
	}
}