/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: WW8FKP.hxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: obo $ $Date: 2008-01-10 11:46:27 $
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

#ifndef INCLUDED_WW8_FKP_HXX
#define INCLUDED_WW8_FKP_HXX

#ifndef INCLUDES_WW8_CP_AND_FC_HXX
#include <WW8CpAndFc.hxx>
#endif
#ifndef INCLUDE_WW8_STRUCT_BASE_HXX
#include <WW8StructBase.hxx>
#endif

namespace writerfilter {
namespace doctok
{

/**
   A formated diskpage (FKP).

   Formatted diskpages are used by Word to store runs of SPRMs (single
   property modifier). A FKP contains a list of file character
   positions (FC). For each FC there is an entry containing the run of
   SPRMs stored for that FC.
 */
class WW8FKP : public WW8StructBase
{
public:
    /**
       Shared pointer to an FKP.
     */
    typedef boost::shared_ptr<WW8FKP> Pointer_t;

    /**
       Create FKP from stream.

       @param rStream    stream to create FKP from
       @param nOffset    offset in stream where the FKP starts.
     */
    WW8FKP(WW8Stream & rStream, sal_uInt32 nOffset)
    : WW8StructBase(rStream, nOffset, 512)
    {
    }
        
    /**
       Return count of entries.
    */
    virtual sal_uInt32 getEntryCount() const = 0;

    /**
       Return an FC.

       @param nIndex   index of the FC to return
     */
    virtual Fc getFc(sal_uInt32 nIndex) const = 0;

    /**
       Returns the first FC of the FKP.
    */
    virtual Fc getFirstFc() const = 0;

    /**
       Returns the last FC of the FKP.
     */
    virtual Fc getLastFc() const = 0;

    /**
       Check if an FKP contains an FC.

       @param rFc   FC to look for
       
       @retval true    FKP contains FC
       @retval false   else
     */
    virtual bool contains(const Fc & rFc) const = 0;

    /**
       Return properties stored in an FKP for a FC.

       @param rFc   FC to look for       
     */
    virtual writerfilter::Reference<Properties>::Pointer_t 
    getProperties(const Fc & rFc) 
        const = 0;
};

/**
   Cache providing FKPs.   
 */
class WW8FKPCache
{
public:
    /**
       Shared pointer to cache.
     */
    typedef boost::shared_ptr<WW8FKPCache> Pointer_t;
        
    virtual ~WW8FKPCache();

    /**
       Return FKP.

       @param nPageNumber    number of page to return
     */
    virtual WW8FKP::Pointer_t get(sal_uInt32 nPageNumber, bool bComplex) = 0;
};
}}

#endif // INCLUDED_WW8_FKP_HXX
