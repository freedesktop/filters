#*************************************************************************
#
#   OpenOffice.org - a multi-platform office productivity suite
#
#   $RCSfile: makefile.mk,v $
#
#   $Revision: 1.10 $
#
#   last change: $Author: obo $ $Date: 2007-03-09 14:31:58 $
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

EXTERNAL_WARNINGS_NOT_ERRORS := TRUE

PRJ=..$/..
BFPRJ=..

PRJNAME=binfilter
TARGET=schart3

NO_HIDS=TRUE

LIBTARGET=NO

# --- Settings -----------------------------------------------------------
.INCLUDE :  settings.mk
INC+= -I$(PRJ)$/inc$/bf_sch
IENV!:=$(IENV);$(RES);..$/res

# --- Files --------------------------------------------------------

RESLIB1LIST=\
    $(SRS)$/sch_app.srs	\
    $(SRS)$/sch_core.srs	\
    $(SRS)$/schslots.srs \
    $(SRS)$/sfx2_sfx.srs

.IF "$(GUI)" == "WIN"
RESLIBSPLIT1NAME=bf_sch
RESLIBSPLIT1SRSFILES=\
    $(RESLIB1LIST)
.ELSE
RESLIB1NAME=bf_sch
RESLIB1SRSFILES=\
    $(RESLIB1LIST)
.ENDIF

LIB2TARGET= $(SLB)$/sch_schmod.lib
LIB2OBJFILES=	$(SLO)$/sch_schmod.obj

LIB3TARGET= $(SLB)$/bf_sch.lib
LIB3FILES=	$(SLB)$/sch_core.lib \
        $(SLB)$/sch_app.lib \
        $(SLB)$/sch_docshell.lib \
        $(SLB)$/sch_func.lib \
        $(SLB)$/sch_view.lib \
        $(SLB)$/sch_chxchart.lib \
        $(SLB)$/sch_xml.lib 


SHL1TARGET= bf_sch$(UPD)$(DLLPOSTFIX)
SHL1IMPLIB= bf_schi

SHL1VERSIONMAP= bf_sch.map
SHL1DEF=$(MISC)$/$(SHL1TARGET).def
DEF1NAME=$(SHL1TARGET)

SHL1STDLIBS=\
    $(BFSVXLIB)	\
    $(LEGACYSMGRLIB)	\
    $(SO2LIB)	\
    $(BFGOODIESLIB)	\
    $(SVTOOLLIB)	\
    $(VCLLIB)	\
    $(SVLLIB)	\
    $(SOTLIB)	\
    $(UNOTOOLSLIB) \
    $(TOOLSLIB)	\
    $(COMPHELPERLIB) \
    $(CPPUHELPERLIB) \
    $(CPPULIB) \
    $(SALLIB)
    
.IF "$(GUI)" == "UNX"
SHL1STDLIBS+= \
            $(BFSFXLIB)
.ENDIF     


SHL1BASE=	0x1d100000
SHL1LIBS=   $(LIB3TARGET)

.IF "$(GUI)"!="UNX"
SHL1OBJS=   $(SLO)$/sch_schdll.obj
.ENDIF # ! UNX

.IF "$(GUI)" == "WNT" || "$(GUI)" == "WIN"
SHL1RES=	$(RCTARGET)
.ENDIF # WNT || WIN

# --- Targets -------------------------------------------------------------

.INCLUDE :  target.mk

