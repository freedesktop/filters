/************************************************************************
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

package org.openoffice.xmerge.converter.xml.sxc.pexcel.Records;
 
import java.io.DataInputStream;
import java.io.OutputStream;
import java.io.InputStream;
import java.io.IOException;

import org.openoffice.xmerge.util.Debug;
import org.openoffice.xmerge.util.EndianConverter;
import org.openoffice.xmerge.converter.xml.sxc.pexcel.Records.PocketExcelBiffConstants;

/**
 * Represents a BIFF Record that describes a blank cell 
 */
public class BlankCell implements BIFFRecord {
    
    private byte[] row  = new byte[2];
    private byte   col; 
    private byte[] ixfe = new byte[2];

    /**
     * Constructs a BlankCell <code>InputStream</code> 
     *
     * @param	is InputStream containing a BlankCell.
     */
    public BlankCell(InputStream is) throws IOException {
       read(is); 
    }

    /**
     * Get the hex code for this particular <code>BIFFRecord</code> 
     *
     * @return the hex code for <code>BlankCell</code>
     */
    public short getBiffType() {
        return PocketExcelBiffConstants.BLANK_CELL;
    }

    public int getRow()
    {
        return EndianConverter.readShort(row);
    }
    
    public void setRow(short row)
    {
        this.row = EndianConverter.writeShort(row);
    }
    
    public int getCol()
    {
        return (int) col;
    }
    
    public void setCol(short col)
    {
        this.col = (byte) col;
    }
    
    public int getXFIndex()
    {
        return EndianConverter.readShort(ixfe);
    }
    
    public void setXFIndex(short ixfe)
    {
        this.ixfe = EndianConverter.writeShort(ixfe);
    }
    
    public void write(OutputStream output) throws IOException {

        output.write(getBiffType());
        output.write(row);
        output.write(col);
        output.write(ixfe);

        Debug.log(Debug.TRACE, "Writing BlankCell record");
    
    }
    
    /**
     * Reads a BlankCell <code>InputStream</code> 
     *
     * @param	is InputStream containing a BlankCell.
     */
    public int read(InputStream input) throws IOException {

        int numOfBytesRead 	= input.read(row);
        numOfBytesRead++; 
        col					+= input.read();
        numOfBytesRead		+= input.read(ixfe);

        Debug.log(Debug.TRACE, "\tRow : "+ EndianConverter.readShort(row) + 
                            " Column : " + col +
                            " ixfe : " + EndianConverter.readShort(ixfe));        
        
        return numOfBytesRead;
    }
    
}
