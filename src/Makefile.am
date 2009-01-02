## Process this file with automake to produce Makefile.in

INCLUDES = \
  -DPACKAGE_LIB_DIR=\""$(libdir)"\" \
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
  grab.c \
  grab.h \
  gnome_theme.c \
  gnome_theme.h \
  kde.c \
  kde.h \
  clientwin.c \
  clientwin.h \
  shortcut.c \
  shortcut.h \
  eventfilter.c \
  eventfilter.h \
  inlinepixbufs.h


alltray_LDFLAGS =-L/usr/X11R6/lib
alltray_LDADD = @PACKAGE_LIBS@
