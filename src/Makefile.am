# main program
bin_PROGRAMS = wether

wether_SOURCES = main.c wether.c
noinst_HEADERS = interface.h callbacks.h wether.h

# wether_LDADD = `pkg-config --cflags --libs gtk+-3.0`
wether_LDADD = @PACKAGE_LIBS@
wether_LDFLAGS = @PACKAGE_LDFLAGS@
AM_CPPFLAGS = @PACKAGE_CFLAGS@

SUBDIRS = ui
