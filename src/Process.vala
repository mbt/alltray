/*
 * Process.vala - Child process management
 * Copyright (c) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation
 */
using GLib;

namespace AllTray {
	public errordomain ProcessError {
		SPAWN_FAILED,
		PROCESS_DOES_NOT_EXIST,
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

		public void run() throws ProcessError {
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
				throw new ProcessError.SPAWN_FAILED(e.message);
				_running = false;
			}

			_app = new AllTray.Application(this);
		}

		private void run_fake(int pid) throws ProcessError {
			Debug.Notification.emit(Debug.Subsystem.Process,
									Debug.Level.Information,
									"run_fake()");
			_running = true;
			_child = (Pid)pid;

			if(!process_num_is_alive(pid)) {
				throw new ProcessError.PROCESS_DOES_NOT_EXIST("The provided "+
															  "PID does not "+
															  "exist.");
			}

			StringBuilder msg = new StringBuilder();
			msg.append_printf("Process %d now attached.",
							  (int)_child);
			Debug.Notification.emit(Debug.Subsystem.Process,
									Debug.Level.Information,
									msg.str);

			Timeout.add(50, fake_child_monitor);
			_app = new AllTray.Application(this);
		}

		private bool fake_child_monitor() {
			if(process_num_is_alive((int)_child)) {
				return(true);
			}

			_running = false;

			StringBuilder msg = new StringBuilder();
			msg.append_printf("Attached process %d has died.",
							  (int)_child);
			Debug.Notification.emit(Debug.Subsystem.Process,
									Debug.Level.Information,
									msg.str);

			process_died(this);
			return(false);
		}

		/*
		 * Watches for the child.  This function assumes that we are running
		 * on a system that properly implements the null signal, specified
		 * in POSIX (IEEE Std 1003.1 and the Single UNIX® Specification v2,
		 * 1997.
		 */
		private bool process_num_is_alive(int pid) {
			int kill_ret = Posix.kill((int)_child, 0);

			// if Posix.kill returns 0, the process is alive.
			if(kill_ret == 0) return(true);

			// if Posix.errno is set to Posix.EPERM (Permission denied),
			// the process is alive.
			if(Posix.errno == Posix.EPERM) return(true);

			// Anything else === process dead.
			return(false);
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