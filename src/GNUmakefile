SOURCES = AllTray.vala \
	Debug.vala \
	Process.vala

CLEAN_FILES = AllTray.c \
	AllTray.h \
	Debug.c \
	Debug.h \
	Process.c \
	Process.h

VALA_FLAGS = -g --pkg gdk-2.0 --pkg gdk-x11-2.0 --pkg stdc \
	--pkg gtk+-2.0 --vapidir ../vapi
VALA_FLAGS += ${VALAC_FLAGS}

alltray: clean
	valac ${SOURCES} ${VALA_FLAGS} -o alltray

clean:
	-rm -f alltray
	-rm -f ${CLEAN_FILES}
