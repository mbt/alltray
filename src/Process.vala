/* -*- mode: vala; c-basic-offset: 2; tab-width: 8; -*-
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
    private bool _running;
    private bool _timerRunning;
    private uint _timerEventSource;
    private string[] _argv;
    private AllTray.Application _app;

    public signal void process_died(Process p);

    public bool running {
      get {
	return(_running);
      }
    }

    public bool visible {
      get {
	return(_app.visible);
      }

      set {
	_app.visible = value;
      }
    }

    public bool timer_running {
      get {
	return(_timerRunning);
      }
    }

    public uint timer_event_source {
      get {
	return(_timerEventSource);
      }
    }

    public Process(string[] argv) {
      _child = (Pid)0;
      _running = false;
      _timerRunning = false;
      _timerEventSource = 0;
      _app = null;
      _argv = argv;
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
	msg.append_printf(_("Child process %d (%s) now running."),
			  (int)_child, _argv[0]);
	Debug.Notification.emit(Debug.Subsystem.Process,
				Debug.Level.Information,
				msg.str);
      } catch(SpawnError e) {
	_running = false;
	throw new ProcessError.SPAWN_FAILED(e.message);
      }

      _app = new AllTray.Application(this);
    }

    public void run_kinda_fake(int pid) throws ProcessError {
      run_fake(pid);
    }

    private void run_fake(int pid) throws ProcessError {
      Debug.Notification.emit(Debug.Subsystem.Process,
			      Debug.Level.Information,
			      "run_fake()");
      _running = true;
      _child = (Pid)pid;

      if(!process_num_is_alive(pid)) {
	throw new ProcessError.PROCESS_DOES_NOT_EXIST(_("The provided PID does not exist."));
      }

      StringBuilder msg = new StringBuilder();
      msg.append_printf("Process %d now attached.",
			(int)_child);
      Debug.Notification.emit(Debug.Subsystem.Process,
			      Debug.Level.Information,
			      msg.str);

      Timeout.add(50, fake_child_monitor);
      if(_app == null) {
	_app = new AllTray.Application(this);
      }
    }

    private bool fake_child_monitor() {
      if(process_num_is_alive((int)_child)) {
	return(true);
      }

      _running = false;

      StringBuilder msg = new StringBuilder();
      msg.append_printf(_("Attached process %d has died."),
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

    /*
     * We try to work around some strange situations that come up
     * here.  We hope to catch applications that we spawn, but die
     * before their children actually display a window (e.g.,
     * shell scripts starting the real application).
     *
     * If we fail to detect certain situations, that is a bug in
     * AllTray which needs reporting for sure.  Maybe the upstream
     * of the other project would like to be made aware of the
     * problem, too, so that they can fix its startup process to
     * play nicely with us if it's _really_ strange.
     */
    public void child_died() {
      _running = false;

      if(_app.caught_window) {
	// Got a window in the process' lifetime, assume that was
	// correct and go ahead and die.
	Debug.Notification.emit(Debug.Subsystem.Process,
				Debug.Level.Information,
				_("Child %d died okay").printf((int)_child));
	process_died(this);
      } else {
	/*
	 * If we haven't caught a window, it's likely that the process
	 * we started actually was a script or something, kicking off
	 * the real process.  We'll wait and see.
	 */
	Debug.Notification.emit(Debug.Subsystem.Process,
				Debug.Level.Information,
				_("Child %d died with nothing!")
				  .printf((int)_child));

	_timerRunning = true;

	// XXX:  This value may need to be tweaked.  Only a default should
	// be hard-coded, really.
	_timerEventSource =
	  GLib.Timeout.add_seconds(3, die_if_not_unset);
      }
    }

    /*
     * Aggressively to find a process that fits.
     *
     * This appears to be required to actually catch KDE apps, who set their
     * PID on the window, but Wnck doesn't see the PID for the application.
     *
     * Odd.
     */
    private bool really_try_hard() {
      int[] procs;
      get_pids_in_pgid((int)Program.pgid, out procs);
      if(procs.length == 0) return(false);

      foreach(int proc in procs) {
	if(proc != Posix.getpid()) {
	  foreach(Wnck.Window w in
		  (List<Wnck.Window>)Program.WnckScreen.get_windows()) {
	    if(w.get_pid() == proc) {
	      _app.maybe_setup_for_pid(w.get_application(),
				       w.get_pid());
	      return(true);
	    }
	  }
	}
      }

      // I _hate_ this... is there a way to avoid doing this?
      stderr.printf(
	_("AllTray was unable to attach to any applications, and AllTray's child\nhas died.\n\nThis problem can be caused by application software that:\n  * is not compliant with the ICCCM and EWMH specs,\n  * backgrounds itself and leaves the AllTray process group, or\n  * unexpectedly died an early death and did not actually start\n\nIf the problem is one of the first two problems, please report a bug\nin AllTray and it will be investigated and reported to the correct\nproject, if necessary.  If the problem is the third one, then you will\nneed to fix whatever went wrong and try again.\n"));
      Posix.exit(10);
      return(false);
    }

    /*
     * If we're called and we still have nothing, then we give up
     * and fire the process_died signal.
     *
     * The way this actually works is we're called after 30
     * seconds.  If we pick up a window before that 30-second
     * window is up, the timer that calls this function must be
     * deactivated so we don't accidentally die after picking up
     * the application.
     */
    private bool die_if_not_unset() {
      if(really_try_hard() == false) {
	process_died(this);
      }

      // Either way, never call us again.
      return(false);
    }
  }
}
