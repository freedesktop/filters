<?xml version="1.0" encoding="UTF-8"?>
<!--
   The Contents of this file are made available subject to the terms of
   either of the following licenses

          - GNU Lesser General Public License Version 2.1
          - Sun Industry Standards Source License Version 1.1

   Sun Microsystems Inc., October, 2000

   GNU Lesser General Public License Version 2.1
   =============================================
   Copyright 2000 by Sun Microsystems, Inc.
   901 San Antonio Road, Palo Alto, CA 94303, USA

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License version 2.1, as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston,
   MA  02111-1307  USA


   Sun Industry Standards Source License Version 1.1
   =================================================
   The contents of this file are subject to the Sun Industry Standards
   Source License Version 1.1 (the "License"); You may not use this file
   except in compliance with the License. You may obtain a copy of the
   License at http://www.openoffice.org/license.html.

   Software provided under this License is provided on an "AS IS" basis,
   WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING,
   WITHOUT LIMITATION, WARRANTIES THAT THE SOFTWARE IS FREE OF DEFECTS,
   MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE, OR NON-INFRINGING.
   See the License for the specific provisions governing your rights and
   obligations concerning the Software.

   The Initial Developer of the Original Code is: Sun Microsystems, Inc.

   Copyright © 2002 by Sun Microsystems, Inc.

   All Rights Reserved.

   Contributor(s): _______________________________________
   
 -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format" xmlns:w="http://schemas.microsoft.com/office/word/2003/wordml" xmlns:wx="http://schemas.microsoft.com/office/word/2003/auxHint" xmlns:o="urn:schemas-microsoft-com:office:office" xmlns:aml="http://schemas.microsoft.com/aml/2001/core" xmlns:dt="uuid:C2F41010-65B3-11d1-A29F-00AA00C14882" xmlns:v="urn:schemas-microsoft-com:vml" xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0" xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0" xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0" xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0" xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0" xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:meta="urn:oasis:names:tc:opendocument:xmlns:meta:1.0" xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0" xmlns:svg="http://www.w3.org/2000/svg" xmlns:chart="urn:oasis:names:tc:opendocument:xmlns:chart:1.0" xmlns:dr3d="urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0" xmlns:math="http://www.w3.org/1998/Math/MathML" xmlns:form="urn:oasis:names:tc:opendocument:xmlns:form:1.0" xmlns:script="urn:oasis:names:tc:opendocument:xmlns:script:1.0" xmlns:config="urn:oasis:names:tc:opendocument:xmlns:config:1.0" xmlns:ooo="http://openoffice.org/2004/office" xmlns:ooow="http://openoffice.org/2004/writer" xmlns:oooc="http://openoffice.org/2004/calc" xmlns:dom="http://www.w3.org/2001/xml-events" exclude-result-prefixes="w wx aml o dt  v">
    <xsl:output method="xml" indent="no" encoding="UTF-8" version="1.0"/>
    <xsl:include href="../../common/measure_conversion.xsl"/>
    <xsl:include href="../common/ms2ooo_docpr.xsl"/>
    <xsl:include href="wordml2ooo_text.xsl"/>
    <xsl:include href="wordml2ooo_settings.xsl"/>
    <xsl:include href="wordml2ooo_table.xsl"/>
    <xsl:include href="wordml2ooo_page.xsl"/>
    <xsl:include href="wordml2ooo_list.xsl"/>
    <xsl:include href="wordml2ooo_draw.xsl"/>
    <xsl:include href="wordml2ooo_field.xsl"/>
    <xsl:key name="paragraph-style" match="w:style[@w:type = 'paragraph']" use="@w:styleId"/>
    <xsl:key name="heading-style" match="w:style[@w:type = 'paragraph' and w:pPr/w:outlineLvl]" use="@w:styleId"/>
    <xsl:template match="/">
        <xsl:apply-templates select="w:wordDocument"/>
    </xsl:template>
    <xsl:template match="w:wordDocument">
        <office:document office:mimetype="application/x-vnd.oasis.openoffice.text" office:version="1.0">
            <xsl:apply-templates select="o:DocumentProperties"/>
            <xsl:apply-templates select="w:docPr"/>
            <xsl:apply-templates select="w:fonts"/>
            <xsl:apply-templates select="w:styles"/>
            <xsl:apply-templates select="w:body"/>
        </office:document>
    </xsl:template>
    <xsl:template match="w:fonts">
        <xsl:element name="office:font-face-decls">
            <!-- MS Word's default font declaration, added for Writer automatically. glu -->
            <style:font-face style:name="Arial" svg:font-family="Arial" style:font-family-generic="roman" style:font-pitch="variable"/>
            <style:font-face style:name="Times New Roman" svg:font-family="'Times New Roman'" style:font-family-generic="roman" style:font-pitch="variable"/>
            <style:font-face style:name="Symbol" svg:font-family="Symbol" style:font-family-generic="roman" style:font-pitch="variable" style:font-charset="x-symbol"/>
            <style:font-face style:name="Courier New" svg:font-family="'Courier New'" style:font-family-generic="modern" style:font-pitch="fixed"/>
            <xsl:if test="not(w:font[@w:name='StarSymbol'])">
                <style:font-face style:name="StarSymbol" svg:font-family="StarSymbol" style:font-charset="x-symbol"/>
            </xsl:if>
            <xsl:for-each select="w:font">
                <xsl:element name="style:font-face">
                    <xsl:attribute name="style:name"><xsl:value-of select="@w:name"/></xsl:attribute>
                    <xsl:attribute name="svg:font-family"><xsl:value-of select="@w:name"/></xsl:attribute>
                    <!-- added by glu, for process special fonts e.g. Marlett, -->
                    <xsl:if test="w:charset/@w:val = '02'">
                        <xsl:attribute name="style:font-charset">x-symbol</xsl:attribute>
                    </xsl:if>
                    <xsl:if test="w:family">
                        <xsl:choose>
                            <xsl:when test="w:family/@w:val = 'Swiss'">
                                <xsl:attribute name="style:font-family-generic">swiss</xsl:attribute>
                            </xsl:when>
                            <xsl:when test="w:family/@w:val='Modern'">
                                <xsl:attribute name="style:font-family-generic">modern</xsl:attribute>
                            </xsl:when>
                            <xsl:when test="w:family/@w:val='Roman'">
                                <xsl:attribute name="style:font-family-generic">roman</xsl:attribute>
                            </xsl:when>
                            <xsl:when test="w:family/@w:val='Script'">
                                <xsl:attribute name="style:font-family-generic">script</xsl:attribute>
                            </xsl:when>
                            <xsl:when test="w:family/@w:val='Decorative'">
                                <xsl:attribute name="style:font-family-generic">decorative</xsl:attribute>
                            </xsl:when>
                            <xsl:when test="w:family/@w:val='System'">
                                <xsl:attribute name="style:font-family-generic">system</xsl:attribute>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:attribute name="style:font-family-generic">system</xsl:attribute>
                            </xsl:otherwise>
                        </xsl:choose>
                    </xsl:if>
                    <xsl:if test="w:pitch and string-length(w:pitch/@w:val) &gt; 0">
                        <xsl:attribute name="style:font-pitch"><xsl:choose><xsl:when test="w:pitch/@w:val = 'default'">variable</xsl:when><xsl:otherwise><xsl:value-of select="w:pitch/@w:val"/></xsl:otherwise></xsl:choose></xsl:attribute>
                    </xsl:if>
                </xsl:element>
            </xsl:for-each>
        </xsl:element>
    </xsl:template>
    <xsl:template match="w:styles">
        <office:styles>
            <!--The next statement Added by wguo,collect the pict's dash and mark-style.The template is implemented in file wordml2ooo_draw.xsl-->
            <xsl:apply-templates select="/w:wordDocument/w:body//w:pict" mode="style4dash_mark"/>
            <xsl:apply-templates select="//v:fill" mode="office-style"/>
            <xsl:call-template name="create-default-paragraph-styles"/>
            <xsl:call-template name="create-default-text-styles"/>
            <xsl:call-template name="create-default-frame-style"/>
            <!-- StarWriter has no default style family 'list'. glu -->
            <xsl:if test="w:style[@w:type = 'paragraph' and w:pPr/w:outlineLvl and w:pPr/w:listPr]">
                <xsl:call-template name="create-outline-style"/>
            </xsl:if>
            <xsl:apply-templates select="w:style[@w:type='table']" mode="table"/>
            <xsl:apply-templates select="w:style[@w:type='list']" mode="list"/>
            <xsl:apply-templates select="w:style[@w:type!='list']"/>
            <xsl:apply-templates select="/w:wordDocument/w:docPr/w:footnotePr" mode="config"/>
            <xsl:apply-templates select="/w:wordDocument/w:docPr/w:endnotePr" mode="config"/>
        </office:styles>
        <office:automatic-styles>
            <xsl:apply-templates select="/w:wordDocument/w:body//w:p" mode="style"/>
            <xsl:apply-templates select="/w:wordDocument/w:body//w:rPr[not(parent::w:pPr)]" mode="style"/>
            <!--The next statement Added by wguo for the pict's draw-style.The template is implemented in file wordml2ooo_draw.xsl-->
            <xsl:apply-templates select="/w:wordDocument/w:body//w:pict" mode="style"/>
            <xsl:apply-templates select="/w:wordDocument/w:body//w:tblPr" mode="style"/>
            <xsl:apply-templates select="/w:wordDocument/w:body//w:gridCol" mode="style"/>
            <xsl:apply-templates select="/w:wordDocument/w:body//w:trPr" mode="style"/>
            <xsl:apply-templates select="/w:wordDocument/w:body//w:tcPr" mode="style"/>
            <xsl:apply-templates select="/w:wordDocument/w:body//w:listPr" mode="style"/>
            <xsl:apply-templates select="/w:wordDocument/w:body//w:sectPr" mode="page-layout"/>
            <xsl:call-template name="default_date_style"/>
            <!--add for generate the date , time style for date , time field  G.Y.-->
            <xsl:apply-templates select="/w:wordDocument/w:body//w:instrText | /w:wordDocument/w:body//w:fldSimple " mode="style"/>
        </office:automatic-styles>
        <office:master-styles>
            <xsl:apply-templates select="/w:wordDocument/w:body//w:sectPr" mode="master-page"/>
        </office:master-styles>
    </xsl:template>
    <xsl:template match="w:style">
        <style:style>
            <xsl:attribute name="style:name"><xsl:value-of select="concat('w',translate(@w:styleId,' ~`!@#$%^*(&#x26;)+/,;?&lt;&gt;{}[]:','_'))"/></xsl:attribute>
            <xsl:if test="w:basedOn">
                <xsl:attribute name="style:parent-style-name"><xsl:value-of select="concat('w',translate(w:basedOn/@w:val,' ~`!@#$%^*(&#x26;)+/,;?&lt;&gt;{}[]:','_'))"/></xsl:attribute>
            </xsl:if>
            <xsl:if test="w:next">
                <xsl:attribute name="style:next-style-name"><xsl:value-of select="concat('w',translate(w:basedOn/@w:val,' ~`!@#$%^*(&#x26;)+/,;?&lt;&gt;{}[]:','_'))"/></xsl:attribute>
            </xsl:if>
            <xsl:choose>
                <xsl:when test="@w:type = 'character'">
                    <xsl:attribute name="style:family">text</xsl:attribute>
                </xsl:when>
                <!-- table, paragraph are the same as in Writer . glu -->
                <xsl:when test="@w:type">
                    <xsl:attribute name="style:family"><xsl:value-of select="@w:type"/></xsl:attribute>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:attribute name="style:family">text</xsl:attribute>
                </xsl:otherwise>
            </xsl:choose>
            <xsl:choose>
                <xsl:when test="@w:type = 'table'">
                    <xsl:element name="style:table-properties">
                        <!-- xsl:apply-templates select="w:tblPr" mode="style"/ -->
                    </xsl:element>
                </xsl:when>
                <xsl:when test="@w:type = 'character' ">
                    <xsl:element name="style:text-properties">
                        <xsl:apply-templates select="w:pPr/w:rPr"/>
                        <xsl:apply-templates select="w:rPr"/>
                    </xsl:element>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:element name="style:paragraph-properties">
                        <xsl:apply-templates select="w:pPr"/>
                    </xsl:element>
                    <xsl:element name="style:text-properties">
                        <xsl:apply-templates select="w:rPr"/>
                        <xsl:apply-templates select="w:pPr/w:rPr"/>
                    </xsl:element>
                </xsl:otherwise>
            </xsl:choose>
        </style:style>
    </xsl:template>
    <xsl:template match="w:body">
        <xsl:element name="office:body">
            <xsl:element name="office:text">
                <!-- to add the sequece variable declaration at the begining of the office:body  G.Y.-->
                <text:sequence-decls>
                    <xsl:call-template name="default_seqence_declaration"/>
                    <xsl:apply-templates select="/w:wordDocument/w:body//w:instrText[substring(normalize-space(text()),1,3) = 'SEQ' ] | /w:wordDocument/w:body//w:fldSimple[substring(normalize-space(@w:instr),1,3) = 'SEQ' ]  " mode="sequence_declare"/>
                </text:sequence-decls>
                <!--  add the user field variables declare for Docpropety fields importing G.Y.-->
                <text:user-field-decls>
                    <xsl:call-template name="user_fields_declare_docproperty"/>
                </text:user-field-decls>
                <xsl:apply-templates select="wx:sect | w:p | w:tbl | w:sectPr"/>
            </xsl:element>
        </xsl:element>
    </xsl:template>
    <xsl:template match="wx:sect">
        <xsl:apply-templates select="wx:sub-section | w:p | w:tbl | w:sectPr"/>
    </xsl:template>
    <xsl:template match="wx:sub-section">
        <xsl:apply-templates select="wx:sub-section | w:p | w:tbl | w:sectPr"/>
    </xsl:template>
    <xsl:template name="create-default-frame-style">
        <!--add for default frame style -->
        <style:style style:name="Frame" style:family="graphic">
            <style:graphic-properties text:anchor-type="paragraph" svg:x="0in" svg:y="0in" style:wrap="parallel" style:number-wrapped-paragraphs="no-limit" style:wrap-contour="false" style:vertical-pos="top" style:vertical-rel="paragraph-content" style:horizontal-pos="center" style:horizontal-rel="paragraph-content"/>
        </style:style>
    </xsl:template>
    <xsl:template name="ConvertMeasure">
        <xsl:param name="TargetMeasure" select="'cm'"/>
        <xsl:param name="TargetTruncate" select=" 'all' "/>
        <xsl:param name="value"/>
        <!-- When TargetTruncate ='all'  it returns the number whichsoever the return value is negative or positive
               When TargetTruncate ='nonNegative' it only returns nonNegative number, all negative number to be returned as 0
               When TargetTruncate ='positive" it only returns positive number, all nonPositive number to be returned as 1 -->
        <xsl:variable name="return_value">
            <xsl:choose>
                <!-- remove the measure mark, if the value is null, the result should be 0. Must be the first case  -->
                <xsl:when test="string-length(translate($value,'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ','')) = 0">0</xsl:when>
                <xsl:when test="string-length(translate($value,'-.0123456789 ','')) = 0">
                    <xsl:value-of select="$value"/>
                </xsl:when>
                <xsl:when test="$TargetMeasure = 'cm'">
                    <xsl:call-template name="convert2cm">
                        <xsl:with-param name="value" select="$value"/>
                    </xsl:call-template>
                </xsl:when>
                <xsl:when test="$TargetMeasure = 'pt'">
                    <xsl:call-template name="convert2pt">
                        <xsl:with-param name="value" select="$value"/>
                    </xsl:call-template>
                </xsl:when>
                <xsl:when test="$TargetMeasure = 'twip'">
                    <xsl:call-template name="convert2twip">
                        <xsl:with-param name="value" select="$value"/>
                    </xsl:call-template>
                </xsl:when>
                <xsl:when test="$TargetMeasure = 'in'">
                    <xsl:call-template name="convert2in">
                        <xsl:with-param name="value" select="$value"/>
                    </xsl:call-template>
                </xsl:when>
            </xsl:choose>
        </xsl:variable>
        <xsl:choose>
            <xsl:when test="$TargetTruncate = 'all' ">
                <xsl:choose>
                    <xsl:when test="number($TargetMeasure) = 'NaN' ">
                        <xsl:value-of select=" '0' "/>
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:value-of select="$return_value"/>
                    </xsl:otherwise>
                </xsl:choose>
            </xsl:when>
            <xsl:when test="$TargetTruncate = 'nonNegative' ">
                <xsl:choose>
                    <xsl:when test="number($TargetMeasure) = 'NaN' ">
                        <xsl:value-of select=" '0' "/>
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:choose>
                            <xsl:when test=" $return_value &lt; 0  ">
                                <xsl:value-of select=" '0' "/>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:value-of select="$return_value"/>
                            </xsl:otherwise>
                        </xsl:choose>
                    </xsl:otherwise>
                </xsl:choose>
            </xsl:when>
            <xsl:when test="$TargetTruncate = 'positive' ">
                <xsl:choose>
                    <xsl:when test="number($TargetMeasure) = 'NaN' ">
                        <xsl:value-of select=" '1' "/>
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:choose>
                            <xsl:when test=" $return_value &lt;= 0 ">
                                <xsl:value-of select=" '1' "/>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:value-of select="$return_value"/>
                            </xsl:otherwise>
                        </xsl:choose>
                    </xsl:otherwise>
                </xsl:choose>
            </xsl:when>
        </xsl:choose>
    </xsl:template>
</xsl:stylesheet>
