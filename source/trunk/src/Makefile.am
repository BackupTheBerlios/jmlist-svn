## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = 1.8

INCLUDES = @PACKAGE_CFLAGS@
LDADD = @PACKAGE_LIBS@

bin_PROGRAMS = jmlist

jmlist_SOURCES = \
        system.h                                        \
        common.h                                        \
        main.c                  main.h                  \
        directory.c             directory.h             \
        file.c                  file.h                  \
        file-mpeg.c             file-mpeg.h             \
        file-mpeg-id3v1.c       file-mpeg-id3v1.h       \
        file-mpeg-id3v2.c       file-mpeg-id3v2.h       \
        style.c                 style.h

if !HAVE_LIBPOPT
INCLUDES += -I$(top_builddir)/lib/popt
jmlist_LDADD = $(LDADD) $(top_builddir)/lib/popt/libpopt.a
else
jmlist_LDADD = $(LDADD)
endif
