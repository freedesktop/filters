/*************************************************************************
 *
 *  $RCSfile: filtercache.cxx,v $
 *
 *  $Revision: 1.2 $
 *
 *  last change: $Author: kz $ $Date: 2004-01-28 15:13:25 $
 *
 *  The Contents of this file are made available subject to the terms of
 *  either of the following licenses
 *
 *         - GNU Lesser General Public License Version 2.1
 *         - Sun Industry Standards Source License Version 1.1
 *
 *  Sun Microsystems Inc., October, 2000
 *
 *  GNU Lesser General Public License Version 2.1
 *  =============================================
 *  Copyright 2000 by Sun Microsystems, Inc.
 *  901 San Antonio Road, Palo Alto, CA 94303, USA
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License version 2.1, as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *  MA  02111-1307  USA
 *
 *
 *  Sun Industry Standards Source License Version 1.1
 *  =================================================
 *  The contents of this file are subject to the Sun Industry Standards
 *  Source License Version 1.1 (the "License"); You may not use this file
 *  except in compliance with the License. You may obtain a copy of the
 *  License at http://www.openoffice.org/license.html.
 *
 *  Software provided under this License is provided on an "AS IS" basis,
 *  WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING,
 *  WITHOUT LIMITATION, WARRANTIES THAT THE SOFTWARE IS FREE OF DEFECTS,
 *  MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE, OR NON-INFRINGING.
 *  See the License for the specific provisions governing your rights and
 *  obligations concerning the Software.
 *
 *  The Initial Developer of the Original Code is: Sun Microsystems, Inc.
 *
 *  Copyright: 2000 by Sun Microsystems, Inc.
 *
 *  All Rights Reserved.
 *
 *  Contributor(s): _______________________________________
 *
 *
 ************************************************************************/

#include "filtercache.hxx"
#include "cacheupdatelistener.hxx"
#include "macros.hxx"
#include "constant.hxx"

/*TODO see using below ... */
#define AS_ENABLE_FILTER_UINAMES
#define WORKAROUND_EXCEPTION_PROBLEM

//_______________________________________________
// includes

#ifndef _COM_SUN_STAR_UTIL_XCHANGESBATCH_HPP_
#include <com/sun/star/util/XChangesBatch.hpp>
#endif

#ifndef _COM_SUN_STAR_LANG_XSINGLESERVICEFACTORY_HPP_
#include <com/sun/star/lang/XSingleServiceFactory.hpp>
#endif

#ifndef _COM_SUN_STAR_BEANS_XPROPERTYSET_HPP_
#include <com/sun/star/beans/XPropertySet.hpp>
#endif

#ifndef _COM_SUN_STAR_BEANS_PROPERTYVALUE_HPP_
#include <com/sun/star/beans/PropertyValue.hpp>
#endif

#ifndef _COMPHELPER_SINGLETONREF_HXX_
#include <comphelper/singletonref.hxx>
#endif

#ifndef _UNOTOOLS_PROCESSFACTORY_HXX_
#include <unotools/processfactory.hxx>
#endif

#ifndef UNOTOOLS_CONFIGPATHES_HXX_INCLUDED
#include <unotools/configpathes.hxx>
#endif

#ifndef _RTL_USTRBUF_HXX_
#include <rtl/ustrbuf.hxx>
#endif

#ifndef _RTL_LOGFILE_HXX_
#include <rtl/logfile.hxx>
#endif

#ifndef _URLOBJ_HXX
#include <tools/urlobj.hxx>
#endif

#ifndef _WLDCRD_HXX
#include <tools/wldcrd.hxx>
#endif

//_______________________________________________
// namespace

namespace filter{
    namespace config{

namespace css = ::com::sun::star;

//_______________________________________________
// definitions

/*-----------------------------------------------
    28.10.2003 08:48
-----------------------------------------------*/
FilterCache::FilterCache()
    : BaseLock    (                                        )
    , m_xSMGR     (::comphelper::getProcessServiceFactory())
    , m_eFillState(E_CONTAINS_NOTHING                      )
{
}

/*-----------------------------------------------
    25.06.2003 10:03
-----------------------------------------------*/
FilterCache::~FilterCache()
{
}

/*-----------------------------------------------
    28.10.2003 09:01
-----------------------------------------------*/
void FilterCache::load(EFillState eRequired,
                       sal_Bool   bByThread)
    throw(css::uno::Exception)
{
/*
    static sal_Bool bIsThreadFirst = bByThread;
    OSL_ENSURE(bIsThreadFirst && bByThread, "Who disturb our \"fill cache on demand\" feature and force it hardly during office startup? Please optimize your code, so a full filled filter cache is not realy neccessary!");
*/
    // SAFE -> ----------------------------------
    ::osl::ResettableMutexGuard aLock(m_aLock);

    // check if required fill state is already reached ...
    // There is nothing to do then.
    if ((m_eFillState & eRequired) == eRequired)
        return;

    // Otherwhise load the missing items.

    // ------------------------------------------
    // a) load some const values from configration.
    //    These values are needed there for loading
    //    config items ...
    //    Further we load some std items from the
    //    configuration so we can try to load the first
    //    office document with a minimal set of values.
    if (m_eFillState == E_CONTAINS_NOTHING)
    {
        impl_getDirectCFGValue(CFGDIRECTKEY_OFFICELOCALE) >>= m_sActLocale;
        if (!m_sActLocale.getLength())
        {
            _FILTER_CONFIG_LOG_1_("FilterCache::ctor() ... could not specify office locale => use default \"%s\"\n", _FILTER_CONFIG_TO_ASCII_(DEFAULT_OFFICELOCALE));
            m_sActLocale = DEFAULT_OFFICELOCALE;
        }

        impl_getDirectCFGValue(CFGDIRECTKEY_FORMATNAME) >>= m_sFormatName;
        if (!m_sFormatName.getLength())
            impl_getDirectCFGValue(CFGDIRECTKEY_PRODUCTNAME) >>= m_sFormatName;

        if (!m_sFormatName.getLength())
        {
            _FILTER_CONFIG_LOG_1_("FilterCache::ctor() ... could not specify format name => use default \"%s\"\n", _FILTER_CONFIG_TO_ASCII_(DEFAULT_FORMATNAME));
            m_sFormatName = DEFAULT_FORMATNAME;
        }

        impl_getDirectCFGValue(CFGDIRECTKEY_FORMATVERSION) >>= m_sFormatVersion;
        if (!m_sFormatVersion.getLength())
        {
            _FILTER_CONFIG_LOG_1_("FilterCache::ctor() ... could not specify format version => use default \"%s\"\n", _FILTER_CONFIG_TO_ASCII_(DEFAULT_FORMATVERSION));
            m_sFormatVersion = DEFAULT_FORMATVERSION;
        }

        // enable "loadOnDemand" feature ...
        // Create uno listener, which waits for finishing the office startup
        // and starts a thread, which calls loadAll() at this filter cache.
        // Note: Its not a leak to create this listener with new here.
        // It kills itself after working!
        CacheUpdateListener* pUpdate = new CacheUpdateListener(m_xSMGR);
    }

    // ------------------------------------------
    // b) If the required fill state was not reached
    //    but std values was already loaded ...
    //    we must load some further missing items.
    impl_load(eRequired);
    // <- SAFE
}

/*-----------------------------------------------
    28.10.2003 09:50
-----------------------------------------------*/
sal_Bool FilterCache::isFillState(FilterCache::EFillState eState) const
    throw(css::uno::Exception)
{
    // SAFE ->
    ::osl::ResettableMutexGuard aLock(m_aLock);
    return ((m_eFillState & eState) == eState);
    // <- SAFE
}

/*-----------------------------------------------
    14.07.2003 10:45
-----------------------------------------------*/
OUStringList FilterCache::getMatchingItemsByProps(      EItemType  eType  ,
                                                  const CacheItem& lIProps,
                                                  const CacheItem& lEProps) const
    throw(css::uno::Exception)
{
    // SAFE ->
    ::osl::ResettableMutexGuard aLock(m_aLock);

    // search for right list
    // An exception is thrown �f "eType" is unknown.
    // => rList will be valid everytimes next line is reached.
    CacheItemList& rList = impl_getItemList(eType);

    OUStringList lKeys;

    // search items, which provides all needed properties of set "lIProps"
    // but not of set "lEProps"!
    for (CacheItemList::const_iterator pIt  = rList.begin();
                                       pIt != rList.end()  ;
                                     ++pIt                 )
    {
        _FILTER_CONFIG_LOG_1_("getMatchingProps for \"%s\"  ...\n",
                              _FILTER_CONFIG_TO_ASCII_(pIt->first))
        if (
            (pIt->second.haveProps(lIProps)    ) &&
            (pIt->second.dontHaveProps(lEProps))
           )
        {
            lKeys.push_back(pIt->first);
        }
    }

    return lKeys;
    // <- SAFE
}

/*-----------------------------------------------
    02.07.2003 09:32
-----------------------------------------------*/
sal_Bool FilterCache::hasItems(EItemType eType) const
    throw(css::uno::Exception)
{
    // SAFE ->
    ::osl::ResettableMutexGuard aLock(m_aLock);

    // search for right list
    // An exception is thrown �f "eType" is unknown.
    // => rList will be valid everytimes next line is reached.
    CacheItemList& rList = impl_getItemList(eType);

    return (rList.size()>0);
    // <- SAFE
}

/*-----------------------------------------------
    02.07.2003 11:48
-----------------------------------------------*/
OUStringList FilterCache::getItemNames(EItemType eType) const
    throw(css::uno::Exception)
{
    // SAFE ->
    ::osl::ResettableMutexGuard aLock(m_aLock);

    // search for right list
    // An exception is thrown �f "eType" is unknown.
    // => rList will be valid everytimes next line is reached.
    CacheItemList& rList = impl_getItemList(eType);

    OUStringList lKeys;
    for (CacheItemList::const_iterator pIt  = rList.begin();
                                       pIt != rList.end()  ;
                                     ++pIt                 )
    {
        lKeys.push_back(pIt->first);
    }
    return lKeys;
    // <- SAFE
}

/*-----------------------------------------------
    27.10.2003 08:37
-----------------------------------------------*/
sal_Bool FilterCache::hasItem(      EItemType        eType,
                              const ::rtl::OUString& sItem)
    throw(css::uno::Exception)
{
    // SAFE ->
    ::osl::ResettableMutexGuard aLock(m_aLock);

    // search for right list
    // An exception is thrown �f "eType" is unknown.
    // => rList will be valid everytimes next line is reached.
    CacheItemList& rList = impl_getItemList(eType);

    // if item could not be found - check if it can be loaded
    // from the underlying configuration layer. Might it was not already
    // loaded into this FilterCache object before.
    CacheItemList::const_iterator pIt = rList.find(sItem);
    if (pIt != rList.end())
        return sal_True;

    try
    {
        impl_loadItemOnDemand(eType, sItem);
        // no exception => item could be loaded!
        return sal_True;
    }
    catch(const css::container::NoSuchElementException&)
    {}

    return sal_False;
    // <- SAFE
}

/*-----------------------------------------------
    27.10.2003 08:14
-----------------------------------------------*/
CacheItem FilterCache::getItem(      EItemType        eType,
                               const ::rtl::OUString& sItem)
    throw(css::uno::Exception)
{
    // SAFE ->
    ::osl::ResettableMutexGuard aLock(m_aLock);

    // search for right list
    // An exception is thrown �f "eType" is unknown.
    // => rList will be valid everytimes next line is reached.
    CacheItemList& rList = impl_getItemList(eType);

    // check if item exists ...
    CacheItemList::const_iterator pIt = rList.find(sItem);
    if (pIt != rList.end())
        return pIt->second;

    // ... or load it on demand from the
    // underlying configuration layer.
    // Note: NoSuchElementException is thrown automaticly here if
    // item could not be loaded!
    pIt = impl_loadItemOnDemand(eType, sItem);
    return pIt->second;
    // <- SAFE
}

/*-----------------------------------------------
    27.10.2003 08:51
-----------------------------------------------*/
void FilterCache::removeItem(      EItemType        eType,
                             const ::rtl::OUString& sItem)
    throw(css::uno::Exception)
{
    // SAFE ->
    ::osl::ResettableMutexGuard aLock(m_aLock);

    // search for right list
    // An exception is thrown �f "eType" is unknown.
    // => rList will be valid everytimes next line is reached.
    CacheItemList& rList = impl_getItemList(eType);

    CacheItemList::iterator pItem = rList.find(sItem);
    if (pItem == rList.end())
        pItem = impl_loadItemOnDemand(eType, sItem);
    rList.erase(pItem);

    impl_addItem2FlushList(eType, sItem);
}

/*-----------------------------------------------
    26.11.2003 13:28
-----------------------------------------------*/
void FilterCache::setItem(      EItemType        eType ,
                          const ::rtl::OUString& sItem ,
                          const CacheItem&       aValue)
    throw(css::uno::Exception)
{
    // SAFE ->
    ::osl::ResettableMutexGuard aLock(m_aLock);

    // search for right list
    // An exception is thrown �f "eType" is unknown.
    // => rList will be valid everytimes next line is reached.
    CacheItemList& rList = impl_getItemList(eType);

    // name must be part of the property set too ... otherwhise our
    // container query cant work correctly
    CacheItem aItem = aValue;
    aItem[PROPNAME_NAME] <<= sItem;
    aItem.validateUINames(m_sActLocale);
    rList[sItem] = aItem;

    impl_addItem2FlushList(eType, sItem);
}

/*-----------------------------------------------
    02.07.2003 09:17
-----------------------------------------------*/
void FilterCache::flush()
    throw(css::uno::Exception)
{
    // SAFE ->
    ::osl::ResettableMutexGuard aLock(m_aLock);

    // renew all dependencies and optimizations
    impl_validateAndOptimize();

    if (m_lChangedTypes.size() > 0)
    {
        css::uno::Reference< css::container::XNameAccess > xConfig(impl_openConfig(E_PROVIDER_TYPES), css::uno::UNO_QUERY);
        css::uno::Reference< css::container::XNameAccess > xSet   ;

        xConfig->getByName(CFGSET_TYPES) >>= xSet;
        impl_flushByList(xSet, E_TYPE, m_lTypes, m_lChangedTypes);

        css::uno::Reference< css::util::XChangesBatch > xFlush(xConfig, css::uno::UNO_QUERY);
        xFlush->commitChanges();
    }

    if (m_lChangedFilters.size() > 0)
    {
        css::uno::Reference< css::container::XNameAccess > xConfig(impl_openConfig(E_PROVIDER_FILTERS), css::uno::UNO_QUERY);
        css::uno::Reference< css::container::XNameAccess > xSet   ;

        xConfig->getByName(CFGSET_FILTERS) >>= xSet;
        impl_flushByList(xSet, E_FILTER, m_lFilters, m_lChangedFilters);

        css::uno::Reference< css::util::XChangesBatch > xFlush(xConfig, css::uno::UNO_QUERY);
        xFlush->commitChanges();
    }

    /*TODO FrameLoader/ContentHandler must be flushed here too ... */
}

/*-----------------------------------------------
    20.10.2003 09:22
-----------------------------------------------*/
void FilterCache::impl_flushByList(const css::uno::Reference< css::container::XNameAccess >& xSet  ,
                                         EItemType                                           eType ,
                                   const CacheItemList&                                      rCache,
                                   const OUStringList&                                       lItems)
    throw(css::uno::Exception)
{
    css::uno::Reference< css::container::XNameContainer >   xAddRemoveSet = css::uno::Reference< css::container::XNameContainer >  (xSet, css::uno::UNO_QUERY);
    css::uno::Reference< css::container::XNameReplace >     xReplaceeSet  = css::uno::Reference< css::container::XNameReplace >    (xSet, css::uno::UNO_QUERY);
    css::uno::Reference< css::lang::XSingleServiceFactory > xFactory      = css::uno::Reference< css::lang::XSingleServiceFactory >(xSet, css::uno::UNO_QUERY);

    for (OUStringList::const_iterator pIt  = lItems.begin();
                                      pIt != lItems.end()  ;
                                    ++pIt                  )
    {
        const ::rtl::OUString& sItem  = *pIt;
              EItemFlushState  eState = impl_specifyFlushOperation(xSet, rCache, sItem);
        switch(eState)
        {
            case E_ITEM_REMOVED :
            {
                xAddRemoveSet->removeByName(sItem);
            }
            break;

            case E_ITEM_ADDED :
            {
                css::uno::Reference< css::container::XNameReplace > xItem (xFactory->createInstance(), css::uno::UNO_QUERY);
                CacheItemList::const_iterator                       pItem = rCache.find(sItem);
                impl_saveItem(xItem, eType, pItem->second);
                xAddRemoveSet->insertByName(sItem, css::uno::makeAny(xItem));
            }
            break;

            case E_ITEM_CHANGED :
            {
                css::uno::Reference< css::container::XNameReplace > xItem;
                xSet->getByName(sItem) >>= xItem;
                CacheItemList::const_iterator pItem = rCache.find(sItem);
                impl_saveItem(xItem, eType, pItem->second);
            }
            break;
        }
    }
}

/*-----------------------------------------------
    03.11.2003 08:38
-----------------------------------------------*/
void FilterCache::detectFlatForURL(const css::util::URL& aURL      ,
                                         FlatDetection&  rFlatTypes) const
    throw(css::uno::Exception)
{
    // extract extension from URL, so it can be used directly as key into our hash map!
    // Note further: It must be converted to lower case, because the optimize hash
    // (which maps extensions to types) work with lower case key strings!
    INetURLObject   aParser    (aURL.Main);
    ::rtl::OUString sExtension = aParser.getExtension(INetURLObject::LAST_SEGMENT       ,
                                                      sal_True                          ,
                                                      INetURLObject::DECODE_WITH_CHARSET);
    sExtension = sExtension.toAsciiLowerCase();

    // SAFE -> ----------------------------------
    ::osl::ResettableMutexGuard aLock(m_aLock);

    //*******************************************
    // i) Step over all well known URL pattern
    //    and add registered types to the return list too
    //    Do it as first one - because: if a type match by a
    //    pattern a following deep detection can be supressed!
    //    Further we can stop after first match ...
    for (CacheItemRegistration::const_iterator pPattReg  = m_lURLPattern2Types.begin();
                                               pPattReg != m_lURLPattern2Types.end()  ;
                                             ++pPattReg                               )
    {
        WildCard aPatternCheck(pPattReg->first);
        if (aPatternCheck.Matches(aURL.Main))
        {
            // first item in pPattReg->second is a preferred type or any type,
            // if no preferred one exists!
            // Add it as result to the [out] parameter and break this method.

            const OUStringList& rTypesForPattern = pPattReg->second;
            css::beans::NamedValue aMatch(*(rTypesForPattern.begin()), css::uno::makeAny(sal_True)); // true => indicates: match by URLPattern!
            rFlatTypes.push_back(aMatch);
            return;
        }
    }

    //*******************************************
    // ii) search types matching to the given extension.
    //     Copy every macthing type without changing its order!
    //     Because preferred types was added as first one during
    //     loading configuration.
    CacheItemRegistration::const_iterator pExtReg = m_lExtensions2Types.find(sExtension);
    if (pExtReg != m_lExtensions2Types.end())
    {
        const OUStringList& rTypesForExtension = pExtReg->second;
        for (OUStringList::const_iterator pIt  = rTypesForExtension.begin();
                                          pIt != rTypesForExtension.end()  ;
                                        ++pIt                              )
        {
            css::beans::NamedValue aMatch(*pIt, css::uno::makeAny(sal_False)); // false => indicates: does not match by URLPattern!
            rFlatTypes.push_back(aMatch);
        }
    }

    aLock.clear();
    // <- SAFE ----------------------------------
}

/*-----------------------------------------------
    03.11.2003 08:38
-----------------------------------------------*/
CacheItemList& FilterCache::impl_getItemList(EItemType eType) const
    throw(css::uno::Exception)
{
    // SAFE -> ----------------------------------
    ::osl::ResettableMutexGuard aLock(m_aLock);

    switch(eType)
    {
        case E_TYPE           : return m_lTypes          ; break;
        case E_FILTER         : return m_lFilters        ; break;
        case E_FRAMELOADER    : return m_lFrameLoaders   ; break;
        case E_CONTENTHANDLER : return m_lContentHandlers; break;
        case E_DETECTSERVICE  : return m_lDetectServices ; break;

        default : throw css::uno::Exception(::rtl::OUString::createFromAscii("unknown sub container requested."),
                                            css::uno::Reference< css::uno::XInterface >()                      );
    }

    // <- SAFE ----------------------------------
}

/*-----------------------------------------------
    21.10.2003 13:20
-----------------------------------------------*/
css::uno::Reference< css::uno::XInterface > FilterCache::impl_openConfig(EConfigProvider eProvider)
    throw(css::uno::Exception)
{
    // SAFE ->
    ::osl::ResettableMutexGuard aLock(m_aLock);

    ::rtl::OUString                              sPath      ;
    css::uno::Reference< css::uno::XInterface >* pConfig = 0;
    ::rtl::OString                               sRtlLog    ;
    switch(eProvider)
    {
        case E_PROVIDER_TYPES :
        {
            if (m_xConfigTypes.is())
                return m_xConfigTypes;
            sPath   = CFGPACKAGE_TD_TYPES;
            pConfig = &m_xConfigTypes;
            sRtlLog = ::rtl::OString("framework (as96863) ::FilterCache::impl_openconfig(E_PROVIDER_TYPES)");
        }
        break;

        case E_PROVIDER_FILTERS :
        {
            if (m_xConfigFilters.is())
                return m_xConfigFilters;
            sPath   = CFGPACKAGE_TD_FILTERS;
            pConfig = &m_xConfigFilters;
            sRtlLog = ::rtl::OString("framework (as96863) ::FilterCache::impl_openconfig(E_PROVIDER_FILTERS)");
        }
        break;

        case E_PROVIDER_OTHERS :
        {
            if (m_xConfigOthers.is())
                return m_xConfigOthers;
            sPath   = CFGPACKAGE_TD_OTHERS;
            pConfig = &m_xConfigOthers;
            sRtlLog = ::rtl::OString("framework (as96863) ::FilterCache::impl_openconfig(E_PROVIDER_OTHERS)");
        }
        break;

        default : throw css::uno::Exception(::rtl::OUString::createFromAscii("These configuration node isnt supported here for open!"), 0);
    }

    {
        RTL_LOGFILE_CONTEXT(aLog, sRtlLog.getStr());
        *pConfig = impl_createConfigAccess(sPath    ,
                                           sal_False,   // bReadOnly
                                           sal_True );  // bLocalesMode
    }
    if (!pConfig->is())
    {
        ::rtl::OUStringBuffer sMsg(256);
        sMsg.appendAscii("Could not open configuration file \"");
        sMsg.append     (sPath                                 );
        sMsg.appendAscii("\"."                                 );
        throw css::uno::Exception(sMsg.makeStringAndClear()                    ,
                                  css::uno::Reference< css::uno::XInterface >());
    }

    /*TODO start listening ... */

    return *pConfig;
    // <- SAFE
}

/*-----------------------------------------------
    17.07.2003 10:10
-----------------------------------------------*/
css::uno::Any FilterCache::impl_getDirectCFGValue(const ::rtl::OUString& sDirectKey)
{
    ::rtl::OUString sRoot;
    ::rtl::OUString sKey ;

    if (
        (!::utl::splitLastFromConfigurationPath(sDirectKey, sRoot, sKey)) ||
        (!sRoot.getLength()                                             ) ||
        (!sKey.getLength()                                              )
       )
        return css::uno::Any();

    css::uno::Reference< css::uno::XInterface > xCfg = impl_createConfigAccess(sRoot    ,
                                                                               sal_True ,  // bReadOnly
                                                                               sal_False); // bLocalesMode
    if (!xCfg.is())
        return css::uno::Any();

    css::uno::Reference< css::container::XNameAccess > xAccess(xCfg, css::uno::UNO_QUERY);
    if (!xAccess.is())
        return css::uno::Any();

    css::uno::Any aValue;
    try
    {
        aValue = xAccess->getByName(sKey);
    }
    catch(const css::uno::RuntimeException&)
        { throw; }
    catch(const css::uno::Exception&)
        { aValue.clear(); }

    return aValue;
}

/*-----------------------------------------------
    17.07.2003 09:49
-----------------------------------------------*/
css::uno::Reference< css::uno::XInterface > FilterCache::impl_createConfigAccess(const ::rtl::OUString& sRoot       ,
                                                                                       sal_Bool         bReadOnly   ,
                                                                                       sal_Bool         bLocalesMode)
{
    // SAFE ->
    ::osl::ResettableMutexGuard aLock(m_aLock);

    css::uno::Reference< css::uno::XInterface > xCfg;
    try
    {
        css::uno::Reference< css::lang::XMultiServiceFactory > xConfigProvider(
            m_xSMGR->createInstance(SERVICE_CONFIGURATIONPROVIDER), css::uno::UNO_QUERY);

        if (!xConfigProvider.is())
            return css::uno::Reference< css::uno::XInterface >();

        ::comphelper::SequenceAsVector< css::uno::Any > lParams;
        css::beans::PropertyValue                       aParam ;

        // set root path
        aParam.Name    = _FILTER_CONFIG_FROM_ASCII_("nodepath");
        aParam.Value <<= sRoot;
        lParams.push_back(css::uno::makeAny(aParam));

        // enable "all locales mode" ... if required
        if (bLocalesMode)
        {
            aParam.Name    = _FILTER_CONFIG_FROM_ASCII_("locale");
            aParam.Value <<= _FILTER_CONFIG_FROM_ASCII_("*"     );
            lParams.push_back(css::uno::makeAny(aParam));
        }

        // open it
        if (bReadOnly)
            xCfg = xConfigProvider->createInstanceWithArguments(SERVICE_CONFIGURATIONACCESS, lParams.getAsConstList());
        else
            xCfg = xConfigProvider->createInstanceWithArguments(SERVICE_CONFIGURATIONUPDATEACCESS, lParams.getAsConstList());
    }
    catch(const css::uno::RuntimeException&)
        { throw; }
    catch(const css::uno::Exception&)
        { xCfg.clear(); }

    return xCfg;
    // <- SAFE
}

/*-----------------------------------------------
    24.10.2003 10:03
-----------------------------------------------*/
void FilterCache::impl_validateAndOptimize()
    throw(css::uno::Exception)
{
    // SAFE ->
    ::osl::ResettableMutexGuard aLock(m_aLock);

    RTL_LOGFILE_CONTEXT( aLog, "framework (as96863) ::FilterCache::impl_validateAndOptimize");

    // Special mode for debug versions only
    // Create a log for all detected problems, which
    // occure in the next feew lines.
    // If there are some real errors throw a RuntimException!
    // If there are some warnings only, show an assertion.
    #if OSL_DEBUG_LEVEL > 1
    sal_Int32             nErrors   = 0;
    sal_Int32             nWarnings = 0;
    ::rtl::OUStringBuffer sLog(256);
    #endif

    for (CacheItemList::iterator pIt  = m_lTypes.begin();
                                 pIt != m_lTypes.end()  ;
                               ++pIt                    )
    {
        ::rtl::OUString sType = pIt->first;
        CacheItem       aType = pIt->second;

        // create list of all known detect services / frame loader / content handler on demand
        // Because these informations are available as type properties!
        ::rtl::OUString sDetectService;
        aType[PROPNAME_DETECTSERVICE ] >>= sDetectService;
        if (sDetectService.getLength())
            impl_resolveItem4TypeRegistration(&m_lDetectServices, sDetectService, sType);

        // get its registration for file Extensions AND(!) URLPattern ...
        // It doesnt matter if these items exists or if our
        // used index access create some default ones ...
        // only in case there is no filled set of Extensions AND
        // no filled set of URLPattern -> we must try to remove this invalid item
        // from this cache!
        css::uno::Sequence< ::rtl::OUString > lExtensions;
        css::uno::Sequence< ::rtl::OUString > lURLPattern;
        aType[PROPNAME_EXTENSIONS] >>= lExtensions;
        aType[PROPNAME_URLPATTERN] >>= lURLPattern;
        sal_Int32 ce = lExtensions.getLength();
        sal_Int32 cu = lURLPattern.getLength();

        if (!ce && !cu)
        {
            #if OSL_DEBUG_LEVEL > 1
            sLog.appendAscii("error\t:\t"                                               );
            sLog.appendAscii("The type \""                                              );
            sLog.append     (sType                                                      );
            sLog.appendAscii("\" does not contain any URL pattern nor any extensions.\n");
            ++nErrors;
            #endif

            m_lTypes.erase(pIt);
            continue;
        }

        // create an optimized registration for this type to
        // its set list of extensions/url pattern. If its a "normal" type
        // set it at the end of this optimized list. But if its
        // a "Preferred" one - set it to the fron of this list.
        // Of course multiple "Preferred" registrations can occure
        // (they shouldnt - but they can!) ... Ignore it. The last
        // preferred type is useable in the same manner then every
        // other type!
        sal_Bool bPreferred = sal_False;
        aType[PROPNAME_PREFERRED] >>= bPreferred;

        const ::rtl::OUString* pExtensions = lExtensions.getConstArray();
        for (sal_Int32 e=0; e<ce; ++e)
        {
            // Note: We must be shure that adress the right hash entry
            // does not depend from any upper/lower case problems ...
            ::rtl::OUString sNormalizedExtension = pExtensions[e].toAsciiLowerCase();

            OUStringList& lTypesForExtension = m_lExtensions2Types[sNormalizedExtension];
            if (::std::find(lTypesForExtension.begin(), lTypesForExtension.end(), sType) != lTypesForExtension.end())
                continue;

            if (bPreferred)
                lTypesForExtension.insert(lTypesForExtension.begin(), sType);
            else
                lTypesForExtension.push_back(sType);
        }

        const ::rtl::OUString* pURLPattern = lURLPattern.getConstArray();
        for (sal_Int32 u=0; u<cu; ++u)
        {
            OUStringList& lTypesForURLPattern = m_lURLPattern2Types[pURLPattern[u]];
            if (::std::find(lTypesForURLPattern.begin(), lTypesForURLPattern.end(), sType) != lTypesForURLPattern.end())
                continue;

            if (bPreferred)
                lTypesForURLPattern.insert(lTypesForURLPattern.begin(), sType);
            else
                lTypesForURLPattern.push_back(sType);
        }

        // check some other properties too ...
        // especialy cross references to other cache items.
        #if OSL_DEBUG_LEVEL > 1
        ::rtl::OUString sPrefFilter;
        aType[PROPNAME_PREFERREDFILTER] >>= sPrefFilter;
        if (!sPrefFilter.getLength())
        {
            sLog.appendAscii("error\t:\t"                                                    );
            sLog.appendAscii("The type \""                                                   );
            sLog.append     (sType                                                           );
            sLog.appendAscii("\" does not contain a valid reference to a preferred filter.\n");
            ++nErrors;
            // make no sense to work with this type any longer ...
            continue;
        }

        CacheItemList::const_iterator pIt = m_lFilters.find(sPrefFilter);
        if (pIt == m_lFilters.end())
        {
            sLog.appendAscii("error\t:\t"                       );
            sLog.appendAscii("The type \""                      );
            sLog.append     (sType                              );
            sLog.appendAscii("\" points to an unknown filter \"");
            sLog.append     (sPrefFilter                        );
            sLog.appendAscii("\" as preferred one.\n"           );
            ++nErrors;
            // make no sense to work with this type any longer ...
            continue;
        }

        CacheItem       aPrefFilter   = pIt->second;
        ::rtl::OUString sFilterTypeReg;
        aPrefFilter[PROPNAME_TYPE] >>= sFilterTypeReg;
        if (sFilterTypeReg != sType)
        {
            sLog.appendAscii("error\t:\t"                       );
            sLog.appendAscii("The preferred filter \""          );
            sLog.append     (sPrefFilter                        );
            sLog.appendAscii("\" of type \""                    );
            sLog.append     (sType                              );
            sLog.appendAscii("is registered for another type \"");
            sLog.append     (sFilterTypeReg                     );
            sLog.appendAscii("\".\n"                            );
            ++nErrors;
            // make no sense to work with this type any longer ...
            continue;
        }
        #endif
    }

    // create dependencies between the global default frame loader
    // and all types (and of course if registered filters), which
    // does not registered for any other loader.
    css::uno::Any   aDirectValue       = impl_getDirectCFGValue(CFGDIRECTKEY_DEFAULTFRAMELOADER);
    ::rtl::OUString sDefaultFrameLoader;

    if (
        (!(aDirectValue >>= sDefaultFrameLoader)) ||
        (!sDefaultFrameLoader.getLength()       )
       )
    {
        throw css::uno::Exception(_FILTER_CONFIG_FROM_ASCII_("There is no valid default loader!?"),
                                  css::uno::Reference< css::uno::XInterface >()                  );
    }

    // a) get list of all well known types
    // b) step over all well known frame loader services
    //    and remove all types from list a), which already
    //    referenced by a loader b)
    OUStringList lTypes = getItemNames(E_TYPE);
    for (  pIt  = m_lFrameLoaders.begin();
           pIt != m_lFrameLoaders.end()  ;
         ++pIt                           )
    {
        // Note: of course the default loader must be ignored here.
        // Because we replace its registration later completly with all
        // types, which are not referenced by any other loader.
        // So we can avaoid our code against the complexity of a diff!
        ::rtl::OUString sLoader = pIt->first;
        if (sLoader.equals(sDefaultFrameLoader))
            continue;

        CacheItem&     rLoader   = pIt->second;
        css::uno::Any& rTypesReg = rLoader[PROPNAME_TYPES];
        OUStringList   lTypesReg (rTypesReg);

        for (OUStringList::const_iterator pTypesReg  = lTypesReg.begin();
                                          pTypesReg != lTypesReg.end()  ;
                                        ++pTypesReg                     )
        {
            OUStringList::iterator pTypeCheck = ::std::find(lTypes.begin(), lTypes.end(), *pTypesReg);
            if (pTypeCheck != lTypes.end())
                lTypes.erase(pTypeCheck);
        }
    }

    CacheItem& rDefaultLoader = m_lFrameLoaders[sDefaultFrameLoader];
    rDefaultLoader[PROPNAME_NAME ] <<= sDefaultFrameLoader;
    rDefaultLoader[PROPNAME_TYPES] <<= lTypes.getAsConstList();

    // <- SAFE
}

/*-----------------------------------------------
    20.10.2003 08:15
-----------------------------------------------*/
void FilterCache::impl_addItem2FlushList(      EItemType        eType,
                                         const ::rtl::OUString& sItem)
    throw(css::uno::Exception)
{
    OUStringList* pList = 0;
    switch(eType)
    {
        case E_TYPE :
                pList = &m_lChangedTypes;
                break;

        case E_FILTER :
                pList = &m_lChangedFilters;
                break;

        case E_FRAMELOADER :
                pList = &m_lChangedFrameLoaders;
                break;

        case E_CONTENTHANDLER :
                pList = &m_lChangedContentHandlers;
                break;

        case E_DETECTSERVICE :
                pList = &m_lChangedDetectServices;
                break;

        default : throw css::uno::Exception(::rtl::OUString::createFromAscii("unsupported item type"), 0);
    }

    OUStringList::const_iterator pItem = ::std::find(pList->begin(), pList->end(), sItem);
    if (pItem == pList->end())
        pList->push_back(sItem);
}

/*-----------------------------------------------
    20.10.2003 08:49
-----------------------------------------------*/
FilterCache::EItemFlushState FilterCache::impl_specifyFlushOperation(const css::uno::Reference< css::container::XNameAccess >& xSet ,
                                                                     const CacheItemList&                                      rList,
                                                                     const ::rtl::OUString&                                    sItem)
    throw(css::uno::Exception)
{
    sal_Bool bExistsInConfigLayer = xSet->hasByName(sItem);
    sal_Bool bExistsInMemory      = (rList.find(sItem) != rList.end());

    EItemFlushState eState;

    // !? ... such situation can occure, if an item was added and(!) removed before it was flushed :-)
    if (!bExistsInConfigLayer && !bExistsInMemory)
        eState = E_ITEM_UNCHANGED;
    else
    if (!bExistsInConfigLayer && bExistsInMemory)
        eState = E_ITEM_ADDED;
    else
    if (bExistsInConfigLayer && bExistsInMemory)
        eState = E_ITEM_CHANGED;
    else
    if (bExistsInConfigLayer && !bExistsInMemory)
        eState = E_ITEM_REMOVED;

    return eState;
}

/*-----------------------------------------------
    14.10.2003 09:26
-----------------------------------------------*/
void FilterCache::impl_resolveItem4TypeRegistration(      CacheItemList*   pList,
                                                    const ::rtl::OUString& sItem,
                                                    const ::rtl::OUString& sType)
    throw(css::uno::Exception)
{
    CacheItem& rItem = (*pList)[sItem];
    // In case its a new created entry (automaticly done by the hash_map index operator!)
    // we must be shure, that this entry has its own name as property available.
    // Its needed later at our container interface!
    rItem[PROPNAME_NAME] <<= sItem;

    OUStringList lTypeRegs(rItem[PROPNAME_TYPES]);
    if (::std::find(lTypeRegs.begin(), lTypeRegs.end(), sType) == lTypeRegs.end())
    {
        lTypeRegs.push_back(sType);
        rItem[PROPNAME_TYPES] <<= lTypeRegs.getAsConstList();
    }
}

/*-----------------------------------------------
    28.10.2003 09:18
-----------------------------------------------*/
void FilterCache::impl_load(EFillState eRequiredState)
    throw(css::uno::Exception)
{
    // SAFE ->
    ::osl::ResettableMutexGuard aLock(m_aLock);

    // Attention: Detect services are part of the standard set!
    // So there is no need to handle it seperatly.

    // ------------------------------------------
    // a) The standard set of config value is needed.
    if (
        ((eRequiredState & E_CONTAINS_STANDARD) == E_CONTAINS_STANDARD) &&
        ((m_eFillState   & E_CONTAINS_STANDARD) != E_CONTAINS_STANDARD)
       )
    {
        // Attention! If config couldnt be opened successfully
        // and exception os thrown automaticly and must be forwarded
        // to our calli ...
        css::uno::Reference< css::container::XNameAccess > xTypes(impl_openConfig(E_PROVIDER_TYPES), css::uno::UNO_QUERY);
        {
            RTL_LOGFILE_CONTEXT( aLog, "framework (as96863) ::FilterCache::load std");
            impl_loadSet(xTypes, E_TYPE, E_READ_STANDARD, &m_lTypes);
        }
    }

    // ------------------------------------------
    // b) We need all type informations ...
    if (
        ((eRequiredState & E_CONTAINS_TYPES) == E_CONTAINS_TYPES) &&
        ((m_eFillState   & E_CONTAINS_TYPES) != E_CONTAINS_TYPES)
       )
    {
        // Attention! If config couldnt be opened successfully
        // and exception os thrown automaticly and must be forwarded
        // to our calli ...
        css::uno::Reference< css::container::XNameAccess > xTypes(impl_openConfig(E_PROVIDER_TYPES), css::uno::UNO_QUERY);
        {
            RTL_LOGFILE_CONTEXT( aLog, "framework (as96863) ::FilterCache::load all types");
            impl_loadSet(xTypes, E_TYPE, E_READ_UPDATE, &m_lTypes);
        }
    }

    // ------------------------------------------
    // c) We need all filter informations ...
    if (
        ((eRequiredState & E_CONTAINS_FILTERS) == E_CONTAINS_FILTERS) &&
        ((m_eFillState   & E_CONTAINS_FILTERS) != E_CONTAINS_FILTERS)
       )
    {
        // Attention! If config couldnt be opened successfully
        // and exception os thrown automaticly and must be forwarded
        // to our calli ...
        css::uno::Reference< css::container::XNameAccess > xFilters(impl_openConfig(E_PROVIDER_FILTERS), css::uno::UNO_QUERY);
        {
            RTL_LOGFILE_CONTEXT( aLog, "framework (as96863) ::FilterCache::load all filters");
            impl_loadSet(xFilters, E_FILTER, E_READ_ALL, &m_lFilters);
        }
    }

    // ------------------------------------------
    // c) We need all frame loader informations ...
    if (
        ((eRequiredState & E_CONTAINS_FRAMELOADERS) == E_CONTAINS_FRAMELOADERS) &&
        ((m_eFillState   & E_CONTAINS_FRAMELOADERS) != E_CONTAINS_FRAMELOADERS)
       )
    {
        // Attention! If config couldnt be opened successfully
        // and exception os thrown automaticly and must be forwarded
        // to our calli ...
        css::uno::Reference< css::container::XNameAccess > xLoaders(impl_openConfig(E_PROVIDER_OTHERS), css::uno::UNO_QUERY);
        {
            RTL_LOGFILE_CONTEXT( aLog, "framework (as96863) ::FilterCache::load all frame loader");
            impl_loadSet(xLoaders, E_FRAMELOADER, E_READ_ALL, &m_lFrameLoaders);
        }
    }

    // ------------------------------------------
    // d) We need all content handler informations ...
    if (
        ((eRequiredState & E_CONTAINS_CONTENTHANDLERS) == E_CONTAINS_CONTENTHANDLERS) &&
        ((m_eFillState   & E_CONTAINS_CONTENTHANDLERS) != E_CONTAINS_CONTENTHANDLERS)
       )
    {
        // Attention! If config couldnt be opened successfully
        // and exception os thrown automaticly and must be forwarded
        // to our calli ...
        css::uno::Reference< css::container::XNameAccess > xHandlers(impl_openConfig(E_PROVIDER_OTHERS), css::uno::UNO_QUERY);
        {
            RTL_LOGFILE_CONTEXT( aLog, "framework (as96863) ::FilterCache::load all content handler");
            impl_loadSet(xHandlers, E_CONTENTHANDLER, E_READ_ALL, &m_lContentHandlers);
        }
    }

    // update fill state. Note: its a bit field, which combines different parts.
    m_eFillState = (EFillState) ((sal_Int32)m_eFillState | (sal_Int32)eRequiredState);

    // any data readed?
    // yes! => validate it and update optimized structures.
    impl_validateAndOptimize();

    // <- SAFE
}

/*-----------------------------------------------
    22.09.2003 14:27
-----------------------------------------------*/
void FilterCache::impl_loadSet(const css::uno::Reference< css::container::XNameAccess >& xConfig,
                                     EItemType                                           eType  ,
                                     EReadOption                                         eOption,
                                     CacheItemList*                                      pCache )
    throw(css::uno::Exception)
{
    // get access to the right configuration set
    ::rtl::OUString sSetName;
    switch(eType)
    {
        case E_TYPE :
            sSetName = CFGSET_TYPES;
            break;

        case E_FILTER :
            sSetName = CFGSET_FILTERS;
            break;

        case E_FRAMELOADER :
            sSetName = CFGSET_FRAMELOADERS;
            break;

        case E_CONTENTHANDLER :
            sSetName = CFGSET_CONTENTHANDLERS;
            break;
    }

    css::uno::Any aVal = xConfig->getByName(sSetName);
    css::uno::Reference< css::container::XNameAccess > xSet;
    if (!(aVal >>= xSet) || !xSet.is())
    {
        ::rtl::OUStringBuffer sMsg(256);
        sMsg.appendAscii("could not open config set \"");
        sMsg.append     (sSetName                      );
        sMsg.appendAscii("\""                          );
        throw css::uno::Exception(sMsg.makeStringAndClear()                    ,
                                  css::uno::Reference< css::uno::XInterface >());
    }

    // get names of all existing sub items of this set
    // step over it and fill internal cache structures.

    // But dont update optimized structures like e.g. hash
    // for mapping extensions to its types!

    const css::uno::Sequence< ::rtl::OUString > lItems = xSet->getElementNames();
    const ::rtl::OUString*                      pItems = lItems.getConstArray();
          sal_Int32                             c      = lItems.getLength();
    for (sal_Int32 i=0; i<c; ++i)
    {
        CacheItemList::iterator pItem = pCache->find(pItems[i]);
        switch(eOption)
        {
            // a) read a standard set of properties only or read all
            case E_READ_STANDARD :
            case E_READ_ALL      :
            {
                (*pCache)[pItems[i]] = impl_loadItem(xSet, eType, pItems[i], eOption);
            }
            break;

            // b) read optional properties only!
            //    All items must already exist inside our cache.
            //    But they must be updated.
            case E_READ_UPDATE :
            {
                if (pItem == pCache->end())
                {
                    ::rtl::OUStringBuffer sMsg(256);
                    sMsg.appendAscii("item \""                 );
                    sMsg.append     (pItems[i]                 );
                    sMsg.appendAscii("\" not found for update!");
                    throw css::uno::Exception(sMsg.makeStringAndClear()                    ,
                                              css::uno::Reference< css::uno::XInterface >());
                }
                CacheItem aItem = impl_loadItem(xSet, eType, pItems[i], eOption);
                pItem->second.update(aItem);
            }
            break;
        }
    }
}

/*-----------------------------------------------
    26.11.2003 12:49
-----------------------------------------------*/
void FilterCache::impl_readPatchUINames(const css::uno::Reference< css::container::XNameAccess >& xNode,
                                              CacheItem&                                          rItem)
    throw(css::uno::Exception)
{
    static ::rtl::OUString FORMATNAME_VAR       = ::rtl::OUString::createFromAscii("%productname%"  );
    static ::rtl::OUString FORMATVERSION_VAR    = ::rtl::OUString::createFromAscii("%formatversion%");

    // SAFE -> ----------------------------------
    ::osl::ResettableMutexGuard aLock(m_aLock);
    ::rtl::OUString sFormatName    = m_sFormatName   ;
    ::rtl::OUString sFormatVersion = m_sFormatVersion;
    ::rtl::OUString sLocale        = m_sActLocale    ;
    aLock.clear();
    // <- SAFE ----------------------------------

    css::uno::Any aVal = xNode->getByName(PROPNAME_UINAME);
    css::uno::Reference< css::container::XNameAccess > xUIName;
    if (!(aVal >>= xUIName) && !xUIName.is())
        return;

    const css::uno::Sequence< ::rtl::OUString >           lLocales = xUIName->getElementNames();
    const ::rtl::OUString*                                pLocales = lLocales.getConstArray();
          sal_Int32                                       c        = lLocales.getLength();
          css::uno::Sequence< css::beans::PropertyValue > lUINames (c);
          css::beans::PropertyValue*                      pUINames = lUINames.getArray();
          sal_Int32                                       nEnUsPos = -1;

    for (sal_Int32 i=0; i<c; ++i)
    {
        ::rtl::OUString sValue;
        xUIName->getByName(pLocales[i]) >>= sValue;

        /*TODO must be disabled if format name isnt used any longer for type/filter uinames ...*/
        // replace %productname%
        sal_Int32 nIndex = sValue.indexOf(FORMATNAME_VAR);
        while(nIndex != -1)
        {
            sValue = sValue.replaceAt(nIndex, FORMATNAME_VAR.getLength(), sFormatName);
            nIndex = sValue.indexOf(FORMATNAME_VAR, nIndex);
        }
        // replace %formatversion%
        nIndex = sValue.indexOf(FORMATVERSION_VAR);
        while(nIndex != -1)
        {
            sValue = sValue.replaceAt(nIndex, FORMATVERSION_VAR.getLength(), sFormatVersion);
            nIndex = sValue.indexOf(FORMATVERSION_VAR, nIndex);
        }

        pUINames[i].Name    = pLocales[i];
        pUINames[i].Value <<= sValue;

        // by the way ... set right UIName for the current locale
        // as seperated property!
        if (pLocales[i].equalsIgnoreAsciiCase(sLocale))
            rItem[PROPNAME_UINAME] = pUINames[i].Value;
        
        // And "hold a reference" to the en-US entry ...
        // In case the current locale isnt supported by this item
        // we must fallback to en-US .. or to any other locale!
        if (pLocales[i].equalsAscii("en-US"))
            nEnUsPos = i;
    }
    
    // Check if the UIName property was set successfully.
    // Otherwhise we have to use some fallback scnearios.
    CacheItem::iterator pActualUIName = rItem.find(PROPNAME_UINAME);
    if (pActualUIName == rItem.end())
    {
        if (nEnUsPos > -1)
            // fallback to en-US
            rItem[PROPNAME_UINAME] = pUINames[nEnUsPos].Value;
        else
        if (c > 0)
            // fallback to any other language
            rItem[PROPNAME_UINAME] = pUINames[0].Value;
    }
    
    aVal <<= lUINames;
    rItem[PROPNAME_UINAMES] = aVal;
}

/*-----------------------------------------------
    26.11.2003 12:56
-----------------------------------------------*/
void FilterCache::impl_savePatchUINames(const css::uno::Reference< css::container::XNameReplace >& xNode,
                                        const CacheItem&                                           rItem)
    throw(css::uno::Exception)
{
    css::uno::Reference< css::container::XNameContainer > xAdd  (xNode, css::uno::UNO_QUERY);
    css::uno::Reference< css::container::XNameAccess >    xCheck(xNode, css::uno::UNO_QUERY);

    css::uno::Sequence< css::beans::PropertyValue > lUINames = rItem.getUnpackedValueOrDefault(PROPNAME_UINAMES, css::uno::Sequence< css::beans::PropertyValue >());
    sal_Int32                                       c        = lUINames.getLength();
    const css::beans::PropertyValue*                pUINames = lUINames.getConstArray();

    for (sal_Int32 i=0; i<c; ++i)
    {
        if (xCheck->hasByName(pUINames[i].Name))
            xNode->replaceByName(pUINames[i].Name, pUINames[i].Value);
        else
            xAdd->insertByName(pUINames[i].Name, pUINames[i].Value);
    }
}

/*-----------------------------------------------
    29.10.2003 13:17
    TODO
        clarify, how the real problem behind the
        wrong constructed CacheItem instance (which
        will force a crash during destruction)
        can be solved ...
-----------------------------------------------*/
CacheItem FilterCache::impl_loadItem(const css::uno::Reference< css::container::XNameAccess >& xSet   ,
                                           EItemType                                           eType  ,
                                     const ::rtl::OUString&                                    sItem  ,
                                           EReadOption                                         eOption)
    throw(css::uno::Exception)
{
    // try to get an API object, which points directly to the
    // requested item. If it fail an exception should occure and
    // break this operation. Of course returned API object must be
    // checked too.
    css::uno::Reference< css::container::XNameAccess > xItem;
    #ifdef WORKAROUND_EXCEPTION_PROBLEM
    try
    {
    #endif
        css::uno::Any aVal = xSet->getByName(sItem);
        if (!(aVal >>= xItem) || !xItem.is())
        {
            ::rtl::OUStringBuffer sMsg(256);
            sMsg.appendAscii("found corrupted item \"");
            sMsg.append     (sItem                    );
            sMsg.appendAscii("\"."                    );
            throw css::uno::Exception(sMsg.makeStringAndClear()                    ,
                                      css::uno::Reference< css::uno::XInterface >());
        }
    #ifdef WORKAROUND_EXCEPTION_PROBLEM
    }
    catch(const css::container::NoSuchElementException&)
    {
        throw;
    }
    #endif

    // The internal name of an item must(!) be part of the property
    // set too. Of course its already used as key into the e.g. outside
    // used hash map ... but some of our API methods provide
    // this property set as result only. But the user of this CacheItem
    // should know, which value the key names has :-) ITS IMPORTANT!
    CacheItem aItem;
    aItem[PROPNAME_NAME] = css::uno::makeAny(sItem);
    switch(eType)
    {
        //---------------------------------------
        case E_TYPE :
        {
            // read standard properties of a type
            if (
                (eOption == E_READ_STANDARD) ||
                (eOption == E_READ_ALL     )
               )
            {
                aItem[PROPNAME_PREFERREDFILTER] = xItem->getByName(PROPNAME_PREFERREDFILTER);
                aItem[PROPNAME_DETECTSERVICE  ] = xItem->getByName(PROPNAME_DETECTSERVICE  );
                aItem[PROPNAME_URLPATTERN     ] = xItem->getByName(PROPNAME_URLPATTERN     );
                aItem[PROPNAME_EXTENSIONS     ] = xItem->getByName(PROPNAME_EXTENSIONS     );
                aItem[PROPNAME_PREFERRED      ] = xItem->getByName(PROPNAME_PREFERRED      );
                aItem[PROPNAME_CLIPBOARDFORMAT] = xItem->getByName(PROPNAME_CLIPBOARDFORMAT);
            }
            // read optional properties of a type
            // no else here! Is an additional switch ...
            if (
                (eOption == E_READ_UPDATE) ||
                (eOption == E_READ_ALL   )
               )
            {
                aItem[PROPNAME_MEDIATYPE      ] = xItem->getByName(PROPNAME_MEDIATYPE      );
                impl_readPatchUINames(xItem, aItem);
            }
        }
        break;

        //---------------------------------------
        case E_FILTER :
        {
            // read standard properties of a filter
            if (
                (eOption == E_READ_STANDARD) ||
                (eOption == E_READ_ALL     )
               )
            {
                aItem[PROPNAME_TYPE             ] = xItem->getByName(PROPNAME_TYPE             );
                aItem[PROPNAME_FILEFORMATVERSION] = xItem->getByName(PROPNAME_FILEFORMATVERSION);
                aItem[PROPNAME_UICOMPONENT      ] = xItem->getByName(PROPNAME_UICOMPONENT      );
                aItem[PROPNAME_FILTERSERVICE    ] = xItem->getByName(PROPNAME_FILTERSERVICE    );
                aItem[PROPNAME_DOCUMENTSERVICE  ] = xItem->getByName(PROPNAME_DOCUMENTSERVICE  );

                // special handling for flags! Convert it from a list of names to its
                // int representation ...
                css::uno::Sequence< ::rtl::OUString > lFlagNames;
                if (xItem->getByName(PROPNAME_FLAGS) >>= lFlagNames)
                    aItem[PROPNAME_FLAGS] <<= FilterCache::impl_convertFlagNames2FlagField(lFlagNames);
            }
            // read optional properties of a filter
            // no else here! Is an additional switch ...
            if (
                (eOption == E_READ_UPDATE) ||
                (eOption == E_READ_ALL   )
               )
            {
                aItem[PROPNAME_USERDATA    ] = xItem->getByName(PROPNAME_USERDATA    );
                aItem[PROPNAME_TEMPLATENAME] = xItem->getByName(PROPNAME_TEMPLATENAME);
//TODO remove it if moving of filter uinames to type uinames
//       will be finished realy
#ifdef AS_ENABLE_FILTER_UINAMES
                impl_readPatchUINames(xItem, aItem);
#endif // AS_ENABLE_FILTER_UINAMES
            }
        }
        break;

        //---------------------------------------
        case E_FRAMELOADER :
        case E_CONTENTHANDLER :
        {
            aItem[PROPNAME_TYPES] = xItem->getByName(PROPNAME_TYPES);
        }
        break;
    }

    return aItem;
}

/*-----------------------------------------------
    27.10.2003 08:47
-----------------------------------------------*/
CacheItemList::iterator FilterCache::impl_loadItemOnDemand(      EItemType        eType,
                                                           const ::rtl::OUString& sItem)
    throw(css::uno::Exception)
{
    CacheItemList*                              pList   = 0;
    css::uno::Reference< css::uno::XInterface > xConfig    ;
    ::rtl::OUString                             sSet       ;

    switch(eType)
    {
        case E_TYPE :
        {
            pList   = &m_lTypes;
            xConfig = impl_openConfig(E_PROVIDER_TYPES);
            sSet    = CFGSET_TYPES;
        }
        break;

        case E_FILTER :
        {
            pList   = &m_lFilters;
            xConfig = impl_openConfig(E_PROVIDER_FILTERS);
            sSet    = CFGSET_FILTERS;
        }
        break;

        case E_FRAMELOADER :
        {
            pList   = &m_lFrameLoaders;
            xConfig = impl_openConfig(E_PROVIDER_OTHERS);
            sSet    = CFGSET_FRAMELOADERS;
        }
        break;

        case E_CONTENTHANDLER :
        {
            pList   = &m_lContentHandlers;
            xConfig = impl_openConfig(E_PROVIDER_OTHERS);
            sSet    = CFGSET_CONTENTHANDLERS;
        }
        break;

        case E_DETECTSERVICE :
        {
            OSL_ENSURE(sal_False, "Cant load detect services on demand. Who use this unsupported feature?");
        }
        break;
    }

    css::uno::Reference< css::container::XNameAccess > xRoot(xConfig, css::uno::UNO_QUERY);
    css::uno::Reference< css::container::XNameAccess > xSet ;
    xRoot->getByName(sSet) >>= xSet;
    CacheItem aItem;
    CacheItem::iterator pDbgTest = aItem.find(PROPNAME_NAME);
    aItem = impl_loadItem(xSet, eType, sItem, E_READ_ALL);
    (*pList)[sItem] = aItem;
    _FILTER_CONFIG_LOG_2_("impl_loadItemOnDemand(%d, \"%s\") ... OK", (int)eType, _FILTER_CONFIG_TO_ASCII_(sItem).getStr())

    return pList->find(sItem);
}

/*-----------------------------------------------
    20.10.2003 09:38
-----------------------------------------------*/
void FilterCache::impl_saveItem(const css::uno::Reference< css::container::XNameReplace >& xItem,
                                      EItemType                                            eType,
                                const CacheItem&                                           aItem)
    throw(css::uno::Exception)
{
    CacheItem::const_iterator pIt;
    switch(eType)
    {
        //---------------------------------------
        case E_TYPE :
        {
            pIt = aItem.find(PROPNAME_PREFERREDFILTER);
            if (pIt != aItem.end()) xItem->replaceByName(PROPNAME_PREFERREDFILTER, pIt->second);
            pIt = aItem.find(PROPNAME_DETECTSERVICE);
            if (pIt != aItem.end()) xItem->replaceByName(PROPNAME_DETECTSERVICE, pIt->second);
            pIt = aItem.find(PROPNAME_URLPATTERN);
            if (pIt != aItem.end()) xItem->replaceByName(PROPNAME_URLPATTERN, pIt->second);
            pIt = aItem.find(PROPNAME_EXTENSIONS);
            if (pIt != aItem.end()) xItem->replaceByName(PROPNAME_EXTENSIONS, pIt->second);
            pIt = aItem.find(PROPNAME_PREFERRED);
            if (pIt != aItem.end()) xItem->replaceByName(PROPNAME_PREFERRED, pIt->second);
            pIt = aItem.find(PROPNAME_MEDIATYPE);
            if (pIt != aItem.end()) xItem->replaceByName(PROPNAME_MEDIATYPE, pIt->second);
            pIt = aItem.find(PROPNAME_CLIPBOARDFORMAT);
            if (pIt != aItem.end()) xItem->replaceByName(PROPNAME_CLIPBOARDFORMAT, pIt->second);

            css::uno::Reference< css::container::XNameReplace > xUIName;
            xItem->getByName(PROPNAME_UINAME) >>= xUIName;
            impl_savePatchUINames(xUIName, aItem);
        }
        break;

        //---------------------------------------
        case E_FILTER :
        {
            pIt = aItem.find(PROPNAME_TYPE);
            if (pIt != aItem.end()) xItem->replaceByName(PROPNAME_TYPE, pIt->second);
            pIt = aItem.find(PROPNAME_FILEFORMATVERSION);
            if (pIt != aItem.end()) xItem->replaceByName(PROPNAME_FILEFORMATVERSION, pIt->second);
            pIt = aItem.find(PROPNAME_UICOMPONENT);
            if (pIt != aItem.end()) xItem->replaceByName(PROPNAME_UICOMPONENT, pIt->second);
            pIt = aItem.find(PROPNAME_FILTERSERVICE);
            if (pIt != aItem.end()) xItem->replaceByName(PROPNAME_FILTERSERVICE, pIt->second);
            pIt = aItem.find(PROPNAME_DOCUMENTSERVICE);
            if (pIt != aItem.end()) xItem->replaceByName(PROPNAME_DOCUMENTSERVICE, pIt->second);
            pIt = aItem.find(PROPNAME_USERDATA);
            if (pIt != aItem.end()) xItem->replaceByName(PROPNAME_USERDATA, pIt->second);
            pIt = aItem.find(PROPNAME_TEMPLATENAME);
            if (pIt != aItem.end()) xItem->replaceByName(PROPNAME_TEMPLATENAME, pIt->second);

            // special handling for flags! Convert it from an integer flag field back
            // to a list of names ...
            // But note: because we work directly on a reference to the cache item,
            // its not allowd to change the value here. We must work on a copy!
            sal_Int32 nFlags;
            pIt = aItem.find(PROPNAME_FLAGS);
            if (pIt != aItem.end())
            {
                pIt->second >>= nFlags;
                css::uno::Any aFlagNameList;
                aFlagNameList <<= FilterCache::impl_convertFlagField2FlagNames(nFlags);
                xItem->replaceByName(PROPNAME_FLAGS, aFlagNameList);
            }

//TODO remove it if moving of filter uinames to type uinames
//       will be finished realy
#ifdef AS_ENABLE_FILTER_UINAMES
            css::uno::Reference< css::container::XNameReplace > xUIName;
            xItem->getByName(PROPNAME_UINAME) >>= xUIName;
            impl_savePatchUINames(xUIName, aItem);
#endif //  AS_ENABLE_FILTER_UINAMES
        }
        break;

        //---------------------------------------
        case E_FRAMELOADER :
        case E_CONTENTHANDLER :
        {
            pIt = aItem.find(PROPNAME_TYPES);
            if (pIt != aItem.end()) xItem->replaceByName(PROPNAME_TYPES, pIt->second);
        }
        break;
    }
}

/*-----------------------------------------------
    20.10.2003 09:45
    static! => no locks neccessary
-----------------------------------------------*/
css::uno::Sequence< ::rtl::OUString > FilterCache::impl_convertFlagField2FlagNames(sal_Int32 nFlags)
{
    OUStringList lFlagNames;

    if ((nFlags & FLAGVAL_3RDPARTYFILTER   ) == FLAGVAL_3RDPARTYFILTER   ) lFlagNames.push_back(FLAGNAME_3RDPARTYFILTER   );
    if ((nFlags & FLAGVAL_ALIEN            ) == FLAGVAL_ALIEN            ) lFlagNames.push_back(FLAGNAME_ALIEN            );
    if ((nFlags & FLAGVAL_ASYNCHRON        ) == FLAGVAL_ASYNCHRON        ) lFlagNames.push_back(FLAGNAME_ASYNCHRON        );
    if ((nFlags & FLAGVAL_BROWSERPREFERRED ) == FLAGVAL_BROWSERPREFERRED ) lFlagNames.push_back(FLAGNAME_BROWSERPREFERRED );
    if ((nFlags & FLAGVAL_CONSULTSERVICE   ) == FLAGVAL_CONSULTSERVICE   ) lFlagNames.push_back(FLAGNAME_CONSULTSERVICE   );
    if ((nFlags & FLAGVAL_DEFAULT          ) == FLAGVAL_DEFAULT          ) lFlagNames.push_back(FLAGNAME_DEFAULT          );
    if ((nFlags & FLAGVAL_EXPORT           ) == FLAGVAL_EXPORT           ) lFlagNames.push_back(FLAGNAME_EXPORT           );
    if ((nFlags & FLAGVAL_IMPORT           ) == FLAGVAL_IMPORT           ) lFlagNames.push_back(FLAGNAME_IMPORT           );
    if ((nFlags & FLAGVAL_INTERNAL         ) == FLAGVAL_INTERNAL         ) lFlagNames.push_back(FLAGNAME_INTERNAL         );
    if ((nFlags & FLAGVAL_NOTINCHOOSER     ) == FLAGVAL_NOTINCHOOSER     ) lFlagNames.push_back(FLAGNAME_NOTINCHOOSER     );
    if ((nFlags & FLAGVAL_NOTINFILEDIALOG  ) == FLAGVAL_NOTINFILEDIALOG  ) lFlagNames.push_back(FLAGNAME_NOTINFILEDIALOG  );
    if ((nFlags & FLAGVAL_NOTINSTALLED     ) == FLAGVAL_NOTINSTALLED     ) lFlagNames.push_back(FLAGNAME_NOTINSTALLED     );
    if ((nFlags & FLAGVAL_OWN              ) == FLAGVAL_OWN              ) lFlagNames.push_back(FLAGNAME_OWN              );
    if ((nFlags & FLAGVAL_PACKED           ) == FLAGVAL_PACKED           ) lFlagNames.push_back(FLAGNAME_PACKED           );
    if ((nFlags & FLAGVAL_PREFERRED        ) == FLAGVAL_PREFERRED        ) lFlagNames.push_back(FLAGNAME_PREFERRED        );
    if ((nFlags & FLAGVAL_READONLY         ) == FLAGVAL_READONLY         ) lFlagNames.push_back(FLAGNAME_READONLY         );
    if ((nFlags & FLAGVAL_SILENTEXPORT     ) == FLAGVAL_SILENTEXPORT     ) lFlagNames.push_back(FLAGNAME_SILENTEXPORT     );
    if ((nFlags & FLAGVAL_SUPPORTSSELECTION) == FLAGVAL_SUPPORTSSELECTION) lFlagNames.push_back(FLAGNAME_SUPPORTSSELECTION);
    if ((nFlags & FLAGVAL_TEMPLATE         ) == FLAGVAL_TEMPLATE         ) lFlagNames.push_back(FLAGNAME_TEMPLATE         );
    if ((nFlags & FLAGVAL_TEMPLATEPATH     ) == FLAGVAL_TEMPLATEPATH     ) lFlagNames.push_back(FLAGNAME_TEMPLATEPATH     );
    if ((nFlags & FLAGVAL_USESOPTIONS      ) == FLAGVAL_USESOPTIONS      ) lFlagNames.push_back(FLAGNAME_USESOPTIONS      );
    if ((nFlags & FLAGVAL_COMBINED         ) == FLAGVAL_COMBINED         ) lFlagNames.push_back(FLAGNAME_COMBINED         );

    return lFlagNames.getAsConstList();
}

/*-----------------------------------------------
    27.06.2003 09:26
    static! => no locks neccessary
-----------------------------------------------*/
sal_Int32 FilterCache::impl_convertFlagNames2FlagField(const css::uno::Sequence< ::rtl::OUString >& lNames)
{
    sal_Int32 nField = 0;

    const ::rtl::OUString* pNames = lNames.getConstArray();
          sal_Int32        c      = lNames.getLength();
    for (sal_Int32 i=0; i<c; ++i)
    {
        if (pNames[i].equals(FLAGNAME_3RDPARTYFILTER))
        {
            nField |= FLAGVAL_3RDPARTYFILTER;
            continue;
        }
        if (pNames[i].equals(FLAGNAME_ALIEN))
        {
            nField |= FLAGVAL_ALIEN;
            continue;
        }
        if (pNames[i].equals(FLAGNAME_ASYNCHRON))
        {
            nField |= FLAGVAL_ASYNCHRON;
            continue;
        }
        if (pNames[i].equals(FLAGNAME_BROWSERPREFERRED))
        {
            nField |= FLAGVAL_BROWSERPREFERRED;
            continue;
        }
        if (pNames[i].equals(FLAGNAME_CONSULTSERVICE))
        {
            nField |= FLAGVAL_CONSULTSERVICE;
            continue;
        }
        if (pNames[i].equals(FLAGNAME_DEFAULT))
        {
            nField |= FLAGVAL_DEFAULT;
            continue;
        }
        if (pNames[i].equals(FLAGNAME_EXPORT))
        {
            nField |= FLAGVAL_EXPORT;
            continue;
        }
        if (pNames[i].equals(FLAGNAME_IMPORT))
        {
            nField |= FLAGVAL_IMPORT;
            continue;
        }
        if (pNames[i].equals(FLAGNAME_INTERNAL))
        {
            nField |= FLAGVAL_INTERNAL;
            continue;
        }
        if (pNames[i].equals(FLAGNAME_NOTINCHOOSER))
        {
            nField |= FLAGVAL_NOTINCHOOSER;
            continue;
        }
        if (pNames[i].equals(FLAGNAME_NOTINFILEDIALOG))
        {
            nField |= FLAGVAL_NOTINFILEDIALOG;
            continue;
        }
        if (pNames[i].equals(FLAGNAME_NOTINSTALLED))
        {
            nField |= FLAGVAL_NOTINSTALLED;
            continue;
        }
        if (pNames[i].equals(FLAGNAME_OWN))
        {
            nField |= FLAGVAL_OWN;
            continue;
        }
        if (pNames[i].equals(FLAGNAME_PACKED))
        {
            nField |= FLAGVAL_PACKED;
            continue;
        }
        if (pNames[i].equals(FLAGNAME_PREFERRED))
        {
            nField |= FLAGVAL_PREFERRED;
            continue;
        }
        if (pNames[i].equals(FLAGNAME_READONLY))
        {
            nField |= FLAGVAL_READONLY;
            continue;
        }
        if (pNames[i].equals(FLAGNAME_SILENTEXPORT))
        {
            nField |= FLAGVAL_SILENTEXPORT;
            continue;
        }
        if (pNames[i].equals(FLAGNAME_SUPPORTSSELECTION))
        {
            nField |= FLAGVAL_SUPPORTSSELECTION;
            continue;
        }
        if (pNames[i].equals(FLAGNAME_TEMPLATE))
        {
            nField |= FLAGVAL_TEMPLATE;
            continue;
        }
        if (pNames[i].equals(FLAGNAME_TEMPLATEPATH))
        {
            nField |= FLAGVAL_TEMPLATEPATH;
            continue;
        }
        if (pNames[i].equals(FLAGNAME_USESOPTIONS))
        {
            nField |= FLAGVAL_USESOPTIONS;
            continue;
        }
        if (pNames[i].equals(FLAGNAME_COMBINED))
        {
            nField |= FLAGVAL_COMBINED;
            continue;
        }
    }

    return nField;
}

    } // namespace config
} // namespace filter
