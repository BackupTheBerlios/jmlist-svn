## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = 1.8

dist_man1_MANS = jmlist.1

$(MANS): manpages.xml man-jmlist.xml
	$(XSLTPROC) --stringparam "chunker.output.encoding" "UTF-8" \
	    http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl \
	    $<

dummy:
