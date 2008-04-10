/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008 by Sun Microsystems, Inc.
 *
 * OpenOffice.org - a multi-platform office productivity suite
 *
 * $RCSfile: DrawModelBroadcaster.hxx,v $
 * $Revision: 1.5 $
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

#ifndef _SC_DRAWMODELBROADCASTER_HXX
#define _SC_DRAWMODELBROADCASTER_HXX


#ifndef _SFXLSTNER_HXX 
#include <bf_svtools/lstner.hxx>
#endif
#ifndef _CPPUHELPER_INTERFACECONTAINER_H_ 
#include <cppuhelper/interfacecontainer.h>
#endif
#ifndef _CPPUHELPER_IMPLBASE1_HXX_ 
#include <cppuhelper/implbase1.hxx>
#endif

#ifndef _COM_SUN_STAR_DOCUMENT_XEVENTBROADCASTER_HPP_ 
#include <com/sun/star/document/XEventBroadcaster.hpp>
#endif
namespace binfilter {

class SdrModel;

class ScDrawModelBroadcaster : public SfxListener,
    public ::cppu::WeakImplHelper1< ::com::sun::star::document::XEventBroadcaster >
{
    mutable ::osl::Mutex maListenerMutex;
    ::cppu::OInterfaceContainerHelper maEventListeners;
    SdrModel *mpDrawModel;

public:

    ScDrawModelBroadcaster( SdrModel *pDrawModel ); 
    virtual ~ScDrawModelBroadcaster();

    virtual void SAL_CALL addEventListener( const ::com::sun::star::uno::Reference< ::com::sun::star::document::XEventListener >& xListener ) 
        throw (::com::sun::star::uno::RuntimeException);
    virtual void SAL_CALL removeEventListener( const ::com::sun::star::uno::Reference< ::com::sun::star::document::XEventListener >& xListener ) 
        throw (::com::sun::star::uno::RuntimeException);

    virtual void		Notify( SfxBroadcaster& rBC, const SfxHint& rHint );
};

} //namespace binfilter
#endif
