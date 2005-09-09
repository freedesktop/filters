/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: chldwrap.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2005-09-09 05:23:12 $
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
#ifndef _CHLDWRAP_HXX
#define _CHLDWRAP_HXX

#ifndef _SFX_CHILDWIN_HXX //autogen
#include <bf_sfx2/childwin.hxx>
#endif

#ifndef _SV_TIMER_HXX //autogen
#include <vcl/timer.hxx>
#endif
namespace binfilter {

class SwDocShell;

class SwChildWinWrapper : public SfxChildWindow
{
//STRIP001 	Timer		aUpdateTimer;
//STRIP001 	SwDocShell*	pDocSh;

//STRIP001 	DECL_LINK( UpdateHdl, void* );

    // Implementation in fldtdlg.cxx
protected:
    SwChildWinWrapper(Window *pParentWindow, USHORT nId):SfxChildWindow(pParentWindow, nId){DBG_BF_ASSERT(0, "STRIP");} //STRIP001 SwChildWinWrapper(Window *pParentWindow, USHORT nId);

//STRIP001 public:
//STRIP001 	virtual BOOL	ReInitDlg(SwDocShell *pDocSh);
//STRIP001 
//STRIP001 	inline SwDocShell*	GetOldDocShell()					{ return pDocSh; }
//STRIP001 	inline void			SetOldDocShell(SwDocShell *pDcSh)	{ pDocSh = pDcSh; }
};


} //namespace binfilter
#endif

