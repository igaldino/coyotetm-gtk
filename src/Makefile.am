## Process this file with automake to produce Makefile.in

bin_PROGRAMS = coyotetm

coyotetm_SOURCES = \
	ctm-app.c \
	ctm-app.h \
	ctm-db.c \
	ctm-db.h \
	ctm-person.c \
	ctm-person.h \
	ctm-project.c \
	ctm-project.h \
	ctm-task.c \
	ctm-task.h \
	ctm-event.c \
	ctm-event.h \
	ctm-window.c \
	ctm-window.h \
	ctm-resource.c \
	main.c

coyotetm_CFLAGS = \
	$(COYOTETM_CFLAGS) \
	$(COYOTETM_WARN_CFLAGS)

coyotetm_LDFLAGS = \
	-Wl,--export-dynamic \
	$(COYOTETM_WARN_LDFLAGS)

coyotetm_LDADD = \
	$(COYOTETM_LIBS)

BUILT_SOURCES = ctm-resource.c

ctm-resource.c: ctm-resource.xml ctm-window.ui
	glib-compile-resources $(srcdir)/ctm-resource.xml \
	--target=$@ --sourcedir=$(srcdir) --generate-source

EXTRA_DIST = ctm-resource.xml \
	ctm-window.ui

CLEANFILES = *~ ctm-resource.c

