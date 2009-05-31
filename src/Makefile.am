# Automake file for AllTray

bin_PROGRAMS = alltray
AM_CFLAGS = @X11_CFLAGS@ @GLIB_CFLAGS@ @GOBJECT_CFLAGS@ @WNCK_CFLAGS@ \
	-DWNCK_I_KNOW_THIS_IS_UNSTABLE -DG_DISABLE_DEPRECATED \
	-DG_DISABLE_SINGLE_INCLUDES
alltray_LDADD = @X11_LIBS@ @GLIB_LIBS@ @GOBJECT_LIBS@ @WNCK_LIBS@

alltray_VALASOURCES = AllTray.vala \
	Application.vala \
	AttachHelper.vala \
	Debug.vala \
	Process.vala \
	PromptDialog.vala

MAINTAINERCLEANFILES = $(alltray_VALASOURCES:.vala=.c) \
	$(alltray_VALASOURCES:.vala=.h)

alltray_SOURCES = $(alltray_VALASOURCES:.vala=.c) \
	x11_glue.c \
	unix_process.c

EXTRA_DIST = $(alltray_VALASOURCES) \
	$(alltray_VALASOURCES:.vala=.h) \
	x11_glue.h

VALA_FLAGS = --pkg gdk-2.0 --pkg gdk-x11-2.0 --pkg stdc \
	--pkg gtk+-2.0 --pkg x11 --pkg libwnck-1.0 --pkg x11_glue \
	--pkg alltray --pkg posix

$(alltray_SOURCES): $(alltray_VALASOURCES)
	$(VALAC) -C --vapidir $(srcdir)/../vapi $(VALA_FLAGS) $^