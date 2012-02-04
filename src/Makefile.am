# Automake file for AllTray

bin_PROGRAMS = alltray alltray-ctt-helper

alltray_CFLAGS = @X11_CFLAGS@ @GLIB_CFLAGS@ @GOBJECT_CFLAGS@ @WNCK_CFLAGS@ \
	-DWNCK_I_KNOW_THIS_IS_UNSTABLE -DG_DISABLE_DEPRECATED \
	-DG_DISABLE_SINGLE_INCLUDES @GTOP_CFLAGS@ -include ../config.h \
	-DLOCALE_DIR=\"@localedir@\"

alltray_LDADD = @X11_LIBS@ @GLIB_LIBS@ @GOBJECT_LIBS@ @WNCK_LIBS@ \
	@GTOP_LIBS@

alltray_SOURCES = About.vala \
	AllTray.vala \
	Application.vala \
	AttachHelper.vala \
	GtkStatusIcon.vala \
	Hotkey.vala \
	Process.vala \
	PromptDialog.vala \
	Ctt.vala \
	x11_glue.c \
	gtop_glue.c

alltray_ctt_helper_CFLAGS = @X11_CFLAGS@ @XPM_CFLAGS@ @XEXT_CFLAGS@ \
	@XCURSOR_CFLAGS@

alltray_ctt_helper_LDADD = @X11_LIBS@ @XPM_LIBS@ @XEXT_LIBS@ \
	@XCURSOR_LIBS@

alltray_ctt_helper_SOURCES = alltray-ctt-helper.c \
	alltray-ctt-interpreter.c \
	alltray-ctt-windowlist.c

MAINTAINERCLEANFILES = $(alltray_VALASOURCES:.vala=.c) \
	$(alltray_ctt_helper_VALASOURCES:.vala=.c) \
	alltray.stamp

EXTRA_DIST = $(alltray_VALASOURCES) \
	x11_glue.h \
	gtop_glue.h \
	alltray-ctt-interpreter.h \
	alltray-ctt-helper.h \
	alltray-ctt-windowlist.h \
	alltray.stamp

alltray_VALAFLAGS = --vapidir $(srcdir)/../vapi --pkg alltray --pkg gdk-2.0 \
	--pkg gdk-x11-2.0 --pkg x11 --pkg gtk+-2.0 --pkg libwnck-1.0 \
	--pkg x11_glue --pkg posix --pkg gtop_glue -X "-I header"

