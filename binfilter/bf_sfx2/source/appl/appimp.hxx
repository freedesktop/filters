/*************************************************************************
 *
 *  $RCSfile: appimp.hxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: hr $ $Date: 2004-11-09 12:18:37 $
 *
 *  The Contents of this file are made available subject to the terms of
 *  either of the following licenses
 *
 *         - GNU Lesser General Public License Version 2.1
 *         - Sun Industry Standards Source License Version 1.1
 *
 *  Sun Microsystems Inc., October, 2000
 *
 *  GNU Lesser General Public License Version 2.1
 *  =============================================
 *  Copyright 2000 by Sun Microsystems, Inc.
 *  901 San Antonio Road, Palo Alto, CA 94303, USA
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License version 2.1, as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *  MA  02111-1307  USA
 *
 *
 *  Sun Industry Standards Source License Version 1.1
 *  =================================================
 *  The contents of this file are subject to the Sun Industry Standards
 *  Source License Version 1.1 (the "License"); You may not use this file
 *  except in compliance with the License. You may obtain a copy of the
 *  License at http://www.openoffice.org/license.html.
 *
 *  Software provided under this License is provided on an "AS IS" basis,
 *  WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING,
 *  WITHOUT LIMITATION, WARRANTIES THAT THE SOFTWARE IS FREE OF DEFECTS,
 *  MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE, OR NON-INFRINGING.
 *  See the License for the specific provisions governing your rights and
 *  obligations concerning the Software.
 *
 *  The Initial Developer of the Original Code is: Sun Microsystems, Inc.
 *
 *  Copyright: 2000 by Sun Microsystems, Inc.
 *
 *  All Rights Reserved.
 *
 *  Contributor(s): _______________________________________
 *
 *
 ************************************************************************/
#ifndef _SFXAPPIMP_HXX
#define _SFXAPPIMP_HXX

#include <tools/time.hxx>
#include <tools/string.hxx>
class MenuBar;
class ResMgr;
class UniqueIndex;
class BasicManager;
class Timer;
class SimpleResMgr;
namespace binfilter {

class SfxTbxCtrlFactArr_Impl;
class SfxStbCtrlFactArr_Impl;
class SfxMenuCtrlFactArr_Impl;
class SfxViewFrameArr_Impl;
class SfxViewShellArr_Impl;
class SfxObjectShellArr_Impl;
class IntroWindow_Impl;
class SfxTemplateDialog;
class SfxDialogLibraryContainer;
class SfxScriptLibraryContainer;
class SfxBasicTestWin;

struct SfxApplication_Impl
{
    Time                        aAutoSaveTime;
    String                      aMemExceptionString;
    String                      aResWarningString;
    String                      aResExceptionString;
    String                      aSysResExceptionString;
    String                      aDoubleExceptionString;
    String                      aBasicSourceName;
    SfxTbxCtrlFactArr_Impl*     pTbxCtrlFac;
    SfxStbCtrlFactArr_Impl*     pStbCtrlFac;
    SfxMenuCtrlFactArr_Impl*    pMenuCtrlFac;
    SfxViewFrameArr_Impl*       pViewFrames;
    SfxViewShellArr_Impl*       pViewShells;
    SfxObjectShellArr_Impl*     pObjShells;
    MenuBar*                    pEmptyMenu;     	// dummy for no-menu
    IntroWindow_Impl*			pIntro;
    ResMgr*                     pSfxResManager;
    SimpleResMgr*				pSimpleResManager;
    UniqueIndex*                pEventHdl;  		// Hook-Liste fuer UserEvents
    SfxTemplateDialog*          pTemplateDlg;
    SfxScriptLibraryContainer*	pBasicLibContainer;
    SfxDialogLibraryContainer*	pDialogLibContainer;
    SfxBasicTestWin*            pBasicTestWin;
    Timer*                		pAutoSaveTimer;
    USHORT                      nDocNo;     		// Laufende Doc-Nummer (AutoName)
    USHORT                      nWarnLevel;
    BOOL                        bConfigLoaded:1;
    BOOL                        bAutoSaveNow:1; 	// ist TRUE, wenn der Timer abgelaufen ist, wenn die App nicht aktiv war
};

}//end of namespace binfilter
#endif

