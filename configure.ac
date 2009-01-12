#
# This file needs to be processed with autoconf to produce ./configure.
#
# The preferred way to do this is to use the project-local autogen.sh
# script.
#

# Initialize autoconf
AC_INIT([AllTray], [0.7.1dev], [https://launchpad.net/alltray/+filebug])

# Initialize automake
AM_INIT_AUTOMAKE(check-news -Wall)

# Minimum required version of autoconf
AC_PREREQ(2.60)

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([src/main.c])

# Check for programs necessary to build and install AllTray.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL

# Library checks.
AC_CHECK_LIB([dl], [dlopen],
             printf "yes", printf "\nError: libdl required."; exit)

# pkg-config checks need to be inserted here.
PKG_CHECK_MODULES(X11, x11, [have_x11=yes], [have_x11=no])
PKG_CHECK_MODULES(GLIB, glib-2.0, [have_glib=yes], [have_glib=no])
PKG_CHECK_MODULES(GTK, gtk+-2.0 > 2.2.0, [have_gtk=yes], [have_gtk=no])
PKG_CHECK_MODULES(GDK, gdk-2.0, [have_gdk=yes], [have_gdk=no])
PKG_CHECK_MODULES(GCONF, gconf-2.0, [have_gconf=yes], [have_gconf=no])
PKG_CHECK_MODULES(GDKPIXBUF, gdk-pixbuf-xlib-2.0, [have_gdkpixbuf=yes],
                             [have_gdk_pixbuf=no])

if test x"$have_x11" = xno; then
   printf "error: X11 is required\n"
   exit 1
fi

if test x"$have_glib" = xno; then
   printf "error: glib is required\n"
   exit 1
fi

if test x"$have_gtk" = xno; then
   printf "error: gtk 2.2.0 or later is required\n"
   exit 1
fi

if test x"$have_gdk" = xno; then
   printf "error: gdk is required\n"
   exit 1
fi

if test x"$have_gdkpixbuf" = xno; then
   printf "error: gdkpixbuf is required\n"
   exit 1
fi

if test x"$have_gconf" = xno; then
   printf "error: gconf is required\n"
   exit 1
fi

AC_SUBST(X11_CFLAGS)
AC_SUBST(X11_LIBS)
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)
AC_SUBST(GDK_CFLAGS)
AC_SUBST(GDK_LIBS)
AC_SUBST(GDKPIXBUF_CFLAGS)
AC_SUBST(GDKPIXBUF_LIBS)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)
AC_SUBST(GCONF_CFLAGS)
AC_SUBST(GCONF_LIBS)

# Configuration wrap-up.
AC_CONFIG_FILES([Makefile
        data/Makefile
        images/Makefile
        man/Makefile
        lib/Makefile
        src/Makefile])

AC_OUTPUT
