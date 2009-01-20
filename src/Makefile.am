# Automake file for AllTray

bin_PROGRAMS = alltray
AM_CFLAGS = @X11_CFLAGS@ @GLIB_CFLAGS@ \
	-DPACKAGE_LIB_DIR=\""$(libdir)"\"
alltray_LDADD = @X11_LIBS@ @GLIB_LIBS@

alltray_SOURCES = main.c \
	alltray.h \
	cmdline.c \
	debug.c \
	global.c \
	wm.c \
	x11.c
