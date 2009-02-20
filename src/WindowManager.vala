/*
 * WindowManager.vala - Window manager for AllTray.
 * Copyright (C) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3 as published by the Free Software Foundation.
 */
using GLib;
using Gdk;

namespace AllTray {
	public errordomain WindowManagerError {
		FAILED
	}

	public class WindowManager : GLib.Object {
		private DisplayManager _at_disp_manager;

		public WindowManager(DisplayManager disp) {
			this._at_disp_manager = disp;
		}
	}
}
