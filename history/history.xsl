<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="2.0"	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="/">
		<html>
			<head>
				<link rel="stylesheet" type="text/css" href="history.css"/>
				<title>
					<xsl:value-of select="//data/@title"/>
				</title>
			</head>
			<body>
				<div class="header">
					<h3>
						<xsl:value-of select="//data/@title"/>
					</h3>
					<xsl:if test="count(//commit[date &gt; '$current_date']) &gt; 0">
						<h4 class="update-alert">
							<xsl:text>Update found!</xsl:text>
						</h4>
					</xsl:if>
				</div>
				<xsl:comment>Records of history</xsl:comment>
				<xsl:apply-templates/>
			</body>
		</html>
	</xsl:template>
	
	<xsl:template match="record">
		<div class="record">
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
			</xsl:choose><p>
				<xsl:apply-templates select="commit"/>
			</p><p>
				<xsl:apply-templates select="description"/>
			</p><p>
				<xsl:apply-templates select="downloads"/>
			</p>
		</div>
	</xsl:template>
	
	<xsl:template match="commit">
		<div class="commit">
			<!--// Date and hyperlink to revision in version control system //-->
			<xsl:text>Revision </xsl:text><a class="revision">
				<xsl:attribute name="href">
					<xsl:text>https://github.com/riuson/lcd-image-converter/commit/</xsl:text>
					<xsl:value-of select="sha1"/>
				</xsl:attribute>
				<xsl:value-of select="substring(sha1, 1, 7)"/>
			</a>
			<xsl:text> </xsl:text>
			<span class="date">
				<xsl:text>from </xsl:text>
				<xsl:value-of select="date"/>
			</span>
		</div>
	</xsl:template>
	
	<xsl:template match="description">
		<div class="description">
			<xsl:call-template name="break">
				<xsl:with-param name="text" select="text()" />
			</xsl:call-template>
		</div>
	</xsl:template>
	
	<xsl:template match="downloads">
		<div class="downloads">
			<xsl:text>Download: </xsl:text>
			<br/>
			<xsl:for-each select="child::item">
				<xsl:text/>
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
		</div>
	</xsl:template>
	
	<xsl:template name="break">
		<!--// http://stackoverflow.com/questions/561235/xslt-replace-n-with-br-only-in-one-node //-->
		<xsl:param name="text" select="."/>
		<xsl:choose>
			<xsl:when test="contains($text, '&#xa;')">
				<xsl:value-of select="substring-before($text, '&#xa;')"/>
				<br/>
				<xsl:call-template name="break">
					<xsl:with-param name="text" select="substring-after($text, '&#xa;')"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$text"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
</xsl:stylesheet>
