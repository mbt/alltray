# Automake file for AllTray

bin_PROGRAMS = alltray alltray-ctt-helper

alltray_CFLAGS = @X11_CFLAGS@ @GLIB_CFLAGS@ @GOBJECT_CFLAGS@ @WNCK_CFLAGS@ \
	-DWNCK_I_KNOW_THIS_IS_UNSTABLE -DG_DISABLE_DEPRECATED \
	-DG_DISABLE_SINGLE_INCLUDES @GTOP_CFLAGS@ -include ../config.h \
	-DLOCALE_DIR=\"@localedir@\"

alltray_LDADD = @X11_LIBS@ @GLIB_LIBS@ @GOBJECT_LIBS@ @WNCK_LIBS@ \
	@GTOP_LIBS@

alltray_VALASOURCES = AllTray.vala \
	Application.vala \
	AttachHelper.vala \
	Debug.vala \
	Process.vala \
	PromptDialog.vala

alltray_SOURCES = $(alltray_VALASOURCES:.vala=.c) \
	x11_glue.c \
	gtop_glue.c

alltray_ctt_helper_CFLAGS = @X11_CFLAGS@ @XPM_CFLAGS@

alltray_ctt_helper_LDADD = @X11_LIBS@ @XPM_LIBS@

alltray_ctt_helper_SOURCES = alltray-ctt-helper.c

BUILT_SOURCES = alltray.stamp

MAINTAINERCLEANFILES = $(alltray_VALASOURCES:.vala=.c) \
	$(alltray_ctt_helper_VALASOURCES:.vala=.c) \
	alltray.stamp

EXTRA_DIST = $(alltray_VALASOURCES) \
	x11_glue.h \
	gtop_glue.h \
	alltray.stamp

alltray_VALA_FLAGS = --pkg alltray --pkg gdk-2.0 --pkg gdk-x11-2.0 \
	--pkg x11 --pkg gtk+-2.0 --pkg libwnck-1.0 --pkg x11_glue \
	--pkg posix --pkg gtop_glue -X "-I header"

alltray.stamp: $(alltray_VALASOURCES) $(alltray_ctt_helper_VALASOURCES)
	$(VALAC) -C --vapidir $(srcdir)/../vapi $(alltray_VALA_FLAGS) \
		$(alltray_VALASOURCES)
	touch $@
