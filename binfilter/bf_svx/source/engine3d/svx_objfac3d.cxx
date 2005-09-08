/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: svx_objfac3d.cxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2005-09-08 05:32:42 $
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

// auto strip #ifndef _SVDPAGE_HXX
// auto strip #include "svdpage.hxx"
// auto strip #endif

#ifndef _E3D_GLOBL3D_HXX
#include "globl3d.hxx"
#endif

#ifndef _E3D_POLYSC3D_HXX
#include "polysc3d.hxx"
#endif

#ifndef _E3D_DLIGHT3D_HXX
#include "dlight3d.hxx"
#endif

#ifndef _E3D_PLIGHT3D_HXX
#include "plight3d.hxx"
#endif

#ifndef _E3D_POLYOB3D_HXX
#include "polyob3d.hxx"
#endif

#ifndef _E3D_CUBE3D_HXX
#include "cube3d.hxx"
#endif

#ifndef _E3D_SPHERE3D_HXX
#include "sphere3d.hxx"
#endif

#ifndef _E3D_EXTRUD3D_HXX
#include "extrud3d.hxx"
#endif

#ifndef _E3D_LATHE3D_HXX
#include "lathe3d.hxx"
#endif

#ifndef _E3D_POLYGON3D_HXX
#include "polygn3d.hxx"
#endif

#ifndef _OBJFAC3D_HXX
#include "objfac3d.hxx"
#endif

// auto strip #ifndef _SVDOBJ_HXX
// auto strip #include "svdobj.hxx"
// auto strip #endif
namespace binfilter {

/*************************************************************************
|*
|* Konstruktor
|*
\************************************************************************/

/*N*/ E3dObjFactory::E3dObjFactory()
/*N*/ {
/*N*/ 	SdrObjFactory::InsertMakeObjectHdl(LINK(this, E3dObjFactory, MakeObject));
/*N*/ }

/*************************************************************************
|*
|* Destruktor
|*
\************************************************************************/

/*N*/ E3dObjFactory::~E3dObjFactory()
/*N*/ {
/*N*/ }

/*************************************************************************
|*
|* Chart-interne Objekte erzeugen
|*
\************************************************************************/

/*N*/ IMPL_LINK( E3dObjFactory, MakeObject, SdrObjFactory*, pObjFactory)
/*N*/ {
/*N*/ 	if ( pObjFactory->nInventor == E3dInventor )
/*N*/ 	{
/*N*/ 		switch ( pObjFactory->nIdentifier )
/*N*/ 		{
/*N*/ 			case E3D_POLYSCENE_ID:
/*N*/ 				pObjFactory->pNewObj = new E3dPolyScene();
/*N*/ 				break;
/*N*/ 			case E3D_LIGHT_ID:
/*N*/ 				pObjFactory->pNewObj = new E3dLight();
/*N*/ 				break;
/*N*/ 			case E3D_DISTLIGHT_ID:
/*N*/ 				pObjFactory->pNewObj = new E3dDistantLight();
/*N*/ 				break;
/*N*/ 			case E3D_POINTLIGHT_ID:
/*N*/ 				pObjFactory->pNewObj = new E3dPointLight();
/*N*/ 				break;
/*N*/ 			case E3D_SPOTLIGHT_ID:
/*N*/ 				break;
/*N*/ 			case E3D_OBJECT_ID:
/*N*/ 				pObjFactory->pNewObj = new E3dObject();
/*N*/ 				break;
/*N*/ 			case E3D_POLYOBJ_ID	:
/*N*/ 				pObjFactory->pNewObj = new E3dPolyObj();
/*N*/ 				break;
/*N*/ 			case E3D_POLYGONOBJ_ID	:
/*N*/ 				pObjFactory->pNewObj = new E3dPolygonObj();
/*N*/ 				break;
/*N*/ 			case E3D_CUBEOBJ_ID	:
/*N*/ 				pObjFactory->pNewObj = new E3dCubeObj();
/*N*/ 				break;
/*N*/ 			case E3D_SPHEREOBJ_ID:
/*N*/ 					// FG: ruft den dummy constructor, da dieser Aufruf nur beim Laden von Dokumenten erfolgt.
/*N*/ 					//     die wirkliche Anzahkl Segmente wird aber erst nach dem Laden der Member festgelegt.
/*N*/ 					//     dies hat zur Folge das die erste Kugel gleich wieder zerstoert wird, obwohl sie nie
/*N*/ 					//     gebraucht worden ist.
/*N*/ 				pObjFactory->pNewObj = new E3dSphereObj(123);
/*N*/ 				break;
/*N*/ 			case E3D_POINTOBJ_ID:
/*N*/ 				pObjFactory->pNewObj = new E3dPointObj();
/*N*/ 				break;
/*N*/ 			case E3D_EXTRUDEOBJ_ID:
/*N*/ 				pObjFactory->pNewObj = new E3dExtrudeObj();
/*N*/ 				break;
/*N*/ 			case E3D_LATHEOBJ_ID:
/*N*/ 				pObjFactory->pNewObj = new E3dLatheObj();
/*N*/ 				break;
/*N*/ 			case E3D_LABELOBJ_ID:
/*N*/ 				pObjFactory->pNewObj = new E3dLabelObj();
/*N*/ 				break;
/*N*/ 			case E3D_COMPOUNDOBJ_ID:
/*N*/ 				pObjFactory->pNewObj = new E3dCompoundObject();
/*N*/ 				break;
/*N*/ 		}
/*N*/ 	}
/*N*/ 	return 0;
/*N*/ }


}
