SOURCES = AllTray.vala \
	Application.vala \
	Debug.vala \
	Process.vala \
	TrayIcon.vala \
	x11_glue.c

CLEAN_FILES = AllTray.c \
	AllTray.h \
	Debug.c \
	Debug.h \
	Process.c \
	Process.h \
	Application.c \
	Application.h

VALA_CFLAGS = -X -DWNCK_I_KNOW_THIS_IS_UNSTABLE \
	-X -I. -X -std=gnu99

VALA_FLAGS = -g --pkg gdk-2.0 --pkg gdk-x11-2.0 --pkg stdc \
	--pkg gtk+-2.0 --pkg libwnck-1.0 --pkg x11_glue --vapidir ../vapi \
	${VALA_CFLAGS}
VALA_FLAGS += ${VALAC_FLAGS}

alltray: clean
	valac ${SOURCES} ${VALA_FLAGS} -o alltray

clean:
	-rm -f alltray
	-rm -f ${CLEAN_FILES}
