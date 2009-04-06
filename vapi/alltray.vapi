/*
 * alltray.vapi - Imports information into Vala from the build system.
 */
[CCode(cheader_filename = "config.h",
	   cprefix="", lower_case_cprefix = "")]
namespace AllTray.Build {
	public const string PACKAGE_VERSION;
}