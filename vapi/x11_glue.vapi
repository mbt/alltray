namespace AllTray {
  [CCode(cname = "alltray_get_x11_window_from_pid",
	 cheader_filename = "x11_glue.h")]
  public ulong c_xwin_from_pid(GLib.Pid pid, void *Display);

  [CCode(cname = "alltray_find_managed_window",
	 cheader_filename = "x11_glue.h")]
  public unowned Wnck.Window c_find_managed_window(ulong xwin, Gdk.Display d);
}

[CCode (cprefix = "", lower_case_cprefix = "",
		cheader_filename = "X11/Xlib.h,X11/Xatom.h,X11/Xutil.h,X11/Xregion.h")]
namespace MX {
  [CCode (cname = XStringToKeysym)]
  public uint string_to_keysym(string str);
}