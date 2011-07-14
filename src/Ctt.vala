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

  public class Ctt {
    private Pid _child_pid;
    private int _child_stdin_fd;
    private FileStream _child_stdin;

    public Ctt() {
      SpawnFlags child_flags = (SpawnFlags.SEARCH_PATH |
				SpawnFlags.DO_NOT_REAP_CHILD);

      try {
	GLib.Process.spawn_async_with_pipes(null,
					    { "alltray-ctt-helper" },
					    null,
					    child_flags,
					    null,
					    out this._child_pid,
					    out this._child_stdin_fd,
					    null,
					    null);
      } catch (SpawnError se) {
	try {
	  child_flags = (SpawnFlags.DO_NOT_REAP_CHILD);
	  GLib.Process.spawn_async_with_pipes(null,
					      { "alltray-ctt-helper" },
					      null,
					      child_flags,
					      null,
					      out this._child_pid,
					      out this._child_stdin_fd,
					      null,
					      null);
	} catch(SpawnError se) {
	  stderr.printf("Failed to spawn helper process\n");
	}
      }

      this._child_stdin = FileStream.fdopen(this._child_stdin_fd, "w");
      ChildWatch.add(this._child_pid, this._helper_died);
    }

    public void attach(ulong window_id) {
      this._child_stdin.printf("ATTACH %lu\n", window_id);
    }

    public void detach(ulong window_id) {
      this._child_stdin.printf("DETACH %lu\n", window_id);
    }

    private void _helper_died(Pid p, int return_status) {
      stderr.printf("Helper child %d died with status %d\n", p, return_status);
    }
  }
}