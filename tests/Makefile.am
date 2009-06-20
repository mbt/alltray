# Automake file for AllTray

bin_PROGRAMS = GtkDemandsAttention GtkIconChange
AM_CFLAGS = @GTK_CFLAGS@ @GOBJECT_CFLAGS@ -DG_DISABLE_DEPRECATED \
	-DG_DISABLE_SINGLE_INCLUDES
GtkDemandsAttention_LDADD = @GTK_LIBS@ @GOBJECT_LIBS@
GtkIconChange_LDADD = @GTK_LIBS@ @GOBJECT_LIBS@

GtkDemandsAttention_VALASOURCES = GtkDemandsAttention.vala
GtkIconChange_VALASOURCES = GtkIconChange.vala

MAINTAINERCLEANFILES = $(GtkDemandsAttention_VALASOURCES:.vala=.c) \
	$(GtkIconChange_VALASOURCES:.vala=.c) \
	.GtkDemandsAttention.stamp \
	.GtkIconChange.stamp

BUILT_SOURCES = .GtkDemandsAttention.stamp \
	.GtkIconChange.stamp

GtkDemandsAttention_SOURCES = $(GtkDemandsAttention_VALASOURCES:.vala=.c)
GtkIconChange_SOURCES = $(GtkIconChange_VALASOURCES:.vala=.c)

EXTRA_DIST = $(GtkDemandsAttention_VALASOURCES) \
	$(GtkIconChange_VALASOURCES) \
	$(BUILT_SOURCES) \
	1.png 2.png

VALA_FLAGS = --pkg gtk+-2.0 --pkg posix

.GtkDemandsAttention.stamp: $(GtkDemandsAttention_VALASOURCES)
	$(VALAC) -C $(VALA_FLAGS) $(GtkDemandsAttention_VALASOURCES)
	touch $@

.GtkIconChange.stamp: $(GtkIconChange_VALASOURCES)
	$(VALAC) -C $(VALA_FLAGS) $(GtkIconChange_VALASOURCES)
	touch $@
