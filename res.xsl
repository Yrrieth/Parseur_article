<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html" version="4.01" encoding="utf-8" doctype-public="-//W3C//DTD HTML 4.01//EN" doctype-system="http://www.w3.org/TR/html4/strict.dtd"/>
	<xsl:template match="/">
	<html>
		<head>
			<title>Résultat</title>
		</head>
		<body>
			<xsl:apply-templates select="texte"/>
		</body>
	</html>
	</xsl:template>
	<xsl:template match="texte">
		<xsl:apply-templates select="node()"/>
	</xsl:template>
	<xsl:template match="text()">
		<xsl:value-of select="."/>
	</xsl:template>
	<xsl:template match="article">
		<font color="#CC00CC">
			<xsl:value-of select="."/>
		</font>
	</xsl:template>
	<xsl:template match="pronom">
		<font color="#0033CC">
			<xsl:value-of select="."/>
		</font>
	</xsl:template>
	<xsl:template match="trouve">
		<font color="#00FF00">
			<xsl:value-of select="."/>
		</font>
	</xsl:template>
</xsl:stylesheet>




