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
#ifndef _SXSALITM_HXX
#define _SXSALITM_HXX

#include <bf_svx/svddef.hxx>

#include <bf_svx/sdangitm.hxx>
namespace binfilter {

//------------------------------
// class SdrHorzShearAllItem
//------------------------------
class SdrHorzShearAllItem: public SdrAngleItem {
public:
    SdrHorzShearAllItem(long nAngle=0): SdrAngleItem(SDRATTR_HORZSHEARALL,nAngle) {}
    SdrHorzShearAllItem(SvStream& rIn): SdrAngleItem(SDRATTR_HORZSHEARALL,rIn)    {}
};

//------------------------------
// class SdrVertShearAllItem
//------------------------------
class SdrVertShearAllItem: public SdrAngleItem {
public:
    SdrVertShearAllItem(long nAngle=0): SdrAngleItem(SDRATTR_VERTSHEARALL,nAngle) {}
    SdrVertShearAllItem(SvStream& rIn): SdrAngleItem(SDRATTR_VERTSHEARALL,rIn)    {}
};

}//end of namespace binfilter
#endif
