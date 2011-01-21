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

#ifndef GRAPHSH_HXX
#define GRAPHSH_HXX

#include <bf_sfx2/shell.hxx>
#include "shellids.hxx"
#include <bf_sfx2/module.hxx>

#include <bf_svx/svdmark.hxx>

class ScViewData;

#include "drawsh.hxx"
namespace binfilter {

class ScGraphicShell: public ScDrawShell
{
public:

    SFX_DECL_INTERFACE(SCID_GRAPHIC_SHELL);

    ScGraphicShell(ScViewData* pData):ScDrawShell(pData){DBG_BF_ASSERT(0, "STRIP");}
    void	Execute(SfxRequest& rReq){DBG_BF_ASSERT(0, "STRIP");}
    void	GetAttrState(SfxItemSet &rSet){DBG_BF_ASSERT(0, "STRIP");}
 
    void	ExecuteFilter(SfxRequest& rReq){DBG_BF_ASSERT(0, "STRIP");}
    void	GetFilterState(SfxItemSet &rSet){DBG_BF_ASSERT(0, "STRIP");}
};

} //namespace binfilter
#endif

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
