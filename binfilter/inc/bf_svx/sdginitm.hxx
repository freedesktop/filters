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

#ifndef _SDGINITM_HXX
#define _SDGINITM_HXX

#ifndef _SDOOITM_HXX
#include <bf_svx/sdooitm.hxx>
#endif
#ifndef _SVDDEF_HXX
#include <bf_svx/svddef.hxx>
#endif
namespace binfilter {

//--------------------
// SdrGrafInvertItem -
//--------------------

class SdrGrafInvertItem : public SdrOnOffItem
{
public:

                            TYPEINFO();

                            SdrGrafInvertItem( BOOL bInvert = FALSE ) : SdrOnOffItem( SDRATTR_GRAFINVERT, bInvert ) {}
                            SdrGrafInvertItem( SvStream& rIn ) : SdrOnOffItem( SDRATTR_GRAFINVERT, rIn ) {}

    virtual SfxPoolItem*	Clone( SfxItemPool* pPool = NULL ) const;
    virtual SfxPoolItem*	Create( SvStream& rIn, USHORT nVer ) const;
};

}//end of namespace binfilter
#endif // _SDGINITM_HXX
