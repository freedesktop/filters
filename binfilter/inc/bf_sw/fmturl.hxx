/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: fmturl.hxx,v $
 *
 *  $Revision: 1.6 $
 *
 *  last change: $Author: rt $ $Date: 2008-03-12 13:41:49 $
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
#ifndef _FMTURL_HXX
#define _FMTURL_HXX


#ifndef _FORMAT_HXX //autogen
#include <format.hxx>
#endif
class IntlWrapper; 

namespace binfilter {
class ImageMap; 


// URL, ServerMap und ClientMap

class SwFmtURL: public SfxPoolItem
{
    String	  sTargetFrameName;	// in diesen Frame soll die URL
    String	  sURL;				//Einfache URL
    String	  sName;			// Name des Anchors
    ImageMap *pMap;				//ClientSide Images

    BOOL      bIsServerMap;		//mit der URL eine ServerSideImageMap

    SwFmtURL& operator=( const SwFmtURL& );

public:
    SwFmtURL();
    SwFmtURL( const SwFmtURL& );
    ~SwFmtURL();

    // "pure virtual Methoden" vom SfxPoolItem
    virtual int             operator==( const SfxPoolItem& ) const;
    virtual SfxPoolItem*	Clone( SfxItemPool* pPool = 0 ) const;
    virtual SfxPoolItem*	Create(SvStream &, USHORT nVer) const;
    virtual SvStream&		Store(SvStream &, USHORT nIVer) const;
    virtual USHORT			GetVersion( USHORT nFFVer ) const;
    virtual	BOOL        	 QueryValue( ::com::sun::star::uno::Any& rVal, BYTE nMemberId = 0 ) const;
    virtual	BOOL			 PutValue( const ::com::sun::star::uno::Any& rVal, BYTE nMemberId = 0 );

    void SetTargetFrameName( const String& rStr ) { sTargetFrameName = rStr; }
    void SetURL( const String &rURL, BOOL bServerMap );
    void SetMap( const ImageMap *pM );	//Pointer wird kopiert!

    const String   &GetTargetFrameName()const { return sTargetFrameName; }
    const String   &GetURL()			const { return sURL; }
          BOOL      IsServerMap()		const { return bIsServerMap; }
    const ImageMap *GetMap() 		    const { return pMap; }
          ImageMap *GetMap()				  { return pMap; }

    const String& GetName() const 			{ return sName; }
    void SetName( const String& rNm )		{ sName = rNm; }
};


#if ! (defined(MACOSX) && ( __GNUC__ < 3 ))
// GrP moved to gcc_outl.cxx; revisit with gcc3
inline const SwFmtURL &SwAttrSet::GetURL(BOOL bInP) const
    { return (const SwFmtURL&)Get( RES_URL,bInP); }

inline const SwFmtURL &SwFmt::GetURL(BOOL bInP) const
    { return aSet.GetURL(bInP); }
#endif

} //namespace binfilter
#endif

