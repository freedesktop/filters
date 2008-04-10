/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008 by Sun Microsystems, Inc.
 *
 * OpenOffice.org - a multi-platform office productivity suite
 *
 * $RCSfile: sfx2_docfilt.cxx,v $
 * $Revision: 1.11 $
 *
 * This file is part of OpenOffice.org.
 *
 * OpenOffice.org is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 3
 * only, as published by the Free Software Foundation.
 *
 * OpenOffice.org is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Lesser General Public License
 * version 3 along with OpenOffice.org.  If not, see
 * <http://www.openoffice.org/license.html>
 * for a copy of the LGPLv3 License.
 *
 ************************************************************************/

// INCLUDE ---------------------------------------------------------------

#ifdef SOLARIS
// HACK: prevent conflict between STLPORT and Workshop headers on Solaris 8
#include <ctime>
#endif

#include <string> // HACK: prevent conflict between STLPORT and Workshop headers


#ifdef _MSC_VER
#pragma hdrstop
#endif

#include "fltfnc.hxx"
#include "sfxuno.hxx"

namespace binfilter {

// STATIC DATA -----------------------------------------------------------

/*N*/ DBG_NAME(SfxFilter)
/*N*/ 
/*N*/ SfxFilter::SfxFilter(  const String &rName,
/*N*/ 					   const String &rWildCard,
/*N*/ 					   SfxFilterFlags nType,
/*N*/ 					   ULONG lFmt,
/*N*/ 					   const String &rMacTyp,
/*N*/ 					   const String &rTypNm,
/*N*/ 					   sal_uInt16 nIcon,
/*N*/ 					   const String &rMimeType,
/*N*/ 					   const SfxFilterContainer* pContainerP,
/*N*/ 					   const String &rUsrDat ):
/*N*/ 	lFormat(lFmt),
/*N*/ 	nFormatType(nType),
/*N*/ 	aWildCard(rWildCard, ';'),
/*N*/ 	aMacType(rMacTyp),
/*N*/ 	aTypeName(rTypNm),
/*N*/ 	nDocIcon(nIcon),
/*N*/ 	aUserData(rUsrDat),
/*N*/ 	aName( rName ),
/*N*/ 	aMimeType( rMimeType ),
/*N*/ 	aFilterName( rName ),
/*N*/ 	pContainer( pContainerP )
/*N*/ {
/*N*/ 	InitMembers_Impl();
/*N*/ 	pContainer = pContainerP;
/*N*/ 	aFilterName = rName;
/*N*/ 	aMimeType = rMimeType;
/*N*/ }


/*?*/ SfxFilter::SfxFilter(  const char* pName, const String &rWildCard,
/*?*/ 					   SfxFilterFlags nType,
/*?*/ 					   const SfxFilterContainer* pContainerP )
/*?*/ 	: lFormat(0),
/*?*/ 	  nFormatType(nType),
/*?*/ 	  aWildCard(rWildCard, ';'),
/*?*/ 	  nDocIcon(0),
/*?*/ 	  pContainer( pContainerP )
/*?*/ {
/*?*/ 	aName = String::CreateFromAscii( pName );
/*?*/ 	aFilterName = String::CreateFromAscii( pName );
/*?*/ 	InitMembers_Impl();
/*?*/ 	pContainer = pContainerP;
/*?*/ }

/*?*/ SfxFilter::SfxFilter(  const char* pName, const String &rWildCard,
/*?*/ 					   SfxFilterFlags nType, const String &rTypeName,
/*?*/ 					   const SfxFilterContainer* pContainerP )
/*?*/ 	: lFormat(0),
/*?*/ 	  nFormatType(nType),
/*?*/ 	  aWildCard(rWildCard, ';'),
/*?*/ 	  aTypeName( rTypeName ),
/*?*/ 	  nDocIcon(0),
/*?*/ 	  pContainer( pContainerP )
/*?*/ {
/*?*/ 	aName = String::CreateFromAscii( pName );
/*?*/ 	aFilterName = String::CreateFromAscii( pName );
/*?*/ 	InitMembers_Impl();
/*?*/ 	pContainer = pContainerP;
/*?*/ }

/*N*/ void SfxFilter::InitMembers_Impl()
/*N*/ {
/*N*/ 	String aExts = GetWildcard()();
/*N*/ 	String aShort, aLong;
/*N*/ 	String aRet;
/*N*/ 	sal_uInt16 nMaxLength =
/*N*/ #if defined( WIN ) || defined( OS2 )
/*N*/ 		3
/*N*/ #else
/*N*/ 		USHRT_MAX
/*N*/ #endif
/*N*/ 		;
/*N*/ 	String aTest;
/*N*/ 	sal_uInt16 nPos = 0;
/*N*/ 	while( ( aRet = aExts.GetToken( nPos++, ';' ) ).Len() )
/*N*/ 	{
/*N*/ 		aTest = aRet;
/*N*/ 		aTest.SearchAndReplace( DEFINE_CONST_UNICODE( "*." ), String() );
/*N*/ 		if( aTest.Len() <= nMaxLength )
/*N*/ 		{
/*N*/ 			if( aShort.Len() ) aShort += ';';
/*N*/ 			aShort += aRet;
/*N*/ 		}
/*N*/ 		else
/*N*/ 		{
/*N*/ 			if( aLong.Len() ) aLong += ';';
/*N*/ 			aLong += aRet;
/*N*/ 		}
/*N*/ 	}
/*N*/ 	if( aShort.Len() && aLong.Len() )
/*N*/ 	{
/*N*/ 		aShort += ';';
/*N*/ 		aShort += aLong;
/*N*/ 	}
/*N*/ 	aWildCard = aShort;
/*N*/ 
/*N*/     nVersion = SOFFICE_FILEFORMAT_50;
/*N*/ 	bPlugDataSearched = 0;
/*N*/ 	pPlugData = 0;
/*N*/ 	aUIName = aFilterName;
/*N*/ }

/*N*/ SfxFilter::~SfxFilter()
/*N*/ {
/*N*/ 	delete pPlugData;
/*N*/ }

/*N*/ const String&	SfxFilter::GetTypeName() const
/*N*/ {
/*N*/ 	return
/*N*/ #ifdef MAC
/*N*/ 	aMacType;
/*N*/ #else
/*N*/ 	aTypeName;
/*N*/ #endif
/*N*/ }

/*?*/ const ::com::sun::star::plugin::PluginDescription* SfxFilter::GetPlugData()
/*?*/ {DBG_BF_ASSERT(0, "STRIP"); return 0;//STRIP001 
/*?*/ }

/*?*/ sal_Bool SfxFilter::IsFirstPlugin() const
/*?*/ {DBG_BF_ASSERT(0, "STRIP"); //STRIP001 
/*?*/ 	return sal_False;
/*?*/ }


/*?*/ String SfxFilter::GetDefaultExtension() const
/*?*/ {
/*?*/ 	return GetWildcard()().GetToken( 0, ';' );
/*?*/ }


/*?*/ String SfxFilter::GetSuffixes() const
/*?*/ {
/*?*/ 	String aRet = GetWildcard()();
/*?*/ 	while( aRet.SearchAndReplaceAscii( "*.", String() ) != STRING_NOTFOUND );
/*?*/ 	while( aRet.SearchAndReplace( ';', ',' ) != STRING_NOTFOUND );
/*?*/ 	return aRet;
/*?*/ }

/*N*/ String SfxFilter::GetFilterNameWithPrefix() const
/*N*/ {
/*N*/     String aName = pContainer->GetName();
/*N*/ 	aName += DEFINE_CONST_UNICODE( ": " );
/*N*/ 	aName += aFilterName;
/*N*/     return aName;
/*N*/ }
}
