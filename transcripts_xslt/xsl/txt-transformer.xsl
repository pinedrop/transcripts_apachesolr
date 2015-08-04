<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

    <xml:output method="xml" encoding="UTF-8" indent="yes"/>

    <xsl:param name="file" select="'Even_Top09_Golikova_Slepcov-Conversation.txt'"/>
    <xsl:param name="tcuData" select="'ELANBegin|start, ELANEnd|end, ELANParticipant|speakers'"/>
    <xsl:param name="tierNames"
               select="'tx|ts_content_even, mb|ts_content_morph, ge|ts_content_igt, ft|ts_content_eng, ru|ts_content_rus'"/>

    <xsl:template match="/">
        <xsl:call-template name="toolbox"/>
    </xsl:template>

    <xsl:template name="toolbox">
        <xsl:choose>
            <xsl:when test="unparsed-text-available($file)">
                <xsl:variable name="tcus" select="unparsed-text($file, 'utf-8')"/>
                <xsl:variable name="tculevel" select="tokenize($tcuData, ',')"/>
                <xsl:variable name="tierlevel" select="tokenize($tierNames, ',')"/>
                <tcus>
                    <xsl:for-each select="tokenize($tcus, '\\block')[position()>1]">
                        <xsl:variable name="tcu" select="."/>

                        <tcu>
                            <xsl:for-each select="$tculevel">
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
                                                        <ss_speaker><xsl:value-of select="$meta"/></ss_speaker>
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
                                <xsl:for-each select="$tierlevel">
                                    <xsl:variable name="res">
                                        <xsl:call-template name="getTier">
                                            <xsl:with-param name="tcu" select="$tcu"/>
                                            <xsl:with-param name="tier"
                                                            select="normalize-space(substring-before(.,'|'))"/>
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
            </xsl:when>
            <xsl:otherwise>
                <error>
                    <xsl:text>Cannot locate : </xsl:text><xsl:value-of select="$file"/>
                </error>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <xsl:template name="getTier">
        <xsl:param name="tcu" select="''"/>
        <xsl:param name="tier" select="''"/>

        <xsl:variable name="return">
            <xsl:for-each select="tokenize($tcu, '\n')">
                <xsl:variable name="tag" select="concat('\', $tier, ' ')"/>
                <xsl:if test="starts-with(., $tag)">
                    <xsl:value-of select="substring(., string-length($tag)+1)"/>
                </xsl:if>
            </xsl:for-each>
        </xsl:variable>

        <xsl:value-of select="normalize-space($return)"/>
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