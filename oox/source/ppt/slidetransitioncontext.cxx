/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
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

#include "oox/ppt/slidetransitioncontext.hxx"

#include "comphelper/anytostring.hxx"
#include "cppuhelper/exc_hlp.hxx"

#include <com/sun/star/beans/XMultiPropertySet.hpp>
#include <com/sun/star/container/XNamed.hpp>

#include <oox/ppt/backgroundproperties.hxx>
#include "oox/ppt/slidefragmenthandler.hxx"
#include "oox/ppt/soundactioncontext.hxx"
#include "oox/drawingml/shapegroupcontext.hxx"
#include "oox/helper/attributelist.hxx"

using rtl::OUString;
using namespace ::com::sun::star;
using namespace ::oox::core;
using namespace ::oox::drawingml;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::xml::sax;
using namespace ::com::sun::star::container;

namespace oox { namespace ppt {


SlideTransitionContext::SlideTransitionContext( FragmentHandler2& rParent, const AttributeList& rAttribs, PropertyMap & aProperties ) throw()
: FragmentHandler2( rParent )
, maSlideProperties( aProperties )
, mbHasTransition( sal_False )
{
    // ST_TransitionSpeed
    maTransition.setOoxTransitionSpeed( rAttribs.getToken( XML_spd, XML_fast ) );

    // TODO
    rAttribs.getBool( XML_advClick, true );

    // careful. if missing, no auto advance... 0 looks like a valid value
    // for auto advance
    if(rAttribs.hasAttribute( XML_advTm ))
        maTransition.setOoxAdvanceTime( rAttribs.getInteger( XML_advTm, -1 ) );
}

SlideTransitionContext::~SlideTransitionContext() throw()
{

}

::oox::core::ContextHandlerRef SlideTransitionContext::onCreateContext( sal_Int32 aElementToken, const AttributeList& rAttribs )
{
    switch( aElementToken )
    {
    case PPT_TOKEN( blinds ):
    case PPT_TOKEN( checker ):
    case PPT_TOKEN( comb ):
    case PPT_TOKEN( randomBar ):
        if (!mbHasTransition)
        {
            mbHasTransition = true;
            maTransition.setOoxTransitionType( aElementToken, rAttribs.getToken( XML_dir, XML_horz ), 0);
            // ST_Direction { XML_horz, XML_vert }
        }
        return this;
    case PPT_TOKEN( cover ):
    case PPT_TOKEN( pull ):
        if (!mbHasTransition)
        {
            mbHasTransition = true;
            maTransition.setOoxTransitionType( aElementToken, rAttribs.getToken( XML_dir, XML_l ), 0 );
            // ST_TransitionEightDirectionType { ST_TransitionSideDirectionType {
            //                                   XML_d, XML_d, XML_r, XML_u },
            //                                   ST_TransitionCornerDirectionType {
            //                                   XML_ld, XML_lu, XML_rd, XML_ru }
        }
        return this;
    case PPT_TOKEN( cut ):
    case PPT_TOKEN( fade ):
        if (!mbHasTransition)
        {
            mbHasTransition = true;
            // CT_OptionalBlackTransition xdb:bool
            maTransition.setOoxTransitionType( aElementToken, rAttribs.getBool( XML_thruBlk, false ), 0);
        }
        return this;
    case PPT_TOKEN( push ):
    case PPT_TOKEN( wipe ):
        if (!mbHasTransition)
        {
            mbHasTransition = true;
            maTransition.setOoxTransitionType( aElementToken, rAttribs.getToken( XML_dir, XML_l ), 0 );
            // ST_TransitionSideDirectionType { XML_d, XML_l, XML_r, XML_u }
        }
        return this;
    case PPT_TOKEN( split ):
        if (!mbHasTransition)
        {
            mbHasTransition = true;
            maTransition.setOoxTransitionType( aElementToken, rAttribs.getToken( XML_orient, XML_horz ),	rAttribs.getToken( XML_dir, XML_out ) );
            // ST_Direction { XML_horz, XML_vert }
            // ST_TransitionInOutDirectionType { XML_out, XML_in }
        }
        return this;
    case PPT_TOKEN( zoom ):
        if (!mbHasTransition)
        {
            mbHasTransition = true;
            maTransition.setOoxTransitionType( aElementToken, rAttribs.getToken( XML_dir, XML_out ), 0 );
            // ST_TransitionInOutDirectionType { XML_out, XML_in }
        }
        return this;
    case PPT_TOKEN( wheel ):
        if (!mbHasTransition)
        {
            mbHasTransition = true;
            maTransition.setOoxTransitionType( aElementToken, rAttribs.getUnsigned( XML_spokes, 4 ), 0 );
            // unsignedInt
        }
        return this;
    case PPT_TOKEN( circle ):
    case PPT_TOKEN( diamond ):
    case PPT_TOKEN( dissolve ):
    case PPT_TOKEN( newsflash ):
    case PPT_TOKEN( plus ):
    case PPT_TOKEN( random ):
    case PPT_TOKEN( wedge ):
        // CT_Empty
        if (!mbHasTransition)
        {
            mbHasTransition = true;
            maTransition.setOoxTransitionType( aElementToken, 0, 0 );
        }
        return this;

    case PPT_TOKEN( sndAc ): // CT_TransitionSoundAction
        //"Sound"
        return new SoundActionContext ( *this, maSlideProperties );
    case PPT_TOKEN( extLst ): // CT_OfficeArtExtensionList
        return this;
    default:
        break;
    }

    return this;
}

void SlideTransitionContext::onEndElement()
{
    if( isCurrentElement(PPT_TOKEN( transition )) )
    {
        if( mbHasTransition )
        {
            maTransition.setSlideProperties( maSlideProperties );
            mbHasTransition = false;
        }
    }
}


} }

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
