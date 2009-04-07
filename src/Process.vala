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

		public Process(string[] argv) {
			_argv = argv;
			_visible = true;
		}

		// Property for Pid not (yet) possible due to Vala bug.
		public Pid get_pid() {
			return(_child);
		}

		public void run() {
			// Special case, if we're attaching, we do not actually
			// spawn a new process.
			if(_argv[0] == "alltray-internal-fake-process") {
				run_fake(_argv[1].to_int());
				return;
			}

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

			_app = new AllTray.Application(this);
		}

		private void run_fake(int pid) {
			_running = true;
			_child = (Pid)pid;
		}

		public void child_died() {
			_running = false;
			StringBuilder msg = new StringBuilder();

			msg.append_printf("Child process %d (%s) died.",
							  (int)_child, _argv[0]);

			Debug.Notification.emit(Debug.Subsystem.Process,
									Debug.Level.Information,
									msg.str);
			process_died(this);
		}
	}
}