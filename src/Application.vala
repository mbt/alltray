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

		public Wnck.Application wnck_app {
			get {
				if(_wnckApp == null) {
					GLib.critical("OH NOES, libwnck won't give me the application!  :(");
				}

				return(_wnckApp);
			}
		}

		public Application(Process p) {
			ulong xWin;

			_process = p;
			xWin = get_xwin_from_pid(p.get_pid());
			_wnckApp = Wnck.Application.get(xWin);

			StringBuilder msg = new StringBuilder();
			msg.append_printf("Got xwin 0x%08lx, for pid %ld",
							  xWin, (long)p.get_pid());

			Debug.Notification.emit(Debug.Subsystem.Process,
									Debug.Level.Information,
									msg.str);
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