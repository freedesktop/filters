/*************************************************************************
 *
 *  $RCSfile: attributes.hxx,v $
 *
 *  $Revision: 1.1 $
 *
 *  last change: $Author: dvo $ $Date: 2003-10-15 14:34:10 $
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
 *  Copyright 2001 by Mizi Research Inc.
 *  Copyright 2003 by Sun Microsystems, Inc.
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
 *  The Initial Developer of the Original Code is: Mizi Research Inc.
 *
 *  Copyright: 2001 by Mizi Research Inc.
 *  Copyright: 2003 by Sun Microsystems, Inc.
 *
 *  All Rights Reserved.
 *
 *  Contributor(s): _______________________________________
 *
 *
 ************************************************************************/


#ifndef _CONFIGMGR_MISC_ATTRIBUTES_HXX_
#define _CONFIGMGR_MISC_ATTRIBUTES_HXX_

#ifndef _COM_SUN_STAR_XML_SAX_XATTRIBUTELIST_HPP_
#include <com/sun/star/xml/sax/XAttributeList.hpp>
#endif

#ifndef _CPPUHELPER_IMPLBASE1_HXX_
#include <cppuhelper/implbase1.hxx>
#endif

/*----------------------------------------
*
* 	Attributlist implementation
*
*----------------------------------------*/

using namespace ::cppu;
using namespace ::rtl;
using namespace ::com::sun::star::xml::sax;
using namespace ::com::sun::star::uno;

struct AttributeListImpl_impl;
class AttributeListImpl : public WeakImplHelper1< XAttributeList >
{
protected:
    ~AttributeListImpl();

public:
    AttributeListImpl();
    AttributeListImpl( const AttributeListImpl & );
    
public:
    virtual sal_Int16 SAL_CALL getLength(void) throw (RuntimeException);
    virtual OUString  SAL_CALL getNameByIndex(sal_Int16 i) throw (RuntimeException);
    virtual OUString  SAL_CALL getTypeByIndex(sal_Int16 i) throw (RuntimeException);
    virtual OUString  SAL_CALL getTypeByName(const OUString& aName) throw (RuntimeException);
    virtual OUString  SAL_CALL getValueByIndex(sal_Int16 i) throw (RuntimeException);
    virtual OUString  SAL_CALL getValueByName(const OUString& aName) throw (RuntimeException);

public:
    void addAttribute( const OUString &sName , const OUString &sType , const OUString &sValue );
    void clear();

private:
    struct AttributeListImpl_impl *m_pImpl;
};

#endif // _CONFIGMGR_MISC_ATTRIBUTES_HXX_


