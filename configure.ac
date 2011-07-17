#
# This file needs to be processed with autoconf to produce ./configure.
#
# The preferred way to do this is to use the project-local autogen.sh
# script.
#

# Initialize autoconf
AC_INIT([AllTray], [0.7.5dev+], [https://launchpad.net/alltray/+filebug])

# Initialize automake
AM_INIT_AUTOMAKE(foreign check-news -Wall)

# Minimum required version of autoconf
AC_PREREQ(2.60)

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([src/AllTray.vala])

# Check for programs necessary to build and install AllTray.
AC_PROG_CC
AM_PROG_CC_C_O
AC_GNU_SOURCE
AM_GNU_GETTEXT
AC_PROG_SED
AC_PATH_PROG(VALAC, valac, valac)
AC_PROG_INSTALL

# pkg-config checks need to be inserted here.
PKG_CHECK_MODULES(X11, x11,, AC_MSG_ERROR([X11 is required]))
PKG_CHECK_MODULES(GLIB, glib-2.0,,AC_MSG_ERROR([glib-2.0 is required]))
PKG_CHECK_MODULES(GOBJECT, gobject-2.0,,AC_MSG_ERROR([gobject-2.0 is required]))
PKG_CHECK_MODULES(GTOP, libgtop-2.0,,AC_MSG_ERROR([libgtop-2.0 is required]))
PKG_CHECK_MODULES(WNCK, libwnck-1.0,,AC_MSG_ERROR([libwnck-1.0 is required]))
PKG_CHECK_MODULES(GTK, gtk+-2.0,,AC_MSG_ERROR([gtk+-2.0 is required]))

# XXX: Only required for Close-To-Tray. Should be optional.
PKG_CHECK_MODULES(XPM, xpm,, AC_MSG_ERROR([xpm is required]))
PKG_CHECK_MODULES(XEXT, xext,, AC_MSG_ERROR([xext is required]))

AC_SUBST(X11_CFLAGS)
AC_SUBST(X11_LIBS)
AC_SUBST(XPM_CFLAGS)
AC_SUBST(XPM_LIBS)
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)
AC_SUBST(GOBJECT_CFLAGS)
AC_SUBST(GOBJECT_LIBS)
AC_SUBST(GTOP_CFLAGS)
AC_SUBST(GTOP_LIBS)
AC_SUBST(WNCK_CFLAGS)
AC_SUBST(WNCK_LIBS)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

# XXX: Only required for Close-To-Tray. Should be optional.
AC_SUBST(XEXT_CFLAGS)
AC_SUBST(XEXT_LIBS)

AC_SUBST(pkglib)
AC_SUBST(VALAC)

GETTEXT_PACKAGE=${PACKAGE}
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "${GETTEXT_PACKAGE}", [GETTEXT_PACKAGE])

AC_DEFINE_UNQUOTED(ALLTRAY_COMPILE_FLAGS, "${CFLAGS}", [CFLAGS])
AC_DEFINE_UNQUOTED(ALLTRAY_C_COMPILER, "$(${CC} --version|head -1)", [Compiler])

if test -f $VALAC -a -x $VALAC; then
 AC_DEFINE_UNQUOTED(ALLTRAY_VALA_COMPILER, "$($VALAC --version)", [Vala ver])
else
 AC_DEFINE_UNQUOTED(ALLTRAY_VALA_COMPILER, "Not installed", [Vala ver])
fi

AC_DEFINE_UNQUOTED(ALLTRAY_COMPILE_OS, "$(uname -s)",
  [Build OS Kernel and Name])
AC_DEFINE_UNQUOTED(ALLTRAY_COMPILE_OS_REL, "$(uname -r)", [Build OS Release])
AC_DEFINE_UNQUOTED(ALLTRAY_COMPILE_OS_VER, "$(uname -v)",
  [Build OS Version])
AC_DEFINE_UNQUOTED(ALLTRAY_COMPILE_OS_HOST, "$(hostname -f)",
  [Build OS hostname])
AC_DEFINE_UNQUOTED(ALLTRAY_COMPILE_BUILD_DATE, "$(TZ=UTC date)",
  [Build Date (UTC)])
AC_DEFINE_UNQUOTED(ALLTRAY_COMPILE_BUILD_DATE_EUS,
  "$(TZ=EST5EDT date)", [Build Date (Author's Timezone)])
AC_DEFINE_UNQUOTED(ALLTRAY_CONFIGURE_FLAGS, "$ac_configure_args", [Conf flags])
AC_DEFINE_UNQUOTED(ALLTRAY_COPYRIGHT_YEARS, ["2009, 2010"], [Copyright years])

if test -d .bzr; then
    BZR_BRANCH_NAME="$(bzr version-info|grep ^branch-nick|cut -f2 -d' ')"
    BZR_REV_ID="$(bzr version-info|grep ^revision-id|cut -f2 -d' ')"
    BZR_REVISION="$(bzr version-info|grep ^revno|cut -f2 -d' ')"

    AC_DEFINE_UNQUOTED(ALLTRAY_BZR_BUILD, "TRUE", [Set if built from bzr tree])
    AC_DEFINE_UNQUOTED(ALLTRAY_BZR_BRANCH,
      "$BZR_BRANCH_NAME", [Branch this was built from])
    AC_DEFINE_UNQUOTED(ALLTRAY_BZR_REVID,
      "$BZR_REV_ID", [Revision ID we are built from])
    AC_DEFINE_UNQUOTED(ALLTRAY_BZR_REVISION,
      "$BZR_REVISION", [Revision number we are built from])
else
    AC_DEFINE_UNQUOTED(ALLTRAY_BZR_BUILD, "FALSE", [TRUE if built from branch])
    AC_DEFINE_UNQUOTED(ALLTRAY_BZR_BRANCH,
      "$BZR_BRANCH_NAME", [Branch this was built from])
    AC_DEFINE_UNQUOTED(ALLTRAY_BZR_REVID,
      "$BZR_REV_ID", [Revision ID we are built from])
    AC_DEFINE_UNQUOTED(ALLTRAY_BZR_REVISION,
      "BZR_REVISION", [Revision number we are built from])
fi

AC_DEFINE_UNQUOTED(ALLTRAY_LICENSE, "$(cat COPYING|tr '\n' '|'|sed 's,|,\\n,g;s,",\\",g')", [The GNU GPL License Text])

# Configuration wrap-up.
AC_CONFIG_FILES([README
        Makefile
        images/Makefile
	data/alltray.desktop
	data/Makefile
	intl/Makefile
        man/Makefile
	po/Makefile.in
        src/Makefile
	vapi/Makefile
        man/alltray.1])
AC_OUTPUT

# Configuration results report
printf "\nConfiguration results (%s %s)\n\n" "$PACKAGE_NAME" "$PACKAGE_VERSION"

printf "C Compiler:\t%s\n" "$CC"

if [[ -f $VALAC -a -x $VALAC ]]; then
  printf "Vala Compiler:\t%s\n" "$(${VALAC} --version)"
else
  printf "Vala Compiler:\tNot installed.\n"
fi

printf "%s version:\t%s\n" "$CC" "$(${CC} --version|head -1)"
printf "C Flags:\t%s\n" "$CFLAGS"
printf "Install path:\t%s\n" "$prefix"
printf "bzr info:\t%s %s\n" "$BZR_BRANCH_NAME" "$BZR_REV_ID"
printf "\n"

