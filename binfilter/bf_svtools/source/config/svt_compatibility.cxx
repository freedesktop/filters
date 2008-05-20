/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008 by Sun Microsystems, Inc.
 *
 * OpenOffice.org - a multi-platform office productivity suite
 *
 * $RCSfile: svt_compatibility.cxx,v $
 * $Revision: 1.4 $
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

// MARKER(update_precomp.py): autogen include statement, do not remove


//_________________________________________________________________________________________________________________
//	includes
//_________________________________________________________________________________________________________________

#include <bf_svtools/compatibility.hxx>

#ifndef _UTL_CONFIGMGR_HXX_
#include <unotools/configmgr.hxx>
#endif

#ifndef _UTL_CONFIGITEM_HXX_
#include <unotools/configitem.hxx>
#endif

#ifndef _TOOLS_DEBUG_HXX
#include <tools/debug.hxx>
#endif

#ifndef _COM_SUN_STAR_UNO_ANY_HXX_
#include <com/sun/star/uno/Any.hxx>
#endif

#ifndef _COM_SUN_STAR_UNO_SEQUENCE_HXX_
#include <com/sun/star/uno/Sequence.hxx>
#endif

#ifndef __SGI_STL_VECTOR
#include <vector>
#endif

#include <itemholder1.hxx>

#include <algorithm>

//_________________________________________________________________________________________________________________
//	namespaces
//_________________________________________________________________________________________________________________

using namespace ::std;
using namespace ::utl;
using namespace ::rtl;
using namespace ::osl;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::beans;

namespace binfilter
{

//_________________________________________________________________________________________________________________
//	const
//_________________________________________________________________________________________________________________

#define ROOTNODE_OPTIONS		OUString( RTL_CONSTASCII_USTRINGPARAM( "Office.Compatibility/" ) )
#define PATHDELIMITER			OUString( RTL_CONSTASCII_USTRINGPARAM( "/" ) )
#define SETNODE_ALLFILEFORMATS	OUString( RTL_CONSTASCII_USTRINGPARAM( "AllFileFormats" ) )

#define PROPERTYNAME_NAME               COMPATIBILITY_PROPERTYNAME_NAME
#define PROPERTYNAME_MODULE             COMPATIBILITY_PROPERTYNAME_MODULE
#define PROPERTYNAME_USEPRTMETRICS      COMPATIBILITY_PROPERTYNAME_USEPRTMETRICS
#define PROPERTYNAME_ADDSPACING         COMPATIBILITY_PROPERTYNAME_ADDSPACING
#define PROPERTYNAME_ADDSPACINGATPAGES  COMPATIBILITY_PROPERTYNAME_ADDSPACINGATPAGES
#define PROPERTYNAME_USEOURTABSTOPS     COMPATIBILITY_PROPERTYNAME_USEOURTABSTOPS
#define PROPERTYNAME_NOEXTLEADING       COMPATIBILITY_PROPERTYNAME_NOEXTLEADING
#define PROPERTYNAME_USELINESPACING     COMPATIBILITY_PROPERTYNAME_USELINESPACING
#define PROPERTYNAME_ADDTABLESPACING    COMPATIBILITY_PROPERTYNAME_ADDTABLESPACING
#define PROPERTYNAME_USEOBJPOS          COMPATIBILITY_PROPERTYNAME_USEOBJECTPOSITIONING
#define PROPERTYNAME_USEOURTEXTWRAP     COMPATIBILITY_PROPERTYNAME_USEOURTEXTWRAPPING
#define PROPERTYNAME_CONSIDERWRAPSTYLE  COMPATIBILITY_PROPERTYNAME_CONSIDERWRAPPINGSTYLE
#define PROPERTYNAME_EXPANDWORDSPACE    COMPATIBILITY_PROPERTYNAME_EXPANDWORDSPACE

#define PROPERTYCOUNT                   13

#define OFFSET_NAME                     0
#define OFFSET_MODULE                   1
#define OFFSET_USEPRTMETRICS            2
#define OFFSET_ADDSPACING               3
#define OFFSET_ADDSPACINGATPAGES        4
#define OFFSET_USEOURTABSTOPS           5
#define OFFSET_NOEXTLEADING             6
#define OFFSET_USELINESPACING           7
#define OFFSET_ADDTABLESPACING          8
#define OFFSET_USEOBJPOS                9
#define OFFSET_USEOURTEXTWRAPPING       10
#define OFFSET_CONSIDERWRAPPINGSTYLE    11
#define OFFSET_EXPANDWORDSPACE          12

//_________________________________________________________________________________________________________________
//	private declarations!
//_________________________________________________________________________________________________________________

/*-****************************************************************************************************************
    @descr  struct to hold information about one compatibility entry
****************************************************************************************************************-*/
struct SvtCompatibilityEntry
{
    public:
        SvtCompatibilityEntry() :
            bUsePrtMetrics( false ), bAddSpacing( false ),
            bAddSpacingAtPages( false ), bUseOurTabStops( false ),
            bNoExtLeading( false ), bUseLineSpacing( false ),
            bAddTableSpacing( false ), bUseObjPos( false ),
            bUseOurTextWrapping( false ), bConsiderWrappingStyle( false ),
            bExpandWordSpace( true ) {}
        SvtCompatibilityEntry(
            const OUString& _rName, const OUString& _rNewModule ) :
                sName( _rName ), sModule( _rNewModule ),
                bUsePrtMetrics( false ), bAddSpacing( false ),
                bAddSpacingAtPages( false ), bUseOurTabStops( false ),
                bNoExtLeading( false ), bUseLineSpacing( false ),
                bAddTableSpacing( false ), bUseObjPos( false ),
                bUseOurTextWrapping( false ), bConsiderWrappingStyle( false ),
                bExpandWordSpace( true ) {}

        inline void		SetUsePrtMetrics( bool _bSet ) { bUsePrtMetrics = _bSet; }
        inline void		SetAddSpacing( bool _bSet ) { bAddSpacing = _bSet; }
        inline void		SetAddSpacingAtPages( bool _bSet ) { bAddSpacingAtPages = _bSet; }
        inline void		SetUseOurTabStops( bool _bSet ) { bUseOurTabStops = _bSet; }
        inline void		SetNoExtLeading( bool _bSet ) { bNoExtLeading = _bSet; }
        inline void		SetUseLineSpacing( bool _bSet ) { bUseLineSpacing = _bSet; }
        inline void		SetAddTableSpacing( bool _bSet ) { bAddTableSpacing = _bSet; }
        inline void		SetUseObjPos( bool _bSet ) { bUseObjPos = _bSet; }
        inline void		SetUseOurTextWrapping( bool _bSet ) { bUseOurTextWrapping = _bSet; }
        inline void     SetConsiderWrappingStyle( bool _bSet ) { bConsiderWrappingStyle = _bSet; }
        inline void     SetExpandWordSpace( bool _bSet ) { bExpandWordSpace = _bSet; }

    public:
        OUString    sName;
        OUString    sModule;
        bool		bUsePrtMetrics;
        bool		bAddSpacing;
        bool		bAddSpacingAtPages;
        bool		bUseOurTabStops;
        bool		bNoExtLeading;
        bool		bUseLineSpacing;
        bool		bAddTableSpacing;
        bool		bUseObjPos;
        bool		bUseOurTextWrapping;
        bool        bConsiderWrappingStyle;
        bool        bExpandWordSpace;
};

/*-****************************************************************************************************************
    @descr  support simple menu structures and operations on it
****************************************************************************************************************-*/
class SvtCompatibility
{
    public:
        //---------------------------------------------------------------------------------------------------------
        // append one entry
        void AppendEntry( const SvtCompatibilityEntry& rEntry )
        {
            lEntries.push_back( rEntry );
        }

        //---------------------------------------------------------------------------------------------------------
        // the only way to free memory!
        void Clear()
        {
            lEntries.clear();
        }

        //---------------------------------------------------------------------------------------------------------
        // convert internal list to external format
        Sequence< Sequence< PropertyValue > > GetList() const
        {
            sal_Int32 nCount = (sal_Int32)lEntries.size();
            sal_Int32 nStep = 0;
            Sequence< PropertyValue > lProperties( PROPERTYCOUNT );
            Sequence< Sequence< PropertyValue > > lResult( nCount );
            const vector< SvtCompatibilityEntry >* pList = &lEntries;

            lProperties[ OFFSET_NAME ].Name	= PROPERTYNAME_NAME;
            lProperties[ OFFSET_MODULE ].Name = PROPERTYNAME_MODULE;
            lProperties[ OFFSET_USEPRTMETRICS ].Name = PROPERTYNAME_USEPRTMETRICS;
            lProperties[ OFFSET_ADDSPACING ].Name = PROPERTYNAME_ADDSPACING;
            lProperties[ OFFSET_ADDSPACINGATPAGES ].Name = PROPERTYNAME_ADDSPACINGATPAGES;
            lProperties[ OFFSET_USEOURTABSTOPS ].Name = PROPERTYNAME_USEOURTABSTOPS;
            lProperties[ OFFSET_NOEXTLEADING ].Name = PROPERTYNAME_NOEXTLEADING;
            lProperties[ OFFSET_USELINESPACING ].Name = PROPERTYNAME_USELINESPACING;
            lProperties[ OFFSET_ADDTABLESPACING ].Name = PROPERTYNAME_ADDTABLESPACING;
            lProperties[ OFFSET_USEOBJPOS ].Name = PROPERTYNAME_USEOBJPOS;
            lProperties[ OFFSET_USEOURTEXTWRAPPING ].Name = PROPERTYNAME_USEOURTEXTWRAP;
            lProperties[ OFFSET_CONSIDERWRAPPINGSTYLE ].Name = PROPERTYNAME_CONSIDERWRAPSTYLE;
            lProperties[ OFFSET_EXPANDWORDSPACE ].Name = PROPERTYNAME_EXPANDWORDSPACE;

            for ( vector< SvtCompatibilityEntry >::const_iterator pItem = pList->begin();
                  pItem != pList->end(); ++pItem )
            {
                lProperties[ OFFSET_NAME ].Value <<= pItem->sName;
                lProperties[ OFFSET_MODULE ].Value <<= pItem->sModule;
                lProperties[ OFFSET_USEPRTMETRICS ].Value <<= pItem->bUsePrtMetrics;
                lProperties[ OFFSET_ADDSPACING ].Value <<= pItem->bAddSpacing;
                lProperties[ OFFSET_ADDSPACINGATPAGES ].Value <<= pItem->bAddSpacingAtPages;
                lProperties[ OFFSET_USEOURTABSTOPS ].Value <<= pItem->bUseOurTabStops;
                lProperties[ OFFSET_NOEXTLEADING ].Value <<= pItem->bNoExtLeading;
                lProperties[ OFFSET_USELINESPACING ].Value <<= pItem->bUseLineSpacing;
                lProperties[ OFFSET_ADDTABLESPACING ].Value <<= pItem->bAddTableSpacing;
                lProperties[ OFFSET_USEOBJPOS ].Value <<= pItem->bUseObjPos;
                lProperties[ OFFSET_USEOURTEXTWRAPPING ].Value <<= pItem->bUseOurTextWrapping;
                lProperties[ OFFSET_CONSIDERWRAPPINGSTYLE ].Value <<= pItem->bConsiderWrappingStyle;
                lProperties[ OFFSET_EXPANDWORDSPACE ].Value <<= pItem->bExpandWordSpace;
                lResult[ nStep ] = lProperties;
                ++nStep;
            }

            return lResult;
        }

        int	size() const
        {
            return lEntries.size();
        }

        const SvtCompatibilityEntry& operator[]( int i )
        {
            return lEntries[i];
        }

    private:
        vector< SvtCompatibilityEntry > lEntries;
};

class SvtCompatibilityOptions_Impl : public ConfigItem
{
    //-------------------------------------------------------------------------------------------------------------
    //	public methods
    //-------------------------------------------------------------------------------------------------------------

    public:

        //---------------------------------------------------------------------------------------------------------
        //	constructor / destructor
        //---------------------------------------------------------------------------------------------------------

         SvtCompatibilityOptions_Impl();
        ~SvtCompatibilityOptions_Impl();

        //---------------------------------------------------------------------------------------------------------
        //	overloaded methods of baseclass
        //---------------------------------------------------------------------------------------------------------

        /*-****************************************************************************************************//**
            @short		called for notify of configmanager
            @descr		These method is called from the ConfigManager before application ends or from the
                         PropertyChangeListener if the sub tree broadcasts changes. You must update your
                        internal values.

            @seealso	baseclass ConfigItem

            @param      "lPropertyNames" is the list of properties which should be updated.
            @return		-

            @onerror	-
        *//*-*****************************************************************************************************/

        virtual void Notify( const Sequence< OUString >& lPropertyNames );

        /*-****************************************************************************************************//**
            @short		write changes to configuration
            @descr		These method writes the changed values into the sub tree
                        and should always called in our destructor to guarantee consistency of config data.

            @seealso	baseclass ConfigItem

            @param		-
            @return		-

            @onerror	-
        *//*-*****************************************************************************************************/

        virtual void Commit();

        //---------------------------------------------------------------------------------------------------------
        //	public interface
        //---------------------------------------------------------------------------------------------------------

        inline bool IsUsePrtDevice() const { return m_aDefOptions.bUsePrtMetrics; }
        inline bool IsAddSpacing() const { return m_aDefOptions.bAddSpacing; }
        inline bool IsAddSpacingAtPages() const { return m_aDefOptions.bAddSpacingAtPages; }
        inline bool IsUseOurTabStops() const { return m_aDefOptions.bUseOurTabStops; }
        inline bool IsNoExtLeading() const { return m_aDefOptions.bNoExtLeading; }
        inline bool IsUseLineSpacing() const { return m_aDefOptions.bUseLineSpacing; }
        inline bool IsAddTableSpacing() const { return m_aDefOptions.bAddTableSpacing; }
        inline bool IsUseObjPos() const { return m_aDefOptions.bUseObjPos; }
        inline bool IsUseOurTextWrapping() const { return m_aDefOptions.bUseOurTextWrapping; }
        inline bool IsConsiderWrappingStyle() const { return m_aDefOptions.bConsiderWrappingStyle; }
        inline bool IsExpandWordSpace() const { return m_aDefOptions.bExpandWordSpace; }

    //-------------------------------------------------------------------------------------------------------------
    //	private methods
    //-------------------------------------------------------------------------------------------------------------

    private:

        /*-****************************************************************************************************//**
            @short      return list of key names of our configuration management which represent one module tree
            @descr		These methods return the current list of key names! We need it to get needed values from our
                        configuration management and support dynamical menu item lists!

            @seealso	-

            @param      -
            @return		A list of configuration key names is returned.

            @onerror	-
        *//*-*****************************************************************************************************/

        Sequence< OUString > impl_GetPropertyNames( Sequence< OUString >& rItems );

        /*-****************************************************************************************************//**
            @short      expand the list for all well known properties to destination
            @descr      -
            @attention  -

            @seealso    method impl_GetPropertyNames()

            @param      "lSource"      ,   original list
            @param      "lDestination" ,   destination of operation
            @return		A list of configuration key names is returned.

            @onerror	-
        *//*-*****************************************************************************************************/

        void impl_ExpandPropertyNames( const Sequence< OUString >& lSource,
                                             Sequence< OUString >& lDestination );

    //-------------------------------------------------------------------------------------------------------------
    //	private member
    //-------------------------------------------------------------------------------------------------------------

    private:

        SvtCompatibility		m_aOptions;
        SvtCompatibilityEntry	m_aDefOptions;
};

//_________________________________________________________________________________________________________________
//	definitions
//_________________________________________________________________________________________________________________

//*****************************************************************************************************************
//	constructor
//*****************************************************************************************************************
SvtCompatibilityOptions_Impl::SvtCompatibilityOptions_Impl()
    // Init baseclasses first
    :   ConfigItem( ROOTNODE_OPTIONS )
    // Init member then...
{
    // Get names and values of all accessable menu entries and fill internal structures.
    // See impl_GetPropertyNames() for further informations.
    Sequence< OUString > lNodes;
    Sequence< OUString > lNames = impl_GetPropertyNames( lNodes );
    sal_uInt32 nCount = lNodes.getLength();
    Sequence< Any > lValues = GetProperties( lNames );

    // Safe impossible cases.
    // We need values from ALL configuration keys.
    // Follow assignment use order of values in relation to our list of key names!
    DBG_ASSERT( !( lNames.getLength()!=lValues.getLength() ), "SvtCompatibilityOptions_Impl::SvtCompatibilityOptions_Impl()\nI miss some values of configuration keys!\n" );

    SvtCompatibilityEntry aItem;
    sal_uInt32 nItem = 0;
    sal_uInt32 nPosition = 0;

    // Get names/values for new menu.
    // 4 subkeys for every item!
    bool bDefaultFound = false;
    for( nItem = 0; nItem < nCount; ++nItem )
    {
        aItem.sName = lNodes[ nItem ];
        lValues[ nPosition++ ] >>= aItem.sModule;
        lValues[ nPosition++ ] >>= aItem.bUsePrtMetrics;
        lValues[ nPosition++ ] >>= aItem.bAddSpacing;
        lValues[ nPosition++ ] >>= aItem.bAddSpacingAtPages;
        lValues[ nPosition++ ] >>= aItem.bUseOurTabStops;
        lValues[ nPosition++ ] >>= aItem.bNoExtLeading;
        lValues[ nPosition++ ] >>= aItem.bUseLineSpacing;
        lValues[ nPosition++ ] >>= aItem.bAddTableSpacing;
        lValues[ nPosition++ ] >>= aItem.bUseObjPos;
        lValues[ nPosition++ ] >>= aItem.bUseOurTextWrapping;
        lValues[ nPosition++ ] >>= aItem.bConsiderWrappingStyle;
        lValues[ nPosition++ ] >>= aItem.bExpandWordSpace;
        m_aOptions.AppendEntry( aItem );

        if ( !bDefaultFound && aItem.sName.equals( COMPATIBILITY_DEFAULT_NAME ) != sal_False )
        {
            m_aDefOptions = aItem;
            bDefaultFound = true;
        }
    }
}

//*****************************************************************************************************************
//	destructor
//*****************************************************************************************************************
SvtCompatibilityOptions_Impl::~SvtCompatibilityOptions_Impl()
{
    // We must save our current values .. if user forget it!
    if( IsModified() == sal_True )
    {
        Commit();
    }
}

//*****************************************************************************************************************
//	public method
//*****************************************************************************************************************
void SvtCompatibilityOptions_Impl::Notify( const Sequence< OUString >& )
{
    DBG_ASSERT( sal_False, "SvtCompatibilityOptions_Impl::Notify()\nNot implemented yet! I don't know how I can handle a dynamical list of unknown properties ...\n" );
}

//*****************************************************************************************************************
//	public method
//*****************************************************************************************************************
void SvtCompatibilityOptions_Impl::Commit()
{
    // Write all properties!
    // Delete complete set first.
    ClearNodeSet( SETNODE_ALLFILEFORMATS );

    SvtCompatibilityEntry aItem;
    OUString sNode;
    Sequence< PropertyValue > lPropertyValues( PROPERTYCOUNT - 1 );
    sal_uInt32 nItem = 0;
    sal_uInt32 nNewCount = m_aOptions.size();
    for( nItem = 0; nItem < nNewCount; ++nItem )
    {
        aItem = m_aOptions[ nItem ];
        sNode = SETNODE_ALLFILEFORMATS + PATHDELIMITER + aItem.sName + PATHDELIMITER;

        lPropertyValues[ OFFSET_MODULE - 1					].Name = sNode + PROPERTYNAME_MODULE;
        lPropertyValues[ OFFSET_USEPRTMETRICS - 1			].Name = sNode + PROPERTYNAME_USEPRTMETRICS;
        lPropertyValues[ OFFSET_ADDSPACING - 1				].Name = sNode + PROPERTYNAME_ADDSPACING;
        lPropertyValues[ OFFSET_ADDSPACINGATPAGES - 1		].Name = sNode + PROPERTYNAME_ADDSPACINGATPAGES;
        lPropertyValues[ OFFSET_USEOURTABSTOPS - 1			].Name = sNode + PROPERTYNAME_USEOURTABSTOPS;
        lPropertyValues[ OFFSET_NOEXTLEADING - 1			].Name = sNode + PROPERTYNAME_NOEXTLEADING;
        lPropertyValues[ OFFSET_USELINESPACING - 1			].Name = sNode + PROPERTYNAME_USELINESPACING;
        lPropertyValues[ OFFSET_ADDTABLESPACING - 1			].Name = sNode + PROPERTYNAME_ADDTABLESPACING;
        lPropertyValues[ OFFSET_USEOBJPOS - 1				].Name = sNode + PROPERTYNAME_USEOBJPOS;
        lPropertyValues[ OFFSET_USEOURTEXTWRAPPING - 1		].Name = sNode + PROPERTYNAME_USEOURTEXTWRAP;
        lPropertyValues[ OFFSET_CONSIDERWRAPPINGSTYLE - 1   ].Name = sNode + PROPERTYNAME_CONSIDERWRAPSTYLE;
        lPropertyValues[ OFFSET_EXPANDWORDSPACE - 1         ].Name = sNode + PROPERTYNAME_EXPANDWORDSPACE;

        lPropertyValues[ OFFSET_MODULE - 1					].Value <<= aItem.sModule;
        lPropertyValues[ OFFSET_USEPRTMETRICS - 1			].Value <<= aItem.bUsePrtMetrics;
        lPropertyValues[ OFFSET_ADDSPACING - 1				].Value <<= aItem.bAddSpacing;
        lPropertyValues[ OFFSET_ADDSPACINGATPAGES - 1		].Value <<= aItem.bAddSpacingAtPages;
        lPropertyValues[ OFFSET_USEOURTABSTOPS - 1			].Value <<= aItem.bUseOurTabStops;
        lPropertyValues[ OFFSET_NOEXTLEADING - 1			].Value <<= aItem.bNoExtLeading;
        lPropertyValues[ OFFSET_USELINESPACING - 1			].Value <<= aItem.bUseLineSpacing;
        lPropertyValues[ OFFSET_ADDTABLESPACING - 1			].Value <<= aItem.bAddTableSpacing;
        lPropertyValues[ OFFSET_USEOBJPOS - 1				].Value <<= aItem.bUseObjPos;
        lPropertyValues[ OFFSET_USEOURTEXTWRAPPING - 1  	].Value <<= aItem.bUseOurTextWrapping;
        lPropertyValues[ OFFSET_CONSIDERWRAPPINGSTYLE - 1   ].Value <<= aItem.bConsiderWrappingStyle;
        lPropertyValues[ OFFSET_EXPANDWORDSPACE - 1         ].Value <<= aItem.bExpandWordSpace;

        SetSetProperties( SETNODE_ALLFILEFORMATS, lPropertyValues );
    }
}

//*****************************************************************************************************************
//	private method
//*****************************************************************************************************************
Sequence< OUString > SvtCompatibilityOptions_Impl::impl_GetPropertyNames( Sequence< OUString >& rItems )
{
    // First get ALL names of current existing list items in configuration!
    rItems = GetNodeNames( SETNODE_ALLFILEFORMATS );
    // expand list to result list ...
    Sequence< OUString > lProperties( rItems.getLength() * ( PROPERTYCOUNT - 1 ) );
    impl_ExpandPropertyNames( rItems, lProperties );
    // Return result.
    return lProperties;
}

//*****************************************************************************************************************
//	private method
//*****************************************************************************************************************
void SvtCompatibilityOptions_Impl::impl_ExpandPropertyNames(
    const Sequence< OUString >& lSource, Sequence< OUString >& lDestination )
{
    OUString sFixPath;
    sal_Int32 nDestStep = 0;
    sal_Int32 nSourceCount = lSource.getLength();
    // Copy entries to destination and expand every item with 2 supported sub properties.
    for( sal_Int32 nSourceStep = 0; nSourceStep < nSourceCount; ++nSourceStep )
    {
        sFixPath = SETNODE_ALLFILEFORMATS;
        sFixPath += PATHDELIMITER;
        sFixPath += lSource[ nSourceStep ];
        sFixPath += PATHDELIMITER;

        lDestination[nDestStep] = sFixPath;
        lDestination[nDestStep] += PROPERTYNAME_MODULE;
        ++nDestStep;
        lDestination[nDestStep] = sFixPath;
        lDestination[nDestStep] += PROPERTYNAME_USEPRTMETRICS;
        ++nDestStep;
        lDestination[nDestStep] = sFixPath;
        lDestination[nDestStep] += PROPERTYNAME_ADDSPACING;
        ++nDestStep;
        lDestination[nDestStep] = sFixPath;
        lDestination[nDestStep] += PROPERTYNAME_ADDSPACINGATPAGES;
        ++nDestStep;
        lDestination[nDestStep] = sFixPath;
        lDestination[nDestStep] += PROPERTYNAME_USEOURTABSTOPS;
        ++nDestStep;
        lDestination[nDestStep] = sFixPath;
        lDestination[nDestStep] += PROPERTYNAME_NOEXTLEADING;
        ++nDestStep;
        lDestination[nDestStep] = sFixPath;
        lDestination[nDestStep] += PROPERTYNAME_USELINESPACING;
        ++nDestStep;
        lDestination[nDestStep] = sFixPath;
        lDestination[nDestStep] += PROPERTYNAME_ADDTABLESPACING;
        ++nDestStep;
        lDestination[nDestStep] = sFixPath;
        lDestination[nDestStep] += PROPERTYNAME_USEOBJPOS;
        ++nDestStep;
        lDestination[nDestStep] = sFixPath;
        lDestination[nDestStep] += PROPERTYNAME_USEOURTEXTWRAP;
        ++nDestStep;
        lDestination[nDestStep] = sFixPath;
        lDestination[nDestStep] += PROPERTYNAME_CONSIDERWRAPSTYLE;
        ++nDestStep;
        lDestination[nDestStep] = sFixPath;
        lDestination[nDestStep] += PROPERTYNAME_EXPANDWORDSPACE;
        ++nDestStep;
    }
}

//*****************************************************************************************************************
//	initialize static member
//	DON'T DO IT IN YOUR HEADER!
//	see definition for further informations
//*****************************************************************************************************************
SvtCompatibilityOptions_Impl*	SvtCompatibilityOptions::m_pDataContainer = NULL;
sal_Int32						SvtCompatibilityOptions::m_nRefCount = 0;

//*****************************************************************************************************************
//	constructor
//*****************************************************************************************************************
SvtCompatibilityOptions::SvtCompatibilityOptions()
{
    // Global access, must be guarded (multithreading!).
    MutexGuard aGuard( GetOwnStaticMutex() );
    // Increase ouer refcount ...
    ++m_nRefCount;
    // ... and initialize ouer data container only if it not already exist!
    if( m_pDataContainer == NULL )
    {
        m_pDataContainer = new SvtCompatibilityOptions_Impl;
        ItemHolder1::holdConfigItem(E_COMPATIBILITY);
    }
}

//*****************************************************************************************************************
//	destructor
//*****************************************************************************************************************
SvtCompatibilityOptions::~SvtCompatibilityOptions()
{
    // Global access, must be guarded (multithreading!)
    MutexGuard aGuard( GetOwnStaticMutex() );
    // Decrease ouer refcount.
    --m_nRefCount;
    // If last instance was deleted ...
    // we must destroy ouer static data container!
    if( m_nRefCount <= 0 )
    {
        delete m_pDataContainer;
        m_pDataContainer = NULL;
    }
}

//*****************************************************************************************************************
//	private method
//*****************************************************************************************************************
Mutex& SvtCompatibilityOptions::GetOwnStaticMutex()
{
    // Initialize static mutex only for one time!
    static Mutex* pMutex = NULL;
    // If these method first called (Mutex not already exist!) ...
    if( pMutex == NULL )
    {
        // ... we must create a new one. Protect follow code with the global mutex -
        // It must be - we create a static variable!
        MutexGuard aGuard( Mutex::getGlobalMutex() );
        // We must check our pointer again - because it can be that another instance of ouer class will be fastr then these!
        if( pMutex == NULL )
        {
            // Create the new mutex and set it for return on static variable.
            static Mutex aMutex;
            pMutex = &aMutex;
        }
    }
    // Return new created or already existing mutex object.
    return *pMutex;
}

}
