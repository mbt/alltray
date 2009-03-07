/*
 * DisplayManager.vala - Display manager for AllTray.
 * Copyright (C) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3 as published by the Free Software Foundation.
 */
using GLib;
using Gdk;

namespace AllTray {
	public errordomain DisplayManagerError {
		NO_DISPLAY,
		NO_PREV_INSTANCE,
		INVAL_OP,
		FAILED
	}

	public class DisplayManager : GLib.Object {
		private static DisplayManager _instance;

		private X.Display _display;
		private X.Screen _screen;
		private X.Window _rootWindow;
		private X.Window _selectionWindow;
		private bool _screenIsComposited;

		public DisplayManager() throws DisplayManagerError {
			if(DisplayManager._instance == null) {
				this._display = X.Display.open(null);

				if(this._display == null) {
					throw new DisplayManagerError.NO_DISPLAY("Unable to open "+
															 "display.");
				} else {
					Debug.Notification.emit(Debug.Subsystem.Display,
											Debug.Level.Information,
											"Opened display via Xlib.");
				}

				this._screen = this._display.get_default_screen();

				DisplayManager._instance = this;
				this._rootWindow = _screen.get_root_window();

				if(_screen.is_composited()) {
					Debug.Notification.emit(Debug.Subsystem.Display,
											Debug.Level.Information,
											"Running on a composited "+
											"screen.");
				}

				_screen.composited_changed += this.on_composite_state_change;

				Debug.Notification.emit(Debug.Subsystem.Display,
										Debug.Level.Information,
										"Initialized DisplayManager.");
			} else {
				// Reached here?  BUG, not error.
				GLib.error("Bug: Only one instance of AllTray.DisplayManager "+
						   "is allowed.");
			}
		}

		public bool is_selection_installed() throws DisplayManagerError {
			bool retval = false;
			Gdk.Window selection_owner = this.get_selection_window();

			if(selection_owner != null) {
				retval = true;
			}

			return(retval);
		}

		public string get_window_manager() {
			return(Gdk.x11_screen_get_window_manager_name(this._screen));
		}

		public void send_args(string[] args) throws DisplayManagerError {
			Gdk.Window selection_owner = this.get_selection_window();
			if(selection_owner == null) {
				throw new DisplayManagerError.INVAL_OP("Application tried to "+
													   "send args, but no "+
													   "running AllTray was "+
													   "found.");
			}
		}

		public void install_selection() throws DisplayManagerError {
			Debug.Notification.emit(Debug.Subsystem.Display,
									Debug.Level.Information,
									"In install_selection()");
			Gdk.Atom selection = Gdk.Atom.intern("_ALLTRAY_IS_RUNNING",
												 false);
			create_selection_window();
			GLib.TimeVal tv = GLib.TimeVal();
			uint32 timestamp = (uint32)tv.tv_sec;
			
			bool success;
			success =
				Gdk.selection_owner_set(_selectionWindow, selection,
										Gdk.CURRENT_TIME, true);

			if(success) {
				Debug.Notification.emit(Debug.Subsystem.Display,
										Debug.Level.Information,
										"Installed selection.");
			} else {
				Debug.Notification.emit(Debug.Subsystem.Display,
										Debug.Level.Fatal,
										"I don't understand what just "+
										"happened.");
			}
		}

		private void create_selection_window() throws DisplayManagerError {
			if(is_selection_installed()) {
				throw new DisplayManagerError.INVAL_OP("Cannot create "+
													   "selection window when "+
													   "one already exists.");
			}

			Gdk.WindowAttr attribs = {};
			int attribs_mask;

			attribs.window_type = Gdk.WindowType.TOPLEVEL;
			attribs.width = 0;
			attribs.height = 0;
			attribs.wclass = Gdk.WindowClass.INPUT_ONLY;
			attribs.title = "AllTray Selection Window";

			attribs_mask = Gdk.WindowAttributesType.TITLE;

			_selectionWindow = new Gdk.Window(null, attribs, attribs_mask);

			if(_selectionWindow == null) {
				Debug.Notification.emit(Debug.Subsystem.Display,
										Debug.Level.Fatal,
										"Oops.  Could not create window, WTF?");
			} else {
				GLib.StringBuilder sb = new GLib.StringBuilder();

				sb.append_printf("Selection-owner window installed.");
				Debug.Notification.emit(Debug.Subsystem.Display,
										Debug.Level.Information,
										sb.str);
			}
		}

		private Gdk.Window get_selection_window() {
			Gdk.Atom selection = Gdk.Atom.intern("_ALLTRAY_IS_RUNNING", false);
			return(Gdk.selection_owner_get(selection));
		}

		private void on_composite_state_change() {
			string msg = "Composite status changed (now " +
				_screen.is_composited().to_string() + ").";

			Debug.Notification.emit(Debug.Subsystem.Display,
									Debug.Level.Information, msg);
		}
	}
}
