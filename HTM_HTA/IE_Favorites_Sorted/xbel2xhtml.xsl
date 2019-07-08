<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  
	<xsl:template match="/">
		<html>
			<head><title>
        <xsl:value-of select="xbel/title" />
      </title></head>
			<body>
				<xsl:apply-templates select="xbel/folder|xbel/bookmark" />
			</body>
		</html>
	</xsl:template>

	<xsl:template match="folder">
    <h3>
		  <xsl:value-of select="title" />
    </h3>
		<ul>
			<xsl:apply-templates select="bookmark|folder" />
		</ul>
    <hr/>	
  </xsl:template>

	<xsl:template match="bookmark">
		<li>
			<xsl:element name="a">
				<xsl:attribute name="href">
					<xsl:value-of select="@href" />
				</xsl:attribute>
				<xsl:value-of select="title" />
			</xsl:element>
		</li>
	</xsl:template>
  
</xsl:stylesheet>
