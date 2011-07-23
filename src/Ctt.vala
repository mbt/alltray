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
    private int _child_stdout_fd;
    private FileStream _child_stdin;
    private IOChannel _child_stdout_channel;

    internal signal void activate(ulong XID);

    public Ctt() {
      Debug.Notification.emit(Debug.Subsystem.Ctt, Debug.Level.Information,
		 "in Ctt() constructor");
      SpawnFlags child_flags = (SpawnFlags.SEARCH_PATH |
				SpawnFlags.DO_NOT_REAP_CHILD);

      try {
	Debug.Notification.emit(Debug.Subsystem.Ctt, Debug.Level.Information,
		   "spawning alltray-ctt-helper (using $PATH)");
	GLib.Process.spawn_async_with_pipes(null,
					    { "alltray-ctt-helper" },
					    null,
					    child_flags,
					    null,
					    out this._child_pid,
					    out this._child_stdin_fd,
					    out this._child_stdout_fd,
					    null);
      } catch (SpawnError se) {
	try {
	  Debug.Notification.emit(Debug.Subsystem.Ctt, Debug.Level.Information,
		     "spawning alltray-ctt-helper (not using $PATH)");
	  child_flags = (SpawnFlags.DO_NOT_REAP_CHILD);
	  GLib.Process.spawn_async_with_pipes(null,
					      { "alltray-ctt-helper" },
					      null,
					      child_flags,
					      null,
					      out this._child_pid,
					      out this._child_stdin_fd,
					      out this._child_stdout_fd,
					      null);
	} catch(SpawnError se) {
	  Debug.Notification.emit(Debug.Subsystem.Ctt, Debug.Level.Error,
		     "Failed to spawn CTT helper process, CTT disabled");
	  stderr.printf("CTT unavailable: failed to spawn helper process\n");
	  Program._ctt_enabled = false;
	}
      }

      Debug.Notification.emit(Debug.Subsystem.Ctt, Debug.Level.Information,
			      "CTT subprocess pid = %d".printf(_child_pid));

      Debug.Notification.emit(Debug.Subsystem.Ctt, Debug.Level.Information,
		 "opening input IOChannel from CTT helper");
      this._child_stdout_channel =
        new IOChannel.unix_new(this._child_stdout_fd);
      this._child_stdout_channel.add_watch(IOCondition.IN | IOCondition.HUP,
					   this._read_from_helper);

      Debug.Notification.emit(Debug.Subsystem.Ctt, Debug.Level.Information,
		 "opening write pipe to CTT helper");
      this._child_stdin = FileStream.fdopen(this._child_stdin_fd, "w");

      ChildWatch.add(this._child_pid, this._helper_died);
    }

    private bool _read_from_helper(IOChannel ch, IOCondition cond) {
      // A reply, or a CTT message.  For now, throw away non-CTT messages.
      string line;
      size_t line_len;
      size_t terminator_pos;
      IOStatus status;

      if(cond == IOCondition.HUP) {
	Debug.Notification.emit(Debug.Subsystem.Ctt, Debug.Level.Error,
				"helper pipe has been hung up on");
	return(false);
      }

      Debug.Notification.emit(Debug.Subsystem.Ctt, Debug.Level.Information,
			      "reading from CTT");

      try {
	status = ch.read_line(out line, out line_len, out terminator_pos);
      } catch(ConvertError e) {
	Debug.Notification.emit(Debug.Subsystem.Ctt, Debug.Level.Error,
				"read from helper failed: convert error");
	return(false);
      } catch(IOChannelError e) {
	Debug.Notification.emit(Debug.Subsystem.Ctt, Debug.Level.Error,
				"read from helper failed: I/O error");
	return(false);
      }
      if(status != IOStatus.NORMAL) {
	Debug.Notification.emit(Debug.Subsystem.Ctt, Debug.Level.Warning,
				"read from CTT not NORMAL IOStatus, removing");
	return(false);
      }

      Debug.Notification.emit(Debug.Subsystem.Ctt, Debug.Level.Information,
			      "read '%s' from CTT".printf(line.chomp()));

      string[] parts = line.chomp().split(" ", 2);
      Debug.Notification.emit(Debug.Subsystem.Ctt, Debug.Level.Information,
			      "Parts: [%s] [%s]".printf(parts[0], parts[1]));

      if(parts[0] == "CTT") {
	ulong XID = parts[1].to_ulong();
	activate(XID);
      }
      return(true);
    }

    public void attach(ulong window_id) {
      Debug.Notification.emit(Debug.Subsystem.Ctt, Debug.Level.Information,
		 "attach called for XID 0x%lx".printf(window_id));
      this._child_stdin.printf("ATTACH %lu\n", window_id);
      this._child_stdin.flush();
    }

    public void detach(ulong window_id) {
      Debug.Notification.emit(Debug.Subsystem.Ctt, Debug.Level.Information,
		 "detach() called for XID 0x%lx".printf(window_id));
      this._child_stdin.printf("DETACH %lu\n", window_id);
      this._child_stdin.flush();
    }

    private void _helper_died(Pid p, int return_status) {
      Debug.Notification.emit(Debug.Subsystem.Ctt, Debug.Level.Information,
			      "helper died");
      stderr.printf("Helper child %d died with status %d\n", p, return_status);
    }
  }
}