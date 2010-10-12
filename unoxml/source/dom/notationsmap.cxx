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

#include "notationsmap.hxx"

namespace DOM
{
    CNotationsMap::CNotationsMap(const CDocumentType* aDocType)
        : m_pDocType(aDocType)
    {
    }

    /**
    The number of nodes in this map.
    */
    sal_Int32 SAL_CALL CNotationsMap::getLength() throw (RuntimeException)
    {
        return 0;
    }

    /**
    Retrieves a node specified by local name     	
    */
  Reference< XNode > SAL_CALL CNotationsMap::getNamedItem(const OUString& /* name */) throw (RuntimeException)
    {
        return Reference< XNode >();
    }

    /**
    Retrieves a node specified by local name and namespace URI.
    */
  Reference< XNode > SAL_CALL CNotationsMap::getNamedItemNS(const OUString& /*namespaceURI*/, const OUString& /*localName*/) 
throw (RuntimeException)
    {
        return Reference< XNode >();
    }

    /**
    Returns the indexth item in the map.
    */
  Reference< XNode > SAL_CALL CNotationsMap::item(sal_Int32 /*index*/) throw (RuntimeException)
    {
        return Reference< XNode >();
    }

    /**
    Removes a node specified by name.
    */
  Reference< XNode > SAL_CALL CNotationsMap::removeNamedItem(const OUString& /*name*/) throw (RuntimeException)
    {
        return Reference< XNode >();
    }

    /**
    // Removes a node specified by local name and namespace URI.
    */
  Reference< XNode > SAL_CALL CNotationsMap::removeNamedItemNS(const OUString& /*namespaceURI*/, const OUString& /*localName*/) throw (RuntimeException)
    {
        return Reference< XNode >();
    }

    /**
    // Adds a node using its nodeName attribute.
    */
  Reference< XNode > SAL_CALL CNotationsMap::setNamedItem(const Reference< XNode >& /*arg*/) throw (RuntimeException)
    {
        return Reference< XNode >();
    }

    /**
    Adds a node using its namespaceURI and localName.
    */
  Reference< XNode > SAL_CALL CNotationsMap::setNamedItemNS(const Reference< XNode >& /*arg*/) throw (RuntimeException)
    {
        return Reference< XNode >();
    }
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
