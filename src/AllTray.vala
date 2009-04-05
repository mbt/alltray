/*
 * AllTray.vala - Entrypoint for AllTray.
 * Copyright (c) 2009 Michael B. Trausch <mike@trausch.us>
 */
using GLib;
using Native;

namespace AllTray {
	public class Program : GLib.Object {
		private string[] _args;
		private Process[] _plist;

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
			Gdk.init(ref _args);
			this.command_line_init(ref _args);

			Debug.Notification.emit(Debug.Subsystem.CommandLine,
									Debug.Level.Information,
									"Command line options parsed.");
		}

		public void command_line_init(ref string[] args) {
			Gtk.init(ref args);

			GLib.OptionContext opt_ctx =
				new GLib.OptionContext("- Dock software to the systray");
			opt_ctx.add_main_entries(this._acceptedCmdLineOptions, null);
			opt_ctx.add_group(Gtk.get_option_group(true));

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
			StdC.Signal.set_new_handler(StdC.Signal.SIGTERM, sighandler);
			StdC.Signal.set_new_handler(StdC.Signal.SIGUSR1, sighandler);
			StdC.Signal.set_new_handler(StdC.Signal.SIGUSR2, sighandler);
			// StdC.Signal.set_new_handler(StdC.Signal.SIGINT, sighandler);

			// The remaining arguments are to be sent to the process.
			string[] a = get_command_line(_args);
			Process p = new Process(a);
			p.run();
			
			if(!p.running) {
				return(1);
			}

			Gtk.main();
			return(0);
		}

		public string[] get_command_line(string[] args) {
			int curItem = 0;
			string[] retval = new string[args.length];
			foreach(string arg in args) {
				if(arg.contains("alltray")) continue;
				retval[curItem++] = arg;
			}

			return(retval);
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
		}
	}
}
