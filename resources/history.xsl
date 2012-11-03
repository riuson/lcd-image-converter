<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0"
        xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
  <html>
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
      <br/>
      <xsl:apply-templates select="description"/>
      <br/>
      <xsl:apply-templates select="download"/>
      <xsl:apply-templates select="accessories"/>
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

<xsl:template match="download">
  <span class="download">
    <a class="download">
      <xsl:attribute name="href">
        <xsl:value-of select="."/>
      </xsl:attribute>
      <xsl:text>Download application</xsl:text>
    </a>
  </span>
  <xsl:text> </xsl:text>
</xsl:template>

<xsl:template match="accessories">
  <span class="accessories">
    <xsl:for-each select="child::item">
       <a>
         <xsl:attribute name="href">
           <xsl:value-of select="download"/>
         </xsl:attribute>
         <xsl:value-of select="description"/>
       </a>
       <xsl:text> </xsl:text>
    </xsl:for-each>
  </span>
</xsl:template>

</xsl:stylesheet>
