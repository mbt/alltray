using Posix;

[CCode(cheader_filename = "unix_process.h")]
namespace AllTray {
	[Compact]
	[CCode(cname = "process_info_t", free_func = "free")]
	public class ProcessInfo {
		public pid_t pid;
		public pid_t ppid;
		public string name;

		[CCode(cname = "get_process_info")]
		public ProcessInfo(pid_t pid);
	}
}