/*************************************************************************
 *
 *  $RCSfile: characterdata.hxx,v $
 *
 *  $Revision: 1.2 $
 *
 *  last change: $Author: lo $ $Date: 2004-02-16 16:41:46 $
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
 *  Copyright: 2004 by Sun Microsystems, Inc.
 *
 *  All Rights Reserved.
 *
 *  Contributor(s): _______________________________________
 *
 *
 ************************************************************************/

#ifndef _CHARACTERDATA_HXX
#define _CHARACTERDATA_HXX

#include <sal/types.h>
#include <cppuhelper/implbase1.hxx>
#include <com/sun/star/uno/Reference.h>
#include <com/sun/star/uno/Exception.hpp>
#include <com/sun/star/xml/dom/XNode.hpp>
#include <com/sun/star/xml/dom/XCharacterData.hpp>
#include <com/sun/star/xml/dom/XElement.hpp>
#include <com/sun/star/xml/dom/XDOMImplementation.hpp>
#include <libxml/tree.h>
#include "node.hxx"

using namespace rtl;
using namespace com::sun::star::uno;
using namespace com::sun::star::lang;
using namespace com::sun::star::xml::dom;

namespace DOM
{
    class CCharacterData : public cppu::ImplInheritanceHelper1< CNode, XCharacterData >
    {        


    protected:
        CCharacterData();
        void init_characterdata(const xmlNodePtr aNodePtr);
        void _dispatchEvent(const OUString& prevValue, const OUString& newValue);

    public:
        /**
        Append the string to the end of the character data of the node.
        */
        virtual void SAL_CALL appendData(const OUString& arg)
            throw (RuntimeException);
        
        /**
        Remove a range of 16-bit units from the node.
        */
        virtual void SAL_CALL deleteData(sal_Int32 offset, sal_Int32 count) 
            throw (RuntimeException);

        /**
        Return the character data of the node that implements this interface.
        */
        virtual OUString SAL_CALL getData() throw (RuntimeException);

        /**
        The number of 16-bit units that are available through data and the
        substringData method below.
        */
        virtual sal_Int32 SAL_CALL getLength() throw (RuntimeException);

        /**
        Insert a string at the specified 16-bit unit offset.
        */
        virtual void SAL_CALL insertData(sal_Int32 offset, const OUString& arg)
            throw (RuntimeException);

        /**
        Replace the characters starting at the specified 16-bit unit offset 
        with the specified string.
        */
        virtual void SAL_CALL replaceData(sal_Int32 offset, sal_Int32 count, const OUString& arg)
            throw (RuntimeException);

        /**
        Set the character data of the node that implements this interface.
        */
        virtual void SAL_CALL setData(const OUString& data)
            throw (RuntimeException);

        /**
        Extracts a range of data from the node.
        */
        virtual OUString SAL_CALL subStringData(sal_Int32 offset, sal_Int32 count)
            throw (RuntimeException);

    };
}

#endif