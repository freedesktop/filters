/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: PropertyIds.hxx,v $
 *
 *  $Revision: 1.7 $
 *
 *  last change: $Author: os $ $Date: 2006-12-28 09:18:19 $
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
/*31*/ , PROP_CHAR_HEIGHT_ASIAN
/*32*/ , PROP_CHAR_FONT_STYLE_ASIAN
/*33*/ , PROP_CHAR_FONT_FAMILY_ASIAN
/*34*/ , PROP_CHAR_FONT_CHAR_SET_ASIAN
/*35*/ , PROP_CHAR_FONT_PITCH_ASIAN
/*36*/ , PROP_CHAR_FONT_NAME_COMPLEX
/*37*/ , PROP_CHAR_FONT_STYLE_COMPLEX
/*38*/ , PROP_CHAR_FONT_FAMILY_COMPLEX
/*39*/ , PROP_CHAR_FONT_CHAR_SET_COMPLEX
/*40*/ , PROP_CHAR_FONT_PITCH_COMPLEX
/*41*/ , PROP_CHAR_HIDDEN
/*42*/ , PROP_CHAR_WEIGHT_ASIAN
/*43*/ , PROP_CHAR_POSTURE_ASIAN
/*44*/ , PROP_PARA_STYLE_NAME
/*45*/ , PROP_CHAR_STYLE_NAME
/*46*/ , PROP_PARA_ADJUST
/*47*/ , PROP_PARA_LAST_LINE_ADJUST
/*48*/ , PROP_PARA_RIGHT_MARGIN
/*49*/ , PROP_PARA_LEFT_MARGIN
/*50*/ , PROP_PARA_FIRST_LINE_INDENT
/*51*/ , PROP_PARA_KEEP_TOGETHER
/*52*/ , PROP_PARA_TOP_MARGIN
/*53*/ , PROP_PARA_BOTTOM_MARGIN
/*54*/ , PROP_PARA_IS_HYPHENATION
/*55*/ , PROP_PARA_LINE_NUMBER_COUNT
/*56*/ , PROP_PARA_IS_HANGING_PUNCTUATION
/*57*/ , PROP_PARA_LINE_SPACING
/*58*/ , PROP_PARA_TAB_STOPS
/*59*/ , PROP_PARA_WIDOWS
/*60*/ , PROP_PARA_ORPHANS
/*61*/ , PROP_NUMBERING_LEVEL
/*62*/ , PROP_NUMBERING_RULES
/*63*/ , PROP_NUMBERING_TYPE
/*64*/ , PROP_START_WITH
/*65*/ , PROP_ADJUST
/*66*/ , PROP_PARENT_NUMBERING
/*67*/ , PROP_LEFT_MARGIN
/*68*/ , PROP_RIGHT_MARGIN
/*69*/ , PROP_TOP_MARGIN
/*70*/ , PROP_BOTTOM_MARGIN
/*71*/ , PROP_FIRST_LINE_OFFSET
/*72*/ , PROP_LEFT_BORDER
/*73*/ , PROP_RIGHT_BORDER
/*74*/ , PROP_TOP_BORDER
/*75*/ , PROP_BOTTOM_BORDER
/*76*/ , PROP_LEFT_BORDER_DISTANCE
/*77*/ , PROP_RIGHT_BORDER_DISTANCE
/*78*/ , PROP_TOP_BORDER_DISTANCE
/*79*/ , PROP_BOTTOM_BORDER_DISTANCE
/*80*/ , PROP_CURRENT_PRESENTATION
/*81*/ , PROP_IS_FIXED
/*82*/ , PROP_SUB_TYPE
/*83*/ , PROP_FILE_FORMAT
/*84*/ , PROP_HYPER_LINK_U_R_L
/*85*/ , PROP_NUMBER_FORMAT
/*86*/ , PROP_NAME
/*87*/ , PROP_IS_INPUT
/*88*/ , PROP_HINT
/*89*/ , PROP_FULL_NAME
/*90*/ , PROP_KEYWORDS
/*91*/ , PROP_DESCRIPTION
/*92*/ , PROP_MACRO_NAME
/*93*/ , PROP_SUBJECT
/*94*/ , PROP_USER_DATA_TYPE
/*95*/ , PROP_TITLE
/*96*/ , PROP_CONTENT
/*97*/ , PROP_DATA_COLUMN_NAME
/*98*/ , PROP_INPUT_STREAM
/*99*/ , PROP_GRAPHIC
/*100*/, PROP_ANCHOR_TYPE
/*101*/, PROP_SIZE
/*102*/ ,PROP_HORI_ORIENT
/*103*/ ,PROP_HORI_ORIENT_POSITION
/*104*/ ,PROP_HORI_ORIENT_RELATION
/*105*/ ,PROP_VERT_ORIENT
/*106*/ ,PROP_VERT_ORIENT_POSITION
/*107*/ ,PROP_VERT_ORIENT_RELATION
/*108*/ ,PROP_GRAPHIC_CROP
/*109*/ ,PROP_SIZE100th_M_M
/*110*/ ,PROP_SIZE_PIXEL
/*111*/ ,PROP_SURROUND
/*112*/ ,PROP_SURROUND_CONTOUR
/*113*/ ,PROP_ADJUST_CONTRAST
/*114*/ ,PROP_ADJUST_LUMINANCE
/*115*/ ,PROP_GRAPHIC_COLOR_MODE
/*116*/ ,PROP_GAMMA
/*117*/ ,PROP_HORI_MIRRORED_ON_EVEN_PAGES
/*118*/ ,PROP_HORI_MIRRORED_ON_ODD_PAGES
/*119*/ ,PROP_VERT_MIRRORED
/*120*/ ,PROP_CONTOUR_OUTSIDE
/*121*/ ,PROP_CONTOUR_POLY_POLYGON
/*122*/ ,PROP_PAGE_TOGGLE
/*123*/ ,PROP_BACK_COLOR
/*124*/ ,PROP_ALTERNATIVE_TEXT
/*125*/ ,PROP_HEADER_TEXT_LEFT
/*126*/ ,PROP_HEADER_TEXT
/*127*/ ,PROP_HEADER_IS_SHARED
/*128*/ ,PROP_HEADER_IS_ON
/*129*/ ,PROP_FOOTER_TEXT_LEFT
/*130*/ ,PROP_FOOTER_TEXT
/*131*/ ,PROP_FOOTER_IS_SHARED
/*132*/ ,PROP_FOOTER_IS_ON
/*133*/ ,PROP_WIDTH
/*134*/ ,PROP_HEIGHT
/*135*/ ,PROP_SEPARATOR_LINE_IS_ON
/*136*/ ,PROP_TEXT_COLUMNS
/*137*/ ,PROP_AUTOMATIC_DISTANCE
/*138*/ ,PROP_IS_LANDSCAPE
/*139*/ ,PROP_PRINTER_PAPER_TRAY_INDEX
/*140*/ ,PROP_FIRST_PAGE
/*141*/ ,PROP_DEFAULT
/*142*/ ,PROP_PAGE_DESC_NAME
/*143*/ ,PROP_PAGE_NUMBER_OFFSET
/*144*/ ,PROP_BREAK_TYPE
/*145*/ ,PROP_FOOTER_IS_DYNAMIC_HEIGHT
/*146*/ ,PROP_FOOTER_DYNAMIC_SPACING
/*147*/ ,PROP_FOOTER_HEIGHT
/*148*/ ,PROP_FOOTER_BODY_DISTANCE
/*149*/ ,PROP_HEADER_IS_DYNAMIC_HEIGHT
/*150*/ ,PROP_HEADER_DYNAMIC_SPACING
/*151*/ ,PROP_HEADER_HEIGHT
/*152*/ ,PROP_HEADER_BODY_DISTANCE
/*153*/ ,PROP_WRITING_MODE
/*154*/ ,PROP_GRID_MODE
/*155*/ ,PROP_GRID_DISPLAY
/*156*/ ,PROP_GRID_PRINT
/*157*/ ,PROP_ADD_EXTERNAL_LEADING
/*158*/ ,PROP_GRID_LINES
/*159*/ ,PROP_GRID_BASE_HEIGHT
/*160*/ ,PROP_GRID_RUBY_HEIGHT
///*161*/ , PROP_
///*162*/ , PROP_
///*163*/ , PROP_
///*164*/ , PROP_
///*165*/ , PROP_
///*166*/ , PROP_
///*167*/ , PROP_
///*168*/ , PROP_
///*169*/ , PROP_
///*170*/ , PROP_
///*171*/ , PROP_
///*172*/ , PROP_
///*173*/ , PROP_
///*174*/ , PROP_
///*175*/ , PROP_
///*176*/ , PROP_
///*177*/ , PROP_
///*178*/ , PROP_
///*179*/ , PROP_



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
