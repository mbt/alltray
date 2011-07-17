/* -*- mode: vala; c-basic-offset: 2; tab-width: 8; -*-
 * Hotkey.vala - Support for hotkeys for hiding/showing applications
 * Copyright (c) 2011 Michael B. Trausch <mike@trausch.us>
 * License: GNU GPL v3.0 as published by the Free Software Foundation
 */
using GLib;
using Gtk;

namespace AllTray {
  public errordomain HotkeyError {
    NOT_IMPLEMENTED,
    INVALID,
    BAD_MODIFIER,
    BAD_KEY,
    FAILED
  }

  public class Hotkey : GLib.Object {
    [flags]
    private enum Modifier {
      NONE = 0x0000,
      SHIFT = 0x0001,
      CTRL = 0x0002,
      ALT = 0x0004,
      SUPER = 0x0008,
      ALTGR = 0x0010,
    }

    public signal void toggle_app_visibility();

    private AllTray.Application app;

    private Modifier modifier;
    private string key;
    private uint x11_keycode;

    public Hotkey(Application app, string hotkey) {
      this.app = app;
      this.modifier = Modifier.NONE;
      this.key = "";
      this.x11_keycode = 0;

      this.parse_hotkey(hotkey);
      this.install();
    }

    /**
     * Parses a hotkey specification.
     *
     * The format of a hotkey specification is Mod+Mod 2+Key; that is,
     * the keys are "+" delimited.  Leading and trailing space is
     * acceptable (because it is removed), and case doesn't matter.
     *
     * Supported modifiers are (with recognized aliases):
     *
     *  - "shift"
     *  - "ctrl" (or "control")
     *  - "alt"
     *  - "altgr" (or "alt gr" or "alt graph" or "altgraph")
     *  - "super" (or "windows" or "command" or "meta")
     *
     * The key itself must be a single letter, number, or punctuation
     * key, or one of Fx (where x is 1 through 12).  As a special
     * case, the modifiers Control+Alt cannot be used with an Fx key
     * (because on most X11 systems, this will take the user out of
     * X11 and put them in a terminal; so it's just better to never
     * allow the hotkey to be set to that at all).
     */
    private void parse_hotkey(string hotkey) {
      string[] keys = hotkey.split("+");
      int parts = keys.length;

      if(parts == 0) {
	throw new HotkeyError.INVALID("no hotkey specified");
      } else if(parts == 1) {
	if(this.is_fkey(keys[0])) {
	  this.modifier = Modifier.NONE;
	  this.key = keys[0];
	  return;
	} else {
	  throw new HotkeyError.INVALID("invalid hotkey specified");
	}
      } else {
	foreach(string key in keys) {
	  if(this.is_modifier(key)) {
	    this.add_modifier(key);
	  } else if(this.is_valid_key(key)) {
	    this.set_key(key);
	  } else {
	    throw new HotkeyError.INVALID("invalid hotkey specified");
	  }
	}
      }
    }

    private void install() {
      unowned Gdk.Display gdk_dpy = Gdk.Display.get_default();
      unowned X.Display x_dpy =
        Gdk.x11_display_get_xdisplay(Gdk.Display.get_default());
      unowned Gdk.Window gdk_root = Gdk.get_default_root_window();

      int keycode = x_dpy.keysym_to_keycode(MX.string_to_keysym(this.key));
      uint modifiers = this.get_x11_modifiers();
      X.Window grab_window = Gdk.x11_get_default_root_xwindow();
      bool owner_events = true;
      int pointer_mode = X.GrabMode.Async;
      int keyboard_mode = X.GrabMode.Async;

      // Grab the key combination four times...
      var ret = x_dpy.grab_key(keycode, modifiers, grab_window, owner_events,
			       pointer_mode, keyboard_mode); // Normal
      ret = x_dpy.grab_key(keycode, 2|modifiers, grab_window, owner_events,
			   pointer_mode, keyboard_mode); // w/ Caps Lock
      ret = x_dpy.grab_key(keycode, 16|modifiers, grab_window, owner_events,
			   pointer_mode, keyboard_mode); // w/ Num Lock
      ret = x_dpy.grab_key(keycode, 18|modifiers, grab_window, owner_events,
			   pointer_mode, keyboard_mode); // w/ Both
      gdk_root.add_filter(filter);
    }

    private Gdk.FilterReturn filter(Gdk.XEvent xevent, Gdk.Event event) {
      unowned X.Event* xev = (X.Event*)(&xevent);

      if(xev->type == X.EventType.KeyRelease) {
	toggle_app_visibility();
      }

      return(Gdk.FilterReturn.CONTINUE);
    }

    private void add_modifier(string key) {
      string k = key.chug().down();
      switch(k) {
      case "shift":
	this.modifier |= Modifier.SHIFT;
        break;

      case "ctrl":
      case "control":
	this.modifier |= Modifier.CTRL;
        break;

      case "alt":
	this.modifier |= Modifier.ALT;
	break;

      case "altgr":
      case "alt gr":
      case "alt graph":
      case "altgraph":
	this.modifier |= Modifier.ALTGR;
        break;

      case "super":
      case "windows":
      case "command":
      case "meta":
	this.modifier |= Modifier.SUPER;
        break;

      default:
	throw new HotkeyError.BAD_MODIFIER("bad modifier passed to add_modifier");
      }

    }

    private void set_key(string key) {
      string k = key.chug().up();
      if(this.is_valid_key(k))
	this.key = k;
      else
	throw new HotkeyError.BAD_KEY("bad key passed to set_key");
    }

    private uint get_x11_modifiers() {
      uint ret = 0; //(1 << 1 /* Numlock? */);

      if((this.modifier & Modifier.SHIFT) != 0) {
	ret = ret | (1 << 0 /* ShiftMak */);
      }

      if((this.modifier & Modifier.CTRL) != 0) {
        ret = ret | (1 << 2 /* ControlMask */);
      }

      if((this.modifier & Modifier.ALT) != 0) {
	ret = ret | (1 << 3 /* Mod1Mask */);
      }

      if((this.modifier & Modifier.SUPER) != 0) {
	ret = ret | (1 << 6 /* Mod4Mask */);
      }

      if((this.modifier & Modifier.ALTGR) != 0) {
	ret = ret | (1 << 7 /* Mod5Mask */);
      }

      if(ret == 0) {
	ret = (1 << 15 /* AnyModifier */);
      }

      return(ret);
    }

    private bool is_fkey(string key) {
      string k = key.chug().down();
      switch(k) {
      case "f1":
      case "f2":
      case "f3":
      case "f4":
      case "f5":
      case "f6":
      case "f7":
      case "f8":
      case "f9":
      case "f10":
      case "f11":
      case "f12":
	return(true);
      default:
	return(false);
      }
    }

    private bool is_modifier(string key) {
      string k = key.chug().down();
      switch(k) {
      case "shift":
      case "ctrl":
      case "control":
      case "alt":
      case "altgr":
      case "alt gr":
      case "alt graph":
      case "altgraph":
      case "super":
      case "windows":
      case "command":
      case "meta":
	return(true);
      default:
	return(false);
      }
    }

    private bool is_valid_key(string key) {
      if(this.is_fkey(key)) return(true);
      string k = key.chug().down();

      switch(k) {
      case "a":
      case "b":
      case "c":
      case "d":
      case "e":
      case "f":
      case "g":
      case "h":
      case "i":
      case "j":
      case "k":
      case "l":
      case "m":
      case "n":
      case "o":
      case "p":
      case "q":
      case "r":
      case "s":
      case "t":
      case "u":
      case "v":
      case "w":
      case "x":
      case "y":
      case "z":
      case "0":
      case "1":
      case "2":
      case "3":
      case "4":
      case "5":
      case "6":
      case "7":
      case "8":
      case "9":
      case "`":
      case "-":
      case "=":
      case "[":
      case "]":
      case "\\":
      case ";":
      case "'":
      case "/":
      case ".":
      case ",":
	return(true);
      default:
	return(false);
      }
    }

    public void register_hotkey(string hotkey) {
      throw new HotkeyError.NOT_IMPLEMENTED("Hotkey support is not yet implemented");
    }
  }
}