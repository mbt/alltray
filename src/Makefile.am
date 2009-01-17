# Automake file for AllTray

bin_PROGRAMS = alltray

alltray_SOURCES = main.c \
	alltray.h \
	cmdline.c \
	debug.c \
	global.c

AM_CFLAGS = @X11_CFLAGS@ @GLIB_CFLAGS@ -DPACKAGE_LIB_DIR=\""$(libdir)"\"
alltray_LDADD = @X11_LIBS@ @GLIB_LIBS@
