/* -*- mode: vala; c-basic-offset: 2; tab-width: 8; -*-
 * AllTray.vala - Entrypoint for AllTray.
 * Copyright (c) 2009 Michael B. Trausch <mike@trausch.us>
 */
using GLib;

namespace AllTray {
  public errordomain AllTrayError {
    SET_PGID_FAILED,
    FAILED
  }

  public class Program : GLib.Object {
    private string[] _args;
    private string[] _cleanArgs;
    private List<Process> _plist;
    private GLib.OptionContext opt_ctx;

    private static bool _attach;
    private static int _pid;

    private static bool _cl_debug;
    private static bool _cl_dopts;
    private static bool _display_ver;
    private static bool _display_extended_ver;
    private static Program _instance;

    public static Wnck.Screen WnckScreen;
    public static List<Wnck.Application> WnckEarlyApps;
    public static Pid pgid;

    private PromptDialog _pd;

    private const GLib.OptionEntry[] _acceptedCmdLineOptions = {
      { "attach", 'a', 0, GLib.OptionArg.NONE, ref _attach,
	"Attach to a running program", null },
      { "debug", 'D', GLib.OptionFlags.HIDDEN, GLib.OptionArg.NONE,
	ref _cl_debug, "Enable debugging messages", null },
      { "list-debug-opts", 'L', GLib.OptionFlags.HIDDEN,
	GLib.OptionArg.NONE, ref _cl_dopts,
	"Show types of debugging messages", null },
      { "process", 'p', 0, GLib.OptionArg.INT, ref _pid,
	"Attach to already-running application",
	"PID" },
      { "version", 'v', 0, GLib.OptionArg.NONE, ref _display_ver,
	"Display AllTray version info and exit", null },
      { "extended-version", 'V', 0, GLib.OptionArg.NONE,
	ref _display_extended_ver,
	"Display extended version info and exit",
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
			      _("Command line options parsed."));
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

      opt_ctx = new GLib.OptionContext(_("- Dock software to the systray"));
      opt_ctx.add_main_entries(this._acceptedCmdLineOptions, null);
      opt_ctx.add_group(Gtk.get_option_group(true));

      if(env_args != null) {
	try {
	  opt_ctx.parse(ref env_args);
	} catch(OptionError e) {
	  stderr.printf(_("error: env flag parsing failed (%s)\n"),
			e.message);
	  Posix.exit(1);
	}
      }

      try {
	opt_ctx.parse(ref args);
      } catch(OptionError e) {
	stderr.printf(_("error: command line parsing failed (%s)\n"),
		      e.message);
	Posix.exit(1);
      }

      if(_display_ver && !_display_extended_ver) {
	display_version();
	Posix.exit(0);
      }

      if(_display_extended_ver) {
	display_extended_version();
	Posix.exit(0);
      }

      if(_cl_dopts) {
	Debug.Notification.display_debug_list();
	Posix.exit(0);
      }

      display_version();
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

    private void maybe_set_pgid() {
      Pid cur_pgid = (Pid)Posix.getpgrp();
      Pid needed_pgid = (Pid)Posix.getpid();

      if(cur_pgid != needed_pgid) {
	int status = Posix.setpgid(0, 0);
	if(status == -1)
	  throw new AllTrayError.SET_PGID_FAILED("set pgid failed");

	Program.pgid = (Pid)Posix.getpgrp();
	return;
      } else {
	// We're already the same, so it's all good.
	Program.pgid = cur_pgid;
	return;
      }

      /*
       * If we're here, then we do not have the correct pgid,
       * and we cannot set the pgid.  This should *not* happen,
       * so we throw a fatal error here if it does, which should
       * be reported to be investigated if it does happen.
       */
      GLib.error(_("Cannot set PGID.  Cannot continue."));
      Posix.abort(); // Meta: we actually don't execute this line.
    }

    public int run() {
      _plist = new List<Process>();

      Wnck.set_client_type(Wnck.ClientType.PAGER);
      maybe_set_pgid();
      install_signal_handlers();

      WnckScreen = Wnck.Screen.get_default();

      if(_attach && (_pid == 0)) {
	prompt_and_attach();
      } else if(_pid > 0) {
	attach_to_pid(_pid);
      } else {
	if(_cleanArgs.length == 1) {
	  display_help();
	  return(1);
	}

	try {
	  spawn_new_process();
	} catch(AllTrayError e) {
	  stderr.printf(e.message);
	  return(1);
	}
      }

      Gtk.main();
      return(0);
    }

    private void display_version() {
      if(Build.ALLTRAY_BZR_BUILD == "TRUE") {
	stdout.printf(_("AllTray %s, from bzr branch %s rev %s,\n rev-id %s\n"),
		      Build.PACKAGE_VERSION, Build.ALLTRAY_BZR_BRANCH,
		      Build.ALLTRAY_BZR_REVISION, Build.ALLTRAY_BZR_REVID);
      } else {
	stdout.printf("AllTray %s\n",
		      Build.PACKAGE_VERSION);
      }

      stdout.printf(_("Copyright (c) %s Michael B. Trausch <mike@trausch.us>\n"), Build.ALLTRAY_COPYRIGHT_YEARS);
      stdout.printf(_("Licensed under the GNU GPL v3.0 as published by the Free Software Foundation.\n\n"));
    }

    private void display_extended_version() {
      display_version();

      stdout.printf(_("Configured %s on %s %s\n"),
		    Build.ALLTRAY_COMPILE_BUILD_DATE,
		    Build.ALLTRAY_COMPILE_OS,
		    Build.ALLTRAY_COMPILE_OS_REL);

      stdout.printf(_("Compilers: %s and %s\n"),
		    Build.ALLTRAY_VALA_COMPILER,
		    Build.ALLTRAY_C_COMPILER);

      if(Build.ALLTRAY_CONFIGURE_FLAGS == "") {
	stdout.printf(_("Configure was run without flags\n"));
      } else {
	stdout.printf(_("Configure flags: %s\n"),
		      Build.ALLTRAY_CONFIGURE_FLAGS);
      }
    }

    private bool delete_pd_window() {
      _pd.destroy();
      Debug.Notification.emit(Debug.Subsystem.Misc,
			      Debug.Level.Information,
			      "delete_pd_window()");
      return(false);
    }

    private void get_app_early(Wnck.Screen scr, Wnck.Application app) {
      StringBuilder msg = new StringBuilder();
      msg.append_printf(_("Adding app (pid %d) to list of recv'd apps"),
			app.get_pid());

      Debug.Notification.emit(Debug.Subsystem.Application,
			      Debug.Level.Information,
			      msg.str);
      WnckEarlyApps.append(app);
    }

    private void prompt_and_attach() {
      _pd = new PromptDialog();

      WnckEarlyApps = new List<Wnck.Application>();
      WnckScreen.application_opened += get_app_early;

      _pd.show_all();

      // Setup and invoke a Gtk.main() that just waits for the user's
      // response.  A new Gtk.main() will be called later that will
      // actually begin the program's real work.
      Idle.add(AttachHelper.get_target_window);
      Gtk.main();

      Idle.add(delete_pd_window);
      WnckScreen.application_opened -= get_app_early;

      if(AttachHelper.success == false) {
	stderr.printf(_("Failed to get a window; exiting.\n"));
	Posix.exit(1);
      }

      _pid = AttachHelper.target_process;
      attach_to_pid(_pid);
    }

    /*
     * Doing it this way reduces the number of code paths in the
     * program and so, while confusing, is preferable.  What we do
     * is give a sentinal "process name" to spawn_new_process()
     * for it to spawn.  Really, though, what happens is the
     * Process sees this sentinel value and just *acts* like it
     * spawned a process.
     */
    private void attach_to_pid(int pid) {
      _cleanArgs = new string[] {
	"alltray-internal-fake-process",
	pid.to_string()
      };

      try {
	spawn_new_process();
      } catch(AllTrayError e) {
	// Shouldn't happen.
      }
    }

    private void spawn_new_process() throws AllTrayError {
      string[] a = get_command_line(_cleanArgs);
      Process p = new Process(a);
      p.process_died += cleanup_for_process;
      try {
	p.run();
      } catch(ProcessError e) {
	Debug.Notification.emit(Debug.Subsystem.Process,
				Debug.Level.Error,
				_("Oops.  Not running?"));
	StringBuilder msg = new StringBuilder();
	msg.append_printf(_("Failed to start process: %s"), e.message);
	throw new AllTrayError.FAILED(msg.str);
      }

      if(!p.running) {
	Debug.Notification.emit(Debug.Subsystem.Process,
				Debug.Level.Error,
				_("Oops.  Not running?"));
	throw new AllTrayError.FAILED(_("Failed to start process"));
      }

      _plist.append(p);
    }

    public static int main(string[] args) {
      Intl.bindtextdomain(Build.GETTEXT_PACKAGE, Build.LOCALE_DIR);
      Intl.textdomain(Build.GETTEXT_PACKAGE);
      Environment.set_application_name(Build.GETTEXT_PACKAGE);

      Program atray = new Program(ref args);
      Program._instance = atray;

      return(atray.run());
    }

    private void cleanup_for_process(Process p) {
      Debug.Notification.emit(Debug.Subsystem.Process,
			      Debug.Level.Information,
			      _("Cleaning up for child..."));
      _plist.remove(p);

      if(_plist.length() == 0) {
	Debug.Notification.emit(Debug.Subsystem.Main,
				Debug.Level.Information,
				_("No more children. Dying."));
	Gtk.main_quit();
      }
    }

    private string[] get_command_line(string[] args) {
      int curItem = 0;
      string[] retval = new string[args.length];

      foreach(string arg in args) {
	StringBuilder sb = new StringBuilder();
	sb.append_printf(_("Evaluating %s"), arg);
	Debug.Notification.emit(Debug.Subsystem.CommandLine,
				Debug.Level.Information,
				sb.str);

	if(arg.contains("alltray") &&
	   !arg.contains("internal-fake-process")) continue;
	Debug.Notification.emit(Debug.Subsystem.CommandLine,
				Debug.Level.Information,
				_("Actually processing arg"));

	retval[curItem++] = arg;
      }

      return(retval);
    }

    private void install_signal_handlers() {
      Posix.signal(Posix.SIGHUP, sighandler);
      Posix.signal(Posix.SIGTERM, sighandler);
      Posix.signal(Posix.SIGINT, sighandler);
    }

    private static void sighandler(int caught_signal) {
      if(caught_signal == Posix.SIGHUP ||
	 caught_signal == Posix.SIGINT ||
	 caught_signal == Posix.SIGTERM) {
	foreach(Process p in _instance._plist) {
	  p.visible = true;
	}
      } else if(caught_signal == Posix.SIGTERM) {
	foreach(Process p in _instance._plist) {
	  // Hate the name, should be send_signal not kill.
	  Posix.kill((int)p.get_pid(), caught_signal);
	}

	// Quit anyway, in case they don't respond (then the user can
	// worry about killing them).
	Gtk.main_quit();
      } else {
	StringBuilder msg = new StringBuilder();
	msg.append_printf(_("Caught unwired signal %d"), caught_signal);
	Debug.Notification.emit(Debug.Subsystem.Signal,
				Debug.Level.Information,
				msg.str);
      }
    }

    private void display_help() {
      stderr.printf(opt_ctx.get_help(true, null));
      Posix.exit(1);
    }
  }
}
