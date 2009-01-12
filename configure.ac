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

# pkg-config checks need to be inserted here.

# Configuration wrap-up.
AC_CONFIG_FILES([Makefile
        data/Makefile
        man/Makefile
        lib/Makefile
        src/Makefile])

AC_OUTPUT
