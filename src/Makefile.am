# Automake file for AllTray

bin_PROGRAMS = alltray
AM_CFLAGS = @X11_CFLAGS@ @GLIB_CFLAGS@ @GOBJECT_CFLAGS@
alltray_LDADD = @X11_LIBS@ @GLIB_LIBS@ @GOBJECT_LIBS@

AM_CFLAGS += -DALLTRAY_HELPER_DIRECTORY=\"$(pkglib)\"

alltray_SOURCES = alltray.h \
	common.c \
	cmdline.c \
	debug.c \
	global.c \
	main.c \
	process.c \
	systray.c \
	wm.c \
	x11.c \
	x11-error.c
