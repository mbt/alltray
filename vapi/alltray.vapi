/*
 * alltray.vapi - Imports information into Vala from the build system.
 */
[CCode(cheader_filename = "config.h",
	   cprefix="", lower_case_cprefix = "")]
namespace AllTray.Build {
	public const string PACKAGE_VERSION;
	public const string ALLTRAY_COPYRIGHT_YEARS;

	public const string ALLTRAY_BZR_BUILD;
	public const string ALLTRAY_BZR_BRANCH;
	public const string ALLTRAY_BZR_REVID;

	public const string ALLTRAY_C_COMPILER;
	public const string ALLTRAY_COMPILE_FLAGS;
	public const string ALLTRAY_COMPILE_OS;
	public const string ALLTRAY_COMPILE_OS_REL;
	public const string ALLTRAY_COMPILE_OS_VER;
	public const string ALLTRAY_COMPILE_OS_HOST;
	public const string ALLTRAY_COMPILE_BUILD_DATE;
	public const string ALLTRAY_COMPILE_BUILD_DATE_EUS;

	public const string ALLTRAY_VALA_COMPILER;

	public const string ALLTRAY_CONFIGURE_FLAGS;
}
