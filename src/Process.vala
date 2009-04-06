/*
 * Process.vala - Child process management
 * Copyright (c) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation
 */
using GLib;

namespace AllTray {
	public errordomain ProcessError {
		SPAWN_FAILED,
		FAILED
	}

	public class Process : GLib.Object {
		private Gtk.StatusIcon _statusIcon;
		private Pid _child;
		private bool _visible;
		private bool _running;
		private string[] _argv;
		private AllTray.Application _app;

		public signal void process_died(Process p);

		public bool running {
			get {
				return(_running);
			}
		}

		// XXX: Not for now, bug in Vala 0.[67].0
		/*
		public Pid pid {
			get {
				return(_child);
			}
		}
		*/

		public Process(string[] argv) {
			_argv = argv;
			_visible = true;
		}

		public Pid get_pid() {
			return(_child);
		}

		public void run() {
			try {
				GLib.Process.spawn_async(null, _argv, null,
										 SpawnFlags.DO_NOT_REAP_CHILD |
										 SpawnFlags.SEARCH_PATH,
										 null, out _child);
				_running = true;
				ChildWatch.add(_child, child_died);
				StringBuilder msg = new StringBuilder();
				msg.append_printf("Child process %d (%s) now running.",
								  (int)_child, _argv[0]);
				Debug.Notification.emit(Debug.Subsystem.Process,
										Debug.Level.Information,
										msg.str);
			} catch(SpawnError e) {
				stdout.printf("An error occurred: %s\n", e.message);
				_running = false;
			}

			_statusIcon =
				new Gtk.StatusIcon.from_stock(Gtk.STOCK_MISSING_IMAGE);

			_statusIcon.set_visible(true);
			_statusIcon.activate += toggle_visibility;
		}

		public void child_died() {
			_running = false;
			_statusIcon.visible = false;
			StringBuilder msg = new StringBuilder();

			msg.append_printf("Child process %d (%s) died.",
							  (int)_child, _argv[0]);

			Debug.Notification.emit(Debug.Subsystem.Process,
									Debug.Level.Information,
									msg.str);
			process_died(this);
		}

		public void toggle_visibility() {
			_visible = !_visible;
			get_application();
			unowned List<Wnck.Window> windows =
				_app.wnck_app.get_windows();
			StringBuilder msg = new StringBuilder();

			if(_visible) {
				msg.append("I would show it, but I don't know how.");
			} else {
				msg.append("I would hide it, but I don't know how.");
			}
			
			foreach(Wnck.Window w in windows) {
				msg.append_printf(" (win 0x%07lx)", w.get_xid());
			}

			Debug.Notification.emit(Debug.Subsystem.Process,
									Debug.Level.Information,
									msg.str);
		}

		private void get_application() {
			if(_app == null) {
				_app = new AllTray.Application(this);
			}
		}
	}
}