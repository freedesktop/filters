/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sharedformulabuffer.cxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: kz $ $Date: 2008-03-05 19:05:36 $
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

#include "oox/xls/sharedformulabuffer.hxx"
#include <rtl/ustrbuf.hxx>
#include <com/sun/star/sheet/XFormulaTokens.hpp>
#include "oox/helper/propertyset.hxx"
#include "oox/helper/recordinputstream.hxx"
#include "oox/xls/addressconverter.hxx"
#include "oox/xls/biffinputstream.hxx"
#include "oox/xls/defnamesbuffer.hxx"
#include "oox/xls/formulaparser.hxx"

using ::rtl::OUString;
using ::rtl::OUStringBuffer;
using ::com::sun::star::uno::Reference;
using ::com::sun::star::uno::Exception;
using ::com::sun::star::uno::UNO_QUERY;
using ::com::sun::star::table::CellAddress;
using ::com::sun::star::table::CellRangeAddress;
using ::com::sun::star::sheet::XFormulaTokens;
using ::com::sun::star::sheet::XNamedRange;

namespace oox {
namespace xls {

// ============================================================================

namespace {

bool operator==( const CellAddress& rAddr1, const CellAddress& rAddr2 )
{
    return
        (rAddr1.Sheet == rAddr2.Sheet) &&
        (rAddr1.Column == rAddr2.Column) &&
        (rAddr1.Row == rAddr2.Row);
}

bool lclContains( const CellRangeAddress& rRange, const CellAddress& rAddr )
{
    return
        (rRange.Sheet == rAddr.Sheet) &&
        (rRange.StartColumn <= rAddr.Column) && (rAddr.Column <= rRange.EndColumn) &&
        (rRange.StartRow <= rAddr.Row) && (rAddr.Row <= rRange.EndRow);
}

} // namespace

// ============================================================================

ExtCellFormulaContext::ExtCellFormulaContext( const WorksheetHelper& rHelper,
        const Reference< XFormulaTokens >& rxTokens, const CellAddress& rCellPos ) :
    SimpleFormulaContext( rxTokens, false, false ),
    WorksheetHelper( rHelper )
{
    setBaseAddress( rCellPos );
}

void ExtCellFormulaContext::setSharedFormula( const CellAddress& rBaseAddr )
{
    getSharedFormulas().setSharedFormulaCell( *this, rBaseAddr );
}

// ============================================================================

SharedFormulaBuffer::SharedFormulaBuffer( const WorksheetHelper& rHelper ) :
    WorksheetHelper( rHelper ),
    maIsSharedProp( CREATE_OUSTRING( "IsSharedFormula" ) )
{
}

void SharedFormulaBuffer::importSharedFmla( const OUString& rFormula, const OUString& rSharedRange, sal_Int32 nSharedId, const CellAddress& rBaseAddr )
{
    CellRangeAddress aFmlaRange;
    if( getAddressConverter().convertToCellRange( aFmlaRange, rSharedRange, getSheetIndex(), true ) )
    {
        // create the defined name representing the shared formula
        OSL_ENSURE( lclContains( aFmlaRange, rBaseAddr ), "SharedFormulaBuffer::importSharedFmla - invalid range for shared formula" );
        BinAddress aMapKey( nSharedId, 0 );
        Reference< XNamedRange > xNamedRange = createDefinedName( aMapKey );
        // convert the formula definition
        Reference< XFormulaTokens > xTokens( xNamedRange, UNO_QUERY );
        if( xTokens.is() )
        {
            SimpleFormulaContext aContext( xTokens, true, false );
            aContext.setBaseAddress( rBaseAddr );
            getFormulaParser().importFormula( aContext, rFormula );
            updateCachedCell( rBaseAddr, aMapKey );
        }
    }
}

void SharedFormulaBuffer::importSharedFmla( RecordInputStream& rStrm, const CellAddress& rBaseAddr )
{
    BinRange aRange;
    rStrm >> aRange;
    CellRangeAddress aFmlaRange;
    if( getAddressConverter().convertToCellRange( aFmlaRange, aRange, getSheetIndex(), true ) )
    {
        // create the defined name representing the shared formula
        OSL_ENSURE( lclContains( aFmlaRange, rBaseAddr ), "SharedFormulaBuffer::importSharedFmla - invalid range for shared formula" );
        BinAddress aMapKey( rBaseAddr );
        Reference< XNamedRange > xNamedRange = createDefinedName( aMapKey );
        // load the formula definition
        Reference< XFormulaTokens > xTokens( xNamedRange, UNO_QUERY );
        if( xTokens.is() )
        {
            SimpleFormulaContext aContext( xTokens, true, false );
            aContext.setBaseAddress( rBaseAddr );
            getFormulaParser().importFormula( aContext, rStrm );
            updateCachedCell( rBaseAddr, aMapKey );
        }
    }
}

void SharedFormulaBuffer::importSharedFmla( BiffInputStream& rStrm, const CellAddress& rBaseAddr )
{
    BinRange aRange;
    aRange.read( rStrm, false );        // always 8bit column indexes
    CellRangeAddress aFmlaRange;
    if( getAddressConverter().convertToCellRange( aFmlaRange, aRange, getSheetIndex(), true ) )
    {
        // create the defined name representing the shared formula
        OSL_ENSURE( lclContains( aFmlaRange, rBaseAddr ), "SharedFormulaBuffer::importSharedFmla - invalid range for shared formula" );
        BinAddress aMapKey( rBaseAddr );
        Reference< XNamedRange > xNamedRange = createDefinedName( aMapKey );
        // load the formula definition
        Reference< XFormulaTokens > xTokens( xNamedRange, UNO_QUERY );
        if( xTokens.is() )
        {
            rStrm.skip( 2 );    // flags
            SimpleFormulaContext aContext( xTokens, true, false );
            aContext.setBaseAddress( rBaseAddr );
            getFormulaParser().importFormula( aContext, rStrm );
            updateCachedCell( rBaseAddr, aMapKey );
        }
    }
}

void SharedFormulaBuffer::setSharedFormulaCell( ExtCellFormulaContext& rContext, const CellAddress& rBaseAddr )
{
    if( !implSetSharedFormulaCell( rContext, BinAddress( rBaseAddr ) ) )
        if( rContext.getBaseAddress() == rBaseAddr )
            mxLastContext.reset( new ExtCellFormulaContext( rContext ) );
}

void SharedFormulaBuffer::setSharedFormulaCell( ExtCellFormulaContext& rContext, sal_Int32 nSharedId )
{
    implSetSharedFormulaCell( rContext, BinAddress( nSharedId, 0 ) );
}

Reference< XNamedRange > SharedFormulaBuffer::createDefinedName( const BinAddress& rMapKey )
{
    OSL_ENSURE( maIndexMap.count( rMapKey ) == 0, "SharedFormulaBuffer::createDefinedName - shared formula exists already" );
    // create the defined name representing the shared formula
    OUString aName = OUStringBuffer().appendAscii( "__shared_" ).
        append( static_cast< sal_Int32 >( getSheetIndex() + 1 ) ).
        append( sal_Unicode( '_' ) ).append( rMapKey.mnRow ).
        append( sal_Unicode( '_' ) ).append( rMapKey.mnCol ).makeStringAndClear();
    Reference< XNamedRange > xNamedRange = getDefinedNames().createDefinedName( aName );
    PropertySet aNameProps( xNamedRange );
    aNameProps.setProperty( maIsSharedProp, true );
    sal_Int32 nTokenIndex = getDefinedNames().getTokenIndex( xNamedRange );
    if( nTokenIndex >= 0 )
        maIndexMap[ rMapKey ] = nTokenIndex;
    return xNamedRange;
}

bool SharedFormulaBuffer::implSetSharedFormulaCell( ExtCellFormulaContext& rContext, const BinAddress& rMapKey )
{
    TokenIndexMap::const_iterator aIt = maIndexMap.find( rMapKey );
    sal_Int32 nTokenIndex = (aIt == maIndexMap.end()) ? -1 : aIt->second;
    if( nTokenIndex >= 0 )
    {
        getFormulaParser().convertNameToFormula( rContext, nTokenIndex );
        return true;
    }
    return false;
}

void SharedFormulaBuffer::updateCachedCell( const CellAddress& rBaseAddr, const BinAddress& rMapKey )
{
    if( mxLastContext.get() && (mxLastContext->getBaseAddress() == rBaseAddr) )
        implSetSharedFormulaCell( *mxLastContext, rMapKey );
    mxLastContext.reset();
}

// ============================================================================

} // namespace xls
} // namespace oox

