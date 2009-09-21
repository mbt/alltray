[CCode(cheader_filename = "gtop_glue.h")]
namespace AllTray {
  [CCode(cname = "alltray_get_ppid_for",
	 cheader_filename = "gtop_glue.h")]
  public int get_ppid_for(int pid);

  [CCode(cname = "alltray_get_processes_in_pgrp"
	 cheader_filename = "gtop_glue.h")]
  public void get_pids_in_pgid(int pgid, out int[] procs);

  [CCode(cname = "alltray_get_process_name"
	 cheader_filename = "gtop_glue.h")]
  public string get_process_name_for(int pid);
}
