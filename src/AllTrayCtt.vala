/* -*- mode: vala; c-basic-offset: 2; tab-width: 8; -*-
 * AllTrayCtt.vala - CTT support for AllTray
 * Copyright (c) 2009 Michael B. Trausch <mike@trausch.us>
 */
using GLib;

namespace AllTray {
  public errordomain AllTrayCttError {
    SPAWN_HELPER_FAILED,
    CTT_ATTACH_FAILED,
    CTT_DETACH_FAILED,
    STATUS_FAILED,
    FAILED
  }
}