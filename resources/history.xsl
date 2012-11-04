<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="2.0"
        xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
  <html>
    <head>
    </head>
    <body>
    <h3>
      <xsl:value-of select="//data/@title"/>
    </h3>

    <xsl:comment>Records of history</xsl:comment>
    <xsl:apply-templates/>

    </body>
  </html>
</xsl:template>

<xsl:template match="record">
  <hr/>
  <div>

    <xsl:choose>
      <xsl:when test="commit/date = '$current_date'">
        <xsl:attribute name="class">
          <xsl:text>record-current</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="commit/date &gt; '$current_date'">
        <xsl:attribute name="class">
          <xsl:text>record-new</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="commit/date &lt; '$current_date'">
        <xsl:attribute name="class">
          <xsl:text>record-old</xsl:text>
        </xsl:attribute>
        </xsl:when>
    </xsl:choose>

    <p>
      <xsl:apply-templates select="commit"/>
    </p>
    <p>
      <xsl:apply-templates select="description"/>
    </p>
    <p>
      <xsl:apply-templates select="downloads"/>
    </p>
  </div>
</xsl:template>

<xsl:template match="commit">

  <span class="commit">
    <xsl:comment>Date and hyperlink to revision in version control system</xsl:comment>

    <xsl:text>Revision: </xsl:text>
    <a class="revision">
      <xsl:attribute name="href">
        <xsl:text>http://code.google.com/p/lcd-image-converter/source/detail?r=</xsl:text>
        <xsl:value-of select="sha1"/>
      </xsl:attribute>
      <xsl:value-of select="substring(sha1, 1, 7)"/>
    </a>
    <xsl:text> </xsl:text>
    <span class="date">
      <xsl:text>from: </xsl:text>
      <xsl:value-of select="date"/>
    </span>
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
