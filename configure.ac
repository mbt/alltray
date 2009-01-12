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

# Library checks.
AC_CHECK_LIB([dl], [dlopen],
             printf "yes", printf "\nError: libdl required."; exit)

# pkg-config checks need to be inserted here.
PKG_CHECK_MODULES(X11, x11, [have_x11=yes], [have_x11=no])
PKG_CHECK_MODULES(GLIB, glib-2.0, [have_glib=yes], [have_glib=no])

if test x"$have_x11" = xno; then
   printf "error: X11 is required\n"
   exit 1
fi

if test x"$have_glib" = xno; then
   printf "error: glib is required\n"
   exit 1
fi

AC_SUBST(X11_CFLAGS)
AC_SUBST(X11_LIBS)
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

# Configuration wrap-up.
AC_CONFIG_FILES([Makefile
        data/Makefile
        man/Makefile
        lib/Makefile
        src/Makefile])

AC_OUTPUT
