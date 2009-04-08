/*
 * AttachHelper.vala - Helper class to attach to a running process
 * Copyright (c) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation
 */
using GLib;
using Gtk;

namespace AllTray {
	public class AttachHelper : GLib.Object {
		private static AttachHelper _sInstance;

		private static unowned Wnck.Window _chosen_window;
		public static int target_process;
		public static bool success;

		public static bool get_target_window() {
			_sInstance = new AttachHelper();
			_chosen_window = null;
			target_process = 0;
			success = false;

			_sInstance.run();
			return(false);
		}

		public AttachHelper() {
			//
		}

		// Based on get_target() in wnckprop, part of libwnck
		// Original language: C, original license: LGPLv2
		public void run() {
			Gdk.GrabStatus status;
			Gdk.Cursor curCross;
			Gdk.Window rootWindow;

			rootWindow = Gdk.get_default_root_window();
			rootWindow.add_filter(target_filter);
			curCross = new Gdk.Cursor(Gdk.CursorType.CROSS);

			status = Gdk.pointer_grab(rootWindow, false,
									  Gdk.EventMask.BUTTON_PRESS_MASK, null,
									  curCross, Gdk.CURRENT_TIME);

			if(status != Gdk.GrabStatus.SUCCESS) {
				GLib.warning("Pointer grab failed");
				clean_up();
				return;
			}

			status = Gdk.keyboard_grab(rootWindow, false, Gdk.CURRENT_TIME);
			if(status != Gdk.GrabStatus.SUCCESS) {
				GLib.warning("Pointer grab failed");
				clean_up();
				return;
			}

			Gdk.flush();
		}

		private Gdk.FilterReturn target_filter(Gdk.XEvent xev, Gdk.Event ev) {
			unowned Native.XLib.XEvent real_xev = (Native.XLib.XEvent)xev;

			switch(real_xev.type) {
			case Native.XLib.ButtonPress:
				handle_button_press_event(ref real_xev.xkey);
				clean_up();
				return(Gdk.FilterReturn.REMOVE);
			case Gdk.EventType.KEY_PRESS:
				// all are abort keys currently.
				clean_up();
				return(Gdk.FilterReturn.REMOVE);
			default:
				break;
			}

			return(Gdk.FilterReturn.CONTINUE);
		}

		private void handle_button_press_event(ref Native.XLib.XKeyEvent ev) {
			if(ev.subwindow == Native.XLib.None) return;

			ulong xwin = ev.subwindow;
			success = true;
			Program.WnckScreen.force_update();
			_chosen_window = get_managed(xwin);

			StringBuilder sb = new StringBuilder();
			sb.append_printf("Looks like we got win 0x%08lx", xwin);
			Debug.Notification.emit(Debug.Subsystem.AttachHelper,
									Debug.Level.Information,
									sb.str);

			target_process = _chosen_window.get_pid();
			sb.truncate(0);
			sb.append_printf("Win 0x%08lx pid = %d", xwin, target_process);
			Debug.Notification.emit(Debug.Subsystem.AttachHelper,
									Debug.Level.Information,
									sb.str);

			
			Gtk.main_quit();
		}

		private unowned Wnck.Window? get_managed(ulong xwin) {
			return(c_find_managed_window(xwin, Gdk.Display.get_default()));
		}

		private void clean_up() {
			Gdk.pointer_ungrab(Gdk.CURRENT_TIME);
			Gdk.keyboard_ungrab(Gdk.CURRENT_TIME);
		}
	}
}