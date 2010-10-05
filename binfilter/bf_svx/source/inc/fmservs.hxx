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
#ifndef _SVX_FMSERVS_HXX
#define _SVX_FMSERVS_HXX

#include "fmstatic.hxx"
namespace binfilter {

namespace svxform
{

    DECLARE_CONSTASCII_USTRING(FM_NUMBER_FORMATTER);

    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_EDIT);		// alter service name (5.0)
    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_TEXTFIELD);

    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_LISTBOX);
    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_COMBOBOX);
    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_RADIOBUTTON);
    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_GROUPBOX);
    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_FIXEDTEXT);
    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_COMMANDBUTTON);
    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_CHECKBOX);

    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_GRID);		// alter service name (5.0)
    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_GRIDCONTROL);

    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_IMAGEBUTTON);
    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_FILECONTROL);
    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_TIMEFIELD);
    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_DATEFIELD);
    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_NUMERICFIELD);
    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_CURRENCYFIELD);
    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_PATTERNFIELD);
    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_FORMATTEDFIELD);

    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_HIDDEN);	// alter service name (5.0)
    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_HIDDENCONTROL);

    DECLARE_CONSTASCII_USTRING(FM_COMPONENT_IMAGECONTROL);

    DECLARE_CONSTASCII_USTRING(FM_CONTROL_GRID);
    DECLARE_CONSTASCII_USTRING(FM_CONTROL_GRIDCONTROL);

    DECLARE_CONSTASCII_USTRING(FM_FORM_CONTROLLER);
    DECLARE_CONSTASCII_USTRING(SRV_SDB_CONNECTION);
    DECLARE_CONSTASCII_USTRING(SRV_SDB_INTERACTION_HANDLER);

    DECLARE_CONSTASCII_USTRING(FM_SUN_COMPONENT_FORM);
    DECLARE_CONSTASCII_USTRING(FM_SUN_COMPONENT_TEXTFIELD);
    DECLARE_CONSTASCII_USTRING(FM_SUN_COMPONENT_LISTBOX);
    DECLARE_CONSTASCII_USTRING(FM_SUN_COMPONENT_COMBOBOX);
    DECLARE_CONSTASCII_USTRING(FM_SUN_COMPONENT_RADIOBUTTON);
    DECLARE_CONSTASCII_USTRING(FM_SUN_COMPONENT_GROUPBOX);
    DECLARE_CONSTASCII_USTRING(FM_SUN_COMPONENT_FIXEDTEXT);
    DECLARE_CONSTASCII_USTRING(FM_SUN_COMPONENT_COMMANDBUTTON);
    DECLARE_CONSTASCII_USTRING(FM_SUN_COMPONENT_CHECKBOX);
    DECLARE_CONSTASCII_USTRING(FM_SUN_COMPONENT_GRIDCONTROL);
    DECLARE_CONSTASCII_USTRING(FM_SUN_COMPONENT_IMAGEBUTTON);
    DECLARE_CONSTASCII_USTRING(FM_SUN_COMPONENT_FILECONTROL);
    DECLARE_CONSTASCII_USTRING(FM_SUN_COMPONENT_TIMEFIELD);
    DECLARE_CONSTASCII_USTRING(FM_SUN_COMPONENT_DATEFIELD);
    DECLARE_CONSTASCII_USTRING(FM_SUN_COMPONENT_NUMERICFIELD);
    DECLARE_CONSTASCII_USTRING(FM_SUN_COMPONENT_CURRENCYFIELD);
    DECLARE_CONSTASCII_USTRING(FM_SUN_COMPONENT_PATTERNFIELD);
    DECLARE_CONSTASCII_USTRING(FM_SUN_COMPONENT_HIDDENCONTROL);
    DECLARE_CONSTASCII_USTRING(FM_SUN_COMPONENT_IMAGECONTROL);
    DECLARE_CONSTASCII_USTRING(FM_SUN_COMPONENT_FORMATTEDFIELD);

    DECLARE_CONSTASCII_USTRING(FM_SUN_CONTROL_GRIDCONTROL);

    void ImplSmartRegisterUnoServices();

}	// namespace svxform

}//end of namespace binfilter
#endif // _SVX_FMSERVS_HXX

