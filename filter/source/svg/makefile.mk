#*************************************************************************
#
# DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
# 
# Copyright 2000, 2010 Oracle and/or its affiliates.
#
# OpenOffice.org - a multi-platform office productivity suite
#
# This file is part of OpenOffice.org.
#
# OpenOffice.org is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License version 3
# only, as published by the Free Software Foundation.
#
# OpenOffice.org is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License version 3 for more details
# (a copy is included in the LICENSE file that accompanied this code).
#
# You should have received a copy of the GNU Lesser General Public License
# version 3 along with OpenOffice.org.  If not, see
# <http://www.openoffice.org/license.html>
# for a copy of the LGPLv3 License.
#
#*************************************************************************

PRJ=..$/..

PRJNAME=filter
TARGET=svgfilter
ENABLE_EXCEPTIONS=TRUE
VISIBILITY_HIDDEN=TRUE

# --- Settings ----------------------------------

.INCLUDE :  settings.mk
.INCLUDE :	libs.mk

# --- Types -------------------------------------

SLOFILES= \
            $(SLO)$/b2dellipse.obj	\
            $(SLO)$/parserfragments.obj \
            $(SLO)$/svgexport.obj		\
            $(SLO)$/svgfilter.obj		\
            $(SLO)$/svgfontexport.obj	\
            $(SLO)$/svgimport.obj		\
            $(SLO)$/svgreader.obj		\
            $(SLO)$/svgwriter.obj		\
            $(SLO)$/tokenmap.obj        \
            $(SLO)$/units.obj

.IF "$(COMID)"=="gcc3"
.IF "$(CCNUMVER)">="000400000000" || "$(SYSTEM_BOOST)"=="YES"
CFLAGS+=-DUSE_MODERN_SPIRIT
.ENDIF
.ENDIF
.IF "$(SYSTEM_BOOST)"=="NO"
CFLAGS+=-DUSE_MODERN_SPIRIT
.ENDIF

# --- Library -----------------------------------

SHL1TARGET=$(TARGET)$(DLLPOSTFIX)

SHL1STDLIBS=\
    $(EDITENGLIB)			\
    $(SVXCORELIB)			\
    $(BASEGFXLIB)		\
    $(XMLOFFLIB)		\
    $(SVTOOLLIB)	    \
    $(VCLLIB)			\
    $(UNOTOOLSLIB)		\
    $(TOOLSLIB)			\
    $(COMPHELPERLIB)	\
    $(SVTOOLLIB)	    \
    $(CPPUHELPERLIB)	\
    $(CPPULIB)			\
    $(SALLIB)			\
    $(LIBXML)

SHL1DEPN=
SHL1IMPLIB=	i$(SHL1TARGET)
SHL1LIBS=	$(SLB)$/$(TARGET).lib
SHL1VERSIONMAP=exports.map
SHL1DEF=	$(MISC)$/$(SHL1TARGET).def

DEF1NAME=$(SHL1TARGET)

# --- Targets ----------------------------------

.INCLUDE : target.mk

ALLTAR : $(MISC)/svgfilter.component

$(MISC)/svgfilter.component .ERRREMOVE : $(SOLARENV)/bin/createcomponent.xslt \
        svgfilter.component
    $(XSLTPROC) --nonet --stringparam uri \
        '$(COMPONENTPREFIX_BASIS_NATIVE)$(SHL1TARGETN:f)' -o $@ \
        $(SOLARENV)/bin/createcomponent.xslt svgfilter.component
$(SLO)$/tokenmap.obj : $(INCCOM)$/tokens.cxx $(INCCOM)$/tokens.hxx

$(SLO)$/parserfragments.obj : $(INCCOM)$/tokens.cxx $(INCCOM)$/tokens.hxx

$(SLO)$/svgreader.obj : $(INCCOM)$/tokens.cxx $(INCCOM)$/tokens.hxx
