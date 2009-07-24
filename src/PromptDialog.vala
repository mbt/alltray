/* -*- mode: vala; c-basic-offset: 2; tab-width: 8; -*-
 * PromptDialog.vala - Dialog window to prompt the user to select an app
 * Copyright (c) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation
 */
using GLib;
using Gtk;

namespace AllTray {
  public class PromptDialog : Gtk.Dialog {
    private Label _lblPrompt;

    public PromptDialog() {
      this.title = _("Select an Application");
      this.has_separator = false;
      this.border_width = 5;
      //set_default_size(250, 50);
      create_widgets();
    }

    private void create_widgets() {
      string labelText
        = _("Click on a window of the software you wish to dock.\n\nNote: do not click on the window border itself. Click inside\nthe window; particularly on Compiz, clicking on the title bar\nor the other sides of the window border DOES NOT WORK.");

      _lblPrompt = new Label(labelText);
      this.vbox.pack_start(_lblPrompt, false, true, 0);
      show_all();
    }
  }
}
