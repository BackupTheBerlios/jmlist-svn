## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = 1.8

dist_man_MANS = jmlist.1

$(MANS): manpages.xml
	$(XSLTPROC) --stringparam "chunker.output.encoding" "UTF-8" \
	    http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl \
	    $<

manpages.xml: man-jmlist.xml

EXTRA_DIST = \
	manpages.xml			\
	man-jmlist.xml
