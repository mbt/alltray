/*
 * stdc.api - Inclusions of some standard C functions/API/functionality
 * Copyright (C) 2009 Michael B. Trausch <mike@trausch.us>
 * License: GNU LGPL v3 as published by the Free Software Foundation
 */

[CCode(cprefix = "", lower_case_cprefix = "")]
namespace Native.StdC {
	[CCode(cheader_filename = "stdlib.h", cprefix = "",
		   lower_case_cprefix = "")]
	namespace Stdlib {
		public void exit(int program_retval);
		public void abort();
	}

	[CCode(cheader_filename = "signal.h", cprefix = "",
		   lower_case_cprefix = "")]
	namespace Signal {
		public const int SIGHUP;
		public const int SIGINT;
		public const int SIGQUIT;
		public const int SIGILL;
		public const int SIGTRAP;
		public const int SIGABRT;
		public const int SIGIOT;
		public const int SIGBUS;
		public const int SIGFPE;
		public const int SIGKILL;
		public const int SIGUSR1;
		public const int SIGSEGV;
		public const int SIGUSR2;
		public const int SIGPIPE;
		public const int SIGALRM;
		public const int SIGTERM;
		public const int SIGSTKFLT;
		public const int SIGCHLD;
		public const int SIGCONT;
		public const int SIGSTOP;
		public const int SIGTSTP;
		public const int SIGTTIN;
		public const int SIGTTOU;
		public const int SIGURG;
		public const int SIGXCPU;
		public const int SIGXFSZ;
		public const int SIGVTALRM;
		public const int SIGPROF;
		public const int SIGWINCH;
		public const int SIGIO;
		public const int SIGPWR;
		public const int SIGSYS;

		public static delegate void SignalHandler(int whichSignal);

		[CCode(cname = "signal")]
		public static SignalHandler set_new_handler(int sig,
													SignalHandler handler);
	}
}
