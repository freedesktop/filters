/*************************************************************************
 *
 *  $RCSfile: sw_SwXPrintPreviewSettings.cxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: hr $ $Date: 2004-08-03 19:29:35 $
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
 *  Contributor(s): Martin Gallwey (gallwey@sun.com)
 *
 *
 ************************************************************************/
#pragma hdrstop
#ifndef _SW_XPRINTPREVIEWSETTINGS_HXX_
#include <SwXPrintPreviewSettings.hxx>
#endif
#ifndef _COMPHELPER_CHAINABLEPROPERTYSETINFO_HXX_
#include <comphelper/ChainablePropertySetInfo.hxx>
#endif
#ifndef _PVPRTDAT_HXX
#include <pvprtdat.hxx>
#endif
#ifndef _SWTYPES_HXX
#include <swtypes.hxx>
#endif
#ifndef _COM_SUN_STAR_BEANS_PropertyAttribute_HPP_
#include <com/sun/star/beans/PropertyAttribute.hpp>
#endif
#ifndef _COM_SUN_STAR_BEANS_PropertyAttribute_HPP_
#include <com/sun/star/beans/PropertyAttribute.hpp>
#endif

#ifndef _HORIORNT_HXX
#include <horiornt.hxx>
#endif

#ifndef _DOC_HXX
#include <doc.hxx>
#endif
#ifndef _SV_SVAPP_HXX
#include <vcl/svapp.hxx>
#endif
namespace binfilter {
using namespace ::com::sun::star;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::beans;
using namespace ::com::sun::star::lang;
using namespace ::comphelper;
using namespace ::rtl;

enum SwPrintSettingsPropertyHandles
{ 	
    HANDLE_PRINTSET_PREVIEW_LEFT_MARGIN,
    HANDLE_PRINTSET_PREVIEW_RIGHT_MARGIN,
    HANDLE_PRINTSET_PREVIEW_TOP_MARGIN,
    HANDLE_PRINTSET_PREVIEW_BOTTOM_MARGIN,
    HANDLE_PRINTSET_PREVIEW_HORIZONTAL_SPACING,
    HANDLE_PRINTSET_PREVIEW_VERTICAL_SPACING,
    HANDLE_PRINTSET_PREVIEW_NUM_ROWS,
    HANDLE_PRINTSET_PREVIEW_NUM_COLUMNS,
    HANDLE_PRINTSET_PREVIEW_LANDSCAPE
};

static ChainablePropertySetInfo * lcl_createPrintPreviewSettingsInfo()
{
    static PropertyInfo aPrintPreviewSettingsMap_Impl[] =
    {
        { RTL_CONSTASCII_STRINGPARAM ( "PreviewPrintLeftMargin" ),	HANDLE_PRINTSET_PREVIEW_LEFT_MARGIN,	CPPUTYPE_INT32, PropertyAttribute::MAYBEVOID, 0 },
        { RTL_CONSTASCII_STRINGPARAM ( "PreviewPrintRightMargin" ),	HANDLE_PRINTSET_PREVIEW_RIGHT_MARGIN,	CPPUTYPE_INT32, PropertyAttribute::MAYBEVOID, 0 },
        { RTL_CONSTASCII_STRINGPARAM ( "PreviewPrintTopMargin" ),	HANDLE_PRINTSET_PREVIEW_TOP_MARGIN,		CPPUTYPE_INT32, PropertyAttribute::MAYBEVOID, 0 },
        { RTL_CONSTASCII_STRINGPARAM ( "PreviewPrintBottomMargin" ),HANDLE_PRINTSET_PREVIEW_BOTTOM_MARGIN,	CPPUTYPE_INT32, PropertyAttribute::MAYBEVOID, 0 },
        { RTL_CONSTASCII_STRINGPARAM ( "PreviewPrintHorizontalSpacing" ),HANDLE_PRINTSET_PREVIEW_HORIZONTAL_SPACING,	CPPUTYPE_INT32, PropertyAttribute::MAYBEVOID, 0 },
        { RTL_CONSTASCII_STRINGPARAM ( "PreviewPrintVerticalSpacing" ),	HANDLE_PRINTSET_PREVIEW_VERTICAL_SPACING,	CPPUTYPE_INT32, PropertyAttribute::MAYBEVOID, 0 },
        { RTL_CONSTASCII_STRINGPARAM ( "PreviewPrintNumRows" ),	HANDLE_PRINTSET_PREVIEW_NUM_ROWS,			CPPUTYPE_INT8, PropertyAttribute::MAYBEVOID, 0 },
        { RTL_CONSTASCII_STRINGPARAM ( "PreviewPrintNumColumns" ),	HANDLE_PRINTSET_PREVIEW_NUM_COLUMNS,	CPPUTYPE_INT8, PropertyAttribute::MAYBEVOID, 0 },
        { RTL_CONSTASCII_STRINGPARAM ( "PreviewPrintLandscape" ),	HANDLE_PRINTSET_PREVIEW_LANDSCAPE,		CPPUTYPE_BOOLEAN, PropertyAttribute::MAYBEVOID, 0 },
        { 0, 0, 0, CPPUTYPE_UNKNOWN, 0, 0 }
    };
    return new ChainablePropertySetInfo ( aPrintPreviewSettingsMap_Impl );
}
SwXPrintPreviewSettings::SwXPrintPreviewSettings( SwDoc *pDoc)
: ChainablePropertySet ( lcl_createPrintPreviewSettingsInfo (), &Application::GetSolarMutex() )
, mbPreviewDataChanged( sal_False )
, mpDoc( pDoc)
{
}
/*-- 17.12.98 12:54:05---------------------------------------------------

  -----------------------------------------------------------------------*/
SwXPrintPreviewSettings::~SwXPrintPreviewSettings()
    throw()
{
}

Any SAL_CALL SwXPrintPreviewSettings::queryInterface( const Type& rType )
    throw(RuntimeException)
{
        return ::cppu::queryInterface ( rType										,
                                        // OWeakObject interfaces
                                        reinterpret_cast< XInterface*		> ( this )	,
                                        static_cast< XWeak*			> ( this )	,
                                        // my own interfaces
                                        static_cast< XServiceInfo*		> ( this )	,
                                        static_cast< XPropertySet*		> ( this )	,
                                        static_cast< XMultiPropertySet*		> ( this ) );
}
void SwXPrintPreviewSettings::acquire ()
    throw ()
{
    OWeakObject::acquire();
}
void SwXPrintPreviewSettings::release ()
    throw ()
{
    OWeakObject::release();
}

void SwXPrintPreviewSettings::_preSetValues ()
    throw(UnknownPropertyException, PropertyVetoException, IllegalArgumentException, WrappedTargetException )
{
    if ( mpDoc )
    {
        const SwPagePreViewPrtData *pConstPrtData = mpDoc->GetPreViewPrtData();
        mpPreViewData = new SwPagePreViewPrtData;
        if ( pConstPrtData )
        {
            mpPreViewData->SetLeftSpace 	( pConstPrtData->GetLeftSpace() );
            mpPreViewData->SetRightSpace 	( pConstPrtData->GetRightSpace() );
            mpPreViewData->SetTopSpace 		( pConstPrtData->GetTopSpace() );
            mpPreViewData->SetBottomSpace 	( pConstPrtData->GetBottomSpace() );
            mpPreViewData->SetHorzSpace 	( pConstPrtData->GetHorzSpace() );
            mpPreViewData->SetVertSpace 	( pConstPrtData->GetVertSpace() );
            mpPreViewData->SetRow 			( pConstPrtData->GetRow() );
            mpPreViewData->SetCol 			( pConstPrtData->GetCol() );
            mpPreViewData->SetLandscape 	( pConstPrtData->GetLandscape() );
        }
    }
}

void SwXPrintPreviewSettings::_setSingleValue( const comphelper::PropertyInfo & rInfo, const ::com::sun::star::uno::Any &rValue ) 
    throw(UnknownPropertyException, PropertyVetoException, IllegalArgumentException, WrappedTargetException )
{
    switch( rInfo.mnHandle )
    {
        case HANDLE_PRINTSET_PREVIEW_LEFT_MARGIN:
        {
            sal_Int32 nVal;
            rValue >>= nVal;
            nVal = MM100_TO_TWIP( nVal );
            if ( nVal != static_cast < sal_Int32 > (mpPreViewData->GetLeftSpace() ) )
            {
                mbPreviewDataChanged = sal_True;
                mpPreViewData->SetLeftSpace( nVal );
            }
        }
        break;
        case HANDLE_PRINTSET_PREVIEW_RIGHT_MARGIN:
        {
            sal_Int32 nVal;
            rValue >>= nVal;
            nVal = MM100_TO_TWIP( nVal );
            if ( nVal != static_cast < sal_Int32 > (mpPreViewData->GetRightSpace() ) )
            {
                mbPreviewDataChanged = sal_True;
                mpPreViewData->SetRightSpace( nVal );
            }
        }
        break;
        case HANDLE_PRINTSET_PREVIEW_TOP_MARGIN:
        {
            sal_Int32 nVal;
            rValue >>= nVal;
            nVal = MM100_TO_TWIP( nVal );
            if ( nVal != static_cast < sal_Int32 > ( mpPreViewData->GetTopSpace() ) )
            {
                mbPreviewDataChanged = sal_True;
                mpPreViewData->SetTopSpace( nVal );
            }
        }
        break;
        case HANDLE_PRINTSET_PREVIEW_BOTTOM_MARGIN:
        {
            sal_Int32 nVal;
            rValue >>= nVal;
            nVal = MM100_TO_TWIP( nVal );
            if ( nVal != static_cast < sal_Int32 > ( mpPreViewData->GetBottomSpace() ) )
            {
                mbPreviewDataChanged = sal_True;
                mpPreViewData->SetBottomSpace( nVal );
            }
        }
        break;
        case HANDLE_PRINTSET_PREVIEW_HORIZONTAL_SPACING:
        {
            sal_Int32 nVal;
            rValue >>= nVal;
            nVal = MM100_TO_TWIP( nVal );
            if ( nVal != static_cast < sal_Int32 > ( mpPreViewData->GetHorzSpace() ) )
            {
                mbPreviewDataChanged = sal_True;
                mpPreViewData->SetHorzSpace( nVal );
            }
        }
        break;
        case HANDLE_PRINTSET_PREVIEW_VERTICAL_SPACING:
        {
            sal_Int32 nVal;
            rValue >>= nVal;
            nVal = MM100_TO_TWIP( nVal );
            if ( nVal != static_cast < sal_Int32 > ( mpPreViewData->GetVertSpace() ) )
            {
                mbPreviewDataChanged = sal_True;
                mpPreViewData->SetVertSpace( nVal );
            }
        }
        break;
        case HANDLE_PRINTSET_PREVIEW_NUM_ROWS:
        {
            sal_Int8 nVal;
            rValue >>= nVal;
            if ( nVal != mpPreViewData->GetRow() )
            {
                mbPreviewDataChanged = sal_True;
                mpPreViewData->SetRow( nVal );
            }
        }
        break;
        case HANDLE_PRINTSET_PREVIEW_NUM_COLUMNS:
        {
            sal_Int8 nVal;
            rValue >>= nVal;
            if ( nVal != mpPreViewData->GetCol() )
            {
                mbPreviewDataChanged = sal_True;
                mpPreViewData->SetCol( nVal );
            }
        }
        break;
        case HANDLE_PRINTSET_PREVIEW_LANDSCAPE:
        {
            sal_Bool bVal = *(sal_Bool*)rValue.getValue();
            if ( bVal != mpPreViewData->GetLandscape() )
            {
                mbPreviewDataChanged = sal_True;
                mpPreViewData->SetLandscape ( bVal );
            }
        }
        break;
        default: 
            throw UnknownPropertyException();
    }
}
void SwXPrintPreviewSettings::_postSetValues ()
    throw(UnknownPropertyException, PropertyVetoException, IllegalArgumentException, WrappedTargetException )
{
    if ( mbPreviewDataChanged )
    {
        mpDoc->SetPreViewPrtData ( mpPreViewData );
        mbPreviewDataChanged = sal_False;
    }
    delete mpPreViewData;
    mpPreViewData = NULL;
}

void SwXPrintPreviewSettings::_preGetValues ()
    throw(UnknownPropertyException, PropertyVetoException, IllegalArgumentException, WrappedTargetException )
{
    if (mpDoc)
        mpConstPreViewData = mpDoc->GetPreViewPrtData();
}
void SwXPrintPreviewSettings::_getSingleValue( const comphelper::PropertyInfo & rInfo, ::com::sun::star::uno::Any & rValue ) 
    throw(UnknownPropertyException, WrappedTargetException )
{
    sal_Bool bBool = TRUE;
    sal_Bool bBoolVal;
    switch( rInfo.mnHandle )
    {
        case HANDLE_PRINTSET_PREVIEW_LEFT_MARGIN:
        {
            bBool = FALSE;
            if ( mpConstPreViewData )
                rValue <<= static_cast < sal_Int32 > ( TWIP_TO_MM100 ( mpConstPreViewData->GetLeftSpace() ) );
        }
        break;
        case HANDLE_PRINTSET_PREVIEW_RIGHT_MARGIN:
        {
            bBool = FALSE;
            if ( mpConstPreViewData )
                rValue <<= static_cast < sal_Int32 > ( TWIP_TO_MM100 ( mpConstPreViewData->GetRightSpace() ) );
        }
        break;
        case HANDLE_PRINTSET_PREVIEW_TOP_MARGIN:
        {
            bBool = FALSE;
            if ( mpConstPreViewData )
                rValue <<= static_cast < sal_Int32 > ( TWIP_TO_MM100 ( mpConstPreViewData->GetTopSpace() ) );
        }
        break;
        case HANDLE_PRINTSET_PREVIEW_BOTTOM_MARGIN:
        {
            bBool = FALSE;
            if ( mpConstPreViewData )
                rValue <<= static_cast < sal_Int32 > ( TWIP_TO_MM100 ( mpConstPreViewData->GetBottomSpace() ) );
        }
        break;
        case HANDLE_PRINTSET_PREVIEW_HORIZONTAL_SPACING:
        {
            bBool = FALSE;
            if ( mpConstPreViewData )
                rValue <<= static_cast < sal_Int32 > ( TWIP_TO_MM100 ( mpConstPreViewData->GetHorzSpace() ) );
        }
        break;
        case HANDLE_PRINTSET_PREVIEW_VERTICAL_SPACING:
        {
            bBool = FALSE;
            if ( mpConstPreViewData )
                rValue <<= static_cast < sal_Int32 > ( TWIP_TO_MM100 ( mpConstPreViewData->GetVertSpace() ) );
        }
        break;
        case HANDLE_PRINTSET_PREVIEW_NUM_ROWS:
        {
            bBool = FALSE;
            if ( mpConstPreViewData )
                rValue <<= static_cast < sal_Int8 > ( mpConstPreViewData->GetRow() );
        }
        break;
        case HANDLE_PRINTSET_PREVIEW_NUM_COLUMNS:
        {
            bBool = FALSE;
            if ( mpConstPreViewData )
                rValue <<= static_cast < sal_Int8 > ( mpConstPreViewData->GetCol() );
        }
        break;
        case HANDLE_PRINTSET_PREVIEW_LANDSCAPE:
        {
            if (mpConstPreViewData)
                bBoolVal = mpConstPreViewData->GetLandscape();
            else
                bBool = FALSE;
        }
        break;
        default:
            throw UnknownPropertyException();
    }
    if(bBool)
        rValue.setValue(&bBoolVal, ::getBooleanCppuType());
}
void SwXPrintPreviewSettings::_postGetValues ()
    throw(UnknownPropertyException, PropertyVetoException, IllegalArgumentException, WrappedTargetException )
{
    mpConstPreViewData = NULL;
}
/* -----------------------------06.04.00 11:02--------------------------------

 ---------------------------------------------------------------------------*/
OUString SwXPrintPreviewSettings::getImplementationName(void) throw( RuntimeException )
{
    return OUString( RTL_CONSTASCII_USTRINGPARAM ( "SwXPrintPreviewSettings" ) );
}
/* -----------------------------06.04.00 11:02--------------------------------

 ---------------------------------------------------------------------------*/
sal_Bool SwXPrintPreviewSettings::supportsService(const OUString& rServiceName) throw( RuntimeException )
{
    return rServiceName.equalsAsciiL ( RTL_CONSTASCII_STRINGPARAM ( "com.sun.star.text.PrintPreviewSettings") );
}
/* -----------------------------06.04.00 11:02--------------------------------

 ---------------------------------------------------------------------------*/
Sequence< OUString > SwXPrintPreviewSettings::getSupportedServiceNames(void) throw( RuntimeException )
{
    Sequence< OUString > aRet(1);
    OUString* pArray = aRet.getArray();
    pArray[0] = OUString( RTL_CONSTASCII_USTRINGPARAM ( "com.sun.star.text.PrintPreviewSettings" ) );
    return aRet;
}

}
