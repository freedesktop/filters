/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008 by Sun Microsystems, Inc.
 *
 * OpenOffice.org - a multi-platform office productivity suite
 *
 * $RCSfile: factory.hxx,v $
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

#ifndef _FACTORY_HXX
#define _FACTORY_HXX

#ifndef _LIST_HXX //autogen
#include <tools/list.hxx>
#endif
#include <bf_so3/so2ref.hxx>
#ifndef _SFXBRDCST_HXX //autogen
#include <bf_svtools/brdcst.hxx>
#endif
#ifndef _GLOBNAME_HXX //autogen
#include <tools/globname.hxx>
#endif
#ifndef _SOT_FACTORY_HXX
#include <sot/factory.hxx>
#endif
#ifndef _COM_SUN_STAR_DATATRANSFER_XTRANSFERABLE_HPP_
#include <com/sun/star/datatransfer/XTransferable.hpp>
#endif

#ifndef INCLUDED_SO3DLLAPI_H
#include "bf_so3/so3dllapi.h"
#endif

namespace binfilter {
class SvObject;
class SvStorage;
class SvFactory_Impl;

typedef SvGlobalName SvUniqueName;

//==================class SvFactory========================================
class SO3_DLLPUBLIC SvFactory : public SotFactory
/*	[Beschreibung]

    Die Methoden der Klasse kann man in zwei Gruppen aufteilen.
    Erstens allgemeine Verwaltung des So-Projektes. Dies bedeutet den
    Zugriff auf Resourcen, Variablen und Methoden, die das Projekt
    zur Verfuegung stellt. Alle diese Methoden sind static.
    Zweitens bereitstellen von speziellen Informationen "uber die
    einzelnen Klassen. Diese Methoden sind nicht static. Factories f"ur
    verschiedene Klassen bekommt man "uber die Methode
    Klasse::ClassFactory (<SvObject::ClassFactory>). Die Factory zu einem
    Objekt bekommt man durch <SvObject::GetSvFactory>.
*/
{
    SvFactory_Impl *	pImp;
    ULONG               nRegisterId;

    SO3_DLLPRIVATE void                Revoke();
protected:
    virtual             ~SvFactory();
public:
                        TYPEINFO();

    static  BOOL        			Init();
    static  void        			DeInit();

    static  String      			GetConfigFileName();

    static  void					SetDefaultPlugInFactory( SvFactory * );
    static  SvFactory * 			GetDefaultPlugInFactory();

    static  void					SetDefaultAppletFactory( SvFactory * );
    static  SvFactory * 			GetDefaultAppletFactory();

    SvObjectRef         Create( const SvGlobalName & rSvClassName ) const;
    SvObjectRef         CreateAndLoad( SvStorage *, BOOL bLink = FALSE ) const;
    SvObjectRef  		CreateAndLoad( SvStream &, ULONG * nLen = NULL ) const;
    SvObjectRef         CreateAndInit( const SvGlobalName & rSvClassName,
                                       SvStorage * ) const;
    SvObjectRef         CreateAndInit( const String & rFileName,
                                       SvStorage *, BOOL bLink = FALSE ) const;

    SvObjectRef CreateAndInit( const ::com::sun::star::uno::Reference<
                               ::com::sun::star::datatransfer::XTransferable>&,
                               SvStorage*) const;
    // do not use this function. bLink and bStorFilled are not supported anymore
    // use CreateAndInit( const Reference<XTransferable>&, SvStorage*) instead

    SvObjectRef CreateAndInit( const ::com::sun::star::uno::Reference<
                               ::com::sun::star::datatransfer::XTransferable>&,
                               SvStorage*, BOOL bLink, BOOL bStorFilled) const;

            SvFactory( const SvGlobalName &,
                       const String & rClassName, CreateInstanceType );

    void                Register();
    BOOL                IsRegistered() const { return nRegisterId != 0; }

                        // Emulieren
    static void			SetAutoTreatAs( const SvGlobalName & rOldClass,
                                        const String & rOldFullUserTypeName,
                                        const SvGlobalName & rNewClass );
    static SvGlobalName	GetAutoTreatAs( const SvGlobalName & rClass );
                        // Konvertieren
    static void			SetAutoConvertTo( const SvGlobalName & rOldClass,
                                        const String & rOldFullUserTypeName,
                                        const SvGlobalName & rNewClass );
    static SvGlobalName	GetAutoConvertTo( const SvGlobalName & rClass );
    static SvGlobalName GetSvClass31( const SvGlobalName & rClass );
    static SvGlobalName GetSvClass( INT32 nFileFormat, const SvGlobalName & rClass );
    static BOOL			IsIntern31( const SvGlobalName & rClass );
    static BOOL			IsIntern( const SvGlobalName & rClass,
                                     long *pFileFormat	);
    static SvGlobalName GetServerName( long nStorageFormat );
    static String	    GetServiceName( const SvGlobalName& );

private:
                        SvFactory( const SvFactory & );
    SvFactory &         operator = ( const SvFactory & );
};

#define SV_IMPL_FACTORY(ClassName)									\
class ClassName : public SvFactory                      			\
{                                                                   \
public:                                                             \
         TYPEINFO();                                                 \
        ClassName( const SvGlobalName & rName,						\
                              const String & rClassName,              \
                              CreateInstanceType pCreateFuncP )       \
            : SvFactory( rName, rClassName, pCreateFuncP )

}

#endif
