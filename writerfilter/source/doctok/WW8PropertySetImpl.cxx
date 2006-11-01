/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: WW8PropertySetImpl.cxx,v $
 *
 *  $Revision: 1.2 $
 *
 *  last change: $Author: hbrinkm $ $Date: 2006-11-01 09:14:31 $
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

#include "WW8DocumentImpl.hxx"
#include "WW8ResourceModelImpl.hxx"
#include <doctok/resourceids.hxx>

namespace doctok
{
bool operator != (const WW8PropertySetIterator & rA,
                  const WW8PropertySetIterator & rB)
{
    return ! (rA.equal(rB));
}

WW8Property::~WW8Property()
{
}

WW8PropertySet::~WW8PropertySet()
{
}
    
WW8PropertySetIterator::~WW8PropertySetIterator()
{
}

WW8PropertyImpl::WW8PropertyImpl(WW8Stream & rStream, 
                                 sal_uInt32 nOffset, sal_uInt32 nCount)
: WW8StructBase(rStream, nOffset, nCount)
{
}

WW8PropertyImpl::WW8PropertyImpl(const WW8StructBase & rBase,
                                 sal_uInt32 nOffset, 
                                 sal_uInt32 nCount)
: WW8StructBase(rBase, nOffset, nCount)
{
}

WW8PropertyImpl::WW8PropertyImpl(WW8StructBase * pBase,
                                 sal_uInt32 nOffset, 
                                 sal_uInt32 nCount)
: WW8StructBase(pBase, nOffset, nCount)
{
}

WW8PropertyImpl::~WW8PropertyImpl()
{
}

sal_uInt32 WW8PropertyImpl::getParam() const
{
    sal_uInt32 nResult = 0;

    switch (get_spra())
    {
    case 0:
    case 1:
        nResult = getU8(2);

        break;
        
    case 2:
    case 4:
    case 5:
        nResult = getU16(2);

        break;

    case 3:
        nResult = getU32(2);

        break;

    case 7:
        nResult = getU16(2) + (getU8(4) << 16);

        break;

    default:
        break;
    }

    return nResult;
}

WW8Stream::Sequence WW8PropertyImpl::getParams() const
{
    return WW8Stream::Sequence(mSequence, 2, getCount() - 2);
}

sal_uInt32 WW8PropertyImpl::getByteLength() const
{
    sal_uInt32 nParamSize = 0;

    switch (get_spra())
    {
    case 0:
    case 1:
        nParamSize = 1;

        break;

    case 2:
    case 4:
    case 5:
        nParamSize = 2;

        break;

    case 3:
        nParamSize = 4;

        break;
        
    case 7:
        nParamSize = 3;

        break;
    default:
        nParamSize = getU8(2) + 1;

        break;
    }

    return nParamSize + 2;
}

sal_uInt32 WW8PropertyImpl::getParamOffset() const
{
    sal_uInt32 nReturn = 0;

    if (get_spra() == 6)
        nReturn = 1;
        
    switch (getId())
    {
    case 0xd608:
    case 0xd609:
        nReturn = 3;
            
        break;
            
    default:
        break;
    }

    return nReturn;
}
    
void WW8PropertyImpl::dump(OutputWithDepth<string> & o) const
{
    o.addItem(toString());
}

string WW8PropertyImpl::toString() const
{
    string aResult;

    aResult += "<sprmcommon";

    char sBuffer[256];
    snprintf(sBuffer, sizeof(sBuffer), " id=\"%lx\"", getId());
    aResult += sBuffer;
    aResult += " name=\"";
    aResult += (*SprmIdToString::Instance())(getId());
    aResult += "\"";
    snprintf(sBuffer, sizeof(sBuffer), " sgc=\"%x\"", get_sgc());
    aResult += sBuffer;
    snprintf(sBuffer, sizeof(sBuffer), " spra=\"%x\"", get_spra());
    aResult += sBuffer;
    snprintf(sBuffer, sizeof(sBuffer), " size=\"%lx\"", getByteLength());
    aResult += sBuffer;
    snprintf(sBuffer, sizeof(sBuffer), " param=\"%lx\"", getParam());
    aResult += sBuffer;
    aResult += ">\n";

    aResult += mSequence.toString();

    aResult += "</sprmcommon>";

    return aResult;
}

WW8PropertySetImpl::WW8PropertySetImpl(WW8Stream & rStream,
                                       sal_uInt32 nOffset,
                                       sal_uInt32 nCount,
                                       bool bPap)
: WW8StructBase(rStream, nOffset, nCount), mbPap(bPap)
{
}

WW8PropertySetImpl::WW8PropertySetImpl(const WW8StructBase & rBase,
                                       sal_uInt32 nOffset,
                                       sal_uInt32 nCount,
                                       bool bPap)
: WW8StructBase(rBase, nOffset, nCount), mbPap(bPap)
{
}

WW8PropertySetImpl::~WW8PropertySetImpl()
{
}

bool WW8PropertySetImpl::isPap() const
{
    return mbPap;
}

sal_uInt32 WW8PropertySetImpl::get_istd() const
{
    sal_uInt32 nResult = 0;

    if (mbPap)
        nResult = getU16(0);

    return nResult;
}

WW8PropertySetIterator::Pointer_t WW8PropertySetImpl::begin()
{
    return WW8PropertySetIterator::Pointer_t
        (new WW8PropertySetIteratorImpl(this, mbPap ? 2 : 0));
}

WW8PropertySetIterator::Pointer_t WW8PropertySetImpl::end()
{
    return WW8PropertySetIterator::Pointer_t
        (new WW8PropertySetIteratorImpl(this, getCount()));
}

WW8PropertySetIteratorImpl::~WW8PropertySetIteratorImpl()
{
}

WW8Property::Pointer_t 
WW8PropertySetImpl::getAttribute(sal_uInt32 nOffset) const
{
    WW8PropertyImpl aTmpAttr(*this, nOffset, 3);
    
    sal_uInt32 nLength = aTmpAttr.getByteLength();
    
    if (nOffset + nLength > getCount())
        nLength = getCount() - nOffset;
    
    return WW8Property::Pointer_t
        (new WW8PropertyImpl(*this, nOffset, nLength));
}

void WW8PropertySetImpl::dump(OutputWithDepth<string> & o) const
{
    WW8StructBase::dump(o);

    WW8PropertySetIterator::Pointer_t pIt = 
        const_cast<WW8PropertySetImpl *>(this)->begin();
    WW8PropertySetIterator::Pointer_t pItEnd = 
        const_cast<WW8PropertySetImpl *>(this)->end();

    while((*pIt) != (*pItEnd))
    {
        WW8Property::Pointer_t pAttr = pIt->get();

        pAttr->dump(o);
        ++(*pIt);
    }
}

void WW8PropertySetImpl::dots(ostream & o)
{
    WW8PropertySetIterator::Pointer_t pIt = begin();
    WW8PropertySetIterator::Pointer_t pItEnd = end();

    while((*pIt) != (*pItEnd))
    {
        WW8Property::Pointer_t pAttr = pIt->get();

        o << "." << endl;

        ++(*pIt);
    }
}

string WW8PropertySetImpl::getType() const
{
    return "WW8PropertySetImpl";
}

void WW8PropertySetImpl::resolveLocal(Sprm & sprm)
{
    switch (sprm.getId())
    {
    case 0x6a03:
        {
            Value::Pointer_t pValue = sprm.getValue();
            getDocument()->setPicLocation(pValue->getInt());
        }
        break;
    default:
        break;
    }
}

void WW8PropertySetImpl::resolve(Properties & rHandler)
{
    if (getCount() >= (isPap() ? 5U : 3U))
    { 
        WW8PropertySetIterator::Pointer_t pIt = begin();
        WW8PropertySetIterator::Pointer_t pItEnd = end();
        
        if (isPap())
        {
            WW8Value::Pointer_t pValue = createValue(getU16(0));
            rHandler.attribute(NS_rtf::LN_ISTD, *pValue);
        }
        
        while((*pIt) != (*pItEnd))
        {
            WW8Sprm aSprm(pIt->get());
            
            rHandler.sprm(aSprm);

            resolveLocal(aSprm);
            
            ++(*pIt);
        }
    }
}

WW8PropertySetIterator & WW8PropertySetIteratorImpl::operator++ ()
{
    WW8Property::Pointer_t pTmpAttr = mpAttrSet->getAttribute(mnOffset);

    mnOffset += dynamic_cast<WW8PropertyImpl *>(pTmpAttr.get())->
        getByteLength();

    if (mnOffset > mpAttrSet->getCount() ||
        mpAttrSet->getCount() - mnOffset < 3)
        mnOffset = mpAttrSet->getCount();
        
    return *this;
}

WW8Property::Pointer_t WW8PropertySetIteratorImpl::get() const
{
    return mpAttrSet->getAttribute(mnOffset);
}

bool WW8PropertySetIteratorImpl::equal
(const WW8PropertySetIterator & rIt) const
{
    const WW8PropertySetIteratorImpl & rMyIt = 
        dynamic_cast<const WW8PropertySetIteratorImpl &>(rIt);

    return mpAttrSet == rMyIt.mpAttrSet && mnOffset == rMyIt.mnOffset;
}

string WW8PropertySetIteratorImpl::toString() const
{
    string sResult = "";

    char sBuffer[256];

    snprintf(sBuffer, sizeof(sBuffer), "(%ld)", mnOffset);
    sResult += sBuffer;
    
    return sResult;
}

SprmIdToString::Pointer_t SprmIdToString::pInstance;

SprmIdToString::Pointer_t SprmIdToString::Instance()
{
    if (pInstance.get() == NULL)
        pInstance = SprmIdToString::Pointer_t(new SprmIdToString());

    return pInstance;
}

string SprmIdToString::operator()(sal_uInt32 nId)
{
    return mMap[nId];
}

}
