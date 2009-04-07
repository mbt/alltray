/*
 * AllTray.vala - Entrypoint for AllTray.
 * Copyright (c) 2009 Michael B. Trausch <mike@trausch.us>
 */
using GLib;
using Native;

namespace AllTray {
	public errordomain AllTrayError {
		FAILED
	}

	public class Program : GLib.Object {
		private string[] _args;
		private string[] _cleanArgs;
		private List<Process> _plist;

		private static bool _cl_debug;
		private static bool _display_ver;
		private static bool _display_extended_ver;
		private static Program _instance;

		public static Wnck.Screen WnckScreen;

		private const GLib.OptionEntry[] _acceptedCmdLineOptions = {
			{ "debug", 'D', 0, GLib.OptionArg.NONE, ref _cl_debug,
			  "Enable debugging messages", null },
			{ "version", 'v', 0, GLib.OptionArg.NONE, ref _display_ver,
			  "Display AllTray version info", null },
			{ "extended-version", 'V', 0, GLib.OptionArg.NONE,
			  ref _display_extended_ver, "Display extended version info",
			  null },
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
			_cleanArgs = this.command_line_init(ref _args);

			Debug.Notification.emit(Debug.Subsystem.CommandLine,
									Debug.Level.Information,
									"Command line options parsed.");
		}

		public string[] command_line_init(ref string[] args) {
			Gtk.init(ref args);

			// First, see if there are environment arguments.
			string? default_flags =
				GLib.Environment.get_variable("ALLTRAY_OPTIONS");
			string[] env_args = null;

			if(default_flags != null) {
				StringBuilder sb = new StringBuilder();
				sb.append_printf("%s", args[0]);
				sb.append_printf(" %s", default_flags);
				env_args = sb.str.split(" ");
			}

			GLib.OptionContext opt_ctx =
				new GLib.OptionContext("- Dock software to the systray");
			opt_ctx.add_main_entries(this._acceptedCmdLineOptions, null);
			opt_ctx.add_group(Gtk.get_option_group(true));

			if(env_args != null) {
				try {
					opt_ctx.parse(ref env_args);
				} catch(OptionError e) {
					stderr.printf("error: env flag parsing failed (%s)\n",
								  e.message);
					Native.StdC.Stdlib.exit(1);
				}
			}

			try {
				opt_ctx.parse(ref args);
			} catch(OptionError e) {
				stderr.printf("error: command line parsing failed (%s)\n",
							  e.message);
				Native.StdC.Stdlib.exit(1);
			}

			if(_display_ver && !_display_extended_ver) {
				display_version();
				Native.StdC.Stdlib.exit(0);
			}

			if(_display_extended_ver) {
				display_extended_version();
				Native.StdC.Stdlib.exit(0);
			}

			if(_cl_debug) Debug.Notification.init();

			// Strip "--" out of the command line arguments
			if(args[1] == "--") {
				StringBuilder sb = new StringBuilder();

				foreach(string arg in args) {
					if(arg == "--") continue;
					sb.append_printf("%s@&@", arg);
				}

				string[] newargs = sb.str.split("@&@");
				return(newargs);
			}

			return(args);
		}

		public int run() {
			_plist = new List<Process>();

			Wnck.set_client_type(Wnck.ClientType.PAGER);
			Debug.Notification.emit(Debug.Subsystem.Misc,
									Debug.Level.Information,
									"Set WNCK Client Type to PAGER");
			install_signal_handlers();

			WnckScreen = Wnck.Screen.get_default();

			try {
				spawn_new_process();
			} catch(AllTrayError e) {
				stderr.printf(e.message);
				return(1);
			}

			Gtk.main();
			return(0);
		}

		private void display_version() {
			if(Build.ALLTRAY_BZR_BUILD == "TRUE") {
				stdout.printf("AllTray %s+, from bzr branch %s,\n  rev-id %s\n",
							  Build.PACKAGE_VERSION, Build.ALLTRAY_BZR_BRANCH,
							  Build.ALLTRAY_BZR_REVID);
			} else {
				stdout.printf("AllTray %s\n",
							  Build.PACKAGE_VERSION);
			}

			stdout.printf("Copyright (c) %s Michael B. Trausch "+
						  "<mike@trausch.us>\n", Build.ALLTRAY_COPYRIGHT_YEARS);
			stdout.printf("Licensed under the GNU GPL v3.0 as published by "+
						  "the Free Software Foundation.\n");
		}

		private void display_extended_version() {
			display_version();

			stdout.printf("Configured %s on %s %s\n",
						  Build.ALLTRAY_COMPILE_BUILD_DATE,
						  Build.ALLTRAY_COMPILE_OS,
						  Build.ALLTRAY_COMPILE_OS_REL);

			stdout.printf("Compilers: %s and %s\n",
						  Build.ALLTRAY_VALA_COMPILER,
						  Build.ALLTRAY_C_COMPILER);

			if(Build.ALLTRAY_CONFIGURE_FLAGS == "") {
				stdout.printf("Configure was run without flags\n");
			} else {
				stdout.printf("Configure flags: %s\n",
							  Build.ALLTRAY_CONFIGURE_FLAGS);
			}
		}

		private void spawn_new_process() throws AllTrayError {
			string[] a = get_command_line(_cleanArgs);
			Process p = new Process(a);
			p.process_died += cleanup_for_process;
			p.run();
			
			if(!p.running) {
				Debug.Notification.emit(Debug.Subsystem.Process,
										Debug.Level.Error,
										"Whoops.  Not running?");
				throw new AllTrayError.FAILED("Failed to start process");
			}

			_plist.append(p);
		}

		public static int main(string[] args) {
			Program atray = new Program(ref args);
			Program._instance = atray;

			return(atray.run());
		}

		private void cleanup_for_process(Process p) {
			Debug.Notification.emit(Debug.Subsystem.Process,
									Debug.Level.Information,
									"Cleaning up for child...");
			_plist.remove(p);

			if(_plist.length() == 0) {
				Debug.Notification.emit(Debug.Subsystem.Process,
										Debug.Level.Information,
										"No more children. Dying.");
				Gtk.main_quit();
			}
		}

		private string[] get_command_line(string[] args) {
			int curItem = 0;
			string[] retval = new string[args.length];
			foreach(string arg in args) {
				if(arg.contains("alltray")) continue;
				retval[curItem++] = arg;
			}

			return(retval);
		}

		private void install_signal_handlers() {
			// For the moment, we _DO NOT_ use these.  We will soon.
			return;

			/*
			StdC.Signal.set_new_handler(StdC.Signal.SIGHUP, sighandler);
			StdC.Signal.set_new_handler(StdC.Signal.SIGTERM, sighandler);
			StdC.Signal.set_new_handler(StdC.Signal.SIGUSR1, sighandler);
			StdC.Signal.set_new_handler(StdC.Signal.SIGUSR2, sighandler);
			StdC.Signal.set_new_handler(StdC.Signal.SIGINT, sighandler);
			*/
		}

		/*
		private static void sighandler(int caught_signal) {
			Debug.Notification.emit(Debug.Subsystem.Misc,
									Debug.Level.Information, "Caught signal: "+
									caught_signal.to_string());
		}
		*/
	}
}
