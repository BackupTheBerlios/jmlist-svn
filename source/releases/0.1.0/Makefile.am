## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = 1.8

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = lib src styles doc
DIST_SUBDIRS = $(SUBDIRS)

EXTRA_DIST = Doxyfile

.PHONY: docs
docs: Doxyfile
	doxygen Doxyfile
