/* -*- mode: vala; c-basic-offset: 2; tab-width: 8; -*-
 * Debug.vala - Debugging system for AllTray.
 * Copyright (C) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3 as published by the Free Software Foundation.
 */
using GLib;

namespace AllTray.Debug {
  public enum Subsystem {
    None = 0x00,
    CommandLine = 0x01,
    Display = 0x02,
    TrayIcon = 0x04,
    WindowManager = 0x08,
    Process = 0x10,
    Misc = 0x20,
    Bug = 0x40,
    Application = 0x80,
    AttachHelper = 0x100,
    Signal = 0x200,
    Main = 0x400,

    All = Subsystem.CommandLine | Subsystem.Display | Subsystem.TrayIcon |
    Subsystem.WindowManager | Subsystem.Process | Subsystem.Misc |
    Subsystem.Bug | Subsystem.Application | Subsystem.AttachHelper |
    Subsystem.Signal | Subsystem.Main
  }

  public enum Level {
    Information,
    Warning,
    Error,
    Fatal
  }

  private struct DebugDescription {
    public Subsystem subsys;
    public string name;
    public string description;
  }

  public static class Notification {
    private static Subsystem _subsys = Subsystem.None;
    private static bool _enabled = false;

    private const DebugDescription[] _debugOptions = {
      { Subsystem.CommandLine, "CL", _("command line parsing") },
      { Subsystem.Display, "DISPLAY", _("X11 display") },
      { Subsystem.TrayIcon, "TRAY", _("System Tray Icon") },
      { Subsystem.WindowManager, "WM", _("Window Manager") },
      { Subsystem.Process, "PROCESS", _("Process information"_) },
      { Subsystem.Misc, "MISC", _("Misc messages") },
      { Subsystem.Bug, "BUG", _("Internal error detail") },
      { Subsystem.Application, "APP", _("Applications") },
      { Subsystem.AttachHelper, "AH", _("AttachHelper class messages") },
      { Subsystem.Signal, "SIG", _("Signal debugging messages") },
      { Subsystem.Main, "MAIN", _("Entrypoint, preinit, etc.") },
      { Subsystem.All, "ALL", _("All messages") }
    };

    public static void display_debug_list() {
      stdout.printf("%-16s%s\n", _("String"), _("Messages are for"));
      stdout.printf("======================================"+
		    "======================================\n");

      foreach(DebugDescription dd in _debugOptions) {
	stdout.printf("%-16s%s\n", dd.name, dd.description);
      }
    }

    public static void init() {
      _enabled = true;
      string? enabled_subsystems =
      GLib.Environment.get_variable("ALLTRAY_DEBUG");

      if(enabled_subsystems != null) {
	string[] values = enabled_subsystems.split(" ");
	foreach(string tmpSubsys in values) {
	  switch(tmpSubsys) {
	  case "AH":
	    _subsys |= Subsystem.AttachHelper;
	    break;
	  case "CL":
	    _subsys |= Subsystem.CommandLine;
	    break;
	  case "DISPLAY":
	    _subsys |= Subsystem.Display;
	    break;
	  case "SYSTRAY":
	    _subsys |= Subsystem.TrayIcon;
	    break;
	  case "WM":
	    _subsys |= Subsystem.WindowManager;
	    break;
	  case "PROCESS":
	    _subsys |= Subsystem.Process;
	    break;
	  case "MISC":
	    _subsys |= Subsystem.Misc;
	    break;
	  case "BUG":
	    _subsys |= Subsystem.Bug;
	    break;
	  case "APP":
	    _subsys |= Subsystem.Application;
	    break;
	  case "SIG":
	    _subsys |= Subsystem.Signal;
	    break;
	  case "MAIN":
	    _subsys |= Subsystem.Main;
	    break;
	  case "ALL":
	    _subsys |= (Subsystem.CommandLine |
			Subsystem.Display |
			Subsystem.TrayIcon |
			Subsystem.WindowManager |
			Subsystem.Process |
			Subsystem.Misc |
			Subsystem.Bug |
			Subsystem.Application |
			Subsystem.AttachHelper |
			Subsystem.Signal |
			Subsystem.Main);
	    break;
	  default:
	    GLib.warning(_("Unrecognized value '%s' in ALLTRAY_DEBUG"),
			 tmpSubsys);
	    break;
	  }
	}

	emit(Subsystem.Misc, Level.Information,
	     _("Debugging subsystem setup complete."));
      } else if((enabled_subsystems == null) ||
		(enabled_subsystems == "")) {
	GLib.warning(_("ALLTRAY_DEBUG not set!"));
      }
    }

    public static void emit(Subsystem subsys, Level lvl, string msg) {
      // Bail out early when we should not run.
      if(!_enabled) return;
      if((_subsys & subsys) != subsys) return;

      string cur_time;

      cur_time = Time.local(time_t()).format("%F %T %Z");

      string lvl_str = lvl_to_string(lvl);
      string subsys_str = subsys_to_string(subsys);
      stdout.printf("[%s] %s/%s: %s\n",
		    cur_time, lvl_str, subsys_str, msg);

      if(lvl == Level.Fatal) {
	emit(subsys, Level.Information,
	     _("Exiting: encountered fatal error."));
	Posix.abort();
      }
    }

    private static string subsys_to_string(Subsystem subsys) {
      string retval = "";

      switch(subsys) {
      case Subsystem.None:
	retval = "None";
	break;

      case Subsystem.AttachHelper:
	retval = "AH";
	break;

      case Subsystem.CommandLine:
	retval = "CL";
	break;

      case Subsystem.Display:
	retval = "DISPLAY";
	break;

      case Subsystem.TrayIcon:
	retval = "TRAY";
	break;

      case Subsystem.WindowManager:
	retval = "WM";
	break;

      case Subsystem.Process:
	retval = "PROCESS";
	break;

      case Subsystem.Misc:
	retval = "MISC";
	break;

      case Subsystem.Application:
	retval = "APP";
	break;

      case Subsystem.Bug:
	retval = "BUG";
	break;

      case Subsystem.Signal:
	retval = "SIG";
	break;

      case Subsystem.Main:
	retval = "MAIN";
	break;

      default:
	GLib.error(_("BUG: Reached default, should not be possible."));
	break;
      }

      return(retval);
    }

    private static string lvl_to_string(Level lvl) {
      string retval = "";

      switch(lvl) {
      case Level.Information:
	retval = "INFO";
	break;

      case Level.Warning:
	retval = "WARN";
	break;

      case Level.Error:
	retval = "ERR";
	break;

      case Level.Fatal:
	retval = "FATAL";
	break;

      default:
	GLib.error(_("BUG: Reached default, should not be possible."));
	break;
      }

      return(retval);
    }
  }
}
