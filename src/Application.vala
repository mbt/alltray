/* -*- mode: vala; c-basic-offset: 2; tab-width: 8; -*-
 * Application.vala - Application-based window management for AllTray
 * Copyright (c) 2009, 2011 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation
 */
using GLib;

namespace AllTray {
  public errordomain ApplicationError {
    FAILED
  }

  public class Application : GLib.Object {
    public signal void icon_changed(Gdk.Pixbuf new_icon);

    private unowned List<Wnck.Window> _windows;
    private Wnck.Application? _wnckApp;
    private GtkStatusIcon _appIcon;
    private Process _process;
    private bool _appVisible;
    private bool _caughtWindow;
    private bool _usingWindowIcon;

    private List<ulong> _attached_xids;
    private Queue<Wnck.Window> _window_enforce_minimize_queue;

    public Wnck.Application wnck_app {
      get {
	return(_wnckApp);
      }
    }

    public bool visible {
      get {
	return(_appVisible);
      }

      set {
	if(_appVisible != value) {
	  toggle_visibility();
	}
      }
    }

    public bool caught_window {
      get { return(_caughtWindow); }
      set {
	if(_caughtWindow) return;
	if(value == false) return;
	_caughtWindow = value;
      }
    }

    public Application(Process p) {
      _process = p;
      _window_enforce_minimize_queue = new Queue<Wnck.Window>();

      Debug.Notification.emit(Debug.Subsystem.Application,
			      Debug.Level.Information,
			      _("Creating a new Application"));

      /*
       * Register interest in learning of new applications on
       * the display, and then we'll be able to perform the rest
       * of the setup after we get notification of the
       * application.
       */
      Program.WnckScreen.application_opened += maybe_setup;
      Program.WnckScreen.application_closed += bye_wnck_app;

      // If we got early notifications, let's work through them.
      if(Program.WnckEarlyApps != null) {
	foreach(Wnck.Application app in Program.WnckEarlyApps) {
	  maybe_setup(Program.WnckScreen, app);
	}
      }
    }

    private void bye_wnck_app(Wnck.Screen scr, Wnck.Application app) {
      if(app == _wnckApp) {
	Debug.Notification.emit(Debug.Subsystem.Application,
				Debug.Level.Information,
				_("WHOA - The app went away?! Looking for it to come back..."));

	scr.window_opened -= maybe_update_window_count;
	scr.window_closed -= maybe_update_window_count;

	Program.WnckScreen.application_opened += maybe_setup;
      }
    }

    private bool are_we_interested(int pid) requires (pid != 0) {
      bool retval = false;
      int app_pid = pid;
      int app_ppid = get_ppid_for(app_pid);
      int app_pgid = Posix.getpgid(app_pid);
      int desired_pid = (int)_process.get_pid();
      int desired_pgid = (int)Program.pgid;
      string proc_name = get_process_name_for(pid);

      if((app_pgid == desired_pgid) && (app_pgid != app_pid)) {
	retval = true;
      } else if(app_pid == desired_pid) {
	retval = true;
      } else if(app_ppid == desired_pid) {
	retval = true;
      }

      if(!retval) {
	StringBuilder msg = new StringBuilder();

	msg.append_printf(_("pid %d (%s) is not ours."), app_pid, proc_name);
	Debug.Notification.emit(Debug.Subsystem.Application,
				Debug.Level.Information,
				msg.str);
      } else {
	if(app_pid != desired_pid) {
	  try {
	    _process.run_kinda_fake(app_pid);
	  } catch(ProcessError pe) {
	    critical("error: %s", pe.message);
	  }
	}
      }

      return(retval);
    }

    public void maybe_setup_for_pid(Wnck.Application app, int pid) {
      if(pid == 0) {
	warning(_("Application %s needs fixed, reports PID 0."),
		app.get_name());
      } else {
	bool interested = _caughtWindow = are_we_interested(pid);
	if(!interested) return;

	do_setup(Program.WnckScreen, app, pid);
      }
    }

    private void maybe_setup(Wnck.Screen scr, Wnck.Application app) {
      bool interested = _caughtWindow = are_we_interested(app.get_pid());

      if(!interested) {
	return;
      } else if(interested && _process.timer_running) {
	// Prevent AllTray from blowing itself up when an app shows up
	GLib.Source.remove(_process.timer_event_source);
      }

      do_setup(scr, app, app.get_pid());
    }

    private void on_ctt(ulong XID) {
      // CTT handler.
      Debug.Notification.emit(Debug.Subsystem.Application,
			      Debug.Level.Information,
			      "got a CTT event, XID %lu".printf(XID));
      foreach(Wnck.Window w in this._windows) {
	if(w.get_xid() == XID) {
	  set_visibility_for_window(w, false);
	  return;
	}
      }
    }

    private void do_setup(Wnck.Screen scr, Wnck.Application app, int pid) {
      scr.application_opened -= maybe_setup;

      _wnckApp = app;
      _appVisible = true;
      _windows = _wnckApp.get_windows();

      if(Program._ctt_enabled == true) {
	Program._ctt_obj.activate += this.on_ctt;

	foreach(Wnck.Window w in _windows) {
	  Program._ctt_obj.attach(w.get_xid());
	  this._attached_xids.append(w.get_xid());
	}
      }

      scr.window_opened += maybe_update_window_count;
      scr.window_closed += maybe_update_window_count;

      if(_appIcon == null) create_icon();
      _appIcon.visible = true;

      // Force an update to catch circumstances where the app
      // disappears and reappears (e.g., The GIMP).
      update_icon();

      if(Program._initially_hide == true) {
	hide_all_windows();
      }
    }

    private void display_menu(uint button, uint activate_time) {
      Debug.Notification.emit(Debug.Subsystem.Application,
			      Debug.Level.Information,
			      _("Requesting menu!"));
      Gtk.Menu appMenu = new Gtk.Menu();

      Gtk.MenuItem mnuToggle =
        new Gtk.MenuItem.with_label(_("Toggle Visibility"));
      Gtk.MenuItem mnuUndock =
        new Gtk.MenuItem.with_label(_("Undock"));
      Gtk.MenuItem mnuSeparator0 =
        new Gtk.SeparatorMenuItem();
      Gtk.MenuItem mnuAbout =
        new Gtk.MenuItem.with_label(_("About AllTray..."));

      Gtk.Menu windowList = create_window_list_menu();
      mnuToggle.set_submenu(windowList);

      // Append, wire and show the menu items.
      appMenu.append(mnuToggle);
      mnuToggle.show();

      appMenu.append(mnuUndock);
      mnuUndock.activate += on_menu_undock;
      mnuUndock.show();

      appMenu.append(mnuSeparator0);
      mnuSeparator0.show();

      appMenu.append(mnuAbout);
      mnuAbout.activate += on_menu_about;
      mnuAbout.show();

      appMenu.popup(null, null, null, button, activate_time);
    }

    private Gtk.Menu create_window_list_menu() {
      Gtk.Menu retval = new Gtk.Menu();

      Gtk.MenuItem mnuAllWindows = new Gtk.MenuItem.with_label(_("All"));
      Gtk.MenuItem mnuSep0 = new Gtk.SeparatorMenuItem();

      retval.append(mnuAllWindows);
      mnuAllWindows.activate += on_menu_toggle_app;
      mnuAllWindows.show();

      retval.append(mnuSep0);
      mnuSep0.show();

      foreach(Wnck.Window w in _windows) {
	Gtk.MenuItem item = new Gtk.MenuItem.with_label(w.get_name());
	item.set_data("target_window", w);
	item.activate += (item) => {
	  Wnck.Window win =
	  (Wnck.Window)item.get_data<Wnck.Window>("target_window");
	  toggle_window_visibility(win);
	};

	retval.append(item);
	item.show();
      }

      return(retval);
    }

    private void maybe_update_window_count(Wnck.Screen scr,
					   Wnck.Window win) {
      string new_tooltip;

      _windows = _wnckApp.get_windows();
      int wincount = _wnckApp.get_n_windows();

      List<ulong> xids_attached = this._attached_xids.copy();
      foreach(Wnck.Window w in _windows) {
	if(xids_attached.index(w.get_xid()) > -1) {
	  xids_attached.remove(w.get_xid());
	} else {
	  // This one needs to be attached _to_.
	  Program._ctt_obj.attach(w.get_xid());
	  _attached_xids.append(w.get_xid());
	}
      }

      // Remaining XIDs may be detached.
      foreach(ulong xid in xids_attached) {
	Program._ctt_obj.detach(xid);
      }

      if(wincount == 1) {
	Wnck.Window first_window = _windows.first().data;
	new_tooltip = _("%s - \"%s\"").printf(_wnckApp.get_name(),
					      first_window.get_name());
      } else {
	string app_name = _wnckApp.get_name();
	new_tooltip = ngettext("%s - %d window", "%s - %d windows",
			       wincount).printf(app_name, wincount);
      }

      _appIcon.set_tooltip(new_tooltip);
    }

    private void update_icon() {
      Gdk.Pixbuf new_icon;

      if(this._usingWindowIcon == true) {
	new_icon = this._windows.first().data.get_mini_icon();
      } else {
	new_icon = this._wnckApp.get_mini_icon();
      }

      icon_changed(new_icon);
    }

    /*
     * This tries to create an icon for the application.  First, we
     * see if the application has an icon (don't know *precisely* what
     * this means from Wnck's perspective... clarification would be
     * useful, as I don't have time to look at their source ATM).  If
     * the application does not have an icon, then we look to see if
     * the window does.
     *
     * If not, then we just use a fallback.
     *
     * Note that this only selects the first window it finds.  There
     * may be a better way to do this; time will tell.
     */
    private void create_icon() {
      _windows = _wnckApp.get_windows();
      Wnck.Window firstWindow = _windows.first().data;
      _usingWindowIcon = false;
      bool fallback = _wnckApp.get_icon_is_fallback();

      if(fallback) {
	_appIcon = new GtkStatusIcon(this, firstWindow.get_mini_icon(),
				     _wnckApp.get_name());
	firstWindow.icon_changed.connect(update_icon);
	_usingWindowIcon = true;
      } else {
	_appIcon = new GtkStatusIcon(this, _wnckApp.get_mini_icon(),
				     _wnckApp.get_name());
	_wnckApp.icon_changed.connect(update_icon);
      }

      _appIcon.activate += on_icon_click;
      _wnckApp.name_changed += update_icon_name;

      string msg = "";
      if(_usingWindowIcon) {
	msg = _("Using Window Icon (fallback was %s)").
	printf(fallback.to_string());
      } else {
	msg = _("Using App Icon (fallback was %s)").
	printf(fallback.to_string());
      }

      Debug.Notification.emit(Debug.Subsystem.Application,
			      Debug.Level.Information,
			      msg);
    }

    private void update_icon_name(Wnck.Application app) {
      _appIcon.set_tooltip(app.get_name());
    }

    private void on_icon_click() {
      //if(icon.blinking == true) icon.blinking = false;
      toggle_visibility();
    }

    /*
     * This looks like it works.  However, the user may notice the
     * window trying to appear for a moment before we catch it...
     *
     * TODO: Find a way to intercept the window activation and
     * prevent that from happening at all, and instead setting the
     * DEMANDS_ATTENTION window manager hint, and generally blink
     * the icon on DEMANDS_ATTENTION instead.
     */
    private void maintain_hiddenness(Wnck.Window win,
				     Wnck.WindowState changed_bits,
				     Wnck.WindowState new_state) {
      if(_appVisible) return;

      string ch_bitmask =
        _("New Bitmask: %s").printf(get_win_state(changed_bits));
      string ch_new_state =
        _("New State: %s").printf(get_win_state(new_state));

      debug_msg(ch_bitmask);
      debug_msg(ch_new_state);

      if((new_state & Wnck.WindowState.MINIMIZED) == 0 &&
	 (changed_bits & Wnck.WindowState.MINIMIZED) ==
	 Wnck.WindowState.MINIMIZED) {
	debug_msg(_("Blinking, window state is: %s")
		  .printf(get_win_state(new_state)));
	win.minimize();
	_appIcon.blinking = true;
      }
    }

    private void debug_msg(string str) {
      Debug.Notification.emit(Debug.Subsystem.Application,
			      Debug.Level.Information,
			      str);
    }

    private string get_win_state(Wnck.WindowState state) {
      StringBuilder sb = new StringBuilder();

      if((state & Wnck.WindowState.MINIMIZED) != 0)
	sb.append(_("MINIMIZED | "));
      if((state & Wnck.WindowState.MAXIMIZED_HORIZONTALLY) != 0)
	sb.append(_("MAXIMIZED_HORIZONTALLY | "));
      if((state & Wnck.WindowState.MAXIMIZED_VERTICALLY) != 0)
	sb.append(_("MAXIMIZED_VERTICALLY | "));
      if((state & Wnck.WindowState.SHADED) != 0)
	sb.append(_("SHADED | "));
      if((state & Wnck.WindowState.SKIP_PAGER) != 0)
	sb.append(_("SKIP_PAGER | "));
      if((state & Wnck.WindowState.SKIP_TASKLIST) != 0)
	sb.append(_("SKIP_TASKLIST | "));
      if((state & Wnck.WindowState.STICKY) != 0)
	sb.append(_("STICKY | "));
      if((state & Wnck.WindowState.HIDDEN) != 0)
	sb.append(_("HIDDEN | "));
      if((state & Wnck.WindowState.FULLSCREEN) != 0)
	sb.append(_("FULLSCREEN | "));
      if((state & Wnck.WindowState.DEMANDS_ATTENTION) != 0)
	sb.append(_("DEMANDS_ATTENTION | "));
      if((state & Wnck.WindowState.URGENT) != 0)
	sb.append(_("URGENT | "));
      if((state & Wnck.WindowState.ABOVE) != 0)
	sb.append(_("ABOVE | "));
      if((state & Wnck.WindowState.BELOW) != 0)
	sb.append(_("BELOW | "));

      sb.truncate(sb.len - 3);
      return(sb.str);
    }

    private void toggle_visibility() {
      _appVisible = !_appVisible;

      foreach(Wnck.Window w in _windows) {
	set_visibility_for_window(w, _appVisible);
      }
    }

    private void show_all_windows() {
      _appVisible = false;
      toggle_visibility();
    }

    private void hide_all_windows() {
      _appVisible = true;
      toggle_visibility();
    }

    private void toggle_window_visibility(Wnck.Window w) {
      Wnck.WindowState ws = w.get_state();

      if(((ws & Wnck.WindowState.MINIMIZED) != 0) &&
	 ((ws & Wnck.WindowState.SKIP_TASKLIST) != 0)) {
	set_visibility_for_window(w, true);
      } else {
	set_visibility_for_window(w, false);
      }
    }

    private void set_visibility_for_window(Wnck.Window w,
					   bool set_visible) {
      TimeVal tv = TimeVal();
      tv.get_current_time();

      StringBuilder msg = new StringBuilder();
      msg.append_printf(_("Setting window 0x%08lx visibility to %s"),
			w.get_xid(), set_visible.to_string());

      if(set_visible) {
	// Forcibly empty the queue (prevent a race).
	_window_enforce_minimize_queue.clear();

	w.state_changed -= maintain_hiddenness;
	w.set_skip_tasklist(false);
	w.set_skip_pager(false);
	w.unminimize((uint32)tv.tv_sec);

	Wnck.Workspace ws = w.get_workspace();
	ws.activate((uint32)tv.tv_sec);
	w.activate((uint32)tv.tv_sec);
      } else {
	w.set_skip_tasklist(true);
	w.set_skip_pager(true);
	w.minimize();

	_window_enforce_minimize_queue.push_head(w);
	Timeout.add(150, set_maintain_hiddenness);
      }

      Debug.Notification.emit(Debug.Subsystem.Application,
			      Debug.Level.Information,
			      msg.str);
    }

    private bool set_maintain_hiddenness() {
      Wnck.Window? w;

      while((w = _window_enforce_minimize_queue.pop_tail()) != null) {
	w.state_changed += maintain_hiddenness;
      }

      /*
       * Only call us again if the queue has had things pop up in
       * it that we didn't handle this time.
       */
      return(_window_enforce_minimize_queue.length == 0);
    }

    private void dialog_destroy(Gtk.AboutDialog which, int resp_id) {
      which.destroy();
    }

    /**********************************************************
     * Context menu handlers.
     **********************************************************/
    private void on_menu_toggle_app(Gtk.MenuItem item) {
      if(_appIcon.blinking == true) _appIcon.blinking = false;
      toggle_visibility();
    }

    private void on_menu_about() {
      // Display an about dialog
      Gtk.AboutDialog about = new Gtk.AboutDialog();
      about.program_name = "AllTray";

      if((Build.PACKAGE_VERSION.chr(-1, '+') != null) &&
	 (Build.ALLTRAY_BZR_BUILD == "TRUE")) {
	about.version = _("%s\nBranch: %s, r%s\n%s").
	  printf(Build.PACKAGE_VERSION,
		 Build.ALLTRAY_BZR_BRANCH,
		 Build.ALLTRAY_BZR_REVISION,
		 Build.ALLTRAY_BZR_REVID);
      } else {
	about.version = Build.PACKAGE_VERSION;
      }

      about.website = "http://alltray.trausch.us/";
      about.copyright = _("Copyright (c) %s")
        .printf(Build.ALLTRAY_COPYRIGHT_YEARS);
      about.comments = _("Dock applications in the system tray.");
      about.license = Build.ALLTRAY_LICENSE;

      about.response += dialog_destroy;

      about.show_all();
    }

    private void on_menu_undock() {
      show_all_windows();
      Posix.exit(0);
    }
  }
}
