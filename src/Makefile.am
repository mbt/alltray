## Process this file with automake to produce Makefile.in

INCLUDES = \
  @PACKAGE_CFLAGS@

bin_PROGRAMS = alltray

AM_CFLAGS =\
  -Wall

alltray_SOURCES = \
  main.c \
  common.h \
  parent.c \
  parent.h \
  child.c \
  child.h \
  utils.c \
  utils.h \
  trayicon.c \
  trayicon.h \
  balloon_message.c \
  balloon_message.h \
  xmms.c \
  xmms.h \
  clickmode.c \
  clickmode.h \
  prefix.c \
  prefix.h \
  inlinepixbufs.h


alltray_LDFLAGS =-L/usr/X11R6/lib
alltray_LDADD = @PACKAGE_LIBS@  -lXmu
