/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright 2000, 2010 Oracle and/or its affiliates.
 *
 * OpenOffice.org - a multi-platform office productivity suite
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

#include "oox/ole/vbacontrol.hxx"
#include <algorithm>
#include <set>
#include <rtl/ustrbuf.hxx>
#include <com/sun/star/awt/XControlModel.hpp>
#include <com/sun/star/container/XNameContainer.hpp>
#include <com/sun/star/io/XInputStreamProvider.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>
#include <xmlscript/xmldlg_imexp.hxx>
#include "properties.hxx"
#include "tokens.hxx"
#include "oox/helper/attributelist.hxx"
#include "oox/helper/binaryinputstream.hxx"
#include "oox/helper/propertymap.hxx"
#include "oox/helper/propertyset.hxx"
#include "oox/helper/storagebase.hxx"
#include "oox/helper/textinputstream.hxx"
#include "oox/ole/vbahelper.hxx"

using ::rtl::OUString;
using ::rtl::OUStringBuffer;
using ::com::sun::star::awt::XControlModel;
using ::com::sun::star::container::XNameContainer;
using ::com::sun::star::io::XInputStreamProvider;
using ::com::sun::star::lang::XMultiServiceFactory;
using ::com::sun::star::uno::Any;
using ::com::sun::star::uno::Exception;
using ::com::sun::star::uno::Reference;
using ::com::sun::star::uno::UNO_QUERY_THROW;
using ::com::sun::star::uno::UNO_SET_THROW;
using ::com::sun::star::uno::XComponentContext;

namespace oox {
namespace ole {

// ============================================================================

namespace {

const sal_uInt16 VBA_SITE_CLASSIDINDEX          = 0x8000;
const sal_uInt16 VBA_SITE_INDEXMASK             = 0x7FFF;
const sal_uInt16 VBA_SITE_FORM                  = 7;
const sal_uInt16 VBA_SITE_IMAGE                 = 12;
const sal_uInt16 VBA_SITE_FRAME                 = 14;
const sal_uInt16 VBA_SITE_SPINBUTTON            = 16;
const sal_uInt16 VBA_SITE_COMMANDBUTTON         = 17;
const sal_uInt16 VBA_SITE_TABSTRIP              = 18;
const sal_uInt16 VBA_SITE_LABEL                 = 21;
const sal_uInt16 VBA_SITE_TEXTBOX               = 23;
const sal_uInt16 VBA_SITE_LISTBOX               = 24;
const sal_uInt16 VBA_SITE_COMBOBOX              = 25;
const sal_uInt16 VBA_SITE_CHECKBOX              = 26;
const sal_uInt16 VBA_SITE_OPTIONBUTTON          = 27;
const sal_uInt16 VBA_SITE_TOGGLEBUTTON          = 28;
const sal_uInt16 VBA_SITE_SCROLLBAR             = 47;
const sal_uInt16 VBA_SITE_MULTIPAGE             = 57;
const sal_uInt16 VBA_SITE_UNKNOWN               = 0x7FFF;

const sal_uInt32 VBA_SITE_TABSTOP               = 0x00000001;
const sal_uInt32 VBA_SITE_VISIBLE               = 0x00000002;
const sal_uInt32 VBA_SITE_DEFAULTBUTTON         = 0x00000004;
const sal_uInt32 VBA_SITE_CANCELBUTTON          = 0x00000008;
const sal_uInt32 VBA_SITE_OSTREAM               = 0x00000010;
const sal_uInt32 VBA_SITE_DEFFLAGS              = 0x00000033;

const sal_uInt8 VBA_SITEINFO_COUNT              = 0x80;
const sal_uInt8 VBA_SITEINFO_MASK               = 0x7F;

// ----------------------------------------------------------------------------

/** Collects names of all controls in a user form or container control. Allows
    to generate unused names for dummy controls separating option groups.
 */
class VbaControlNamesSet
{
public:
    explicit            VbaControlNamesSet();

    /** Inserts the name of the passed control. */
    void                insertName( const VbaFormControl& rControl );
    /** Returns a name that is not contained in this set. */
    OUString            generateDummyName();

private:
    typedef ::std::set< OUString > OUStringSet;
    OUStringSet         maCtrlNames;
    const OUString      maDummyBaseName;
    sal_Int32           mnIndex;
};

VbaControlNamesSet::VbaControlNamesSet() :
    maDummyBaseName( CREATE_OUSTRING( "DummyGroupSep" ) ),
    mnIndex( 0 )
{
}

void VbaControlNamesSet::insertName( const VbaFormControl& rControl )
{
    OUString aName = rControl.getControlName();
    if( aName.getLength() > 0 )
        maCtrlNames.insert( aName );
}

OUString VbaControlNamesSet::generateDummyName()
{
    OUString aCtrlName;
    do
    {
        aCtrlName = OUStringBuffer( maDummyBaseName ).append( ++mnIndex ).makeStringAndClear();
    }
    while( maCtrlNames.count( aCtrlName ) > 0 );
    maCtrlNames.insert( aCtrlName );
    return aCtrlName;
}

// ----------------------------------------------------------------------------

/** Functor that inserts the name of a control into a VbaControlNamesSet. */
struct VbaControlNameInserter
{
public:
    VbaControlNamesSet& mrCtrlNames;
    inline explicit     VbaControlNameInserter( VbaControlNamesSet& rCtrlNames ) : mrCtrlNames( rCtrlNames ) {}
    inline void         operator()( const VbaFormControl& rControl ) { mrCtrlNames.insertName( rControl ); }
};

// ----------------------------------------------------------------------------

/** A dummy invisible form control (fixed label without text) that is used to
    separate two groups of option buttons.
 */
class VbaDummyFormControl : public VbaFormControl
{
public:
    explicit            VbaDummyFormControl( const OUString& rName );
};

VbaDummyFormControl::VbaDummyFormControl( const OUString& rName )
{
    mxSiteModel.reset( new VbaSiteModel );
    mxSiteModel->importProperty( XML_Name, rName );
    mxSiteModel->importProperty( XML_VariousPropertyBits, OUString( sal_Unicode( '0' ) ) );

    mxCtrlModel.reset( new AxLabelModel );
    mxCtrlModel->setAwtModelMode();
    mxCtrlModel->importProperty( XML_Size, CREATE_OUSTRING( "10;10" ) );
}

} // namespace

// ============================================================================

VbaSiteModel::VbaSiteModel() :
    maPos( 0, 0 ),
    mnId( 0 ),
    mnHelpContextId( 0 ),
    mnFlags( VBA_SITE_DEFFLAGS ),
    mnStreamLen( 0 ),
    mnTabIndex( -1 ),
    mnClassIdOrCache( VBA_SITE_UNKNOWN ),
    mnGroupId( 0 )
{
}

VbaSiteModel::~VbaSiteModel()
{
}

void VbaSiteModel::importProperty( sal_Int32 nPropId, const OUString& rValue )
{
    switch( nPropId )
    {
        case XML_Name:                  maName = rValue;                                    break;
        case XML_Tag:                   maTag = rValue;                                     break;
        case XML_VariousPropertyBits:   mnFlags = AttributeList::decodeUnsigned( rValue );  break;
    }
}

bool VbaSiteModel::importBinaryModel( BinaryInputStream& rInStrm )
{
    AxBinaryPropertyReader aReader( rInStrm );
    aReader.readStringProperty( maName );
    aReader.readStringProperty( maTag );
    aReader.readIntProperty< sal_Int32 >( mnId );
    aReader.readIntProperty< sal_Int32 >( mnHelpContextId );
    aReader.readIntProperty< sal_uInt32 >( mnFlags );
    aReader.readIntProperty< sal_uInt32 >( mnStreamLen );
    aReader.readIntProperty< sal_Int16 >( mnTabIndex );
    aReader.readIntProperty< sal_uInt16 >( mnClassIdOrCache );
    aReader.readPairProperty( maPos );
    aReader.readIntProperty< sal_uInt16 >( mnGroupId );
    aReader.skipUndefinedProperty();
    aReader.readStringProperty( maToolTip );
    aReader.skipStringProperty();   // license key
    aReader.readStringProperty( maLinkedCell );
    aReader.readStringProperty( maSourceRange );
    return aReader.finalizeImport();
}

void VbaSiteModel::moveRelative( const AxPairData& rDistance )
{
    maPos.first += rDistance.first;
    maPos.second += rDistance.second;
}

bool VbaSiteModel::isVisible() const
{
    return getFlag( mnFlags, VBA_SITE_VISIBLE );
}

bool VbaSiteModel::isContainer() const
{
    return !getFlag( mnFlags, VBA_SITE_OSTREAM );
}

sal_uInt32 VbaSiteModel::getStreamLength() const
{
    return isContainer() ? 0 : mnStreamLen;
}

OUString VbaSiteModel::getSubStorageName() const
{
    if( mnId >= 0 )
    {
        OUStringBuffer aBuffer;
        aBuffer.append( sal_Unicode( 'i' ) );
        if( mnId < 10 )
            aBuffer.append( sal_Unicode( '0' ) );
        aBuffer.append( mnId );
        return aBuffer.makeStringAndClear();
    }
    return OUString();
}

ControlModelRef VbaSiteModel::createControlModel( const AxClassTable& rClassTable ) const
{
    ControlModelRef xCtrlModel;

    sal_Int32 nTypeIndex = static_cast< sal_Int32 >( mnClassIdOrCache & VBA_SITE_INDEXMASK );
    if( !getFlag( mnClassIdOrCache, VBA_SITE_CLASSIDINDEX ) )
    {
        switch( nTypeIndex )
        {
            case VBA_SITE_COMMANDBUTTON:    xCtrlModel.reset( new AxCommandButtonModel );   break;
            case VBA_SITE_LABEL:            xCtrlModel.reset( new AxLabelModel );           break;
            case VBA_SITE_IMAGE:            xCtrlModel.reset( new AxImageModel );           break;
            case VBA_SITE_TOGGLEBUTTON:     xCtrlModel.reset( new AxToggleButtonModel );    break;
            case VBA_SITE_CHECKBOX:         xCtrlModel.reset( new AxCheckBoxModel );        break;
            case VBA_SITE_OPTIONBUTTON:     xCtrlModel.reset( new AxOptionButtonModel );    break;
            case VBA_SITE_TEXTBOX:          xCtrlModel.reset( new AxTextBoxModel );         break;
            case VBA_SITE_LISTBOX:          xCtrlModel.reset( new AxListBoxModel );         break;
            case VBA_SITE_COMBOBOX:         xCtrlModel.reset( new AxComboBoxModel );        break;
            case VBA_SITE_SPINBUTTON:       /*xCtrlModel.reset( new AxSpinButtonModel );*/  break;  // not supported (?)
            case VBA_SITE_SCROLLBAR:        xCtrlModel.reset( new AxScrollBarModel );       break;
            case VBA_SITE_TABSTRIP:                                                         break;  // not supported
            case VBA_SITE_FRAME:            xCtrlModel.reset( new AxFrameModel );           break;
            case VBA_SITE_MULTIPAGE:                                                        break;  // not supported
            case VBA_SITE_FORM:                                                             break;  // not supported
            default:    OSL_ENSURE( false, "VbaSiteModel::createControlModel - unknown type index" );
        }
    }
    else
    {
        const OUString* pGuid = ContainerHelper::getVectorElement( rClassTable, nTypeIndex );
        OSL_ENSURE( pGuid, "VbaSiteModel::createControlModel - invalid class table index" );
        if( pGuid )
        {
            if( pGuid->equalsAscii( COMCTL_GUID_SCROLLBAR_60 ) )
                xCtrlModel.reset( new ComCtlScrollBarModel( 6 ) );
            else if( pGuid->equalsAscii( COMCTL_GUID_PROGRESSBAR_50 ) )
                xCtrlModel.reset( new ComCtlProgressBarModel( 5 ) );
            else if( pGuid->equalsAscii( COMCTL_GUID_PROGRESSBAR_60 ) )
                xCtrlModel.reset( new ComCtlProgressBarModel( 6 ) );
        }
    }

    if( xCtrlModel.get() )
    {
        // user form controls are AWT models
        xCtrlModel->setAwtModelMode();

        // check that container model matches container flag in site data
        bool bModelIsContainer = dynamic_cast< const AxContainerModelBase* >( xCtrlModel.get() ) != 0;
        bool bTypeMatch = bModelIsContainer == isContainer();
        OSL_ENSURE( bTypeMatch, "VbaSiteModel::createControlModel - container type does not match container flag" );
        if( !bTypeMatch )
            xCtrlModel.reset();
    }
    return xCtrlModel;
}

void VbaSiteModel::convertProperties( PropertyMap& rPropMap,
        const ControlConverter& rConv, ApiControlType eCtrlType, sal_Int32 nCtrlIndex ) const
{
    rPropMap.setProperty( PROP_Name, maName );
    rPropMap.setProperty( PROP_Tag, maTag );

    if( eCtrlType != API_CONTROL_DIALOG )
    {
        rPropMap.setProperty( PROP_HelpText, maToolTip );
        rPropMap.setProperty( PROP_EnableVisible, getFlag( mnFlags, VBA_SITE_VISIBLE ) );
        // we need to set the passed control index to make option button groups work
        if( (0 <= nCtrlIndex) && (nCtrlIndex <= SAL_MAX_INT16) )
            rPropMap.setProperty( PROP_TabIndex, static_cast< sal_Int16 >( nCtrlIndex ) );
        // progress bar and group box support TabIndex, but not Tabstop...
        if( (eCtrlType != API_CONTROL_PROGRESSBAR) && (eCtrlType != API_CONTROL_GROUPBOX) && (eCtrlType != API_CONTROL_FRAME) && (eCtrlType != API_CONTROL_PAGE) )
            rPropMap.setProperty( PROP_Tabstop, getFlag( mnFlags, VBA_SITE_TABSTOP ) );
        rConv.convertPosition( rPropMap, maPos );
    }
}

// ============================================================================

VbaFormControl::VbaFormControl()
{
}

VbaFormControl::~VbaFormControl()
{
}

void VbaFormControl::importModelOrStorage( BinaryInputStream& rInStrm, StorageBase& rStrg, const AxClassTable& rClassTable )
{
    if( mxSiteModel.get() )
    {
        if( mxSiteModel->isContainer() )
        {
            StorageRef xSubStrg = rStrg.openSubStorage( mxSiteModel->getSubStorageName(), false );
            OSL_ENSURE( xSubStrg.get(), "VbaFormControl::importModelOrStorage - cannot find storage for embedded control" );
            if( xSubStrg.get() )
                importStorage( *xSubStrg, rClassTable );
        }
        else if( !rInStrm.isEof() )
        {
            sal_Int64 nNextStrmPos = rInStrm.tell() + mxSiteModel->getStreamLength();
            importControlModel( rInStrm, rClassTable );
            rInStrm.seek( nNextStrmPos );
        }
    }
}

OUString VbaFormControl::getControlName() const
{
    return mxSiteModel.get() ? mxSiteModel->getName() : OUString();
}

sal_Int32 VbaFormControl::getControlId() const
{
    return mxSiteModel.get() ? mxSiteModel->getId() : -1;
}

void VbaFormControl::createAndConvert( sal_Int32 nCtrlIndex,
        const Reference< XNameContainer >& rxParentNC, const ControlConverter& rConv ) const
{
    if( rxParentNC.is() && mxSiteModel.get() && mxCtrlModel.get() ) try
    {
        // create the control model
        OUString aServiceName = mxCtrlModel->getServiceName();
        Reference< XMultiServiceFactory > xModelFactory( rxParentNC, UNO_QUERY_THROW );
        Reference< XControlModel > xCtrlModel( xModelFactory->createInstance( aServiceName ), UNO_QUERY_THROW );

        // convert all properties and embedded controls
        if( convertProperties( xCtrlModel, rConv, nCtrlIndex ) )
        {
            // insert into parent container
            const OUString& rCtrlName = mxSiteModel->getName();
            OSL_ENSURE( !rxParentNC->hasByName( rCtrlName ), "VbaFormControl::createAndConvert - multiple controls with equal name" );
            ContainerHelper::insertByName( rxParentNC, rCtrlName, Any( xCtrlModel ) );
        }
    }
    catch( Exception& )
    {
    }
}

// protected ------------------------------------------------------------------

void VbaFormControl::importControlModel( BinaryInputStream& rInStrm, const AxClassTable& rClassTable )
{
    createControlModel( rClassTable );
    if( mxCtrlModel.get() )
        mxCtrlModel->importBinaryModel( rInStrm );
}

void VbaFormControl::importStorage( StorageBase& rStrg, const AxClassTable& rClassTable )
{
    createControlModel( rClassTable );
    AxContainerModelBase* pContainerModel = dynamic_cast< AxContainerModelBase* >( mxCtrlModel.get() );
    OSL_ENSURE( pContainerModel, "VbaFormControl::importStorage - missing container control model" );
    if( pContainerModel )
    {
        /*  Open the 'f' stream containing the model of this control and a list
            of site models for all child controls. */
        BinaryXInputStream aFStrm( rStrg.openInputStream( CREATE_OUSTRING( "f" ) ), true );
        OSL_ENSURE( !aFStrm.isEof(), "VbaFormControl::importStorage - missing 'f' stream" );

        /*  Read the properties of this container control and the class table
            (into the maClassTable vector) containing a list of GUIDs for
            exotic embedded controls. */
        if( !aFStrm.isEof() && pContainerModel->importBinaryModel( aFStrm ) && pContainerModel->importClassTable( aFStrm, maClassTable ) )
        {
            /*  Read the site models of all embedded controls (this fills the
                maControls vector). Ignore failure of importSiteModels() but
                try to import as much controls as possible. */
            importEmbeddedSiteModels( aFStrm );

            /*  Open the 'o' stream containing models of embedded simple
                controls. Stream may be empty or missing, if this control
                contains no controls or only container controls. */
            BinaryXInputStream aOStrm( rStrg.openInputStream( CREATE_OUSTRING( "o" ) ), true );

            /*  Iterate over all embedded controls, import model from 'o'
                stream (for embedded simple controls) or from the substorage
                (for embedded container controls). */
            maControls.forEachMem( &VbaFormControl::importModelOrStorage,
                ::boost::ref( aOStrm ), ::boost::ref( rStrg ), ::boost::cref( maClassTable ) );
                
            /*  Reorder the controls (sorts all option buttons of an option
                group together), and move all children of all embedded frames
                (group boxes) to this control (UNO group boxes cannot contain
                other controls). */
            finalizeEmbeddedControls();
        }
    }
}

bool VbaFormControl::convertProperties( const Reference< XControlModel >& rxCtrlModel,
        const ControlConverter& rConv, sal_Int32 nCtrlIndex ) const
{
    if( rxCtrlModel.is() && mxSiteModel.get() && mxCtrlModel.get() )
    {
        const OUString& rCtrlName = mxSiteModel->getName();
        OSL_ENSURE( rCtrlName.getLength() > 0, "VbaFormControl::convertProperties - control without name" );
        if( rCtrlName.getLength() > 0 )
        {
            // convert all properties
            PropertyMap aPropMap;
            mxSiteModel->convertProperties( aPropMap, rConv, mxCtrlModel->getControlType(), nCtrlIndex );
            mxCtrlModel->convertProperties( aPropMap, rConv );
            mxCtrlModel->convertSize( aPropMap, rConv );
            PropertySet aPropSet( rxCtrlModel );
            aPropSet.setProperties( aPropMap );

            // create and convert all embedded controls
            if( !maControls.empty() ) try
            {
                Reference< XNameContainer > xCtrlModelNC( rxCtrlModel, UNO_QUERY_THROW );
                /*  Call conversion for all controls. Pass vector index as new
                    tab order to make option button groups work correctly. */
                maControls.forEachMemWithIndex( &VbaFormControl::createAndConvert,
                    ::boost::cref( xCtrlModelNC ), ::boost::cref( rConv ) );
            }
            catch( Exception& )
            {
                OSL_ENSURE( false, "VbaFormControl::convertProperties - cannot get control container interface" );
            }

            return true;
        }
    }
    return false;
}

// private --------------------------------------------------------------------

void VbaFormControl::createControlModel( const AxClassTable& rClassTable )
{
    // derived classes may have created their own control model
    if( !mxCtrlModel && mxSiteModel.get() )
        mxCtrlModel = mxSiteModel->createControlModel( rClassTable );
}

bool VbaFormControl::importSiteModel( BinaryInputStream& rInStrm )
{
    mxSiteModel.reset( new VbaSiteModel );
    return mxSiteModel->importBinaryModel( rInStrm );
}

bool VbaFormControl::importEmbeddedSiteModels( BinaryInputStream& rInStrm )
{
    sal_uInt64 nAnchorPos = rInStrm.tell();
    sal_uInt32 nSiteCount, nSiteDataSize;
    rInStrm >> nSiteCount >> nSiteDataSize;
    sal_Int64 nSiteEndPos = rInStrm.tell() + nSiteDataSize;

    // skip the site info structure
    sal_uInt32 nSiteIndex = 0;
    while( !rInStrm.isEof() && (nSiteIndex < nSiteCount) )
    {
        rInStrm.skip( 1 ); // site depth
        sal_uInt8 nTypeCount = rInStrm.readuInt8(); // 'type-or-count' byte
        if( getFlag( nTypeCount, VBA_SITEINFO_COUNT ) )
        {
            /*  Count flag is set: the 'type-or-count' byte contains the number
                of controls in the lower bits, the type specifier follows in
                the next byte. The type specifier should always be 1 according
                to the specification. */
            rInStrm.skip( 1 );
            nSiteIndex += (nTypeCount & VBA_SITEINFO_MASK);
        }
        else
        {
            /*  Count flag is not set: the 'type-or-count' byte contains the
                type specifier of *one* control in the lower bits (this type
                should be 1, see above). */
            ++nSiteIndex;
        }
    }
    // align the stream to 32bit, relative to start of entire site info
    rInStrm.alignToBlock( 4, nAnchorPos );

    // import the site models for all embedded controls
    maControls.clear();
    bool bValid = !rInStrm.isEof();
    for( nSiteIndex = 0; bValid && (nSiteIndex < nSiteCount); ++nSiteIndex )
    {
        VbaFormControlRef xControl( new VbaFormControl );
        maControls.push_back( xControl );
        bValid = xControl->importSiteModel( rInStrm );
    }

    rInStrm.seek( nSiteEndPos );
    return bValid;
}

void VbaFormControl::finalizeEmbeddedControls()
{
    /*  This function performs two tasks:

        1)  Reorder the controls appropriately (sort all option buttons of an
            option group together to make grouping work).
        2)  Move all children of all embedded frames (group boxes) to this
            control (UNO group boxes cannot contain other controls).
     */
                
    // first, sort all controls by original tab index
    ::std::sort( maControls.begin(), maControls.end(), &compareByTabIndex );

    /*  Collect the programmatical names of all embedded controls (needed to be
        able to set unused names to new dummy controls created below). Also
        collect the names of all children of embedded frames (group boxes).
        Luckily, names of controls must be unique in the entire form, not just
        in the current container. */
    VbaControlNamesSet aControlNames;
    VbaControlNameInserter aInserter( aControlNames );
    maControls.forEach( aInserter );
    for( VbaFormControlVector::iterator aIt = maControls.begin(), aEnd = maControls.end(); aIt != aEnd; ++aIt )
        if( (*aIt)->mxCtrlModel.get() && ((*aIt)->mxCtrlModel->getControlType() == API_CONTROL_GROUPBOX) )
            (*aIt)->maControls.forEach( aInserter );

    /*  Reprocess the sorted list and collect all option button controls that
        are part of the same option group (determined by group name). All
        controls will be stored in a vector of vectors, that collects every
        option button group in one vector element, and other controls between
        these option groups (or leading or trailing controls) in other vector
        elements. If an option button group follows another group, a dummy
        separator control has to be inserted. */
    typedef RefVector< VbaFormControlVector > VbaFormControlVectorVector;
    VbaFormControlVectorVector aControlGroups;

    typedef RefMap< OUString, VbaFormControlVector > VbaFormControlVectorMap;
    VbaFormControlVectorMap aOptionGroups;

    typedef VbaFormControlVectorMap::mapped_type VbaFormControlVectorRef;
    bool bLastWasOptionButton = false;
    for( VbaFormControlVector::iterator aIt = maControls.begin(), aEnd = maControls.end(); aIt != aEnd; ++aIt )
    {
        VbaFormControlRef xControl = *aIt;
        const ControlModelBase* pCtrlModel = xControl->mxCtrlModel.get();

        if( const AxOptionButtonModel* pOptButtonModel = dynamic_cast< const AxOptionButtonModel* >( pCtrlModel ) )
        {
            // check if a new option group needs to be created
            const OUString& rGroupName = pOptButtonModel->getGroupName();
            VbaFormControlVectorRef& rxOptionGroup = aOptionGroups[ rGroupName ];
            if( !rxOptionGroup )
            {
                /*  If last control was an option button too, we have two
                    option groups following each other, so a dummy separator
                    control is needed. */
                if( bLastWasOptionButton )
                {
                    VbaFormControlVectorRef xDummyGroup( new VbaFormControlVector );
                    aControlGroups.push_back( xDummyGroup );
                    OUString aName = aControlNames.generateDummyName();
                    VbaFormControlRef xDummyControl( new VbaDummyFormControl( aName ) );
                    xDummyGroup->push_back( xDummyControl );
                }
                rxOptionGroup.reset( new VbaFormControlVector );
                aControlGroups.push_back( rxOptionGroup );
            }
            /*  Append the option button to the control group (which is now
                referred by the vector aControlGroups and by the map
                aOptionGroups). */
            rxOptionGroup->push_back( xControl );
            bLastWasOptionButton = true;
        }
        else
        {
            // open a new control group, if the last group is an option group
            if( bLastWasOptionButton || aControlGroups.empty() )
            {
                VbaFormControlVectorRef xControlGroup( new VbaFormControlVector );
                aControlGroups.push_back( xControlGroup );
            }
            // append the control to the last control group
            VbaFormControlVector& rLastGroup = *aControlGroups.back();
            rLastGroup.push_back( xControl );
            bLastWasOptionButton = false;

            // if control is a group box, move all its children to this control
            if( pCtrlModel && (pCtrlModel->getControlType() == API_CONTROL_GROUPBOX) )
            {
                /*  Move all embedded controls of the group box relative to the
                    position of the group box. */
                xControl->moveEmbeddedToAbsoluteParent();
                /*  Insert all children of the group box into the last control
                    group (following the group box). */
                rLastGroup.insert( rLastGroup.end(), xControl->maControls.begin(), xControl->maControls.end() );
                xControl->maControls.clear();
                // check if last control of the group box is an option button
                bLastWasOptionButton = dynamic_cast< const AxOptionButtonModel* >( rLastGroup.back()->mxCtrlModel.get() ) != 0;
            }
        }
    }

    // flatten the vector of vectors of form controls to a single vector
    maControls.clear();
    for( VbaFormControlVectorVector::iterator aIt = aControlGroups.begin(), aEnd = aControlGroups.end(); aIt != aEnd; ++aIt )
        maControls.insert( maControls.end(), (*aIt)->begin(), (*aIt)->end() );
}

void VbaFormControl::moveRelative( const AxPairData& rDistance )
{
    if( mxSiteModel.get() )
        mxSiteModel->moveRelative( rDistance );
}

void VbaFormControl::moveEmbeddedToAbsoluteParent()
{
    if( mxSiteModel.get() && !maControls.empty() )
    {
        // distance to move is equal to position of this control in its parent
        AxPairData aDistance = mxSiteModel->getPosition();

        /*  For group boxes: add half of the font height to Y position (VBA
            positions relative to frame border line, not to 'top' of frame). */
        const AxFontDataModel* pFontModel = dynamic_cast< const AxFontDataModel* >( mxCtrlModel.get() );
        if( pFontModel && (pFontModel->getControlType() == API_CONTROL_GROUPBOX) )
        {
            // convert points to 1/100 mm (1 pt = 1/72 inch = 2.54/72 cm = 2540/72 1/100 mm)
            sal_Int32 nFontHeight = static_cast< sal_Int32 >( pFontModel->getFontHeight() * 2540 / 72 );
            aDistance.second += nFontHeight / 2;
        }

        // move the embedded controls
        maControls.forEachMem( &VbaFormControl::moveRelative, ::boost::cref( aDistance ) );
    }
}

/*static*/ bool VbaFormControl::compareByTabIndex( const VbaFormControlRef& rxLeft, const VbaFormControlRef& rxRight )
{
    // sort controls without model to the end
    sal_Int32 nLeftTabIndex = rxLeft->mxSiteModel.get() ? rxLeft->mxSiteModel->getTabIndex() : SAL_MAX_INT32;
    sal_Int32 nRightTabIndex = rxRight->mxSiteModel.get() ? rxRight->mxSiteModel->getTabIndex() : SAL_MAX_INT32;
    return nLeftTabIndex < nRightTabIndex;
}

// ============================================================================

namespace {

OUString lclGetQuotedString( const OUString& rCodeLine )
{
    OUStringBuffer aBuffer;
    sal_Int32 nLen = rCodeLine.getLength();
    if( (nLen > 0) && (rCodeLine[ 0 ] == '"') )
    {
        bool bExitLoop = false;
        for( sal_Int32 nIndex = 1; !bExitLoop && (nIndex < nLen); ++nIndex )
        {
            sal_Unicode cChar = rCodeLine[ nIndex ];
            // exit on closing quote char (but check on double quote chars)
            bExitLoop = (cChar == '"') && ((nIndex + 1 == nLen) || (rCodeLine[ nIndex + 1 ] != '"'));
            if( !bExitLoop )
            {
                aBuffer.append( cChar );
                // skip second quote char
                if( cChar == '"' )
                    ++nIndex;
            }
        }
    }
    return aBuffer.makeStringAndClear();
}

} // namespace

// ----------------------------------------------------------------------------

VbaUserForm::VbaUserForm( const Reference< XMultiServiceFactory >& rxGlobalFactory,
        const GraphicHelper& rGraphicHelper, bool bDefaultColorBgr ) :
    ControlConverter( rGraphicHelper, bDefaultColorBgr ),
    mxGlobalFactory( rxGlobalFactory )
{
    OSL_ENSURE( mxGlobalFactory.is(), "VbaUserForm::VbaUserForm - missing service factory" );
}

void VbaUserForm::importForm( const Reference< XNameContainer >& rxDialogLib,
        StorageBase& rVbaFormStrg, const OUString& rModuleName, rtl_TextEncoding eTextEnc )
{
    OSL_ENSURE( rxDialogLib.is(), "VbaUserForm::importForm - missing dialog library" );
    if( !mxGlobalFactory.is() || !rxDialogLib.is() )
        return;

    // check that the '03VBFrame' stream exists, this is required for forms
    BinaryXInputStream aInStrm( rVbaFormStrg.openInputStream( CREATE_OUSTRING( "\003VBFrame" ) ), true );
    OSL_ENSURE( !aInStrm.isEof(), "VbaUserForm::importForm - missing \\003VBFrame stream" );
    if( aInStrm.isEof() )
        return;

    // scan for the line 'Begin {GUID} <FormName>'
    TextInputStream aFrameTextStrm( aInStrm, eTextEnc );
    const OUString aBegin = CREATE_OUSTRING( "Begin" );
    OUString aLine;
    bool bBeginFound = false;
    while( !bBeginFound && !aFrameTextStrm.isEof() )
    {
        aLine = aFrameTextStrm.readLine().trim();
        bBeginFound = VbaHelper::eatKeyword( aLine, aBegin );
    }
    // check for the specific GUID that represents VBA forms
    if( !bBeginFound || !VbaHelper::eatKeyword( aLine, CREATE_OUSTRING( "{C62A69F0-16DC-11CE-9E98-00AA00574A4F}" ) ) )
        return;

    // remaining line is the form name
    OUString aFormName = aLine.trim();
    OSL_ENSURE( aFormName.getLength() > 0, "VbaUserForm::importForm - missing form name" );
    OSL_ENSURE( rModuleName.equalsIgnoreAsciiCase( aFormName ), "VbaUserForm::importFrameStream - form and module name mismatch" );
    if( aFormName.getLength() == 0 )
        aFormName = rModuleName;
    if( aFormName.getLength() == 0 )
        return;
    mxSiteModel.reset( new VbaSiteModel );
    mxSiteModel->importProperty( XML_Name, aFormName );

    // read the form properties (caption is contained in this '03VBFrame' stream, not in the 'f' stream)
    mxCtrlModel.reset( new AxUserFormModel );
    OUString aKey, aValue;
    bool bExitLoop = false;
    while( !bExitLoop && !aFrameTextStrm.isEof() )
    {
        aLine = aFrameTextStrm.readLine().trim();
        bExitLoop = aLine.equalsIgnoreAsciiCaseAsciiL( RTL_CONSTASCII_STRINGPARAM( "End" ) );
        if( !bExitLoop && VbaHelper::extractKeyValue( aKey, aValue, aLine ) )
        {
            if( aKey.equalsIgnoreAsciiCaseAsciiL( RTL_CONSTASCII_STRINGPARAM( "Caption" ) ) )
                mxCtrlModel->importProperty( XML_Caption, lclGetQuotedString( aValue ) );
            else if( aKey.equalsIgnoreAsciiCaseAsciiL( RTL_CONSTASCII_STRINGPARAM( "Tag" ) ) )
                mxSiteModel->importProperty( XML_Tag, lclGetQuotedString( aValue ) );
        }
    }

    // use generic container control functionality to import the embedded controls
    importStorage( rVbaFormStrg, AxClassTable() );

    try
    {
        // create the dialog model
        OUString aServiceName = mxCtrlModel->getServiceName();
        Reference< XControlModel > xDialogModel( mxGlobalFactory->createInstance( aServiceName ), UNO_QUERY_THROW );
        Reference< XNameContainer > xDialogNC( xDialogModel, UNO_QUERY_THROW );

        // convert properties and embedded controls
        if( convertProperties( xDialogModel, *this, 0 ) )
        {
            // export the dialog to XML and insert it into the dialog library
            PropertySet aFactoryProps( mxGlobalFactory );
            Reference< XComponentContext > xCompContext( aFactoryProps.getAnyProperty( PROP_DefaultContext ), UNO_QUERY_THROW );
            Reference< XInputStreamProvider > xDialogSource( ::xmlscript::exportDialogModel( xDialogNC, xCompContext ), UNO_SET_THROW );
            OSL_ENSURE( !rxDialogLib->hasByName( aFormName ), "VbaUserForm::importForm - multiple dialogs with equal name" );
            ContainerHelper::insertByName( rxDialogLib, aFormName, Any( xDialogSource ) );
        }
    }
    catch( Exception& )
    {
    }
}

// ============================================================================

} // namespace ole
} // namespace oox
