<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:exslt="http://exslt.org/common">

  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>

  <xsl:param name="tcuData" select="'ELANBegin|start, ELANEnd|end, ELANParticipant|speakers'"/>
  <xsl:param name="tierNames" select="'tx|ts_content_qya, mb|ts_content_morph, ge|ts_content_igt, ft|ts_content_epo'"/>

  <xsl:template match="/">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="text">
    <xsl:variable name="tcus">
      <xsl:call-template name="tokenize">
        <xsl:with-param name="text" select="substring-after(., '\block')"/>
        <xsl:with-param name="delim" select="'\block'"/>
        <xsl:with-param name="replace" select="''"/>
        <xsl:with-param name="op" select="'block'"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="tculevel">
      <xsl:call-template name="tokenize">
        <xsl:with-param name="text" select="$tcuData"/>
        <xsl:with-param name="delim" select="','"/>
        <xsl:with-param name="replace" select="''"/>
        <xsl:with-param name="op" select="'wrap'"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="tierlevel">
      <xsl:call-template name="tokenize">
        <xsl:with-param name="text" select="$tierNames"/>
        <xsl:with-param name="delim" select="','"/>
        <xsl:with-param name="replace" select="''"/>
        <xsl:with-param name="op" select="'wrap'"/>
      </xsl:call-template>
    </xsl:variable>

    <tcus>
      <xsl:for-each select="exslt:node-set($tcus)/block">
        <xsl:variable name="tcu" select="."/>

        <tcu>
          <xsl:for-each select="exslt:node-set($tculevel)/wrap">
            <xsl:variable name="meta">
              <xsl:call-template name="getTier">
                <xsl:with-param name="tcu" select="$tcu"/>
                <xsl:with-param name="tier" select="normalize-space(substring-before(.,'|'))"/>
              </xsl:call-template>
            </xsl:variable>
            <xsl:variable name="cat" select="normalize-space(substring-after(.,'|'))"/>
            <xsl:choose>
              <xsl:when test="$cat='start' or $cat='end'">
                <xsl:if test="not($meta='')">
                  <xsl:element name="{$cat}">
                    <xsl:call-template name="convert-time">
                      <xsl:with-param name="time" select="$meta"/>
                    </xsl:call-template>
                  </xsl:element>
                </xsl:if>
              </xsl:when>
              <xsl:otherwise>
                <xsl:if test="not($meta='')">
                  <xsl:choose>
                    <xsl:when test="$cat='speakers'">
                      <speakers>
                        <ss_speaker>
                          <xsl:value-of select="$meta"/>
                        </ss_speaker>
                      </speakers>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:element name="{$cat}">
                        <xsl:value-of select="$meta"/>
                      </xsl:element>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:if>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:for-each>
          <tiers>
            <xsl:for-each select="exslt:node-set($tierlevel)/wrap">
              <xsl:variable name="res">
                <xsl:call-template name="getTier">
                  <xsl:with-param name="tcu" select="$tcu"/>
                  <xsl:with-param name="tier" select="normalize-space(substring-before(.,'|'))"/>
                </xsl:call-template>
              </xsl:variable>
              <xsl:if test="not($res='')">
                <xsl:element name="{normalize-space(substring-after(.,'|'))}">
                  <xsl:value-of select="$res"/>
                </xsl:element>
              </xsl:if>
            </xsl:for-each>
          </tiers>
        </tcu>
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
      <xsl:otherwise>
        <xsl:element name="{$op}">
          <xsl:value-of select="$arg"/>
        </xsl:element>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="getTier">
    <xsl:param name="tcu" select="''"/>
    <xsl:param name="tier" select="''"/>

    <xsl:variable name="tag" select="concat('\', $tier, ' ')"/>

    <xsl:variable name="return">
      <xsl:call-template name="extractTierData">
        <xsl:with-param name="text" select="$tcu"/>
        <xsl:with-param name="delim" select="'&#x0a;'"/>
        <xsl:with-param name="tag" select="$tag"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:value-of select="normalize-space($return)"/>
  </xsl:template>

  <xsl:template name="extractTierData">
    <xsl:param name="text"/>
    <xsl:param name="delim"/>
    <xsl:param name="tag"/>

    <xsl:variable name="after" select="substring-after($text, $delim)"/>
    <xsl:choose>
      <xsl:when test="$after = ''">
        <xsl:if test="starts-with($text, $tag)">
          <xsl:value-of select="substring($text, string-length($tag)+1)"/>
        </xsl:if>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="line" select="substring-before($text, $delim)"/>
        <xsl:if test="starts-with($line, $tag)">
          <xsl:value-of select="substring($line, string-length($tag)+1)"/>
        </xsl:if>
        <xsl:call-template name="extractTierData">
          <xsl:with-param name="text" select="$after"/>
          <xsl:with-param name="delim" select="$delim"/>
          <xsl:with-param name="tag" select="$tag"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="convert-time">
    <xsl:param name="time" select="'0'"/>

    <xsl:variable name="h" select="number(substring($time,1,2))"/>
    <xsl:variable name="m" select="number(substring($time,4,2))"/>
    <xsl:variable name="s" select="number(substring($time,7,6))"/>
    <!-- ss.msec -->

    <xsl:value-of select="format-number($h*3600 + $m*60 + $s,'0.000')"/>
  </xsl:template>

</xsl:stylesheet>
