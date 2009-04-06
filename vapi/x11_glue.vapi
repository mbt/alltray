namespace AllTray {
	[CCode(cheader_filename = "x11_glue.h",
		   cname = "alltray_get_x11_window_from_pid")]
	public ulong c_xwin_from_pid(GLib.Pid pid, void *Display);
}