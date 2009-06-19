/*
 * GtkIconChange.vala - provide a window that changes window icons.
 * Copyright (C) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation
 */
using GLib;
using Posix;

namespace AllTray.Test {
	public class Test : Object {
		Gtk.Window w;
		Gtk.Button b;

		public static int main(string[] args) {
			bool status = Gtk.init_check(ref args);
			if(!status) {
				stderr.printf("Oops, cannot run.\n");
				exit(1);
			}

			Test obj = new Test();
			return(0);
		}

		public Test() {
			w = new Gtk.Window(Gtk.WindowType.TOPLEVEL);
			w.set_default_size(300, 50);
			w.title = "Window Icon Test";
			b = new Gtk.Button.with_label("Click to change window icon");

			b.clicked += (source) => {
				string[] iconNames = {
					"1.png",
					"2.png"
				};

				w.set_urgency_hint(!w.get_urgency_hint());
				int which = (w.get_urgency_hint() ? 0 : 1);
				w.set_icon_from_file(iconNames[which]);
			};

			b.destroy += Gtk.main_quit;

			w.add(b);
			w.show_all();
			Gtk.main();
		}
	}
}
