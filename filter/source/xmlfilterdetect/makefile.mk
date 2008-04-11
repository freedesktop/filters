#*************************************************************************
#
# DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
# 
# Copyright 2008 by Sun Microsystems, Inc.
#
# OpenOffice.org - a multi-platform office productivity suite
#
# $RCSfile: makefile.mk,v $
#
# $Revision: 1.7 $
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

PRJNAME=			filter
TARGET=				fl_xmlfilterdetect

NO_BSYMBOLIC=		TRUE
ENABLE_EXCEPTIONS=	TRUE

# --- Settings -----------------------------------------------------

.INCLUDE :  		settings.mk

# --- defines ------------------------------------------------------

CDEFS+=-DCOMPMOD_NAMESPACE=framework

# --- Targets ------------------------------------------------------

SHL1TARGET=		xmlfd$(DLLPOSTFIX)
SHL1IMPLIB=		i$(SHL1TARGET)

SHL1VERSIONMAP= xmlfd.map
SHL1DEF=		$(MISC)$/$(SHL1TARGET).def
DEF1NAME=		$(SHL1TARGET)

SHL1OBJS=		$(SLO)$/fdcomp.obj		\
                $(SLO)$/filterdetect.obj	

SHL1STDLIBS=	$(UCBHELPERLIB)	 \
                $(CPPUHELPERLIB) \
                $(CPPULIB) \
                $(SALLIB)

.INCLUDE :			target.mk
