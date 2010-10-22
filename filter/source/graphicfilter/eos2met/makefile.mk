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

PRJ=..$/..$/..

PRJNAME=filter
TARGET=eos2met
TARGET2=eme
DEPTARGET=veos2met

# --- Settings -----------------------------------------------------------

.INCLUDE :  settings.mk

# --- Allgemein ----------------------------------------------------------

.IF "$(editdebug)"!="" || "$(EDITDEBUG)"!=""
CDEFS+= -DEDITDEBUG
.ENDIF

SRS1NAME=$(TARGET)
SRC1FILES= 	dlgeos2.src	\
            eos2mstr.src
.IF "$(L10N_framework)"==""
SLOFILES = $(EXCEPTIONSFILES)
EXCEPTIONSFILES= \
            $(SLO)$/eos2met.obj \
            $(SLO)$/dlgeos2.obj

.ENDIF
# ==========================================================================

RESLIB1NAME=$(TARGET2)
RESLIB1SRSFILES=$(SRS)$/$(TARGET).srs
.IF "$(L10N_framework)"==""
SHL1TARGET=     eme$(DLLPOSTFIX)
SHL1IMPLIB=     eos2met
SHL1STDLIBS=    $(TOOLSLIB) $(VCLLIB) $(SVTOOLLIB) $(CPPULIB) $(SALLIB)

SHL1LIBS=       $(SLB)$/eos2met.lib
.IF "$(GUI)" != "UNX"
.IF "$(COM)" != "GCC"
SHL1OBJS=       $(SLO)$/eos2met.obj
.ENDIF
.ENDIF

SHL1VERSIONMAP=exports.map
SHL1DEF=        $(MISC)$/$(SHL1TARGET).def
DEF1NAME=$(SHL1TARGET)
.ENDIF
# ==========================================================================

.INCLUDE :  target.mk
