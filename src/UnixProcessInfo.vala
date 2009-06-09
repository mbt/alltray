/*
 * UnixProcessInfo.vala - Get information on processes running on the system.
 * Copyright (C) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation
 */
using GLib;
using Glibtop;

namespace AllTray {
	public class UnixProcessInfo {
		private int _pid;
		private UnixProcessInfo? _parent;
		private string _name;
		private unowned Glibtop.glibtop _glibtop_handler;

		public int pid {
			get {
				return(this._pid);
			}
		}

		public int ppid {
			get {
				if(_parent != null) {
					return(_parent.pid);
				} else {
					return(0);
				}
			}
		}

		public string name {
			get {
				return(this._name);
			}
		}

		public UnixProcessInfo(int pid) {
			_glibtop_handler = glibtop.init();
		}
	}
}
