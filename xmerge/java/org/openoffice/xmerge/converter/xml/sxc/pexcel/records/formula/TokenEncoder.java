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
 *  Copyright: 2001 by Sun Microsystems, Inc.
 *
 *  All Rights Reserved.
 *
 *  Contributor(s): _______________________________________
 *
 *
 ************************************************************************/


package org.openoffice.xmerge.converter.xml.sxc.pexcel.records.formula;

import java.io.*;
import java.util.Vector;
import java.util.Enumeration;

import org.openoffice.xmerge.util.Debug;
import org.openoffice.xmerge.util.EndianConverter;
import org.openoffice.xmerge.converter.xml.sxc.pexcel.records.Workbook;
import org.openoffice.xmerge.converter.xml.sxc.pexcel.records.BoundSheet;
import org.openoffice.xmerge.converter.xml.sxc.pexcel.records.DefinedName;

/**
 * The TokenEncoder encodes a Token to an equivalent pexcel byte[]. The only
 * public method apart from the default constructor is the getByte method.
 * This method picks an encoder based onthe Token's type or id field and uses
 * that encoder to return a byte[] which it returns. This Encoder supports
 * Operands		Floating point's, Cell references (absolute and relative), 
 *				cell ranges
 * Operators	+,-,*,/,&lt;,&gt;.&lt;=,&gt;=,&lt;&gt;
 * Functions	All pexcel fixed and varaible argument functions
 * 
 */
public class TokenEncoder {
    
    private FunctionLookup fl;
    private String parseString;
    private int index;
    private Workbook wb;
   
    /**
     * Default Constructor
     */
    public TokenEncoder() {

        parseString = new String();
        fl = new FunctionLookup();
    }
   
      /**
     * Sets global workbook data needed for defined names
     */
       public void setWorkbook(Workbook wb) {
    
        this.wb = wb;
    }
   
   
    /**
     * Return the byte[] equivalent of a <code>Token</code>. The various
     * encoders return <code>Vector</code> of <code>Byte</code> instead 
     * of byte[] because the number of bytes returned varies with each 
     * <code>Token</code> encoded. After the encoding is finished the Vector
     * in converted to a byte[].
     *
     * @param t The <code>Token</code> to be encoded
     * @return An equivalent Pocket Excel byte[]
     */
    public byte[] getByte(Token t) throws IOException {
        
        Vector tmpByteArray = null;     // we use this cause we don't know till after 
                                        // the encoding takes place how big the byte [] will be
           index=0; 						// This class is declared static in
                                        // FormulaHelper so better make sure our index is 0
        if(t.getTokenType()==ParseToken.TOKEN_OPERATOR) {
            tmpByteArray = OperatorEncoder(t);
        } else if (t.getTokenType()==ParseToken.TOKEN_FUNCTION_VARIABLE || t.getTokenType()==ParseToken.TOKEN_FUNCTION_FIXED){
            tmpByteArray = FunctionEncoder(t);
        } else {                                    // Operands and functions     
            switch(t.getTokenID()) {
                case TokenConstants.TNAME : 
                    tmpByteArray = NameDefinitionEncoder(t);
                    break;
                case TokenConstants.TREF3D : 
                    tmpByteArray = ThreeDCellRefEncoder(t);
                    break;
                case TokenConstants.TAREA3D: 
                    tmpByteArray = ThreeDAreaRefEncoder(t);
                    break;
                case TokenConstants.TREF : 
                    tmpByteArray = CellRefEncoder(t);
                    break;
                case TokenConstants.TAREA : 
                    tmpByteArray = AreaRefEncoder(t);
                    break;
                case TokenConstants.TNUM :
                    tmpByteArray = NumEncoder(t);
                    break;
                case TokenConstants.TSTRING :
                    tmpByteArray = StringEncoder(t);
                    break;
                default :
                    Debug.log(Debug.ERROR, "Encoder found unrecognized Token");                     
            }
        }

        byte cellRefArray[] = new byte[tmpByteArray.size()];
        int i = 0;
        String s = new String();
        for(Enumeration e = tmpByteArray.elements();e.hasMoreElements();) {
            Byte tmpByte = (Byte) e.nextElement();
            s = s + tmpByte + " ";
            cellRefArray[i] = tmpByte.byteValue();
            i++;
        }
        Debug.log(Debug.TRACE, "Encoding Token " + t.getValue() + " as [" + s + "]");
        return cellRefArray;        
    }
    
    /** 
     * An Operator Encoder. 
     * 
     * @param t <code>Token</code> to be encoded
     * @return A <code>Vector</code> of pexcel <code>Byte</code>
     */
    private Vector OperatorEncoder(Token t) {

        Vector tmpByteArray = new Vector();
        tmpByteArray.add(new Byte((byte)t.getTokenID()));
        return tmpByteArray;
    }


    /** 
     * A String Encoder. 
     * 
     * @param t <code>Token</code> to be encoded
     * @return A <code>Vector</code> of pexcel <code>Byte</code>
     */
    private Vector StringEncoder(Token t) throws IOException{

        Vector tmpByteArray = new Vector();
        tmpByteArray.add(new Byte((byte)t.getTokenID()));
        tmpByteArray.add(new Byte((byte)(t.getValue().length())));
        tmpByteArray.add(new Byte((byte)0x01));
        byte [] stringBytes = t.getValue().getBytes("UTF-16LE");
        for (int i=0; i<stringBytes.length; i++) {
            tmpByteArray.add(new Byte(stringBytes[i]));
        }
        return tmpByteArray;
    }


    /** 
     * An Integer Encoder. 
     * 
     * @param t <code>Token</code> to be encoded
     * @return A <code>Vector</code> of pexcel <code>Byte</code>
     */
    private Vector NumEncoder(Token t) {

        Vector tmpByteArray = new Vector();
        
        double cellLong = (double) Double.parseDouble(t.getValue());
        tmpByteArray.add(new Byte((byte)t.getTokenID()));
        byte[] tempByte = EndianConverter.writeDouble(cellLong);
        for(int byteIter=0;byteIter<tempByte.length;byteIter++) {
            tmpByteArray.add(new Byte(tempByte[byteIter]));
        }
        return tmpByteArray;
    }
    
    /** 
     * Converts a char to an int. It is zero based
     * so a=0, b=1 etc.
     * 
     * @param ch the character to be converted
     * @return -1 if not a character otherwise a 0 based index
     */
    private int char2int(char ch) {
        if(!Character.isLetter(ch)) 
            return -1;
        
        ch = Character.toUpperCase(ch);
        return ch-'A';
    }
    
     /**
      * Identify letters
     * 
     * @param  c The character which is to be identified
     * @return A boolean returning the result of the comparison 
      */    
    private boolean isAlpha(char c) {
            return(Character.isLetter(c));
    }
    
     /**
      * Identify numbers
     * 
     * @param  c The character which is to be identified
     * @return A boolean returning the result of the comparison 
      */
    private boolean isDigit(char c) {
            return(Character.isDigit(c));
    }

     /**
      * Identify letters or numbers
     * 
     * @param  c The character which is to be identified
     * @return A boolean returning the result of the comparison 
      */ 	
    private boolean isAlphaNum(char c) {
        return(isAlpha(c) || isDigit(c));
    }
    
    /**
     * Parses a column reference and returns it's integer equivalent. (eg.
     * A=0, D=3, BA=27)
     * 
     * @return an 0 based index to a column
     */
    private int column() {
        char ch = parseString.charAt(index);
        String columnStr = new String();
        int col = 0;
        
        while(isAlpha(ch)) {
            columnStr += ch;
            index++;
            ch = parseString.charAt(index);
        }
        
        if(columnStr.length()==1) {
            col = char2int(columnStr.charAt(0));
        } else if (columnStr.length()==2) {
            col = char2int(columnStr.charAt(0)) + 1;
            col = (col*26) + char2int(columnStr.charAt(1));
        } else {
            Debug.log(Debug.ERROR, "Invalid Column Reference " + columnStr );
        }
        

        return col;
    }

    /**
     * Parses a column reference and returns it's integer equivalent. (eg.
     * A=0, D=3, BA=27)
     * 
     * @return an 0 based index to a column
     */    
    private int row() {
        char ch = parseString.charAt(index);
        String rowStr = new String();
        int row = 0;
        boolean status = true;
        
        do {
            rowStr += ch;
            index++;
            if(index>=parseString.length()) {
                status = false;
            } else  {
                ch = parseString.charAt(index);
            }
        } while(isDigit(ch) && status);
        return Integer.parseInt(rowStr)-1;  // Pexcel uses a 0 based index
    }

    /** 
     * A Cell Reference Encoder. It supports absolute and relative addressing
     * but not sheetnames.
     * 
     * @param t <code>Token</code> to be encoded
     * @return A <code>Vector</code> of pexcel <code>Byte</code>
     */
    private byte[] EncodeCellCoordinates() {
        int col = 0, row = 0;        
        int addressing = 0xC000;
        Debug.log(Debug.TRACE,"Encoding cell coordinates" + parseString);
        if(parseString.charAt(index)=='$') {
            addressing &= 0x8000;
            index++;
        }
        col = column();  
        if(parseString.charAt(index)=='$') {
            addressing &= 0x4000;
            index++;
        }        
        row = row();    // Pexcel uses a 0 based index
        row |= addressing;
        byte tokenBytes[] = new byte[3]; 
        tokenBytes[0] = (byte)row;
        tokenBytes[1] = (byte)(row>>8);
        tokenBytes[2] = (byte)col;
        return tokenBytes;
    }
    
    /** 
     * A Cell Reference Encoder. It supports absolute and relative addressing
     * but not sheetnames.
     * 
     * @param t <code>Token</code> to be encoded
     * @return A <code>Vector</code> of pexcel <code>Byte</code>
     */
    private Vector NameDefinitionEncoder(Token t) {
    
        Vector tmpByteArray = new Vector();
        
        String nameString = t.getValue();
        Debug.log(Debug.TRACE,"NameDefinitionEncoder : " + nameString);
        tmpByteArray.add(new Byte((byte)t.getTokenID()));  
        Enumeration e = wb.getDefinedNames();
        DefinedName dn;
        String name;
        int definedNameIndex = 0;
        do {			
            dn = (DefinedName)e.nextElement();
            name = dn.getName();
            Debug.log(Debug.TRACE,"Name pulled from DefinedName : " + name);
            definedNameIndex++;
        } while(!nameString.equalsIgnoreCase(name) && e.hasMoreElements());

        tmpByteArray.add(new Byte((byte)definedNameIndex));
        tmpByteArray.add(new Byte((byte)0x00));
            
        for(int i = 0;i < 12;i++) {
            tmpByteArray.add(new Byte((byte)0x00));
        }

        return tmpByteArray;
    }	
    /** 
     * A Cell Reference Encoder. It supports absolute and relative addressing
     * but not sheetnames.
     * 
     * @param t <code>Token</code> to be encoded
     * @return A <code>Vector</code> of pexcel <code>Byte</code>
     */
    private Vector CellRefEncoder(Token t) {
    
        Vector tmpByteArray = new Vector();
        
        parseString = t.getValue();
        tmpByteArray.add(new Byte((byte)t.getTokenID()));     
        byte cellRefBytes[] = EncodeCellCoordinates();
        for(int i = 0;i < cellRefBytes.length;i++) {
            tmpByteArray.add(new Byte(cellRefBytes[i]));
        }
        return tmpByteArray;
    }

    /** 
     * A Cell Reference Encoder. It supports absolute and relative addressing
     * but not sheetnames.
     * 
     * @param t <code>Token</code> to be encoded
     * @return A <code>Vector</code> of pexcel <code>Byte</code>
     */
    private short FindNameDefinitionIndex(String s) {
    
        short sheetIndex = 0;
        String savedName;
        String sheetName; 
        if (s.startsWith("$")) {
            sheetName = s.substring(1,s.length());	// Remove $
        } else {
            sheetName = s.substring(0,s.length());	
        }
        Debug.log(Debug.TRACE,"Searching for Name Definition : " + sheetName);
        Vector names = wb.getWorksheetNames();
        Enumeration e = names.elements();
        do {
            savedName = (String) e.nextElement();
            sheetIndex++;
        } while(!savedName.equalsIgnoreCase(sheetName) && e.hasMoreElements());
        
        Debug.log(Debug.TRACE,"Setting sheetindex to " + sheetIndex);
        return (short)(sheetIndex-1);
    }

    /** 
     * A Cell Reference Encoder. It supports absolute and relative addressing
     * but not sheetnames.
     * 
     * @param t <code>Token</code> to be encoded
     * @return A <code>Vector</code> of pexcel <code>Byte</code>
     */
    private Vector ThreeDCellRefEncoder(Token t) {
    
        Vector tmpByteArray = new Vector();
        parseString = t.getValue();
        Debug.log(Debug.TRACE,"Encoding 3D Cell reference " + t);
        tmpByteArray.add(new Byte((byte)t.getTokenID()));     
        tmpByteArray.add(new Byte((byte)0xFF));
        tmpByteArray.add(new Byte((byte)0xFF));
        for(int i = 0;i < 8;i++) {
            tmpByteArray.add(new Byte((byte)0x00));
        }
        
        String sheetRef = parseString.substring(0, parseString.indexOf('!') + 1);
        if (sheetRef.indexOf(':')!=-1) {
            sheetRef = parseString.substring(0, parseString.indexOf(':'));
            short sheetNum1 = FindNameDefinitionIndex(sheetRef);
            sheetRef = parseString.substring(parseString.indexOf(':') + 1, parseString.length());
            short sheetNum2 = FindNameDefinitionIndex(sheetRef);
            tmpByteArray.add(new Byte((byte)sheetNum1));
            tmpByteArray.add(new Byte((byte)0x00));
            tmpByteArray.add(new Byte((byte)sheetNum2));
            tmpByteArray.add(new Byte((byte)0x00));
        } else {
            sheetRef = parseString.substring(0, parseString.indexOf('!'));
            short sheetNum = FindNameDefinitionIndex(sheetRef);
            tmpByteArray.add(new Byte((byte)sheetNum));
            tmpByteArray.add(new Byte((byte)0x00));
            tmpByteArray.add(new Byte((byte)sheetNum));
            tmpByteArray.add(new Byte((byte)0x00));
        }
        parseString = parseString.substring(parseString.indexOf('!') + 1, parseString.length());
        Debug.log(Debug.TRACE,"Parsing : " + parseString);
        index=0;
        byte cellRefBytes[] = EncodeCellCoordinates();
        for(int i = 0;i < cellRefBytes.length;i++) {
            tmpByteArray.add(new Byte(cellRefBytes[i]));
        }
        return tmpByteArray;
    } 
    /** 
     * A Cell Reference Encoder. It supports absolute and relative addressing
     * but not sheetnames.
     * 
     * @param t <code>Token</code> to be encoded
     * @return A <code>Vector</code> of pexcel <code>Byte</code>
     */
    private Vector ThreeDAreaRefEncoder(Token t) {
    
        Vector tmpByteArray = new Vector();
        parseString = t.getValue();
        Debug.log(Debug.TRACE,"Encoding 3D Area reference " + t);
        tmpByteArray.add(new Byte((byte)t.getTokenID()));     
        tmpByteArray.add(new Byte((byte)0xFF));
        tmpByteArray.add(new Byte((byte)0xFF));
        for(int i = 0;i < 8;i++) {
            tmpByteArray.add(new Byte((byte)0x00));
        }
        
        String sheetRef = parseString.substring(0, parseString.indexOf('!') + 1);
        if (sheetRef.indexOf(':')!=-1) {
            sheetRef = parseString.substring(0, parseString.indexOf(':'));
            short sheetNum1 = FindNameDefinitionIndex(sheetRef);
            sheetRef = parseString.substring(parseString.indexOf(':') + 1, parseString.length());
            short sheetNum2 = FindNameDefinitionIndex(sheetRef);
            tmpByteArray.add(new Byte((byte)sheetNum1));
            tmpByteArray.add(new Byte((byte)0x00));
            tmpByteArray.add(new Byte((byte)sheetNum2));
            tmpByteArray.add(new Byte((byte)0x00));
        } else {
            sheetRef = parseString.substring(0, parseString.indexOf('!'));
            short sheetNum = FindNameDefinitionIndex(sheetRef);
            tmpByteArray.add(new Byte((byte)sheetNum));
            tmpByteArray.add(new Byte((byte)0x00));
            tmpByteArray.add(new Byte((byte)sheetNum));
            tmpByteArray.add(new Byte((byte)0x00));
        }
        parseString = parseString.substring(parseString.indexOf('!') + 1, parseString.length());
        Debug.log(Debug.TRACE,"Parsing : " + parseString);
        index=0;

        byte cellRefBytes1[] = EncodeCellCoordinates();

        if(parseString.charAt(index)==':') {
            index++; 
        } else {
            Debug.log(Debug.ERROR, "Invalid Cell Range, could not find :");            
        }

        byte cellRefBytes2[] = EncodeCellCoordinates();
        
        tmpByteArray.add(new Byte(cellRefBytes1[0]));
        tmpByteArray.add(new Byte(cellRefBytes1[1]));
        
        tmpByteArray.add(new Byte(cellRefBytes2[0]));
        tmpByteArray.add(new Byte(cellRefBytes2[1]));
        
        tmpByteArray.add(new Byte(cellRefBytes1[2]));
        tmpByteArray.add(new Byte(cellRefBytes2[2]));

        return tmpByteArray;
    }	
    
    /** 
     * A Cell Range Encoder. 
     * 
     * @param t <code>Token</code> to be encoded
     * @return A <code>Vector</code> of pexcel <code>Byte</code>
     */    
    private Vector AreaRefEncoder(Token t) {

        Vector tmpByteArray = new Vector();
        
        tmpByteArray.add(new Byte((byte)t.getTokenID())); 
        parseString = t.getValue();

        byte cellRefBytes1[] = EncodeCellCoordinates();

        if(parseString.charAt(index)==':') {
            index++; 
        } else {
            Debug.log(Debug.ERROR, "Invalid Cell Range, could not find :");            
        }

        byte cellRefBytes2[] = EncodeCellCoordinates();
        
        tmpByteArray.add(new Byte(cellRefBytes1[0]));
        tmpByteArray.add(new Byte(cellRefBytes1[1]));
        
        tmpByteArray.add(new Byte(cellRefBytes2[0]));
        tmpByteArray.add(new Byte(cellRefBytes2[1]));
        
        tmpByteArray.add(new Byte(cellRefBytes1[2]));
        tmpByteArray.add(new Byte(cellRefBytes2[2]));

        return tmpByteArray;
    }
    
    /** 
     * A Function Encoder. 
     * 
     * @param t <code>Token</code> to be encoded
     * @return A <code>Vector</code> of pexcel <code>Byte</code>
     */    
    private Vector FunctionEncoder(Token t) {
        Vector tmpByteArray = new Vector();
        
        int id = t.getTokenID();
        if(t.getTokenType()==ParseToken.TOKEN_FUNCTION_VARIABLE) {
            tmpByteArray.add(new Byte((byte)TokenConstants.TFUNCVAR));  
            tmpByteArray.add(new Byte((byte)t.getNumArgs()));  
        } else {
            tmpByteArray.add(new Byte((byte)TokenConstants.TFUNC));  
        }
       
        tmpByteArray.add(new Byte((byte)id));     
        tmpByteArray.add(new Byte((byte)(id>>8)));  
        return tmpByteArray;
    }
    

}
