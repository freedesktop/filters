/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: PropertyIds.hxx,v $
 *
 *  $Revision: 1.2 $
 *
 *  last change: $Author: os $ $Date: 2006-11-02 12:37:24 $
 *
 *  The Contents of this file are made available subject to
 *  the terms of GNU Lesser General Public License Version 2.1.
 *
 *
 *    GNU Lesser General Public License Version 2.1
 *    =============================================
 *    Copyright 2005 by Sun Microsystems, Inc.
 *    901 San Antonio Road, Palo Alto, CA 94303, USA
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License version 2.1, as published by the Free Software Foundation.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *    MA  02111-1307  USA
 *
 ************************************************************************/
#ifndef INCLUDED_DMAPPER_PROPERTYIDS_HXX
#define INCLUDED_DMAPPER_PROPERTYIDS_HXX

namespace rtl{ class OUString;}
namespace dmapper{
enum PropertyIds
{
        PROP_ID_START = 1
 /* 1*/ ,PROP_CHAR_WEIGHT = PROP_ID_START
 /* 2*/ ,PROP_CHAR_POSTURE
 /* 3*/ ,PROP_CHAR_STRIKEOUT
 /* 4*/ ,PROP_CHAR_CONTOURED
 /* 5*/ ,PROP_CHAR_SHADOWED
 /* 6*/ ,PROP_CHAR_CASEMAP
 /* 7*/ ,PROP_CHAR_COLOR
 /* 8*/ ,PROP_CHAR_RELIEF
 /* 9*/ ,PROP_CHAR_UNDERLINE
 /*10*/ ,PROP_CHAR_WORD_MODE
 /*11*/ ,PROP_CHAR_ESCAPEMENT
 /*12*/ ,PROP_CHAR_ESCAPEMENT_HEIGHT
 /*13*/ ,PROP_CHAR_HEIGHT
 /*14*/ ,PROP_CHAR_HEIGHT_COMPLEX
 /*15*/ ,PROP_CHAR_LOCALE
 /*16*/ ,PROP_CHAR_LOCALE_ASIAN
 /*17*/ ,PROP_CHAR_WEIGHT_COMPLEX
 /*18*/ ,PROP_CHAR_POSTURE_COMPLEX
 /*19*/ ,PROP_CHAR_AUTO_KERNING
 /*20*/ ,PROP_CHAR_CHAR_KERNING
 /*21*/ ,PROP_CHAR_SCALE_WIDTH
 /*22*/ ,PROP_CHAR_LOCALE_COMPLEX
 /*23*/ ,PROP_CHAR_UNDERLINE_COLOR
/*24*/ , PROP_CHAR_UNDERLINE_HAS_COLOR
/*25*/ , PROP_CHAR_FONT_NAME
/*26*/ , PROP_CHAR_FONT_STYLE
/*27*/ , PROP_CHAR_FONT_FAMILY
/*28*/ , PROP_CHAR_FONT_CHAR_SET
/*29*/ , PROP_CHAR_FONT_PITCH
/*30*/ , PROP_CHAR_FONT_NAME_ASIAN
/*31*/ , PROP_CHAR_FONT_STYLE_ASIAN
/*32*/ , PROP_CHAR_FONT_FAMILY_ASIAN
/*33*/ , PROP_CHAR_FONT_CHAR_SET_ASIAN
/*34*/ , PROP_CHAR_FONT_PITCH_ASIAN
/*35*/ , PROP_CHAR_FONT_NAME_COMPLEX
/*36*/ , PROP_CHAR_FONT_STYLE_COMPLEX
/*37*/ , PROP_CHAR_FONT_FAMILY_COMPLEX
/*38*/ , PROP_CHAR_FONT_CHAR_SET_COMPLEX
/*39*/ , PROP_CHAR_FONT_PITCH_COMPLEX
/*40*/ , PROP_CHAR_HIDDEN
/*41*/ , PROP_PARA_STYLE_NAME
/*42*/ , PROP_CHAR_STYLE_NAME
/*43*/ , PROP_PARA_ADJUST
/*44*/ , PROP_PARA_LAST_LINE_ADJUST
/*45*/ , PROP_PARA_RIGHT_MARGIN
/*46*/ , PROP_PARA_LEFT_MARGIN
/*47*/ , PROP_PARA_FIRST_LINE_INDENT
/*48*/ , PROP_PARA_KEEP_TOGETHER
/*49*/ , PROP_PARA_TOP_MARGIN
/*50*/ , PROP_PARA_BOTTOM_MARGIN
/*51*/ , PROP_PARA_IS_HYPHENATION
/*52*/ , PROP_PARA_LINE_NUMBER_COUNT
/*53*/ , PROP_PARA_IS_HANGING_PUNCTUATION
/*54*/ , PROP_PARA_LINE_SPACING
/*55*/ , PROP_PARA_TAB_STOPS
/*56*/ , PROP_NUMBERING_LEVEL
/*57*/ , PROP_NUMBERING_RULES
/*58*/ , PROP_NUMBERING_TYPE
/*59*/ , PROP_START_WITH
/*60*/ , PROP_ADJUST
/*61*/ , PROP_PARENT_NUMBERING
/*62*/ , PROP_LEFT_MARGIN
/*63*/ , PROP_RIGHT_MARGIN
/*64*/ , PROP_FIRST_LINE_OFFSET
/*65*/ , PROP_LEFT_BORDER
/*66*/ , PROP_RIGHT_BORDER
/*67*/ , PROP_TOP_BORDER
/*68*/ , PROP_BOTTOM_BORDER
/*69*/ , PROP_LEFT_BORDER_DISTANCE
/*70*/ , PROP_RIGHT_BORDER_DISTANCE
/*71*/ , PROP_TOP_BORDER_DISTANCE
/*72*/ , PROP_BOTTOM_BORDER_DISTANCE
/*73*/ , PROP_CURRENT_PRESENTATION
/*74*/ , PROP_IS_FIXED
/*75*/ , PROP_SUB_TYPE
/*76*/ , PROP_FILE_FORMAT
/*77*/ , PROP_HYPER_LINK_U_R_L
/*78*/ , PROP_NUMBER_FORMAT
/*79*/ , PROP_NAME
/*80*/ , PROP_IS_INPUT
/*81*/ , PROP_HINT
/*82*/ , PROP_FULL_NAME
/*83*/ , PROP_KEYWORDS
/*84*/ , PROP_DESCRIPTION
/*85*/ , PROP_MACRO_NAME
/*86*/ , PROP_SUBJECT
/*87*/ , PROP_USER_DATA_TYPE
/*88*/ , PROP_TITLE
/*89*/ , PROP_CONTENT
/*90*/ , PROP_DATA_COLUMN_NAME
///*91*/ , PROP_
///*92*/ , PROP_
///*93*/ , PROP_
///*94*/ , PROP_
///*95*/ , PROP_
///*96*/ , PROP_
///*97*/ , PROP_

};
struct PropertyNameSupplier_Impl;
class PropertyNameSupplier
{
    PropertyNameSupplier_Impl* m_pImpl;
public:
    PropertyNameSupplier();
    ~PropertyNameSupplier();
    const rtl::OUString& GetName( PropertyIds eId );

    static PropertyNameSupplier& GetPropertyNameSupplier();
};
} //namespace dmapper
#endif
