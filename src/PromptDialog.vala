/*
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
			this.title = "Select an Application";
			this.has_separator = false;
			this.border_width = 5;
			//set_default_size(250, 50);
			create_widgets();
		}

		private void create_widgets() {
			_lblPrompt = new Label("Click on a window of the application you "+
								   "want to dock.");
			this.vbox.pack_start(_lblPrompt, false, true, 0);
			show_all();
		}
	}
}