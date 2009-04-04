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

		private string[] _argv;

		public Process(string[] argv) {
			_argv = argv;
			_child = (Pid)0;
		}

		public void run() {
			try {
				GLib.Process.spawn_async(null, _argv, null,
										 SpawnFlags.DO_NOT_REAP_CHILD,
										 null, out _child);
			} catch(SpawnError e) {
				string msg = "";
				msg.printf("Error: failed to spawn child process: %s",
						   e.message);
				throw new ProcessError.SPAWN_FAILED(msg);
			}

			_statusIcon = new Gtk.StatusIcon.from_stock(Gtk.STOCK_MISSING_IMAGE);
			Idle.add(child_monitor);
		}

		public bool child_monitor() {
			stderr.printf("In child_monitor()\n");
			return(true);
		}
	}
}