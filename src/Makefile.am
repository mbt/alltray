## Process this file with automake to produce Makefile.in

INCLUDES = \
  @PACKAGE_CFLAGS@
  
  
bin_PROGRAMS = alltray

AM_CFLAGS =\
  -Wall

alltray_SOURCES = \
	main.c  \
  gtray.h \
  utils.c \
  utils.h \
  trayicon.c \
  trayicon.h \
  inlinepixbufs.h


alltray_LDFLAGS =-L/usr/X11R6/lib
alltray_LDADD = @PACKAGE_LIBS@  -lXmu
