/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: docary.hxx,v $
 *
 *  $Revision: 1.8 $
 *
 *  last change: $Author: rt $ $Date: 2008-03-12 13:36:15 $
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
#ifndef _DOCARY_HXX
#define _DOCARY_HXX

namespace binfilter {//STRIP009
class SwFieldType;
class SwFrmFmt;
class SwCharFmt;
class SwBookmark;
class SwTOXType;
class SwUndo;
class SwSectionFmt;
class SwNumRule;
class SwRedline;
class SwUnoCrsr;
class SwOLENode;
} //namespace binfilter

#ifdef sun
#undef sun
#endif

namespace com { namespace sun { namespace star { namespace i18n {
    struct ForbiddenCharacters;    // comes from the I18N UNO interface
}}}}


#ifndef _SWTYPES_HXX //autogen
#include <swtypes.hxx>
#endif

#ifndef _SVARRAY_HXX //autogen
#include <bf_svtools/svarray.hxx>
#endif
namespace binfilter {

typedef SwFieldType* SwFldTypePtr;
#define GROW_FLDTYPES	16

//PageDescriptor-Schnittstelle
//typedef SwPageDesc * SwPageDescPtr;
//SV_DECL_PTRARR_DEL(SwPageDescs, SwPageDescPtr,1,1)//STRIP008 ;

typedef SwFrmFmt* SwFrmFmtPtr;
SV_DECL_PTRARR_DEL(SwFrmFmts,SwFrmFmtPtr,4,4)

//Spezifische Frameformate (Rahmen, DrawObjecte)
SV_DECL_PTRARR_DEL(SwSpzFrmFmts,SwFrmFmtPtr,0,4)

typedef SwCharFmt* SwCharFmtPtr;
SV_DECL_PTRARR_DEL(SwCharFmts,SwCharFmtPtr,4,4)

SV_DECL_PTRARR_DEL( SwFldTypes, SwFldTypePtr, INIT_FLDTYPES, GROW_FLDTYPES )

//Bookmarks (nach Dokumentpositionen sortiertes Array)
typedef SwBookmark* SwBookmarkPtr;
SV_DECL_PTRARR_SORT(SwBookmarks, SwBookmarkPtr,0,1)

typedef SwTOXType* SwTOXTypePtr;
SV_DECL_PTRARR_DEL( SwTOXTypes, SwTOXTypePtr, 0, 1 )

// Undo
#define INIT_UNDOS 5
#define GROW_UNDOS 5
// Das Array der Undo-History
typedef SwUndo* SwUndoPtr;
SV_DECL_PTRARR_DEL( SwUndos, SwUndoPtr, INIT_UNDOS, GROW_UNDOS )

typedef SwSectionFmt* SwSectionFmtPtr;
SV_DECL_PTRARR_DEL(SwSectionFmts,SwSectionFmtPtr,0,4)


typedef SwNumRule* SwNumRulePtr;
SV_DECL_PTRARR_DEL( SwNumRuleTbl, SwNumRulePtr, 0, 5 )

typedef SwRedline* SwRedlinePtr;
SV_DECL_PTRARR_SORT_DEL( _SwRedlineTbl, SwRedlinePtr, 0, 16 )

class SwRedlineTbl : private _SwRedlineTbl
{
public:
    SwRedlineTbl( BYTE nSize = 0, BYTE nG = 16 )
        : _SwRedlineTbl( nSize, nG ) {}
    ~SwRedlineTbl() {}

    BOOL SavePtrInArr( SwRedlinePtr p ) { return _SwRedlineTbl::Insert( p ); }

/*N*/ 	BOOL Insert( SwRedlinePtr& p, BOOL bIns = TRUE ); //SW50.SDW
/*N*/ 	BOOL InsertWithValidRanges( SwRedlinePtr& p, USHORT* pInsPos = 0 ); //SW50.SDW

/*N*/ 	void Remove( USHORT nP, USHORT nL = 1 ); //SW50.SDW

    // suche den naechsten oder vorherigen Redline mit dergleichen Seq.No
    // Mit dem Lookahead kann die Suche eingeschraenkt werden. 0 oder
    // USHRT_MAX suchen im gesamten Array.

    _SwRedlineTbl::Count;
    _SwRedlineTbl::operator[];
    _SwRedlineTbl::GetObject;
    _SwRedlineTbl::Seek_Entry;
    _SwRedlineTbl::GetPos;
};

typedef SwUnoCrsr* SwUnoCrsrPtr;
SV_DECL_PTRARR_DEL( SwUnoCrsrTbl, SwUnoCrsrPtr, 0, 4 )

typedef SwOLENode* SwOLENodePtr;
SV_DECL_PTRARR(SwOLENodes,SwOLENodePtr,16,16)


} //namespace binfilter
#endif	//_DOCARY_HXX

