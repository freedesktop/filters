#*************************************************************************
#
#   OpenOffice.org - a multi-platform office productivity suite
#
#   $RCSfile: makefile.mk,v $
#
#   $Revision: 1.11 $
#
#   last change: $Author: rt $ $Date: 2005-09-08 21:42:28 $
#
#   The Contents of this file are made available subject to
#   the terms of GNU Lesser General Public License Version 2.1.
#
#
#     GNU Lesser General Public License Version 2.1
#     =============================================
#     Copyright 2005 by Sun Microsystems, Inc.
#     901 San Antonio Road, Palo Alto, CA 94303, USA
#
#     This library is free software; you can redistribute it and/or
#     modify it under the terms of the GNU Lesser General Public
#     License version 2.1, as published by the Free Software Foundation.
#
#     This library is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#     Lesser General Public License for more details.
#
#     You should have received a copy of the GNU Lesser General Public
#     License along with this library; if not, write to the Free Software
#     Foundation, Inc., 59 Temple Place, Suite 330, Boston,
#     MA  02111-1307  USA
#
#*************************************************************************

PRJ=..$/..
PRJNAME=filter
TARGET=flash

ENABLE_EXCEPTIONS=TRUE
#GEN_HID=TRUE

# --- Settings ----------------------------------

.INCLUDE :  	settings.mk

.IF "$(SYSTEM_ZLIB)" == "YES"
CFLAGS+=-DSYSTEM_ZLIB
.ENDIF
# --- Types -------------------------------------

UNOTYPES=\
    com.sun.star.uno.RuntimeException							\
    com.sun.star.uno.TypeClass									\
    com.sun.star.uno.XInterface									\
    com.sun.star.uno.XWeak										\
    com.sun.star.registry.XRegistryKey							\
    com.sun.star.io.XInputStream								\
    com.sun.star.io.XOutputStream								\
    com.sun.star.lang.XComponent								\
    com.sun.star.lang.XInitialization							\
    com.sun.star.lang.XMultiServiceFactory						\
    com.sun.star.lang.XSingleServiceFactory						\
    com.sun.star.lang.XServiceInfo								\
    com.sun.star.loader.XImplementationLoader					\
    com.sun.star.registry.XImplementationRegistration			\
    com.sun.star.registry.XRegistryKey							\
    com.sun.star.registry.XSimpleRegistry						\
    com.sun.star.document.XFilter								\
    com.sun.star.document.XExporter								\
    com.sun.star.drawing.XDrawPagesSupplier						\
    com.sun.star.container.XIndexAccess
# --- Files -------------------------------------

SRS1NAME=$(TARGET)
SRC1FILES =	impswfdialog.src				

SLOFILES=	$(SLO)$/swffilter.obj								\
            $(SLO)$/swfwriter.obj								\
            $(SLO)$/swfwriter1.obj								\
            $(SLO)$/swfwriter2.obj								\
            $(SLO)$/swfuno.obj									\
            $(SLO)$/swfexporter.obj							\
            $(SLO)$/swfdialog.obj							\
            $(SLO)$/impswfdialog.obj

# --- Library -----------------------------------

RESLIB1NAME=$(TARGET)
RESLIB1SRSFILES= $(SRS)$/$(TARGET).srs

SHL1TARGET=$(TARGET)$(UPD)$(DLLPOSTFIX)

# static libraries must come at the end for MACOSX
.IF "$(OS)" != "MACOSX"
SHL1STDLIBS+=\
    $(ZLIB3RDLIB)
.ENDIF


# dynamic libraries
SHL1STDLIBS+=\
    $(SVTOOLLIB)		\
    $(GOODIESLIB)		\
    $(VCLLIB)			\
    $(UNOTOOLSLIB)		\
    $(TOOLSLIB)			\
    $(COMPHELPERLIB)	\
    $(CPPUHELPERLIB)	\
    $(CPPULIB)			\
    $(SALLIB) 

#	$(ONELIB)			\
#	$(VOSLIB)			\

# static libraries must come at the end for MACOSX
.IF "$(OS)" == "MACOSX"
SHL1STDLIBS+=\
    $(ZLIB3RDLIB)
.ENDIF

SHL1DEPN=
SHL1IMPLIB=	i$(SHL1TARGET)
SHL1VERSIONMAP=exports.map
SHL1LIBS=	$(SLB)$/$(TARGET).lib
SHL1DEF=	$(MISC)$/$(SHL1TARGET).def

DEF1NAME=$(SHL1TARGET)

# --- Targets ----------------------------------

.INCLUDE : target.mk
