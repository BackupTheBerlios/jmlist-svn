<?xml version='1.0' encoding='utf-8' standalone='yes' ?>
<!--
================================================================================
 jMList - Media file listing generator
 http://www.juliano.info/projects/jMList/
================================================================================

   Author: Juliano Ravasi Ferraz <jferraz at users sourceforge net>
  License: GNU General Public License

 Stylesheet used to generate a plain-text and tabular list of song files.

-->
<xsl:stylesheet
 xml:lang='en'
 xmlns:xsl='http://www.w3.org/1999/XSL/Transform'
 xmlns='http://www.w3.org/1999/xhtml'
 version='1.0'
>
<xsl:output method='text' encoding='utf-8' media-type='text/plain'/>
<xsl:template match='jmlist'>
<xsl:text>jMList - Media file listing generator
================================================================================

Artist             | Title              | File
-------------------+--------------------+---------------------------------------
</xsl:text>
<xsl:for-each select="descendant::file">
<xsl:value-of select="substring(concat(tag/artist, '                   '), 1, 18)"/>
<xsl:text> | </xsl:text>
<xsl:value-of select="substring(concat(tag/title, '                   '), 1, 18)"/>
<xsl:text> | </xsl:text>
<xsl:value-of select="@name"/><xsl:text>
</xsl:text></xsl:for-each>
</xsl:template>

</xsl:stylesheet>
<!-- vim: set sw=1 et: -->
