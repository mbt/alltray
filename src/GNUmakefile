SOURCES = AllTray.vala \
	Debug.vala \
	DisplayManager.vala \
	WindowManager.vala \
	XWrapper.vala

VALA_FLAGS = --pkg gdk-2.0 --pkg gdk-x11-2.0 --pkg stdc --pkg libx11 \
	--vapidir ../vapi --save-temps

alltray: clean
	valac ${SOURCES} ${VALA_FLAGS} -o alltray

clean:
	touch alltray
	rm alltray
