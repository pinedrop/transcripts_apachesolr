<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

    <xml:output method="xml" encoding="UTF-8"/>

    <xsl:param name="file" select="'transcript.srt'"/>
    <xsl:param name="tier_id" select="'ts_content_und'"/>
    <xsl:param name="speaker_display_id" select="'ss_speaker'"/>

    <xsl:template match="/">
        <xsl:call-template name="srt"/>
    </xsl:template>

    <xsl:template name="srt">
        <xsl:choose>
            <xsl:when test="unparsed-text-available($file)">
                <xsl:variable name="data"
                              select="replace(replace(unparsed-text($file, 'utf-8'), '&#xD;&#x0a;', '&#x0a;'), '&#xD;', '&#x0a;')"/>
                <xsl:variable name="tcus">
                        <xsl:for-each select="tokenize($data, '&#x0a;{2,}')">
                            <xsl:variable name="tcu" select="substring-after(., '&#x0a;')"/> <!-- ignore id line -->
                            <xsl:if test="normalize-space($tcu) != ''">
                                <tcu>
                                    <xsl:variable name="times" select="substring-before($tcu, '&#x0a;')"/>
                                    <xsl:variable name="text"
                                                  select="normalize-space(substring-after($tcu, '&#x0a;'))"/>
                                    <start>
                                        <xsl:call-template name="convert-time">
                                            <xsl:with-param name="time"
                                                            select="normalize-space(substring-before($times, '--&gt;'))"/>
                                        </xsl:call-template>
                                    </start>
                                    <end>
                                        <xsl:call-template name="convert-time">
                                            <xsl:with-param name="time"
                                                            select="normalize-space(substring-after($times, '--&gt;'))"/>
                                        </xsl:call-template>
                                    </end>
                                    <xsl:choose>
                                        <xsl:when test="starts-with($text, '&gt;&gt;')">
                                            <speakers>
                                                <xsl:element name="{$speaker_display_id}">
                                                    <xsl:value-of
                                                            select="normalize-space(substring(substring-before($text, ':'), 3))"/>
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
                        </xsl:for-each>
                </xsl:variable>
                <tcus>
                    <xsl:for-each select="$tcus/tcu">
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
            </xsl:when>
            <xsl:otherwise>
                <error>
                    <xsl:text>Cannot locate : </xsl:text><xsl:value-of select="$file"/>
                </error>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <xsl:template name="convert-time">
        <xsl:param name="time" select="'0'"/>

        <!-- HH:MM:SS,MIL (hours, minutes, seconds, milliseconds) -->
        <xsl:choose>
            <xsl:when test="matches($time, '^[0-9]{2}:[0-9]{2}:[0-9]{2},[0-9]{3}$')"> <!-- milliseconds -->
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