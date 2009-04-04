SOURCES = AllTray.vala \
	Debug.vala \
	Process.vala

VALA_FLAGS = --pkg gdk-2.0 --pkg gdk-x11-2.0 --pkg stdc \
	--pkg gtk+-2.0 --vapidir ../vapi --save-temps

alltray: clean
	valac ${SOURCES} ${VALA_FLAGS} -o alltray

clean:
	-rm -f alltray
	-rm -f *.c *.h
