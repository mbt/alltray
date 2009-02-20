SOURCES = AllTray.vala \
	Debug.vala \
	DisplayManager.vala \
	WindowManager.vala

VALA_FLAGS = --pkg gdk-2.0 --pkg gdk-x11-2.0 --pkg stdc --vapidir ../vapi \
	--save-temps

alltray:
	valac ${SOURCES} ${VALA_FLAGS} -o alltray

clean:
	rm alltray
