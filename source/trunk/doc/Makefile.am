## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = 1.8

dist_man_MANS = jmlist.1

$(MANS): manpages.xml man-jmlist.xml
	$(XSLTPROC) --stringparam "chunker.output.encoding" "UTF-8" \
	    http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl \
	    $<

.PHONY: manual-html
manual-html: manpages.xml
	$(XSLTPROC) --stringparam "chunker.output.encoding" "UTF-8" \
	    --stringparam "base.dir" "manual-html/" \
	    --stringparam "html.stylesheet" "default.css" \
	    http://docbook.sourceforge.net/release/xsl/current/xhtml/chunk.xsl \
	    $<

EXTRA_DIST = \
	manpages.xml			\
	man-jmlist.xml
