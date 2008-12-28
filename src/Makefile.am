## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = alltray

AM_CFLAGS =\
	 -g


alltray_SOURCES = \
	main.c  \
  gtray.h \
  utils.c \
  utils.h \
  trayicon.c \
  trayicon.h


alltray_LDFLAGS =-L/usr/X11R6/lib
alltray_LDADD = @PACKAGE_LIBS@  -lXmu
