/*
 * Process.vala - Child process management
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

		private string[] _argv;

		public bool running {
			get; set;
		}

		public Process(string[] argv) {
			_argv = argv;
			_child = (Pid)0;
			_visible = true;
		}

		public void run() {
			try {
				GLib.Process.spawn_async(null, _argv, null,
										 SpawnFlags.DO_NOT_REAP_CHILD |
										 SpawnFlags.SEARCH_PATH,
										 null, out _child);
				running = true;
				ChildWatch.add(_child, child_died);
			} catch(SpawnError e) {
				stdout.printf("An error occurred: %s\n", e.message);
				running = false;
			}

			_statusIcon = new Gtk.StatusIcon.from_stock(Gtk.STOCK_MISSING_IMAGE);
			_statusIcon.set_visible(true);
			_statusIcon.activate += toggle_visibility;
		}

		public void child_died() {
			running = false;
			_statusIcon.visible = false;
			StringBuilder msg = new StringBuilder();

			msg.append_printf("Child process %d (%s) died.",
							  (int)_child, _argv[0]);

			Debug.Notification.emit(Debug.Subsystem.Process,
									Debug.Level.Information,
									msg.str);
		}

		public void toggle_visibility() {
			_visible = !_visible;
			string msg;
			if(_visible) {
				msg = "I would show it.";
			} else {
				msg = "I would hide it.";
			}

			Debug.Notification.emit(Debug.Subsystem.Process,
									Debug.Level.Information,
									msg);
		}
	}
}