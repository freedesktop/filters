/*************************************************************************
 *
 *  $RCSfile: txtpaint.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: hr $ $Date: 2004-08-03 17:58:28 $
 *
 *  The Contents of this file are made available subject to the terms of
 *  either of the following licenses
 *
 *         - GNU Lesser General Public License Version 2.1
 *         - Sun Industry Standards Source License Version 1.1
 *
 *  Sun Microsystems Inc., October, 2000
 *
 *  GNU Lesser General Public License Version 2.1
 *  =============================================
 *  Copyright 2000 by Sun Microsystems, Inc.
 *  901 San Antonio Road, Palo Alto, CA 94303, USA
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License version 2.1, as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *  MA  02111-1307  USA
 *
 *
 *  Sun Industry Standards Source License Version 1.1
 *  =================================================
 *  The contents of this file are subject to the Sun Industry Standards
 *  Source License Version 1.1 (the "License"); You may not use this file
 *  except in compliance with the License. You may obtain a copy of the
 *  License at http://www.openoffice.org/license.html.
 *
 *  Software provided under this License is provided on an "AS IS" basis,
 *  WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING,
 *  WITHOUT LIMITATION, WARRANTIES THAT THE SOFTWARE IS FREE OF DEFECTS,
 *  MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE, OR NON-INFRINGING.
 *  See the License for the specific provisions governing your rights and
 *  obligations concerning the Software.
 *
 *  The Initial Developer of the Original Code is: Sun Microsystems, Inc.
 *
 *  Copyright: 2000 by Sun Microsystems, Inc.
 *
 *  All Rights Reserved.
 *
 *  Contributor(s): _______________________________________
 *
 *
 ************************************************************************/
#ifndef _TXTPAINT_HXX
#define _TXTPAINT_HXX

// auto strip #ifndef _SV_REGION_HXX //autogen
// auto strip #include <vcl/region.hxx>
// auto strip #endif
#ifndef _SV_OUTDEV_HXX //autogen
#include <vcl/outdev.hxx>
#endif


class SwRect;				// SwSaveClip

#ifdef VERTICAL_LAYOUT
// auto strip #ifndef _TXTFRM_HXX
// auto strip #include <txtfrm.hxx>
// auto strip #endif
#endif
namespace binfilter {

/*************************************************************************
 *						class SwSaveClip
 *************************************************************************/

class SwSaveClip
{
    Region	 aClip;
    const sal_Bool	   bOn;
          sal_Bool	   bChg;
protected:
    OutputDevice *pOut;
//STRIP001 #ifdef VERTICAL_LAYOUT
//STRIP001     void _ChgClip( const SwRect &rRect, const SwTxtFrm* pFrm,
//STRIP001                    sal_Bool bEnlargeRect );
//STRIP001 #else
//STRIP001     void _ChgClip( const SwRect &rRect, sal_Bool bEnlargeRect );
//STRIP001 #endif
public:
    inline SwSaveClip( OutputDevice *pOut );
    inline ~SwSaveClip();
//STRIP001 #ifdef VERTICAL_LAYOUT
//STRIP001     inline void ChgClip( const SwRect &rRect, const SwTxtFrm* pFrm = 0,
//STRIP001                          sal_Bool bEnlargeRect = sal_False)
//STRIP001              { if( pOut ) _ChgClip( rRect, pFrm, bEnlargeRect ); }
//STRIP001 #else
//STRIP001     inline void ChgClip( const SwRect &rRect, sal_Bool bEnlargeRect = sal_False )
//STRIP001              { if( pOut ) _ChgClip( rRect, bEnlargeRect ); }
//STRIP001 #endif
    void Reset(){DBG_BF_ASSERT(0, "STRIP");} ;//STRIP001 		   void Reset();
    inline sal_Bool IsOn()	const { return bOn; }
    inline sal_Bool IsChg() const { return bChg; }
    inline sal_Bool IsOut() const { return 0 != pOut; }
    inline OutputDevice *GetOut() { return pOut; }
};

inline SwSaveClip::SwSaveClip( OutputDevice *pOut ) :
    pOut(pOut),
    bOn( pOut && pOut->IsClipRegion() ),
    bChg( sal_False )
{}

inline SwSaveClip::~SwSaveClip()
{
    Reset();
}

#ifndef PRODUCT

/*************************************************************************
 *							class SwDbgOut
 *************************************************************************/

class SwDbgOut
{
protected:
        OutputDevice *pOut;
public:
        inline SwDbgOut( OutputDevice *pOutDev, const sal_Bool bOn = sal_True );
};

/*************************************************************************
 *							class DbgPen
 *************************************************************************/

//class DbgPen : public SwDbgOut
//{
//		Pen aPen;
//public:
//		inline DbgPen( OutputDevice *pOutDev, const sal_Bool bOn = sal_True,
//					   const ColorName eColor = COL_BLACK );
//		inline ~DbgPen();
//};

/*************************************************************************
 *							class DbgColor
 *************************************************************************/

class DbgColor
{
    Font *pFnt;
    Color aColor;
public:
        inline DbgColor( Font *pFont, const sal_Bool bOn = sal_True,
                         const ColorData eColor = COL_BLUE );
        inline ~DbgColor();
};

/*************************************************************************
 *							class DbgBrush
 *************************************************************************/

class DbgBackColor : public SwDbgOut
{
        Color 	aOldFillColor;
public:
        DbgBackColor( OutputDevice *pOut, const sal_Bool bOn = sal_True,
                  ColorData nColor = COL_YELLOW );
       ~DbgBackColor();
};

/*************************************************************************
 *							class DbgRect
 *************************************************************************/

class DbgRect : public SwDbgOut
{
public:
        DbgRect( OutputDevice *pOut, const Rectangle &rRect,
                 const sal_Bool bOn = sal_True,
                 ColorData eColor = COL_LIGHTBLUE );
};

/*************************************************************************
 *						Inline-Implementierung
 *************************************************************************/

inline SwDbgOut::SwDbgOut( OutputDevice *pOutDev, const sal_Bool bOn )
               :pOut( bOn ? pOutDev : 0 )
{ }

//inline DbgPen::DbgPen( OutputDevice *pOutDev, const sal_Bool bOn,
//			   const ColorName eColor )
//	: SwDbgOut( pOutDev, bOn)
//{
//	if( pOut )
//	{
//		const Color aColor( eColor );
//		Pen aTmpPen( aColor );
//		aPen = pOut->GetPen( );
//		pOut->SetPen( aTmpPen );
//	}
//}

//inline DbgPen::~DbgPen()
//{
//	if( pOut )
//		pOut->SetPen(aPen);
//}

inline DbgColor::DbgColor( Font *pFont, const sal_Bool bOn,
                 const ColorData eColor )
    :pFnt( bOn ? pFont : 0 )
{
    if( pFnt )
    {
        aColor = pFnt->GetColor();
        pFnt->SetColor( Color( eColor ) );
    }
}

inline DbgColor::~DbgColor()
{
    if( pFnt )
        pFnt->SetColor( aColor );
}

inline DbgBackColor::DbgBackColor( OutputDevice *pOutDev, const sal_Bool bOn,
                           ColorData eColor )
    :SwDbgOut( pOutDev, bOn )
{
    if( pOut )
    {
        aOldFillColor = pOut->GetFillColor();
        pOut->SetFillColor( Color(eColor) );
    }
}

inline DbgBackColor::~DbgBackColor()
{
    if( pOut )
    {
        pOut->SetFillColor( aOldFillColor );
    }
}

inline DbgRect::DbgRect( OutputDevice *pOutDev, const Rectangle &rRect,
                         const sal_Bool bOn,
                         ColorData eColor )
    : SwDbgOut( pOutDev, bOn )
{
    if( pOut )
    {
        const Color aColor( eColor );
        Color aLineColor = pOut->GetLineColor();
        pOut->SetLineColor( aColor );
        Color aFillColor = pOut->GetFillColor();
        pOut->SetFillColor( Color(COL_TRANSPARENT) );
        pOut->DrawRect( rRect );
        pOut->SetLineColor( aLineColor );
        pOut->SetFillColor( aFillColor );
    }
}

#endif



} //namespace binfilter
#endif
