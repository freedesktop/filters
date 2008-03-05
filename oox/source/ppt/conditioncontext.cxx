/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: conditioncontext.cxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: kz $ $Date: 2008-03-05 18:46:23 $
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

#include "conditioncontext.hxx"

#include "comphelper/anytostring.hxx"
#include "cppuhelper/exc_hlp.hxx"
#include <osl/diagnose.h>

#include <com/sun/star/animations/XTimeContainer.hpp>
#include <com/sun/star/animations/XAnimationNode.hpp>
#include <com/sun/star/animations/AnimationEndSync.hpp>
#include <com/sun/star/animations/EventTrigger.hpp>

#include "oox/helper/attributelist.hxx"
#include "oox/core/namespaces.hxx"
#include "oox/core/contexthandler.hxx"
#include "oox/ppt/animationspersist.hxx"
#include "animationtypes.hxx"

#include "timetargetelementcontext.hxx"
#include "tokens.hxx"

using namespace ::oox::core;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::xml::sax;
using namespace ::com::sun::star::animations;

namespace oox { namespace ppt {

    CondContext::CondContext( ContextHandler& rParent, const Reference< XFastAttributeList >& xAttribs,
                const TimeNodePtr & pNode, AnimationCondition & aValue )
        :  TimeNodeContext( rParent, NMSP_PPT|XML_cond, xAttribs, pNode )
        , maCond( aValue )
    {
        maEvent.Trigger =  EventTrigger::NONE;
        maEvent.Repeat = 0;

        AttributeList attribs( xAttribs );
        if( attribs.hasAttribute( XML_evt ) )
        {
            sal_Int32 nEvent = xAttribs->getOptionalValueToken( XML_evt, 0 );
            switch( nEvent )
            {
            case XML_onBegin:
                maEvent.Trigger = EventTrigger::ON_BEGIN;
                break;
            case XML_onEnd:
                maEvent.Trigger = EventTrigger::ON_END;
                break;
            case XML_begin:
                maEvent.Trigger = EventTrigger::BEGIN_EVENT;
                break;
            case XML_end:
                maEvent.Trigger = EventTrigger::END_EVENT;
                break;
            case XML_onClick:
                maEvent.Trigger = EventTrigger::ON_CLICK;
                break;
            case XML_onDblClick:
                maEvent.Trigger = EventTrigger::ON_DBL_CLICK;
                break;
            case XML_onMouseOver:
                maEvent.Trigger = EventTrigger::ON_MOUSE_ENTER;
                break;
            case XML_onMouseOut:
                maEvent.Trigger = EventTrigger::ON_MOUSE_LEAVE;
                break;
            case XML_onNext:
                maEvent.Trigger = EventTrigger::ON_NEXT;
                break;
            case XML_onPrev:
                maEvent.Trigger = EventTrigger::ON_PREV;
                break;
            case XML_onStopAudio:
                maEvent.Trigger = EventTrigger::ON_STOP_AUDIO;
                break;
            default:
                break;
            }
        }
        if( attribs.hasAttribute( XML_delay ) || ( maEvent.Trigger == EventTrigger::NONE ) )
        {
            maEvent.Offset = GetTime( xAttribs->getOptionalValue( XML_delay ) );
        }
    }

    CondContext::~CondContext( ) throw( )
    {
        if( maCond.mnType == 0 )
        {
            maCond.maValue = (maEvent.Trigger == EventTrigger::NONE) ? maEvent.Offset : makeAny( maEvent );
        }
    }

    Reference< XFastContextHandler > SAL_CALL CondContext::createFastChildContext( ::sal_Int32 aElementToken, const Reference< XFastAttributeList >& xAttribs ) throw ( SAXException, RuntimeException )
    {
        Reference< XFastContextHandler > xRet;

        switch( aElementToken )
        {
        case NMSP_PPT|XML_rtn:
        {
            // ST_TLTriggerRuntimeNode { first, last, all }
            sal_Int32 aTok;
            sal_Int16 nEnum;
            aTok = xAttribs->getOptionalValueToken( XML_val, XML_first );
            switch( aTok )
            {
            case XML_first:
                nEnum = AnimationEndSync::FIRST;
                break;
            case XML_last:
                nEnum = AnimationEndSync::LAST;
                break;
            case XML_all:
                nEnum = AnimationEndSync::ALL;
                break;
            default:
                break;
            }
            maCond.mnType = aElementToken;
            maCond.maValue = makeAny( nEnum );
            break;
        }
        case NMSP_PPT|XML_tn:
        {
            maCond.mnType = aElementToken;
            AttributeList attribs( xAttribs );
            sal_uInt32 nId = attribs.getUnsignedInteger( XML_val, 0 );
            maCond.maValue = makeAny( nId );
            break;
        }
        case NMSP_PPT|XML_tgtEl:
            // CT_TLTimeTargetElement
            xRet.set( new TimeTargetElementContext( *this, maCond.getTarget() ) );
            break;
        default:
            break;
        }

        if( !xRet.is() )
            xRet.set( this );

        return xRet;

    }



    /** CT_TLTimeConditionList */
    CondListContext::CondListContext(
            ContextHandler& rParent, sal_Int32  aElement,
            const Reference< XFastAttributeList >& xAttribs,
            const TimeNodePtr & pNode,
            AnimationConditionList & aCond )
        : TimeNodeContext( rParent, aElement, xAttribs, pNode )
        , maConditions( aCond )
    {
    }

    CondListContext::~CondListContext( )
        throw( )
    {
    }

    Reference< XFastContextHandler > CondListContext::createFastChildContext( ::sal_Int32 aElement, const Reference< XFastAttributeList >& xAttribs ) throw ( SAXException, RuntimeException )
    {
        Reference< XFastContextHandler > xRet;

        switch( aElement )
        {
        case NMSP_PPT|XML_cond:
            // add a condition to the list
            maConditions.push_back( AnimationCondition() );
            xRet.set( new CondContext( *this, xAttribs, mpNode, maConditions.back() ) );
            break;
        default:
            break;
        }

        if( !xRet.is() )
            xRet.set( this );

        return xRet;
    }


} }

