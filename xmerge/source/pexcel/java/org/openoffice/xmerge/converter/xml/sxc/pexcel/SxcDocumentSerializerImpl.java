/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008 by Sun Microsystems, Inc.
 *
 * OpenOffice.org - a multi-platform office productivity suite
 *
 * $RCSfile: SxcDocumentSerializerImpl.java,v $
 * $Revision: 1.4 $
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

package org.openoffice.xmerge.converter.xml.sxc.pexcel;

import org.w3c.dom.NodeList;
import org.w3c.dom.Node;

import java.io.IOException;

import org.openoffice.xmerge.Document;
import org.openoffice.xmerge.ConvertData;
import org.openoffice.xmerge.ConvertException;
import org.openoffice.xmerge.converter.xml.sxc.SxcDocumentSerializer;
import org.openoffice.xmerge.converter.xml.sxc.pexcel.records.Workbook;

/**
 *  <p>Pocket Excel implementation of <code>SxcDocumentDeserializer</code> 
 *  for the {@link
 *  org.openoffice.xmerge.converter.xml.sxc.pexcel.PluginFactoryImpl
 *  PluginFactoryImpl}.</p>
 *
 *  <p>This converts StarOffice XML format to a set of files in
 *  Pocket Excel PXL format.</p>
 *
 *  @author      Paul Rank
 *  @author      Mark Murnane
 */
public final class SxcDocumentSerializerImpl extends SxcDocumentSerializer {


    /**
     *  Constructor.
     *
     *  @param  document  The <code>Document</code> to convert.
     */
    public SxcDocumentSerializerImpl(Document document) {
        super(document);
    }

    
    public ConvertData serialize() throws ConvertException, IOException {

        // Get the server side document name.  This value should not
        // contain a path or the file extension.
        String docName = sxcDoc.getName();

        // TODO - get real values for password when implemnted in XML
        // Passwords are not currently stored in StarCalc XML format.
        String password = null;

        encoder = new PocketExcelEncoder(docName, password);

        // get dom document
        org.w3c.dom.Document domDoc = sxcDoc.getContentDOM();

        // load the styles
        loadStyles(sxcDoc);
        //  Traverse to the office:body element.
        //  There should only be one.
        NodeList list = domDoc.getElementsByTagName(TAG_OFFICE_BODY);
        int len = list.getLength();

        if (len > 0) {
            Node node = list.item(0);
            traverseBody(node);
        }

        // get settings for this document 
        org.w3c.dom.Document settingsDoc = sxcDoc.getSettingsDOM();
        if(settingsDoc!=null) {
            NodeList settingsList = settingsDoc.getElementsByTagName(TAG_OFFICE_SETTINGS);
            int slen = settingsList.getLength();

            if (slen > 0) {
                Node settingsNode = settingsList.item(0);
                traverseSettings(settingsNode);
            }
        }

        // Get the number of sheets in the workbook
        // This will equal the number of PDBs we need
        ConvertData cd = new ConvertData();
        Workbook wb = ((PocketExcelEncoder) encoder).getWorkbook();
        cd.addDocument(wb);

        return cd;
    }
    

    /**
     *  A cell reference in a StarOffice formula looks like
     *  [.C2] (for cell C2).  MiniCalc is expecting cell references
     *  to look like C2.  This method strips out the braces and
     *  the period.
     *
     *  @param  formula  A StarOffice formula <code>String</code>.
     *
     *  @return  A MiniCalc formula <code>String</code>.
     */
    protected String parseFormula(String formula) {

        return null;
    }
}

