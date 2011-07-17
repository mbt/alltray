/* -*- mode: vala; c-basic-offset: 2; tab-width: 8; -*-
 * GtkStatusIcon.vala - GTK+ status icon widget and menu
 * Copyright (c) 2011 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation
 */
using GLib;
using Gtk;

namespace AllTray {
  public errordomain GtkStatusIconError {
    FAILED
  }

  public class GtkStatusIcon : StatusIcon {
    public signal void hide_all();
    public signal void show_all();
    public signal void toggle_app_visibility();
    public signal void toggle_window_visibility(Wnck.Window w);

    private AllTray.Application _app;

    public GtkStatusIcon(Application app, Gdk.Pixbuf pixbuf, string tooltip) {
      this._app = app;
      this.set_from_pixbuf(pixbuf);
      this.set_tooltip(tooltip);

      // Connect to signals from AllTray.Application that we need.
      this._app.icon_changed.connect(this.set_from_pixbuf);
      this.popup_menu.connect(this.on_popup_menu);
      this.activate.connect(this.on_activate);
    }

    public void set_tooltip(string tooltip) {
      base.set_tooltip_markup("<b>AllTray:</b> %s".printf(tooltip));
    }

    private void on_activate() {
      if(this.blinking == true) this.blinking = false;
      toggle_app_visibility();
    }

    private void on_popup_menu(uint button, uint activate_time) {
      Menu pm = new Gtk.Menu();

      MenuItem miToggle = new MenuItem.with_label(_("Toggle Visibility"));
      MenuItem miSep0 = new SeparatorMenuItem();
      MenuItem miShowAll = new MenuItem.with_label(_("Force Show All"));
      MenuItem miHideAll = new MenuItem.with_label(_("Force Hide All"));
      MenuItem miUndock = new MenuItem.with_label(_("Undock"));
      MenuItem miSep1 = new SeparatorMenuItem();
      MenuItem miAbout = new MenuItem.with_label(_("About AllTray..."));

      miToggle.set_submenu(create_window_list());

      pm.append(miToggle); miToggle.show();
      pm.append(miSep0); miSep0.show();
      pm.append(miShowAll); miShowAll.show();
      miShowAll.activate.connect(() => { show_all(); });
      pm.append(miHideAll); miHideAll.show();
      miHideAll.activate.connect(() => { hide_all(); });
      pm.append(miUndock); miUndock.show();
      miUndock.activate.connect(() => { show_all(); Posix.exit(0); });
      pm.append(miSep1); miSep1.show();
      pm.append(miAbout); miAbout.show();
      miAbout.activate.connect(() => { new AllTray.AboutDialog(); });

      pm.popup(null, null, null, button, activate_time);
    }

    private Menu create_window_list() {
      Menu ret = new Menu();
      MenuItem miAllWindows = new MenuItem.with_label(_("All Windows"));
      miAllWindows.activate.connect(() => {
	  toggle_app_visibility();
	});
      MenuItem miSep0 = new SeparatorMenuItem();

      ret.append(miAllWindows); miAllWindows.show();
      ret.append(miSep0); miSep0.show();

      unowned List<Wnck.Window> ws = this._app.wnck_app.get_windows();
      foreach(Wnck.Window w in ws) {
	MenuItem miW = new MenuItem.with_label(w.get_name());
	miW.set_data("target_window", w);
	miW.activate.connect((miW) => {
	    Wnck.Window mw =
	      (Wnck.Window)miW.get_data<Wnck.Window>("target_window");
	    toggle_window_visibility(mw);
	  });

	ret.append(miW); miW.show();
      }

      return(ret);
    }
  }
}