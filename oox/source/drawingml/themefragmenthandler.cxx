/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008 by Sun Microsystems, Inc.
 *
 * OpenOffice.org - a multi-platform office productivity suite
 *
 * $RCSfile: themefragmenthandler.cxx,v $
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

#include "oox/drawingml/themefragmenthandler.hxx"
#include "oox/drawingml/objectdefaultcontext.hxx"
#include "oox/drawingml/themeelementscontext.hxx"

using ::rtl::OUString;
using namespace ::oox::core;

namespace oox {
namespace drawingml {

// ============================================================================

ThemeFragmentHandler::ThemeFragmentHandler( XmlFilterBase& rFilter, const OUString& rFragmentPath, Theme& rTheme ) :
    FragmentHandler2( rFilter, rFragmentPath ),
    mrTheme( rTheme )
{
}

ThemeFragmentHandler::~ThemeFragmentHandler()
{
}

ContextWrapper ThemeFragmentHandler::onCreateContext( sal_Int32 nElement, const AttributeList& )
{
    // CT_OfficeStyleSheet
    switch( getCurrentElement() )
    {
        case XML_ROOT_CONTEXT:
            return nElement == (NMSP_DRAWINGML|XML_theme);

        case NMSP_DRAWINGML|XML_theme:
            switch( nElement )
            {
                case NMSP_DRAWINGML|XML_themeElements:              // CT_BaseStyles
                    return new themeElementsContext( *this, mrTheme );
                case NMSP_DRAWINGML|XML_objectDefaults:             // CT_ObjectStyleDefaults
                    return new objectDefaultContext( *this, mrTheme );
                case NMSP_DRAWINGML|XML_extraClrSchemeLst:          // CT_ColorSchemeList
                    return false;
                case NMSP_DRAWINGML|XML_custClrLst:                 // CustomColorList
                    return false;
                case NMSP_DRAWINGML|XML_ext:                        // CT_OfficeArtExtension
                    return false;
            }
        break;
    }
    return false;
}

// ============================================================================

} // namespace drawingml
} // namespace oox

