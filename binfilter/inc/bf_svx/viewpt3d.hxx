/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: viewpt3d.hxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: rt $ $Date: 2006-10-28 04:23:26 $
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

#ifndef _VIEWPT3D_HXX
#define _VIEWPT3D_HXX

#ifndef _B3D_HMATRIX_HXX
#include <goodies/hmatrix.hxx>
#endif
namespace binfilter {

class Volume3D;
class SdrObjIOHeader;

/*************************************************************************
|*
|* enums fuer Projektion und Seitenverhaeltnis
|*
\************************************************************************/

enum ProjectionType	{ PR_PARALLEL, PR_PERSPECTIVE };
enum AspectMapType	{ AS_NO_MAPPING, AS_HOLD_SIZE, AS_HOLD_X, AS_HOLD_Y };

/*************************************************************************
|*
|* 3D-Viewport nach PHIGS
|*
\************************************************************************/

class Viewport3D
{
 protected:
    Matrix4D		aViewTf;		// die eigentliche Transformationsmatrix
    Vector3D		aVRP;			// View Reference Point
    Vector3D		aVPN;			// View Plane Normal
    Vector3D		aVUV;			// View Up Vector
    Vector3D		aPRP;			// Projection Reference Point(View-Koordinaten)
                                    // bisher wird nur die Z-Koordinate beachtet
    double			fVPD;		 	// View Plane Distance
    double			fNearClipDist;	// Abstand der vorderen Clippingebene
    double			fFarClipDist;	// Abstand der hinteren Clippingebene

    ProjectionType	eProjection;	// Art der Projektion
    AspectMapType	eAspectMapping;	// Flag fuer Seitenverhaeltnis-Anpassung
                                    // bei Ausgabe auf das Geraet
    Rectangle aDeviceRect;			// Position und Groesse des Ausgabebereichs

    struct
    {
        double X, Y, W, H;			// Position und Groesse des View-Windows
    } aViewWin;						// in View-Koordinaten

    Vector3D		aViewPoint;		// Beobachterstandpunkt in Weltkoordinaten;
                                    // wird mit der Transformation berechnet
    BOOL			bTfValid;	 	// Flag, ob Transformation gueltig ist

    double fWRatio;					// Device/View-Seitenverhaeltnisse
    double fHRatio;

    virtual void MakeTransform(void);

 public:
    Viewport3D();

    void SetVRP(const Vector3D& rNewVRP);
    void SetVPN(const Vector3D& rNewVPN);
    void SetVUV(const Vector3D& rNewVUV);
    void SetPRP(const Vector3D& rNewPRP);
    void SetVPD(double fNewVPD);

    const Vector3D&	GetVRP() const	{ return aVRP; }
    const Vector3D&	GetVPN() const	{ return aVPN; }
    const Vector3D&	GetVUV() const	{ return aVUV; }
    const Vector3D&	GetPRP() const	{ return aPRP; }
    double	GetVPD() const			{ return fVPD; }
    double	GetNearClipDist() const	{ return fNearClipDist; }
    double	GetFarClipDist() const	{ return fFarClipDist; }

    void SetProjection(ProjectionType ePrj)
        { eProjection = ePrj; bTfValid = FALSE; }
    ProjectionType GetProjection() const { return(eProjection); }

    void SetAspectMapping(AspectMapType eAsp)
        { eAspectMapping = eAsp; bTfValid = FALSE; }
    AspectMapType GetAspectMapping() { return eAspectMapping; }

    void SetViewWindow(double fX, double fY, double fW, double fH);

    // View-Window genau um das mit rTransform transformierte Volumen legen
    void FitViewToVolume(const Volume3D& rVolume, Matrix4D aTransform);

    void SetDeviceWindow(const Rectangle& rRect);
    const Rectangle& GetDeviceWindow() const { return aDeviceRect; }

    // Beobachterstandpunkt in Weltkoordinaten zurueckgeben
    const Vector3D&	GetViewPoint();

    // View-Transformationen
    const Matrix4D&	GetViewTransform();

    // Projektion und Mapping
    Vector3D&   DoProjection(Vector3D&) const;
    Vector3D	MapToDevice(const Vector3D&) const;


    virtual void WriteData(SvStream& rOut) const;
    virtual void ReadData(const SdrObjIOHeader& rHead, SvStream& rIn);

    // Wird nur vom Stream-Operator gerufen
    virtual void ReadData31(SvStream& rIn);
    virtual void WriteData31(SvStream& rOut) const;
};

// FG: eigentlich sollten die Stream-Funktionen in der 3D-Engine nicht verwendet werden
//     sondern WriteData und ReadData, da man insbesondere bei ReadData einen
//     zusaetzlichen Parameter hat der die File-Revision enthaelt.
//     Sie sind aus Kompatibilitaetsgruenden noch vorhanden.
SvStream& operator<<(SvStream& rOStream, const Viewport3D&);
SvStream& operator>>(SvStream& rIStream, Viewport3D&);

}//end of namespace binfilter
#endif		// _VIEWPT3D_HXX
