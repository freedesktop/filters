/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* WordPerfectCollector: Collects sections and runs of text from a
 * wordperfect file (and styles to go along with them) and writes them
 * to a Writer target document
 *
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 *
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#if defined _MSC_VER
#pragma warning( push, 1 )
#endif
#include "WordPerfectCollector.hxx"
#include <libwpd/WPDocument.h>
#if defined _MSC_VER
#pragma warning( pop )
#endif

WordPerfectCollector::WordPerfectCollector(WPSInputStream *pInput, DocumentHandler *pHandler) :
    DocumentCollector(pInput, pHandler)
{
}

WordPerfectCollector::~WordPerfectCollector()
{
}

bool WordPerfectCollector::parseSourceDocument(WPSInputStream &input)
{
        WPDResult result = WPDocument::parse(&input, static_cast<WPXHLListenerImpl *>(this));
        if (result != WPD_OK)
                return false;

    return true;
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
