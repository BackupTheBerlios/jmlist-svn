<?xml version="1.0" encoding="UTF-8"?>
<!--
================================================================================
 jMList - Media file listing generator
 http://www.juliano.info/projects/jMList/
================================================================================

   Author: Juliano Ravasi Ferraz <jferraz at users sourceforge net>
  License: GNU General Public License

  Stylesheet used to generate a XHTML, javascript-driven, collapsible tree
  of your music library.

-->
<xsl:stylesheet
  xml:lang='en'
  xmlns:xsl='http://www.w3.org/1999/XSL/Transform'
  xmlns='http://www.w3.org/1999/xhtml'
  version='1.0'
  >

  <!-- The name and version number of this stylesheet.
    This parameter is not intended to be overriden. -->
  <xsl:param name='x-stylesheet'>xhtml-collapsible 0.1</xsl:param>

  <!-- Defines the directory where support files (stylesheets, images,
    scripts) can be found. Should NOT end with a '/'. -->
  <xsl:param name='support-dir'>xhtml-collapsible-xsl</xsl:param>

  <!-- Defines the title of the page. -->
  <xsl:param name='page-title'>Media List</xsl:param>

  <!-- If defined, we generate <a> links using this as a prefix for the
    href attribute -->
  <xsl:param name='href-base'/>


  <xsl:output
    method='xml'
    encoding='utf-8'
    media-type='application/xhtml+xml'
    doctype-public='-//W3C//DTD XHTML 1.0 Strict//EN'
    doctype-system='http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'/>

  <xsl:template match='/'>
    <xsl:processing-instruction name="xml-stylesheet">type='text/css' href='<xsl:value-of select='$support-dir'/>/default.css'</xsl:processing-instruction>
    <xsl:apply-templates select='jmlist'/>
  </xsl:template>

  <xsl:template match='jmlist'>
    <html>
      <head>
        <title>
          <xsl:text>jMList Media list for </xsl:text>
          <xsl:value-of select='directory/@name'/>
        </title>
        <link rel='stylesheet' type='text/css' href='{$support-dir}/default.css'/>
        <script type="text/javascript" src="{$support-dir}/xhtml-collapsible.js"></script>
      </head>
      <body>
        <div id='heading'>
          <h1><xsl:value-of select='$page-title'/></h1>
          <xsl:if test='@generated-by'>
            <p class='generated-by'>
              <xsl:text>Generated by: </xsl:text>
              <a href='{@generator-url}'><xsl:value-of select='@generated-by'/></a>
            </p>
          </xsl:if>
        </div>
        <br/>
        <ul class='listing root'>
          <xsl:apply-templates select='directory'>
            <xsl:with-param name='root' select='1'/>
          </xsl:apply-templates>
        </ul>
        <div style='height: 2em'/>
        <div id='footer'>
          <xsl:text>Powered by </xsl:text>
          <a href='http://jmlist.berlios.de/' title='jMList website'><xsl:text>jMList</xsl:text></a><xsl:text> </xsl:text>
          <xsl:value-of select='$x-stylesheet'/><xsl:text> stylesheet.</xsl:text>
        </div>
      </body>
    </html>
  </xsl:template>

  <xsl:template match='directory'>
    <xsl:param name='thisid'>d<xsl:number level='any' count='directory'/></xsl:param>
    <xsl:param name='path'></xsl:param>
    <xsl:param name='root'/>

    <li class='directory' id='{$thisid}'>

      <div class='name'>
        <xsl:if test='not($root)'>
          <a href='' onclick="toggle('{$thisid}','{$support-dir}'); return false;">
            <img id='{$thisid}_cl' class='collapse' src='{$support-dir}/plus.png' alt='+'/>
          </a>
        </xsl:if>

        <img class='icon' src='{$support-dir}/folder.png' alt=''/>
        <xsl:text> </xsl:text>
        <span class='name'><xsl:value-of select='@name'/></span>
      </div>

      <xsl:element name='ul'>
        <xsl:attribute name='class'>listing</xsl:attribute>
        <xsl:attribute name='id'><xsl:value-of select='$thisid'/>_ls</xsl:attribute>
        <xsl:if test='not($root)'>
          <xsl:attribute name='style'>display: none</xsl:attribute>
        </xsl:if>

        <xsl:variable name='fullpath'>
          <xsl:if test='$path != ""'><xsl:value-of select='$path'/>/</xsl:if><xsl:value-of select='@name'/>
        </xsl:variable>

        <xsl:apply-templates select='directory'>
          <xsl:sort select='@name'/>
          <xsl:with-param name='path'><xsl:value-of select='$fullpath'/></xsl:with-param>
        </xsl:apply-templates>

        <xsl:apply-templates select='file'>
          <xsl:sort select='@name'/>
          <xsl:with-param name='path'><xsl:value-of select='$fullpath'/></xsl:with-param>
        </xsl:apply-templates>
      </xsl:element>

    </li>
  </xsl:template>

  <xsl:template match='file'>
    <xsl:param name='thisid'>f<xsl:number level='any' count='file'/></xsl:param>
    <xsl:param name='path'></xsl:param>

    <li class='file'>
      <div class='name'>
        <img class='icon' src='{$support-dir}/file.png' alt=''/>
        <xsl:text> </xsl:text>
        <span class='name'>
          <xsl:choose>
            <xsl:when test='$href-base'>
              <a href='{$href-base}{$path}/{@name}'>
                <xsl:value-of select='@name'/>
              </a>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select='@name'/>
            </xsl:otherwise>
          </xsl:choose>
        </span>
      </div>

    </li>
  </xsl:template>

</xsl:stylesheet>
<!-- vim: set sw=2 et: -->
