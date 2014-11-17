<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet version="2.0"	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    
    <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
    
    <xsl:template match="/">
        <xsl:apply-templates select="TITLE"/>
    </xsl:template>
    
    <xsl:template match="TITLE">
        <tcus>
            <xsl:apply-templates select="TEXT/S"/>
        </tcus>
    </xsl:template>

    
<!-- Here's what a transcript sentence looks like:

      <S who="N400005" id="d148e29">
         <FORM xml:lang="bo">དེ་རིང་གནམ་ཡག་པོ་ར་ཅིག་༿འདྲ་ཅིག༾མི་འདུག་གས།</FORM>
         <FORM xml:lang="bo-Latn">de ring gnam yag po ra cig {'dra cig}mi 'dug gas/</FORM>
         <TRANSL xml:lang="en">Isn't it a nice day today?</TRANSL>
         <TRANSL xml:lang="zh">今天的天气多好啊, 是吧!</TRANSL>
         <AUDIO end="8.925999997392298" start="7.63"/>
      </S>
-->

    <xsl:template match="S">
        <xsl:variable name="sid" select="@id"/>
        <tcu>
        	<speaker><xsl:value-of select="substring(//SPEAKER[@personId=current()/@who]/name_bod,1,255)"/></speaker>
        	<xsl:if test="string(AUDIO/@start)">
        	   <start><xsl:value-of select="format-number(AUDIO/@start,'0.000')"/></start>
        	   <xsl:if test="string(AUDIO/@end)">
        	       <end><xsl:value-of select="format-number(AUDIO/@end,'0.000')"/></end>
        	   </xsl:if>
        	</xsl:if>
        	<tiers>
        	    <xsl:if test="normalize-space(FORM[@xml:lang='bo'])">
                    <content_bod><xsl:value-of select="normalize-space(FORM[@xml:lang='bo'])"/></content_bod>
                </xsl:if>
                <xsl:if test="normalize-space(FORM[@xml:lang='bo-Latn'])">
                    <ts_content_wylie><xsl:value-of select="normalize-space(FORM[@xml:lang='bo-Latn'])"/></ts_content_wylie>
                </xsl:if>
                <xsl:if test="normalize-space(TRANSL[@xml:lang='en'])">
                    <ts_content_eng><xsl:value-of select="normalize-space(TRANSL[@xml:lang='en'])"/></ts_content_eng>
                </xsl:if>
                <xsl:if test="normalize-space(TRANSL[@xml:lang='zh'])">
                    <ts_content_zho><xsl:value-of select="normalize-space(TRANSL[@xml:lang='zh'])"/></ts_content_zho>
                </xsl:if>
        	</tiers>
        </tcu>
    </xsl:template>

</xsl:stylesheet>
