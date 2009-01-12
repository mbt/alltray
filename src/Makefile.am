# Automake file for AllTray

bin_PROGRAMS = alltray

alltray_SOURCES = main.c \
	common.h \
	parent.c parent.h \
	child.c child.h \
	utils.c utils.h \
	trayicon.c trayicon.h \
	balloon_message.c balloon_message.h \
	xmms.c xmms.h \
	clickmode.c clickmode.h \
	grab.c grab.h \
	gnome_theme.c gnome_theme.h \
	kde.c kde.h \
	clientwin.c clientwin.h \
	shortcut.c shortcut.h \
	eventfilter.c eventfilter.h \
	inlinepixbufs.h

AM_CFLAGS = @X11_CFLAGS@ @GLIB_CFLAGS@ @GDK_CFLAGS@ @GDKPIXBUF_CFLAGS@ \
	@GTK_CFLAGS@ @GCONF_CFLAGS@ -DPACKAGE_LIB_DIR=\""$(libdir)"\"
alltray_LDADD = @X11_LIBS@ @GLIB_LIBS@ @GDK_LIBS@ @GDKPIXBUF_LIBS@ \
	@GTK_LIBS@ @GCONF_LIBS@
