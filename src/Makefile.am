# Automake file for AllTray

bin_PROGRAMS = alltray
AM_CFLAGS = @X11_CFLAGS@ @GLIB_CFLAGS@ @GOBJECT_CFLAGS@
alltray_LDADD = @X11_LIBS@ @GLIB_LIBS@ @GOBJECT_LIBS@

AM_CFLAGS += -DALLTRAY_HELPER_DIRECTORY=\"$(pkglib)\"

alltray_VALASOURCES = AllTray.vala \
	Application.vala \
	Debug.vala \
	Process.vala \
	TrayIcon.vala

alltray_SOURCES = $(alltray_VALASOURCES:.vala=.c) \
	x11_glue.c

EXTRA_DIST = $(alltray_VALASOURCES) \
	x11_glue.h

VALA_FLAGS = -g --pkg gtk-2.0 --pkg gdk-x11-2.0 --pkg stdc \
	--pkg gtk+-2.0 --pkg libwnck-1.0 --pkg x11_glue

$(alltray_SOURCES): $(alltray_VALASOURCES)
	$(VALAC) -C --vapidir $(srcdir)/../vapi $(VALA_FLAGS) $^