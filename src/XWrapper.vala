/*
 * XWrapper.vala - A wrapper around Native.XLib to provide a decent
 * interface for the functions of the Xlib library.
 * Copyright (c) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU LGPL v3 as published by the Free Software Foundation
 */

namespace AllTray.X {
	public enum ByteOrder {
		LSB_FIRST = Native.XLib.LSBFirst,
		MSB_FIRST = Native.XLib.MSBFirst
	}

	public enum CloseDownMode {
		DESTROY_ALL = Native.XLib.DestroyAll,
		RETAIN_PERMANENT = Native.XLib.RetainPermanent,
		RETAIN_TEMPORARY = Native.XLib.RetainTemporary
	}

	public errordomain ScreenError {
		FAILED
	}

	public class Screen : GLib.Object {
		private weak Native.XLib.Screen _screen;
		private int _screen_number;

		public int screen_number {
			get {
				return(this._screen_number);
			}
		}

		internal Screen(Native.XLib.Screen screen) {
			this._screen = screen;
			this._screen_number = this._screen.get_screen_number();
		}

		public int get_depth() {
			return(this._screen.get_depth());
		}
	}

	public errordomain SelectionError {
		NO_SELECTION,
		FAILED
	}

	public class SelectionManager : GLib.Object {
		private Display _display;

		public SelectionManager(Display d) {
			this._display = d;
		}
	}

	public struct Coordinates {
		int x;
		int y;
	}

	public errordomain WindowError {
		INVALID_WINDOW,
		CANNOT_CREATE,
		WINDOW_STATE_INVALID,
		FAILED
	}

	public class Window : GLib.Object {
		private Native.XLib.Window _window_handle;
		private Display _display;
		private bool _is_valid;

		public static Window create_default_window(Display d) {
			Window w = new Window(d);
			w.initialize_default();

			return(w);
		}

		public static Window create_from_xid(Display d, Native.XLib.Window xw) {
			Window w = new Window(d);
			w._window_handle = xw;
			w._is_valid = true;

			return(w);
		}

		public bool is_valid {
			get {
				return(this._is_valid);
			}
		}

		public ulong native_window {
			get {
				if(!this._is_valid)
					throw new WindowError.WINDOW_STATE_INVALID("Window has not"+
															   " been init'd "+
															   "yet.");
				return(this._window_handle);
			}
		}

		public Window(Display d) {
			this._display = d;
			this._is_valid = false;
		}

		private void initialize_default() {
			Window parentWindow = this._display.default_root_window;
			Coordinates position = { 0, 0 };
			Coordinates size = { 1, 1 };
			uint border_width = 0;
			int depth;

			this._display.default_screen.get_depth();
		}
	}

	public errordomain XError {
		NO_DISPLAY,
		INVALID_SCREEN,
		FAILED
	}

	public class Display : GLib.Object {
		private Native.XLib.Display _display;
		private CloseDownMode _close_down_mode;
		private bool _needs_keepalive;

		public ulong all_planes {
			get {
				return(this._display.get_all_planes());
			}
		}

		public int connection_number {
			get {
				return(this._display.get_connection_number());
			}
		}

		public int screen_count {
			get {
				return(this._display.get_number_of_screens());
			}
		}

		public Window default_root_window {
			owned get {
				Native.XLib.Window root =
					this._display.get_default_root_window();

				return(Window.create_from_xid(this, root));
			}
		}

		public Screen default_screen {
			owned get {
				unowned Native.XLib.Screen scrn = this._display.get_default_screen();
				return(new Screen(scrn));
			}
		}

		public string display_string {
			owned get {
				return(this._display.get_display_string());
			}
		}

		public long max_request_size {
			get {
				return(this._display.get_max_request_size());
			}
		}

		public long max_extended_request_size {
			get {
				return(this._display.get_max_extended_request_size());
			}
		}

		public ulong last_known_request_processed {
			get {
				return(this._display.get_last_known_request_processed());
			}
		}

		public ulong next_request_id {
			get {
				return(this._display.get_next_request_id());
			}
		}

		public int protocol_version {
			get {
				return(this._display.get_protocol_version());
			}
		}

		public int protocol_revision {
			get {
				return(this._display.get_protocol_revision());
			}
		}

		public int event_queue_length {
			get {
				return(this._display.get_event_queue_length());
			}
		}

		public string display_server_vendor {
			owned get {
				return(this._display.get_xserver_vendor_name());
			}
		}

		public string display_server_release {
			owned get {
				return(this._display.get_xserver_vendor_release());
			}
		}

		public ByteOrder image_byte_order {
			get {
				ByteOrder bo = (ByteOrder)this._display.get_image_byte_order();
				return(bo);
			}
		}

		public ByteOrder bitmap_byte_order {
			get {
				ByteOrder bo = (ByteOrder)this._display.get_bitmap_bit_order();
				return(bo);
			}
		}

		public int bitmap_scanline_unit {
			get {
				return(this._display.get_bitmap_scanline_unit());
			}
		}

		public int bitmap_scanline_padding {
			get {
				return(this._display.get_bitmap_scanline_padding());
			}
		}

		// Only effective if using a GLib.MainLoop!
		public bool needs_keepalive {
			get {
				return(this._needs_keepalive);
			}

			set {
				this._needs_keepalive = value;
			}
		}

		public CloseDownMode close_down_mode {
			get {
				return(this._close_down_mode);
			}

			set {
				this._display.set_close_down_mode(value);
				this._close_down_mode = value;
			}
		}

		public Display(string? display_name) {
			this._display = new Native.XLib.Display(display_name);
			if(this._display == null) {
				throw new XError.NO_DISPLAY("Unable to open display");
			}

			this._close_down_mode = CloseDownMode.DESTROY_ALL;
		}

		public Screen get_screen_by_id(int id) throws XError {
			unowned Native.XLib.Screen? scrn = this._display.get_screen_by_id(id);
			if(scrn == null) {
				throw new XError.INVALID_SCREEN("Invalid screen specified");
			}

			return(new Screen(scrn));
		}

		public void no_op() {
			this._display.no_operation();
		}

		public void acquire_lock() {
			this._display.acquire_lock();
		}

		public void release_lock() {
			this._display.release_lock();
		}

		public SelectionManager get_selection_manager() {
			return(new SelectionManager(this));
		}

		public unowned Native.XLib.Display get_xlib_display() {
			return(this._display);
		}
	}
}
