<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="2.0" 
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:template match="/">
    <html>
      <head></head>
      <body>
        <h1>
          <xsl:value-of select="//data/@title"/>
        </h1>
        <xsl:if test="count(//commit[date &gt; '$current_date']) &gt; 0">
          <h2 class="update-alert">
            <xsl:text>Update found!</xsl:text>
          </h2>
        </xsl:if>
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
      <xsl:apply-templates select="commit"/>
      <xsl:choose>
        <xsl:when test="count(changes/item) > 0">
          <xsl:apply-templates select="changes"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:apply-templates select="description" />
        </xsl:otherwise>
      </xsl:choose>
      <xsl:apply-templates select="downloads" />
    </div>
  </xsl:template>
  <xsl:template match="commit">
    <h2>
      <span class="commit">
        <xsl:comment>Date and hyperlink to revision in version control system</xsl:comment>
        <xsl:text>Revision </xsl:text>
        <a class="revision">
          <xsl:attribute name="href">
            <xsl:text>https://github.com/riuson/lcd-image-converter/commit/</xsl:text>
            <xsl:value-of select="sha1"/>
          </xsl:attribute>
          <xsl:value-of select="substring(sha1, 1, 7)"/>
        </a>
        <xsl:text></xsl:text>
        <span class="date">
          <xsl:text> from </xsl:text>
          <xsl:value-of select="date"/>
        </span>
      </span>
    </h2>
  </xsl:template>
  <xsl:template match="changes">
    <xsl:call-template name="display_change_items">
      <xsl:with-param name="title">Added</xsl:with-param>
      <xsl:with-param name="category">added</xsl:with-param>
    </xsl:call-template>
    <xsl:call-template name="display_change_items">
      <xsl:with-param name="title">Changed</xsl:with-param>
      <xsl:with-param name="category">changed</xsl:with-param>
    </xsl:call-template>
    <xsl:call-template name="display_change_items">
      <xsl:with-param name="title">Fixed</xsl:with-param>
      <xsl:with-param name="category">fixed</xsl:with-param>
    </xsl:call-template>
    <xsl:call-template name="display_change_items">
      <xsl:with-param name="title">Deprecated</xsl:with-param>
      <xsl:with-param name="category">deprecated</xsl:with-param>
    </xsl:call-template>
    <xsl:call-template name="display_change_items">
      <xsl:with-param name="title">Removed</xsl:with-param>
      <xsl:with-param name="category">removed</xsl:with-param>
    </xsl:call-template>
  </xsl:template>
  <xsl:template name="display_change_items">
    <xsl:param name="category">none</xsl:param>
    <xsl:param name="title">none</xsl:param>
    <xsl:if test="count(item[@category=$category]) > 0">
      <h3>
        <xsl:value-of select="$title"/>
      </h3>
      <ul>
        <xsl:for-each select="child::item">
          <xsl:if test="@category = $category">
            <li>
              <xsl:value-of select="."/>
            </li>
          </xsl:if>
        </xsl:for-each>
      </ul>
    </xsl:if>
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
      <br/>
      <h3>
        <xsl:text>Download</xsl:text>
      </h3>
      <ul>
        <xsl:for-each select="child::item">
          <li>
            <a>
              <xsl:attribute name="href">
                <xsl:value-of select="url"/>
              </xsl:attribute>
              <xsl:value-of select="name"/>
            </a>
          </li>
        </xsl:for-each>
      </ul>
    </span>
  </xsl:template>
</xsl:stylesheet>