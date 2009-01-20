#
# This file needs to be processed with autoconf to produce ./configure.
#
# The preferred way to do this is to use the project-local autogen.sh
# script.
#

# Initialize autoconf
AC_INIT([AllTray], [0.7.1dev], [https://launchpad.net/alltray/+filebug])

# Initialize automake
AM_INIT_AUTOMAKE(foreign check-news -Wall)

# Minimum required version of autoconf
AC_PREREQ(2.60)

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([src/main.c])

# Check for programs necessary to build and install AllTray.
AC_PROG_CC
AC_PROG_SED

# Check for ISO C99 support in the C compiler.
AC_MSG_CHECKING([for c99 support in gcc])

old_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS -std=gnu99"
AC_COMPILE_IFELSE([dnl
    int foo(int a) { for(int i = 0; i < a; i++) if (i % 4) break; int s = a; return(s); }],
    ac_cv_c99=yes, ac_cv_c99=no)
CFLAGS="$old_CFLAGS"
if test x"$ac_cv_c99" != xyes; then
   AC_MSG_RESULT(no)
   AC_MSG_ERROR([gcc with c99 support is required])
else
   AC_MSG_RESULT(yes)
   CFLAGS="$CFLAGS -std=gnu99"
fi

AC_MSG_CHECKING([for warn_unused_result support])
AC_COMPILE_IFELSE([dnl
  int foo(int a) __attribute__((warn_unused_result));
  int foo(int a) {
    return(a);
  } ],
  AC_MSG_RESULT(yes)
  AC_DEFINE([WARN_UNUSED_RESULT], [__attribute__((warn_unused_result))],
            [Hint to GCC to warn if the result of a function is unused.]),
  AC_MSG_RESULT(no)
  AC_DEFINE([WARN_UNUSED_RESULT], [], []))

AC_PROG_INSTALL
AC_PROG_LIBTOOL

# Library checks.
# AC_CHECK_LIB([dl], [dlopen],,libdl_external=no)
AC_SEARCH_LIBS([dlopen], [dl],,AC_MSG_ERROR([dlopen() is missing]))

# pkg-config checks need to be inserted here.
PKG_CHECK_MODULES(X11, x11,, AC_MSG_ERROR([X11 is required]))
PKG_CHECK_MODULES(GLIB, glib-2.0,, AC_MSG_ERROR([glib-2.0 is required]))

AC_SUBST(X11_CFLAGS)
AC_SUBST(X11_LIBS)
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)
AC_DEFINE_UNQUOTED(ALLTRAY_COMPILE_FLAGS, "${CFLAGS}", [CFLAGS])
AC_DEFINE_UNQUOTED(ALLTRAY_COMPILE_COMPILER, "$(${CC} --version|head -1)",
  [Compiler])
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

# Configuration wrap-up.
AC_CONFIG_FILES([README
        Makefile
        data/Makefile
        images/Makefile
        man/Makefile
        lib/Makefile
        src/Makefile
        man/alltray.1])

AC_OUTPUT

# Configuration results report
printf "\nConfiguration results (%s %s)\n\n" "$PACKAGE_NAME" "$PACKAGE_VERSION"

printf "C Compiler:\t%s\n" "$CC"
printf "C Flags:\t%s\n" "$CFLAGS"
printf "Install path:\t%s\n" "$prefix"
printf "\n"

