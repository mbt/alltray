[CCode(cheader_filename = "x11_glue.h")]
namespace AllTray {
	[CCode(cname = "alltray_get_x11_window_from_pid")]
	public ulong c_xwin_from_pid(GLib.Pid pid, void *Display);

	[CCode(cname = "alltray_find_managed_window")]
	public unowned Wnck.Window c_find_managed_window(ulong xwin, Gdk.Display d);
}