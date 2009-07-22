/* -*- mode: vala; c-basic-offset: 2; tab-width: 8; -*-
 * Application.vala - Application-based window management for AllTray
 * Copyright (c) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation
 */
using GLib;

namespace AllTray {
  public errordomain ApplicationError {
    FAILED
  }

  public class Application : GLib.Object {
    private unowned List<Wnck.Window> _windows;
    private Wnck.Application? _wnckApp;
    private AllTray.LocalGtk.StatusIcon _appIcon;
    private Process _process;
    private bool _appVisible;
    private bool _caughtWindow;
    private bool _usingWindowIcon;

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
      Debug.Notification.emit(Debug.Subsystem.Application,
			      Debug.Level.Information,
			      "Hey hey, creating a new Application");

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
				"WHOA - The app went away‽  "+
				"Looking for it to come back...");

	scr.window_opened -= maybe_update_window_count;
	scr.window_closed -= maybe_update_window_count;

	Program.WnckScreen.application_opened += maybe_setup;
      }
    }

    private bool are_we_interested(int pid) {
      bool retval = false;
      int app_pid = pid;
      int app_ppid = get_ppid_for(app_pid);
      int app_pgid = Posix.getpgid(app_pid);
      int desired_pid = (int)_process.get_pid();
      int desired_pgid = (int)Program.pgid;
      string proc_name = get_process_name_for(pid);

      // Bail on invalid value, some apps are odd and buggy.
      if(app_pid == 0) {
	GLib.warning("Got PID 0; some application is buggy!");
	return(false);
      }

      if((app_pgid == desired_pgid) && (app_pgid != app_pid)) {
	retval = true;
      } else if(app_pid == desired_pid) {
	retval = true;
      } else if(app_ppid == desired_pid) {
	retval = true;
      }

      if(!retval) {
	StringBuilder msg = new StringBuilder();

	msg.append_printf("pid %d (%s) is not ours.\n  Want pid = %d, "+
			  "ppid = %d, or pgid = %d, have pid = "+
			  "%d, ppid = %d, pgid = "+
			  "%d", app_pid, proc_name, desired_pid,
			  desired_pid, desired_pgid, app_pid, app_ppid,
			  app_pgid);
	Debug.Notification.emit(Debug.Subsystem.Application,
				Debug.Level.Information,
				msg.str);
      } else {
	if(app_pid != desired_pid) {
	  _process.run_kinda_fake(app_pid);
	}
      }

      return(retval);
    }

    public void maybe_setup_for_pid(Wnck.Application app, int pid) {
      bool interested = _caughtWindow = are_we_interested(pid);
      if(!interested) return;

      do_setup(Program.WnckScreen, app, pid);
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

    private void do_setup(Wnck.Screen scr, Wnck.Application app, int pid) {
      scr.application_opened -= maybe_setup;

      _wnckApp = app;
      _appVisible = true;
      _windows = _wnckApp.get_windows();

      scr.window_opened += maybe_update_window_count;
      scr.window_closed += maybe_update_window_count;

      if(_appIcon == null) create_icon();
      _appIcon.visible = true;
      _appIcon.popup_menu += display_menu;

      // Force an update to catch circumstances where the app
      // disappears and reappears (e.g., The GIMP).
      if(_usingWindowIcon) {
	update_icon_win_image(_windows.first().data);
      } else {
	update_icon_app_image(_wnckApp);
      }
    }

    private void display_menu(uint button, uint activate_time) {
      Debug.Notification.emit(Debug.Subsystem.Application,
			      Debug.Level.Information,
			      "Requesting menu!");
      Gtk.Menu appMenu = new Gtk.Menu();

      Gtk.MenuItem mnuToggle =
        new Gtk.MenuItem.with_label("Toggle Visibility");
      Gtk.MenuItem mnuUndock =
        new Gtk.MenuItem.with_label("Undock");
      Gtk.MenuItem mnuSeparator0 =
        new Gtk.SeparatorMenuItem();
      Gtk.MenuItem mnuAbout =
        new Gtk.MenuItem.with_label("About AllTray…");

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

      Gtk.MenuItem mnuAllWindows = new Gtk.MenuItem.with_label("All");
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
	  (Wnck.Window)item.get_data("target_window");
	  toggle_window_visibility(win);
	};

	retval.append(item);
	item.show();
      }

      return(retval);
    }

    private void maybe_update_window_count(Wnck.Screen scr,
					   Wnck.Window win) {
      StringBuilder sb = new StringBuilder();
      _windows = _wnckApp.get_windows();
      int wincount = _wnckApp.get_n_windows();
      string plural = (wincount != 1 ? "s" : "");

      sb.append_printf("%s - %d window%s", _wnckApp.get_name(),
		       wincount, plural);

      _appIcon.set_tooltip(sb.str);
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
	_appIcon = new LocalGtk.StatusIcon.
	from_pixbuf(firstWindow.get_mini_icon());
	firstWindow.icon_changed += update_icon_win_image;
	_usingWindowIcon = true;
      } else {
	_appIcon = new LocalGtk.StatusIcon.
	from_pixbuf(_wnckApp.get_mini_icon());
	_wnckApp.icon_changed += update_icon_app_image;
      }

      _appIcon.set_tooltip(_wnckApp.get_name());
      _appIcon.activate += on_icon_click;
      _wnckApp.name_changed += update_icon_name;

      string msg = "";
      if(_usingWindowIcon) {
	msg = "Using Window Icon (fallback was %s)".
	printf(fallback.to_string());
      } else {
	msg = "Using App Icon (fallback was %s)".
	printf(fallback.to_string());
      }

      Debug.Notification.emit(Debug.Subsystem.Application,
			      Debug.Level.Information,
			      msg);
    }

    private void update_icon_app_image(Wnck.Application app) {
      unowned Gdk.Pixbuf new_icon = app.get_icon();

      _appIcon.set_from_pixbuf(new_icon);

      if(new_icon == null) {
	_appIcon.visible = false;
	return;
      }
    }

    private void update_icon_win_image(Wnck.Window win) {
      unowned Gdk.Pixbuf new_icon = win.get_mini_icon();

      _appIcon.set_from_pixbuf(new_icon);

      if(new_icon == null) {
	_appIcon.visible = false;
	return;
      }
    }

    private void update_icon_name(Wnck.Application app) {
      _appIcon.set_tooltip(app.get_name());
    }

    private void on_icon_click(LocalGtk.StatusIcon icon) {
      if(icon.blinking == true) icon.blinking = false;
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
        "New Bitmask: %s".printf(get_new_windowstate(changed_bits));
      string ch_new_state =
        "New State: %s".printf(get_new_windowstate(new_state));

      debug_msg(ch_bitmask);
      debug_msg(ch_new_state);

      if((new_state & Wnck.WindowState.MINIMIZED) == 0) {
	win.minimize();
	_appIcon.blinking = true;
      }
    }

    private void debug_msg(string str) {
      Debug.Notification.emit(Debug.Subsystem.Application,
			      Debug.Level.Information,
			      str);
    }

    private string get_new_windowstate(Wnck.WindowState state) {
      StringBuilder sb = new StringBuilder();

      if((state & Wnck.WindowState.MINIMIZED) != 0)
	sb.append("MINIMIZED | ");
      if((state & Wnck.WindowState.MAXIMIZED_HORIZONTALLY) != 0)
	sb.append("MAXIMIZED_HORIZONTALLY | ");
      if((state & Wnck.WindowState.MAXIMIZED_VERTICALLY) != 0)
	sb.append("MAXIMIZED_VERTICALLY | ");
      if((state & Wnck.WindowState.SHADED) != 0)
	sb.append("SHADED | ");
      if((state & Wnck.WindowState.SKIP_PAGER) != 0)
	sb.append("SKIP_PAGER | ");
      if((state & Wnck.WindowState.SKIP_TASKLIST) != 0)
	sb.append("SKIP_TASKLIST | ");
      if((state & Wnck.WindowState.STICKY) != 0)
	sb.append("STICKY | ");
      if((state & Wnck.WindowState.HIDDEN) != 0)
	sb.append("HIDDEN | ");
      if((state & Wnck.WindowState.FULLSCREEN) != 0)
	sb.append("FULLSCREEN | ");
      if((state & Wnck.WindowState.DEMANDS_ATTENTION) != 0)
	sb.append("DEMANDS_ATTENTION | ");
      if((state & Wnck.WindowState.URGENT) != 0)
	sb.append("URGENT | ");
      if((state & Wnck.WindowState.ABOVE) != 0)
	sb.append("ABOVE | ");
      if((state & Wnck.WindowState.BELOW) != 0)
	sb.append("BELOW | ");

      sb.truncate(sb.len - 3);
      return(sb.str);
    }

    private void toggle_visibility() {
      _appVisible = !_appVisible;

      foreach(Wnck.Window w in _windows) {
	set_visibility_for_window(w, _appVisible);
      }
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
      msg.append_printf("Setting window 0x%08lx visibility to %s",
			w.get_xid(), _appVisible.to_string());

      if(set_visible) {
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
	w.state_changed += maintain_hiddenness;
      }

      Debug.Notification.emit(Debug.Subsystem.Application,
			      Debug.Level.Information,
			      msg.str);
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
      about.version = Build.PACKAGE_VERSION;
      about.website = "http://alltray.trausch.us/";
      about.copyright = "Copyright © "+Build.ALLTRAY_COPYRIGHT_YEARS;
      about.comments = "Dock applications in the system tray.";
      about.license = Build.ALLTRAY_LICENSE;

      about.response += dialog_destroy;

      about.show_all();
    }

    private void on_menu_undock() {
      Posix.exit(0);
    }
  }
}
