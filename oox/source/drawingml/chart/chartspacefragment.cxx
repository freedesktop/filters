/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008 by Sun Microsystems, Inc.
 *
 * OpenOffice.org - a multi-platform office productivity suite
 *
 * $RCSfile: chartspacefragment.cxx,v $
 *
 * $Revision: 1.2 $
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

#include "oox/drawingml/chart/chartspacefragment.hxx"
#include "oox/drawingml/shapepropertiescontext.hxx"
#include "oox/drawingml/textbodycontext.hxx"
#include "oox/drawingml/chart/chartspacemodel.hxx"
#include "oox/drawingml/chart/plotareacontext.hxx"
#include "oox/drawingml/chart/titlecontext.hxx"

using ::rtl::OUString;
using ::oox::core::ContextWrapper;
using ::oox::core::XmlFilterBase;

namespace oox {
namespace drawingml {
namespace chart {

// ============================================================================

ChartSpaceFragment::ChartSpaceFragment( XmlFilterBase& rFilter, const OUString& rFragmentPath, ChartSpaceModel& rModel ) :
    FragmentBase< ChartSpaceModel >( rFilter, rFragmentPath, rModel )
{
}

ChartSpaceFragment::~ChartSpaceFragment()
{
}

ContextWrapper ChartSpaceFragment::onCreateContext( sal_Int32 nElement, const AttributeList& rAttribs )
{
    switch( getCurrentElement() )
    {
        case XML_ROOT_CONTEXT:
            return  (nElement == C_TOKEN( chartSpace ));

        case C_TOKEN( chartSpace ):
            switch( nElement )
            {
                case C_TOKEN( chart ):
                    return true;
                case C_TOKEN( spPr ):
                    return new ShapePropertiesContext( *this, mrModel.mxShapeProp.create() );
                case C_TOKEN( style ):
                    mrModel.mnStyle = rAttribs.getInteger( XML_val, -1 );
                    return false;
                case C_TOKEN( txPr ):
                    return new TextBodyContext( *this, mrModel.mxTextProp.create() );
            }
        break;

        case C_TOKEN( chart ):
            switch( nElement )
            {
                case C_TOKEN( autoTitleDeleted ):
                    mrModel.mbAutoTitleDel = rAttribs.getBool( XML_val, true );
                    return false;
                case C_TOKEN( dispBlanksAs ):
                    mrModel.mnDispBlanksAs = rAttribs.getToken( XML_val, XML_zero );
                    return false;
                case C_TOKEN( legend ):
                    return new LegendContext( *this, mrModel.mxLegend.create() );
                case C_TOKEN( plotArea ):
                    return new PlotAreaContext( *this, mrModel.mxPlotArea.create() );
                case C_TOKEN( plotVisOnly ):
                    mrModel.mbPlotVisOnly = rAttribs.getBool( XML_val, true );
                    return false;
                case C_TOKEN( showDLblsOverMax ):
                    mrModel.mbShowLabelsOverMax = rAttribs.getBool( XML_val, true );
                    return false;
                case C_TOKEN( title ):
                    return new TitleContext( *this, mrModel.mxTitle.create() );
                case C_TOKEN( view3D ):
                    return new View3DContext( *this, mrModel.mxView3D.create() );
            }
        break;
    }
    return false;
}

// ============================================================================

} // namespace chart
} // namespace drawingml
} // namespace oox

