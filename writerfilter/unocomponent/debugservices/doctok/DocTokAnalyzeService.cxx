/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: DocTokAnalyzeService.cxx,v $
 *
 *  $Revision: 1.4 $
 *
 *  last change: $Author: hbrinkm $ $Date: 2006-11-01 09:14:37 $
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

/**
  Copyright 2005 Sun Microsystems, Inc.
  */

#include "DocTokAnalyzeService.hxx"
#include <stdio.h>
#include <wchar.h>
#include <com/sun/star/io/XStream.hpp>
#include <com/sun/star/io/XInputStream.hpp>
#include <com/sun/star/io/XSeekable.hpp>
#include <com/sun/star/io/XTruncate.hpp>
#include <com/sun/star/task/XStatusIndicator.hpp>
#include <com/sun/star/container/XNameContainer.hpp>
#include <ucbhelper/contentbroker.hxx>
#include <com/sun/star/ucb/XSimpleFileAccess.hpp>
#include <osl/process.h>
#include <rtl/string.hxx>
#include <hash_set>
#include <assert.h>
#include <cppuhelper/implbase2.hxx>
#include <com/sun/star/embed/XTransactedObject.hpp>
#include <com/sun/star/embed/XStorage.hpp>
#include <com/sun/star/util/XCloseable.hpp>
#include <comphelper/storagehelper.hxx>
#include <com/sun/star/embed/XTransactedObject.hpp>
#include <com/sun/star/beans/PropertyValue.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <comphelper/seqstream.hxx>
#include <com/sun/star/ucb/XSimpleFileAccess.hpp>
#include <com/sun/star/io/XInputStream.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/lang/XMultiComponentFactory.hpp>
#include <com/sun/star/uno/Any.hxx>
#include <com/sun/star/container/XNameContainer.hpp>
#include <doctok/WW8ResourceModel.hxx>
#include <doctok/exceptions.hxx>
#include <doctok/WW8Document.hxx>

#include <ctype.h>

using namespace ::com::sun::star;

namespace writerfilter { namespace doctoktest  {

const sal_Char AnalyzeService::SERVICE_NAME[40] = "debugservices.doctok.AnalyzeService";
const sal_Char AnalyzeService::IMPLEMENTATION_NAME[40] = "debugservices.doctok.AnalyzeService";

class URLLister
{
    uno::Reference<io::XInputStream> xInputStream;
    rtl::OUString mString;
    rtl::OUString mCRLF;
    rtl::OUString mLF;

    sal_uInt32 getEOLIndex()
    {
        sal_Int32 nIndex = mString.indexOf(mCRLF);

        if (nIndex == -1)
        {
            nIndex = mString.indexOf(mLF);            
        }

        return nIndex;
    }

public:
    URLLister(uno::Reference<com::sun::star::uno::XComponentContext> xContext,
              uno::Reference<lang::XMultiComponentFactory> xFactory,
              rtl::OUString absFileUrl)
    {
        uno::Reference<com::sun::star::ucb::XSimpleFileAccess> xFileAccess
            (xFactory->createInstanceWithContext
             (::rtl::OUString(RTL_CONSTASCII_USTRINGPARAM
                              ("com.sun.star.ucb.SimpleFileAccess")), 
              xContext), uno::UNO_QUERY_THROW);
        xInputStream = xFileAccess->openFileRead(absFileUrl) ;

        mLF = rtl::OUString::createFromAscii("\n");
        mCRLF = rtl::OUString::createFromAscii("\r\n");
    }

    rtl::OUString getURL()
    {
        rtl::OUString aResult;

        sal_Int32 nIndex = getEOLIndex();

        while (nIndex == -1)
        {
            uno::Sequence<sal_Int8> aSeq;
            sal_uInt32 nCount = xInputStream->readBytes(aSeq, 1024);

            if (nCount > 0)
            {
                rtl_String * pNew = 0;
                rtl_string_newFromStr_WithLength
                    (&pNew, reinterpret_cast<const sal_Char *>(&aSeq[0]),
                     nCount);

                rtl::OString aTmp(pNew);
                rtl::OUString aTail(rtl::OStringToOUString
                                    (aTmp, RTL_TEXTENCODING_ASCII_US));
                mString = mString.concat(aTail);
            }

            nIndex = getEOLIndex();

            if (nCount != 1024)
                break;
        }

        if (nIndex > 0)
        {
            aResult = mString.copy(0, nIndex);

            if (mString.match(mCRLF, nIndex))
                mString = mString.copy(nIndex + 2);
            else
                mString = mString.copy(nIndex + 1);
        }

        return aResult;
    }
};

AnalyzeService::AnalyzeService(const uno::Reference< uno::XComponentContext > &xContext_) :
xContext( xContext_ )
{
}

sal_Int32 SAL_CALL AnalyzeService::run
( const uno::Sequence< rtl::OUString >& aArguments ) 
    throw (uno::RuntimeException)
{
    uno::Sequence<uno::Any> aUcbInitSequence(2);
    aUcbInitSequence[0] <<= rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("Local"));
    aUcbInitSequence[1] <<= 
        rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("Office"));
    uno::Reference<lang::XMultiServiceFactory> 
        xServiceFactory(xContext->getServiceManager(), uno::UNO_QUERY_THROW);
    uno::Reference<lang::XMultiComponentFactory> 
        xFactory(xContext->getServiceManager(), uno::UNO_QUERY_THROW );

    if (::ucb::ContentBroker::initialize(xServiceFactory, aUcbInitSequence))
    {
        rtl::OUString arg=aArguments[0];

        rtl_uString *dir=NULL;
        osl_getProcessWorkingDir(&dir);
                
        rtl::OUString absFileUrlUrls;
        osl_getAbsoluteFileURL(dir, arg.pData, &absFileUrlUrls.pData);

        URLLister aLister(xContext, xFactory, absFileUrlUrls);

        fprintf(stdout, "<analyze>\n");

        rtl::OUString aURL = aLister.getURL();

        while (aURL.getLength() > 0)
        {
            uno::Reference<com::sun::star::ucb::XSimpleFileAccess> xFileAccess
                (xFactory->createInstanceWithContext
                 (::rtl::OUString(RTL_CONSTASCII_USTRINGPARAM
                                  ("com.sun.star.ucb.SimpleFileAccess")), 
                  xContext), uno::UNO_QUERY_THROW );
            
            rtl::OString aStr;
            aURL.convertToString(&aStr, RTL_TEXTENCODING_ASCII_US,
                                 OUSTRING_TO_OSTRING_CVTFLAGS);
          
            fprintf(stdout, "<file><name>%s</name>\n", aStr.getStr());
        
            try 
            {
                try
                {
                    uno::Reference<io::XInputStream> xInputStream = 
                        xFileAccess->openFileRead(aURL);
                    doctok::WW8Stream::Pointer_t pDocStream = 
                        doctok::WW8DocumentFactory::createStream
                        (xContext, xInputStream);
                    
                    doctok::WW8Document::Pointer_t pDocument = 
                        doctok::WW8DocumentFactory::createDocument(pDocStream);
                    
                    doctok::Stream::Pointer_t pAnalyzer = 
                        doctok::createAnalyzer();
                    pDocument->resolve(*pAnalyzer);
                    
                    fprintf(stdout, "<status>ok</status>\n");
                }
                catch (doctok::Exception e)
                {
                    fprintf(stdout, "<exception>%s</exception>\n", 
                            e.getText().c_str());
                    fprintf(stdout, "<status>failed</status>\n");
                }
            }
            catch (...)
            {
                fprintf(stdout, "<exception>unknown</exception>\n");
                fprintf(stdout, "<status>failed</status>\n");                
            }
            aURL = aLister.getURL();

            fprintf(stdout, "</file>\n");
        }
        
        fprintf(stdout, "</analyze>\n");

        rtl_uString_release(dir);
        ::ucb::ContentBroker::deinitialize();
    }
    else
    {
        fprintf(stdout, "can't initialize UCB");
    }
    return 0;
}

::rtl::OUString AnalyzeService_getImplementationName ()
{
    return rtl::OUString::createFromAscii ( AnalyzeService::IMPLEMENTATION_NAME );
}

sal_Bool SAL_CALL AnalyzeService_supportsService( const ::rtl::OUString& ServiceName )
{
    return ServiceName.equals( rtl::OUString::createFromAscii( AnalyzeService::SERVICE_NAME ) );
}
uno::Sequence< rtl::OUString > SAL_CALL AnalyzeService_getSupportedServiceNames(  ) throw (uno::RuntimeException)
{
    uno::Sequence < rtl::OUString > aRet(1);
    rtl::OUString* pArray = aRet.getArray();
    pArray[0] =  rtl::OUString::createFromAscii ( AnalyzeService::SERVICE_NAME );
    return aRet;
}

uno::Reference< uno::XInterface > SAL_CALL AnalyzeService_createInstance( const uno::Reference< uno::XComponentContext > & xContext) throw( uno::Exception )
{
    return (cppu::OWeakObject*) new AnalyzeService( xContext );
}

} } /* end namespace writerfilter::doctok */
