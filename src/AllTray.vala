/*
 * AllTray.vala - Entrypoint for AllTray.
 * Copyright (c) 2009 Michael B. Trausch <mike@trausch.us>
 */
using GLib;
using Native;

namespace AllTray {
	public class Program : GLib.Object {
		private string[] _args;
		private AllTray.DisplayManager _dispManager;
		private AllTray.WindowManager _winManager;
		private GLib.MainLoop _main_loop;

		private static bool _cl_debug;
		private static Program _instance;

		private const GLib.OptionEntry[] _acceptedCmdLineOptions = {
			{ "debug", 'D', 0, GLib.OptionArg.NONE, ref _cl_debug,
			  "Enable debugging messages", null },
			{ null }
		};

		public string[] args {
			get {
				return(this._args);
			}
		}

		public Program(ref unowned string[] args) {
			this._args = args;
			Gdk.init(ref args);
			this.command_line_init(ref args);

			Debug.Notification.emit(Debug.Subsystem.CommandLine,
									Debug.Level.Information,
									"Command line options parsed.");

			this._dispManager = new AllTray.DisplayManager();

			if(!this._dispManager.is_selection_installed()) {
				Debug.Notification.emit(Debug.Subsystem.Misc,
										Debug.Level.Information,
										"No previous instance found.");
			} else {
				Debug.Notification.emit(Debug.Subsystem.Misc,
										Debug.Level.Information,
										"AllTray already running on display.");
				this._dispManager.send_args(args);
			}

			this._winManager = new AllTray.WindowManager(_dispManager);

			_main_loop = new GLib.MainLoop(null, false);
		}

		public void command_line_init(ref unowned string[] args) {
			GLib.OptionContext opt_ctx =
				new GLib.OptionContext("- Dock software to the systray");
			opt_ctx.add_main_entries(this._acceptedCmdLineOptions, null);
			try {
				opt_ctx.parse(ref args);
			} catch(OptionError e) {
				stderr.printf("error: command line parsing failed (%s)\n",
							  e.message);
				Native.StdC.Stdlib.exit(1);
			}

			if(_cl_debug) Debug.Notification.init();
		}

		public int run() {
			StdC.Signal.set_new_handler(StdC.Signal.SIGHUP, sighandler);
			StdC.Signal.set_new_handler(StdC.Signal.SIGINT, sighandler);
			StdC.Signal.set_new_handler(StdC.Signal.SIGTERM, sighandler);
			StdC.Signal.set_new_handler(StdC.Signal.SIGUSR1, sighandler);
			StdC.Signal.set_new_handler(StdC.Signal.SIGUSR2, sighandler);

			_main_loop.run();

			return(0);
		}

		public static int main(string[] args) {
			Program atray = new Program(ref args);
			Program._instance = atray;

			return(atray.run());
		}

		private static void sighandler(int caught_signal) {
			Debug.Notification.emit(Debug.Subsystem.Misc,
									Debug.Level.Information, "Caught signal: "+
									caught_signal.to_string());

			// Exit because user pressed Control+C.
			if(caught_signal == 2) {
				stderr.printf("User sent SIGINT, exiting...\n");
				Program._instance._main_loop.quit();
			}
		}
	}
}
