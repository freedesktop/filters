/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: tblrwcl.hxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: rt $ $Date: 2005-09-08 09:40:34 $
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
#ifndef _TBLRWCL_HXX
#define _TBLRWCL_HXX

#ifndef _SVSTDARR_HXX
#define _SVSTDARR_USHORTS
#include <svtools/svstdarr.hxx>
#endif

#ifndef _SWTYPES_HXX
#include <swtypes.hxx>
#endif
#ifndef _TBLSEL_HXX
#include <tblsel.hxx>
#endif
#ifndef _SWTABLE_HXX
#include <swtable.hxx>
#endif
class SfxPoolItem; 
namespace binfilter {

class SvxBorderLine;
class SwDoc;
class SwTableNode;
class _FndLine;
class _FndBox;
class SwTableLine;
class SwTableBox;
class SwTableBoxFmt;
class SwTableLineFmt;
class SwHistory;
class SwCntntNode;
 
class SwShareBoxFmts;
class SwFmtFrmSize;

// Funktions Deklarationen:
BOOL lcl_CopyRow( const _FndLine*& rpFndLine, void* pPara );
BOOL lcl_CopyCol( const _FndBox*& rpFndBox, void* pPara );

BOOL lcl_MergeGCBox( const SwTableBox*& rpBox, void* pPara );
BOOL lcl_MergeGCLine( const SwTableLine*& rpLine, void* pPara );

BOOL lcl_Merge_MoveBox( const _FndBox*& rpFndBox, void* pPara );
BOOL lcl_Merge_MoveLine( const _FndLine*& rpFndLine, void* pPara );

BOOL lcl_CopyBoxToDoc( const _FndBox*& rpFndBox, void* pPara );
BOOL lcl_CopyLineToDoc( const _FndLine*& rpFndLn, void* pPara );

BOOL lcl_BoxSetHeadCondColl( const SwTableBox*& rpBox, void* pPara );
BOOL lcl_LineSetHeadCondColl( const SwTableLine*& rpLine, void* pPara );


#ifndef PRODUCT
//STRIP001 void _CheckBoxWidth( const SwTableLine& rLine, SwTwips nSize );
#endif

void _InsTblBox( SwDoc* pDoc, SwTableNode* pTblNd,
                SwTableLine* pLine, SwTableBoxFmt* pBoxFrmFmt,
                SwTableBox* pBox, USHORT nInsPos, USHORT nCnt = 1 );

void _DeleteBox( SwTable& rTbl, SwTableBox* pBox, SwUndo* pUndo = 0,
                BOOL bCalcNewSize = TRUE, const BOOL bCorrBorder = TRUE,
                SwShareBoxFmts* pShareFmts = 0 );

// Klasse fuers SplitTable
// sammelt von einer Line die Boxen aller obersten oder untersten Lines
// in einem Array. Zusaetzlich werden die Positionen vermerkt.
// ( die Implementierung steht im ndtbl.cxx)

//STRIP001 class SwCollectTblLineBoxes
//STRIP001 {
//STRIP001 	SvUShorts aPosArr;
//STRIP001 	SwSelBoxes_SAR aBoxes;
//STRIP001 	SwHistory* pHst;
//STRIP001 	USHORT nMode, nWidth;
//STRIP001 	BOOL bGetFromTop : 1;
//STRIP001 	BOOL bGetValues : 1;
//STRIP001 
//STRIP001 public:
//STRIP001 	SwCollectTblLineBoxes( BOOL bTop, USHORT nMd = 0, SwHistory* pHist=0 )
//STRIP001 		: aPosArr( 16, 16 ), aBoxes( 16, 16 ),
//STRIP001 		bGetFromTop( bTop ), bGetValues( TRUE ), nMode( nMd ),
//STRIP001 		nWidth( 0 ), pHst( pHist )
//STRIP001 	{}
//STRIP001 
//STRIP001 	void AddBox( const SwTableBox& rBox );
//STRIP001 	const SwTableBox* GetBoxOfPos( const SwTableBox& rBox );
//STRIP001 	void AddToUndoHistory( const SwCntntNode& rNd );
//STRIP001 
//STRIP001 	USHORT Count() const				{ return aBoxes.Count(); }
//STRIP001 	const SwTableBox& GetBox( USHORT nPos, USHORT* pWidth = 0 ) const
//STRIP001 		{
//STRIP001 			// hier wird die EndPos der Spalte benoetigt!
//STRIP001 			if( pWidth )
//STRIP001 				*pWidth = nPos+1 == aPosArr.Count() ? nWidth
//STRIP001 													: aPosArr[ nPos+1 ];
//STRIP001 			return *aBoxes[ nPos ];
//STRIP001 		}
//STRIP001 
//STRIP001 	BOOL IsGetFromTop() const 			{ return bGetFromTop; }
//STRIP001 	BOOL IsGetValues() const			{ return bGetValues; }
//STRIP001 
//STRIP001 	USHORT GetMode() const				{ return nMode; }
//STRIP001 	void SetValues( BOOL bFlag )		{ bGetValues = FALSE; nWidth = 0;
//STRIP001 										  bGetFromTop = bFlag; }
//STRIP001 	FASTBOOL Resize( USHORT nOffset, USHORT nWidth );
//STRIP001 };

//STRIP001 BOOL lcl_Box_CollectBox( const SwTableBox*& rpBox, void* pPara );
//STRIP001 BOOL lcl_Line_CollectBox( const SwTableLine*& rpLine, void* pPara );

//STRIP001 BOOL lcl_BoxSetSplitBoxFmts( const SwTableBox*& rpBox, void* pPara );




struct _SwGCLineBorder
{
    const SwTableLines* pLines;
    SwShareBoxFmts* pShareFmts;
    USHORT nLinePos;

    _SwGCLineBorder( const SwTable& rTable )
        : pLines( &rTable.GetTabLines() ), nLinePos( 0 ), pShareFmts(0) {}

    _SwGCLineBorder( const SwTableBox& rBox )
        : pLines( &rBox.GetTabLines() ), nLinePos( 0 ), pShareFmts(0) {}
    BOOL IsLastLine() const { return nLinePos + 1 >= pLines->Count(); }
};

class _SwGCBorder_BoxBrd
{
    const SvxBorderLine* pBrdLn;
    BOOL bAnyBorderFnd;
public:
    _SwGCBorder_BoxBrd() : pBrdLn( 0 ), bAnyBorderFnd( FALSE ) {}

    void SetBorder( const SvxBorderLine& rBorderLine )
        { pBrdLn = &rBorderLine; bAnyBorderFnd = FALSE; }

    // checke, ob die linke Border dieselbe wie die gesetzte ist
    // returnt FALSE falls gar keine Border gesetzt ist
    BOOL CheckLeftBorderOfFormat( const SwFrmFmt& rFmt );

    BOOL IsAnyBorderFound() const { return bAnyBorderFnd; }
};

BOOL lcl_GC_Line_Border( const SwTableLine*& , void* pPara );
BOOL lcl_GC_Box_Border( const SwTableBox*& , void* pPara );

BOOL lcl_GCBorder_ChkBoxBrd_L( const SwTableLine*& , void* pPara );
BOOL lcl_GCBorder_ChkBoxBrd_B( const SwTableBox*& , void* pPara );

BOOL lcl_GCBorder_GetLastBox_L( const SwTableLine*& , void* pPara );
BOOL lcl_GCBorder_GetLastBox_B( const SwTableBox*& , void* pPara );


class SwShareBoxFmt
{
    const SwFrmFmt* pOldFmt;
    SvPtrarr aNewFmts;

public:
    SwShareBoxFmt( const SwFrmFmt& rFmt )
        : pOldFmt( &rFmt ), aNewFmts( 1, 4 )
    {}

    const SwFrmFmt& GetOldFormat() const { return *pOldFmt; }

//STRIP001 	SwFrmFmt* GetFormat( long nWidth ) const;
//STRIP001 	SwFrmFmt* GetFormat( const SfxPoolItem& rItem ) const;
//STRIP001 	void AddFormat( const SwFrmFmt& rFmt );
//STRIP001 	// returnt TRUE, wenn geloescht werden kann
//STRIP001 	FASTBOOL RemoveFormat( const SwFrmFmt& rFmt );
};


SV_DECL_PTRARR_DEL( _SwShareBoxFmts, SwShareBoxFmt*, 8, 8 )

class SwShareBoxFmts
{
    _SwShareBoxFmts aShareArr;
    BOOL Seek_Entry( const SwFrmFmt& rFmt, USHORT* pPos ) const;

//STRIP001 	void ChangeFrmFmt( SwTableBox* pBox, SwTableLine* pLn, SwFrmFmt& rFmt );
//STRIP001 
public:
    SwShareBoxFmts() {}
    ~SwShareBoxFmts();

//STRIP001 	SwFrmFmt* GetFormat( const SwFrmFmt& rFmt, long nWidth ) const;
//STRIP001 	SwFrmFmt* GetFormat( const SwFrmFmt& rFmt, const SfxPoolItem& ) const;
//STRIP001 
//STRIP001 	void AddFormat( const SwFrmFmt& rOld, const SwFrmFmt& rNew );
//STRIP001 
//STRIP001 	void SetSize( SwTableBox& rBox, const SwFmtFrmSize& rSz );
//STRIP001 	void SetAttr( SwTableBox& rBox, const SfxPoolItem& rItem );
//STRIP001 	void SetAttr( SwTableLine& rLine, const SfxPoolItem& rItem );
//STRIP001 
//STRIP001 	void RemoveFormat( const SwFrmFmt& rFmt );
};



} //namespace binfilter
#endif
