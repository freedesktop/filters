/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008 by Sun Microsystems, Inc.
 *
 * OpenOffice.org - a multi-platform office productivity suite
 *
 * $RCSfile: SchXMLParagraphContext.hxx,v $
 * $Revision: 1.3 $
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
#ifndef _SCH_XMLPARAGRAPHCONTEXT_HXX_
#define _SCH_XMLPARAGRAPHCONTEXT_HXX_

#ifndef _XMLOFF_XMLICTXT_HXX
#include "xmlictxt.hxx"
#endif

#ifndef _RTL_USTRING_HXX_
#include "rtl/ustring.hxx"
#endif
#ifndef _RTL_USTRBUF_HXX_
#include "rtl/ustrbuf.hxx"
#endif
namespace com { namespace sun { namespace star { namespace xml { namespace sax {
        class XAttributeList;
}}}}}
namespace binfilter {

class SchXMLImport;


class SchXMLParagraphContext : public SvXMLImportContext
{
private:
    ::rtl::OUString& mrText;
    ::rtl::OUStringBuffer maBuffer;

public:
    SchXMLParagraphContext( SvXMLImport& rImport,
                            const ::rtl::OUString& rLocalName,
                            ::rtl::OUString& rText );
    virtual ~SchXMLParagraphContext();	
    virtual void EndElement();
    
    virtual SvXMLImportContext* CreateChildContext(
        USHORT nPrefix,
        const ::rtl::OUString& rLocalName,
        const ::com::sun::star::uno::Reference< ::com::sun::star::xml::sax::XAttributeList >& xAttrList );

    virtual void Characters( const ::rtl::OUString& rChars );
};

}//end of namespace binfilter
#endif	// _SCH_XMLPARAGRAPHCONTEXT_HXX_
