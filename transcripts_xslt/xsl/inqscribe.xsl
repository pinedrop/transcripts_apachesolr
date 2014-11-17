<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="2.0">

   <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
    
    <!-- here's what an inqscribe sentence looks like:
    	<scene id="1" in="00:00:00.00" out="00:00:03.06">:མཆོད་མཆོད་མཆོད་ཕ་ལྷ་མ་ལྷ་མཆོད།།</scene>
    -->

   <xsl:template match="transcript">
      <tcus>
         <xsl:for-each select="scene">
            <xsl:variable name="sid" select="concat('s',format-number(@id,'000'))" />
            <xsl:variable name="begintime">
               <xsl:call-template name="convert-time">
                  <xsl:with-param name="time" select="@in" />
               </xsl:call-template>
            </xsl:variable>
            <xsl:variable name="endtime">
               <xsl:call-template name="convert-time">
                  <xsl:with-param name="time" select="@out" />
               </xsl:call-template>
            </xsl:variable>
            <tcu>
               <xsl:if test="normalize-space(@speaker)">
                  <speaker><xsl:value-of select="@speaker"/></speaker>
               </xsl:if>
               <start><xsl:value-of select="normalize-space($begintime)"/></start>
               <end><xsl:value-of select="normalize-space($endtime)"/></end>
               <tiers>
                  <xsl:variable name="bod" select="normalize-space(replace(.,'[^\p{IsTibetan}\s]+',''))" />
                  <xsl:if test="$bod">
                     <content_bod><xsl:value-of select="$bod"/></content_bod>
                  </xsl:if>
                  <xsl:variable name="eng" select="normalize-space(replace(.,'[\p{IsTibetan}]+',''))" />
                  <xsl:if test="$eng">
                     <ts_content_eng><xsl:value-of select="$eng"/></ts_content_eng>
                  </xsl:if>
               </tiers>
            </tcu>
         </xsl:for-each>
      </tcus>
   </xsl:template>

   <xsl:template name="convert-time">
      <xsl:param name="time" select="'0'" />

      <xsl:variable name="h" select="number(substring($time,1,2))" />
      <xsl:variable name="m" select="number(substring($time,4,2))" />
      <xsl:variable name="s" select="number(substring($time,7,2))" />
      <xsl:variable name="f" select="number(substring($time,10,2))" /> <!-- assume 30 fps -->

      <xsl:value-of select="format-number($h*3600 + $m*60 + $s + $f div 30,'0.000')" />

   </xsl:template>

</xsl:stylesheet>
