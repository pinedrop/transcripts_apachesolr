<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:exslt="http://exslt.org/common">

  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>

  <xsl:param name="tier_id" select="'ts_content_und'"/>
  <xsl:param name="speaker_display_id" select="'ss_speaker'"/>

  <xsl:template match="/">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="text">
    <xsl:variable name="data">
      <xsl:call-template name="tokenize">
        <xsl:with-param name="text">
          <xsl:call-template name="tokenize">
            <xsl:with-param name="text" select="."/>
            <xsl:with-param name="delim" select="'&#xD;&#x0a;'"/>
            <xsl:with-param name="replace" select="'&#x0a;'"/>
            <xsl:with-param name="op" select="'ident'"/>
          </xsl:call-template>
        </xsl:with-param>
        <xsl:with-param name="delim" select="'&#xD;'"/>
        <xsl:with-param name="replace" select="'&#x0a;'"/>
        <xsl:with-param name="op" select="'ident'"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="tcus">
      <xsl:call-template name="tokenize">
        <xsl:with-param name="text" select="$data"/>
        <xsl:with-param name="delim" select="'&#x0a;&#x0a;'"/>
        <xsl:with-param name="replace" select="''"/>
        <xsl:with-param name="op" select="'createTcu'"/>
      </xsl:call-template>
    </xsl:variable>

    <tcus>
      <xsl:for-each select="exslt:node-set($tcus)/*">
        <xsl:variable name="last-speaker" select="./preceding-sibling::tcu[speakers][1]/speakers"/>
        <xsl:choose>
          <xsl:when test="not(./speakers) and $last-speaker">
            <tcu>
              <xsl:copy-of select="start"/>
              <xsl:copy-of select="end"/>
              <xsl:copy-of select="$last-speaker"/>
              <xsl:copy-of select="tiers"/>
            </tcu>
          </xsl:when>
          <xsl:otherwise>
            <xsl:copy-of select="."/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:for-each>
    </tcus>
  </xsl:template>

  <xsl:template name="tokenize">
    <xsl:param name="text"/>
    <xsl:param name="delim"/>
    <xsl:param name="replace"/>
    <xsl:param name="op"/>

    <xsl:variable name="after" select="substring-after($text, $delim)"/>
    <xsl:choose>
      <xsl:when test="$after = ''">
        <xsl:call-template name="item">
          <xsl:with-param name="op" select="$op"/>
          <xsl:with-param name="arg" select="$text"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="item">
          <xsl:with-param name="op" select="$op"/>
          <xsl:with-param name="arg" select="substring-before($text, $delim)"/>
        </xsl:call-template>
        <xsl:value-of select="$replace"/>
        <xsl:call-template name="tokenize">
          <xsl:with-param name="text" select="$after"/>
          <xsl:with-param name="delim" select="$delim"/>
          <xsl:with-param name="replace" select="$replace"/>
          <xsl:with-param name="op" select="$op"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="item">
    <xsl:param name="op"/>
    <xsl:param name="arg"/>

    <xsl:choose>
      <xsl:when test="$op = 'ident'">
        <xsl:value-of select="$arg"/>
      </xsl:when>
      <xsl:when test="$op = 'createTcu'">
        <xsl:call-template name="createTcu">
          <xsl:with-param name="arg" select="$arg"/>
        </xsl:call-template>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="createTcu">
    <xsl:param name="arg"/>

    <xsl:variable name="tcu" select="substring-after($arg, '&#x0a;')"/> <!-- ignore id line -->
    <xsl:if test="normalize-space($tcu) != ''">
      <tcu>
        <xsl:variable name="times" select="substring-before($tcu, '&#x0a;')"/>
        <xsl:variable name="text" select="normalize-space(substring-after($tcu, '&#x0a;'))"/>
        <start>
          <xsl:call-template name="convert-time">
            <xsl:with-param name="time" select="normalize-space(substring-before($times, '--&gt;'))"/>
          </xsl:call-template>
        </start>
        <end>
          <xsl:call-template name="convert-time">
            <xsl:with-param name="time" select="normalize-space(substring-after($times, '--&gt;'))"/>
          </xsl:call-template>
        </end>
        <xsl:choose>
          <xsl:when test="starts-with($text, '&gt;&gt;')">
            <speakers>
              <xsl:element name="{$speaker_display_id}">
                <xsl:value-of select="normalize-space(substring(substring-before($text, ':'), 3))"/>
              </xsl:element>
            </speakers>
            <tiers>
              <xsl:element name="{$tier_id}">
                <xsl:value-of
                  select="normalize-space(substring-after($text, ':'))"/>
              </xsl:element>
            </tiers>
          </xsl:when>
          <xsl:otherwise>
            <tiers>
              <xsl:element name="{$tier_id}">
                <xsl:value-of select="$text"/>
              </xsl:element>
            </tiers>
          </xsl:otherwise>
        </xsl:choose>
      </tcu>
    </xsl:if>
  </xsl:template>

  <xsl:template name="convert-time">
    <xsl:param name="time" select="'0'"/>

    <!-- HH:MM:SS,MIL (hours, minutes, seconds, milliseconds) -->
    <xsl:choose>
      <xsl:when test="string-length(substring-after($time, ',')) = 3"> <!-- SRT has comma before milliseconds -->
        <xsl:variable name="h" select="number(substring($time,1,2))"/>
        <xsl:variable name="m" select="number(substring($time,4,2))"/>
        <xsl:variable name="s" select="number(substring($time,7,2))"/>
        <xsl:variable name="x" select="number(substring($time,10,3))"/>
        <xsl:value-of select="format-number($h*3600 + $m*60 + $s + $x div 1000,'0.000')"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>0</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
