[CCode(cprefix = "", lower_case_prefix = "", 
	   cheader_filename = "unix_process.h")]
namespace AllTray {
	[Compact]
	[CCode(cprefix = "", lower_case_prefix = "", cname = "process_info_t",
		   free_func = "free", cheader_filename = "unix_process.h")]
	public class ProcessInfo {
		public Posix.pid_t pid;
		public Posix.pid_t ppid;
		public string name;

		[CCode(cname = "get_process_info")]
		public ProcessInfo(Posix.pid_t pid);
	}
}