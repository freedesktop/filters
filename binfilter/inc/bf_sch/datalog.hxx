/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: datalog.hxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: rt $ $Date: 2005-09-09 11:44:49 $
 *
 *  The Contents of this file are made available subject to
 *  the terms of GNU Lesser General Public License Version 2.1.
 *
 *
 *    GNU Lesser General Public License Version 2.1
 *    =============================================
 *    Copyright 2005 by Sun Microsystems, Inc.
 *    901 San Antonio Road, Palo Alto, CA 94303, USA
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License version 2.1, as published by the Free Software Foundation.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *    MA  02111-1307  USA
 *
 ************************************************************************/
#ifndef _SCH_DATALOG
#define _SCH_DATALOG

#include "memchrt.hxx"
#include <float.h>
#include <math.h>
#define SCH_DATALOG_ANY -1
namespace binfilter {

class SchDataLogBook
{
    long* mpRowCoordinates;
    long* mpColCoordinates;

    long dummy;

    long mnRows;
    long mnCols;
    long mnColsInitial;
    long mnRowsInitial;

    long mnRowsAdded;
    long mnColsAdded;
    long mnRowsLeft;
    long mnColsLeft;

    BOOL mbValid;
    BOOL mbRowChanged;
    BOOL mbColChanged;
    BOOL mbGetCol;

    void IncreaseRowCount();
    void IncreaseColCount();

public:

     void Reset();
     SchDataLogBook(const SchMemChart& aMemChart);
    ~SchDataLogBook();

//STRIP001 	BOOL IsValid() const;

//STRIP001 	BOOL IsChanged()    const;
//STRIP001 	BOOL IsRowChanged() const;
//STRIP001 	BOOL IsColChanged() const;


//STRIP001 	void SetValid(BOOL bValid)  {mbValid=bValid;}
//STRIP001 
//STRIP001 	long GetColCount()          {return mnCols;}
//STRIP001 	long GetRowCount()          {return mnRows;}
//STRIP001 
//STRIP001 	long GetColsAdded()         {return mnColsAdded;}
//STRIP001 	long GetRowsAdded()         {return mnRowsAdded;}
//STRIP001 
//STRIP001 	long GetColsInitial()       {return mnColsInitial;}
//STRIP001 	long GetRowsInitial()       {return mnRowsInitial;}
//STRIP001 
//STRIP001 	long GetCount()             {if(mbGetCol)return mnCols;return mnRows;}
//STRIP001 	long GetAdded()             {if(mbGetCol)return mnColsAdded;return mnRowsAdded;}
//STRIP001 	long GetInitial()           {if(mbGetCol)return mnColsInitial;return mnRowsInitial;}
//STRIP001 
//STRIP001 
//STRIP001 	void DeleteRow(const long nRow);
//STRIP001 	void DeleteCol(const long nCol);
//STRIP001 	void InsertRow(const long nRow);
//STRIP001 	void InsertCol(const long nCol);
//STRIP001 	void SwapCols(long nCol1,long nCol2);
//STRIP001 	void SwapRows(long nRow1,long nRow2);
//STRIP001 
//STRIP001 	void SetColMode(const BOOL bCol){mbGetCol =  bCol;};
//STRIP001 	void SetRowMode(const BOOL bCol){mbGetCol = !bCol;};


    //Get*Id MUSS ..._ANY fuer nicht existierende Elemente zurueckgeben:
//STRIP001 	long GetColId(const long nCol) const
//STRIP001 			{if(nCol<mnCols)return mpColCoordinates[nCol];return SCH_DATALOG_ANY;}
//STRIP001 	long GetRowId(const long nRow) const
//STRIP001 			{if(nRow<mnRows)return mpRowCoordinates[nRow];return SCH_DATALOG_ANY;}
//STRIP001 
//STRIP001 	long GetId(const long n)
//STRIP001 	{
//STRIP001 		if(mbGetCol)return GetColId(n);
//STRIP001 		return GetRowId(n);
//STRIP001 	}
};

} //namespace binfilter
#endif
