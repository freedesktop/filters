/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: sch_chdescr.cxx,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2005-09-07 21:52:09 $
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
#ifndef _SFXENUMITEM_HXX
#include <svtools/eitem.hxx>
#endif

#include "chdescr.hxx"
// auto strip #include "chaxis.hxx"

// auto strip #ifndef _SVDPAGE_HXX //autogen
// auto strip #include <bf_svx/svdpage.hxx>
// auto strip #endif
#include "float.h"
// auto strip #include "globfunc.hxx"
#include "schgroup.hxx"
#include "chtscene.hxx"
namespace binfilter {

/*N*/ ChartDataDescription::ChartDataDescription(long nCols, long nRows, SdrObjList *pList,
/*N*/ 										   ChartModel* pModel, BOOL bEnable) :
/*N*/ 		mnRows(nRows),
/*N*/ 		mnCols(nCols),
/*N*/ 		mpList(pList),
/*N*/ 		mpModel(pModel),
/*N*/ 		mbEnable(bEnable),
/*N*/ 		mpDescrLists(NULL),
/*N*/ 		mpDescrArray(NULL),
/*N*/ 		mpDescrGroups(NULL)
/*N*/ {
/*N*/ }

/*N*/ void ChartDataDescription::Build( BOOL bRowDescr )
/*N*/ {
/*N*/ 	Dirty2D( bRowDescr );
/*N*/ 	if(mpList && mpDescrGroups)
/*N*/ 	{
/*?*/ 		for( long nRow=0; nRow<mnRows; nRow++ )
/*?*/ 		{
/*?*/ 			if(mpDescrGroups[nRow])
/*?*/ 				mpList->NbcInsertObject( mpDescrGroups[nRow] );
/*?*/ 		}
/*?*/ 	}
/*N*/ }

/*N*/ void ChartDataDescription::Build3D( ChartScene *pScene, Matrix4D* pMatrix )
/*N*/ {
/*N*/ 	if(pScene && mpDescrGroups)
/*N*/ 	{
/*?*/ 		for( long nRow=0; nRow<mnRows; nRow++ )
/*?*/ 		{
/*?*/ 			for( long nCol=0; nCol<mnCols; nCol++ )
/*?*/ 			{
/*?*/ 				long nIndex = nCol + nRow * mnCols;
/*?*/ 				if( mpDescrArray[nIndex].fValue != DBL_MIN )
/*?*/ 				{
/*?*/ 					mpDescrArray[nIndex].pLabelObj->SetMarkProtect(TRUE);
/*?*/ 					E3dLabelObj* pLabel=new E3dLabelObj(mpDescrArray[nIndex].aTextPos3D,
/*?*/ 														mpDescrArray[nIndex].pLabelObj);
/*?*/ 
/*?*/ 					CHART_TRACE3( "Descr::Build3D TextPos = (%ld, %ld, %ld)",
/*?*/ 								  mpDescrArray[ nIndex ].aTextPos3D.X(),
/*?*/ 								  mpDescrArray[ nIndex ].aTextPos3D.Y(),
/*?*/ 								  mpDescrArray[ nIndex ].aTextPos3D.Z() );
/*?*/ 					
/*?*/ 					pLabel->SetMarkProtect(TRUE);
/*?*/ 					pScene->Insert3DObj(pLabel);
/*?*/ 					if(pMatrix)
/*?*/ 						pLabel->NbcSetTransform(*pMatrix);
/*?*/ 				}
/*?*/ 			}
/*?*/ 		}
/*?*/ 	}
/*N*/ }

/*N*/ ChartDataDescription::~ChartDataDescription()
/*N*/ {
/*N*/ 	if(mpDescrLists)
/*?*/ 		delete[] mpDescrLists;
/*N*/ 	if(mpDescrGroups)
/*?*/ 		delete[] mpDescrGroups;
/*N*/ 	if(mpDescrArray)
/*?*/ 		delete[] mpDescrArray;
/*N*/ }

//STRIP001 void ChartDataDescription::Create(long nRow)
//STRIP001 {
//STRIP001 	if(!mpDescrArray)
//STRIP001 	{
//STRIP001 		mpDescrArray = new DataDescription[ mnRows * mnCols ];
//STRIP001 		long nElements = mnRows * mnCols;
//STRIP001 		while(nElements)
//STRIP001 		{
//STRIP001 			nElements--;
//STRIP001 			mpDescrArray[nElements].bSymbol = FALSE;
//STRIP001 			mpDescrArray[nElements].pLabelObj   = NULL;
//STRIP001 			mpDescrArray[nElements].fValue  = DBL_MIN;
//STRIP001 			mpDescrArray[nElements].aTextPos2D = Point(0,0);
//STRIP001 		}
//STRIP001 	}
//STRIP001 	if(!mpDescrLists)
//STRIP001 	{
//STRIP001 		mpDescrLists =new SdrObjList*[mnRows];
//STRIP001 		mpDescrGroups=new SchObjGroup*[mnRows];
//STRIP001 		for(long n=0;n<mnRows;n++)
//STRIP001 		{
//STRIP001 			mpDescrLists[n] =NULL;
//STRIP001 			mpDescrGroups[n]=NULL;
//STRIP001 		}
//STRIP001 	}
//STRIP001 	
//STRIP001 	if(!mpDescrLists[nRow])
//STRIP001 	{
//STRIP001 		mpDescrGroups[nRow] = (SchObjGroup*)CreateSimpleGroup( CHOBJID_DIAGRAM_DESCRGROUP, TRUE, TRUE );
//STRIP001 		mpDescrGroups[nRow]->InsertUserData( new SchDataRow(nRow) );
//STRIP001 		mpDescrLists[nRow] = mpDescrGroups[nRow]->GetSubList();
//STRIP001 	}
//STRIP001 }

/*N*/ DataDescription* ChartDataDescription::Insert( long nCol, long nRow, const SfxItemSet& rAttr, Point aPos,
/*N*/ 											   BOOL bPercent, ChartAdjust eAdjust, ChartAxis* pAxis )
/*N*/ {
/*N*/ 	SvxChartDataDescr eDescr = ((const SvxChartDataDescrItem&)rAttr.Get(SCHATTR_DATADESCR_DESCR)).GetValue();
/*N*/ 
/*N*/ 	if(mbEnable && eDescr != CHDESCR_NONE)
/*N*/ 	{
/*?*/ 		DBG_BF_ASSERT(0, "STRIP"); //STRIP001 Create(nRow); //evtl. Array und Liste erstellen
//STRIP001 /*?*/ 		long nIndex = nCol + nRow * mnCols;
//STRIP001 /*?*/ 
//STRIP001 /*?*/ 		mpDescrArray[nIndex].eDescr  = eDescr;
//STRIP001 /*?*/ 		mpDescrArray[nIndex].bSymbol = ((const SfxBoolItem&)rAttr.Get(SCHATTR_DATADESCR_SHOW_SYM)).GetValue();
//STRIP001 /*?*/ 		mpDescrArray[nIndex].fValue  = mpModel->GetData(nCol,nRow,bPercent);
//STRIP001 /*?*/ 		mpDescrArray[nIndex].eAdjust = eAdjust;
//STRIP001 /*?*/ 		mpDescrArray[nIndex].aTextPos2D = aPos;
//STRIP001 /*?*/ 		mpModel->CreateDataDescr(mpDescrArray[nIndex],nCol,nRow,pAxis,bPercent);
//STRIP001 /*?*/ 		return &mpDescrArray[nIndex];
/*N*/ 	}
/*N*/ 	return NULL;
/*N*/ }

/*N*/ void ChartDataDescription::Dirty2D( BOOL bRowDescr )
/*N*/ {
/*N*/ 
/*N*/ 	if(mpDescrLists)
/*N*/ 	{
/*?*/ 		for( long nRow = 0; nRow < mnRows; nRow ++ )
/*?*/ 		{
/*?*/ 			if( mpDescrLists[nRow] )
/*?*/ 			{
/*?*/ 				for( long nCol = 0; nCol < mnCols; nCol++ )
/*?*/ 				{
/*?*/ 						long nIndex = nCol + nRow * mnCols;
/*?*/ 						if (mpDescrArray[nIndex].fValue != DBL_MIN)
/*?*/ 							if (mpDescrArray[nIndex].pLabelObj)
/*?*/ 								mpDescrLists[nRow]->NbcInsertObject(mpDescrArray[nIndex].pLabelObj);
/*?*/ 				}
/*?*/ 			}
/*?*/ 		}
/*N*/ 	}
/*N*/ }

}
