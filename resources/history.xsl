<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="2.0"
        xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
  <html>
    <head>
    </head>
    <body>
    <h2>Revisions list</h2>

    <xsl:comment>Records of history</xsl:comment>
    <xsl:apply-templates/>

    </body>
  </html>
</xsl:template>

<xsl:template match="record">
  <hr/>
  <div class="record">
    <p>
      <xsl:apply-templates select="sha1"/>
      <xsl:text> </xsl:text>
      <xsl:apply-templates select="date"/>
      <xsl:apply-templates select="description"/>
      <br/>
      <br/>
      <xsl:apply-templates select="downloads"/>
    </p>
  </div>
</xsl:template>

<xsl:template match="sha1">

  <xsl:comment>Hyperlink to revision in version control system</xsl:comment>

  Revision:
  <a class="revision">
    <xsl:attribute name="href">
      <xsl:text>http://code.google.com/p/lcd-image-converter/source/detail?r=</xsl:text>
      <xsl:value-of select="."/>
    </xsl:attribute>
    <xsl:value-of select="substring(., 1, 7)"/>
  </a>
</xsl:template>

<xsl:template match="date">
  <span class="date">
    <xsl:text>from: </xsl:text>
    <xsl:value-of select="."/>
  </span>
</xsl:template>

<xsl:template match="description">
  <span class="description">
    <pre>
      <xsl:value-of select="."/>
    </pre>
  </span>
</xsl:template>

<xsl:template match="downloads">
  <span class="downloads">
    <xsl:text>Download: </xsl:text>
    <br/>
    <xsl:for-each select="child::item">
       <xsl:text> </xsl:text>
       <a>
         <xsl:attribute name="href">
           <xsl:value-of select="url"/>
         </xsl:attribute>
         <xsl:value-of select="name"/>
       </a>
       <xsl:if test="not (position()=last())">
            <br/>
       </xsl:if>
    </xsl:for-each>
  </span>
</xsl:template>

</xsl:stylesheet>
