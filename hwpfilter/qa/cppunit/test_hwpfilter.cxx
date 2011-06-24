/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * Version: MPL 1.1 / GPLv3+ / LGPLv3+
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Initial Developer of the Original Code is
 *        Caolán McNamara <caolanm@redhat.com> (Red Hat, Inc.)
 * Portions created by the Initial Developer are Copyright (C) 2011 the
 * Initial Developer. All Rights Reserved.
 *
 * Contributor(s): Caolán McNamara <caolanm@redhat.com>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 3 or later (the "GPLv3+"), or
 * the GNU Lesser General Public License Version 3 or later (the "LGPLv3+"),
 * in which case the provisions of the GPLv3+ or the LGPLv3+ are applicable
 * instead of those above.
 */

#include <sal/cppunit.h>

#include <cppuhelper/bootstrap.hxx>
#include <comphelper/processfactory.hxx>

#include <com/sun/star/lang/XMultiComponentFactory.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/document/XFilter.hpp>

#include <osl/file.hxx>
#include <osl/process.h>
#include <osl/thread.h>

#include <ucbhelper/contentbroker.hxx>

using namespace ::com::sun::star;

namespace
{
    class HwpFilterTest : public ::CppUnit::TestFixture
    {
    public:
        HwpFilterTest();
        ~HwpFilterTest();

        virtual void setUp();
        virtual void tearDown();

        void recursiveScan(const rtl::OUString &rURL, bool bExpected);
        bool load(const rtl::OUString &rURL);
        void test();

        CPPUNIT_TEST_SUITE(HwpFilterTest);
        CPPUNIT_TEST(test);
        CPPUNIT_TEST_SUITE_END();
    private:
        uno::Reference<uno::XComponentContext> m_xContext;
        uno::Reference<lang::XMultiComponentFactory> m_xFactory;
        uno::Reference<lang::XMultiServiceFactory> m_xMSF;
        uno::Reference<document::XFilter> m_xFilter;

        ::rtl::OUString m_aSrcRoot;
        int m_nLoadedDocs;
    };

    HwpFilterTest::HwpFilterTest() : m_aSrcRoot( RTL_CONSTASCII_USTRINGPARAM( "file://" ) ), m_nLoadedDocs(0)
    {
        m_xContext = cppu::defaultBootstrap_InitialComponentContext();
        m_xFactory = m_xContext->getServiceManager();
        m_xMSF = uno::Reference<lang::XMultiServiceFactory>(m_xFactory, uno::UNO_QUERY_THROW);
        m_xFilter = uno::Reference< document::XFilter >(m_xMSF->createInstance(
            ::rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("com.sun.comp.hwpimport.HwpImportFilter"))),
            uno::UNO_QUERY_THROW);

        const char* pSrcRoot = getenv( "SRC_ROOT" );
        CPPUNIT_ASSERT_MESSAGE("SRC_ROOT env variable not set", pSrcRoot != NULL && pSrcRoot[0] != 0);

#ifdef WNT
        if (pSrcRoot[1] == ':')
            m_aSrcRoot += rtl::OUString::createFromAscii( "/" );
#endif
        m_aSrcRoot += rtl::OUString::createFromAscii( pSrcRoot );

        //Without this we're crashing because callees are using
        //getProcessServiceFactory.  In general those should be removed in favour
        //of retaining references to the root ServiceFactory as its passed around
        comphelper::setProcessServiceFactory(m_xMSF);

        // initialise UCB-Broker
        uno::Sequence<uno::Any> aUcbInitSequence(2);
        aUcbInitSequence[0] <<= rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("Local"));
        aUcbInitSequence[1] <<= rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("Office"));
        bool bInitUcb = ucbhelper::ContentBroker::initialize(m_xMSF, aUcbInitSequence);
        CPPUNIT_ASSERT_MESSAGE("Should be able to initialize UCB", bInitUcb);

        uno::Reference<ucb::XContentProviderManager> xUcb =
            ucbhelper::ContentBroker::get()->getContentProviderManagerInterface();
        uno::Reference<ucb::XContentProvider> xFileProvider(m_xMSF->createInstance(
            rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("com.sun.star.ucb.FileContentProvider"))), uno::UNO_QUERY);
        xUcb->registerContentProvider(xFileProvider, rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("file")), sal_True);
    }

    HwpFilterTest::~HwpFilterTest()
    {
    }

    void HwpFilterTest::setUp()
    {
    }

    void HwpFilterTest::tearDown()
    {
    }

    bool HwpFilterTest::load(const rtl::OUString &rURL)
    {
        uno::Sequence< beans::PropertyValue > aDescriptor(1);
        aDescriptor[0].Name = rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("URL"));
        aDescriptor[0].Value <<= rURL;
        sal_Bool bRet = m_xFilter->filter(aDescriptor);

        ++m_nLoadedDocs;

        return bRet;
    }

    void HwpFilterTest::recursiveScan(const rtl::OUString &rURL, bool bExpected)
    {
        osl::Directory aDir(rURL);

        CPPUNIT_ASSERT(osl::FileBase::E_None == aDir.open());
        osl::DirectoryItem aItem;
        osl::FileStatus aFileStatus(osl_FileStatus_Mask_FileURL|osl_FileStatus_Mask_Type);
        while (aDir.getNextItem(aItem) == osl::FileBase::E_None)
        {
            aItem.getFileStatus(aFileStatus);
            rtl::OUString sURL = aFileStatus.getFileURL();
            if (aFileStatus.getFileType() == osl::FileStatus::Directory)
                recursiveScan(sURL, bExpected);
            else
            {
                bool bRes = load(sURL);
                rtl::OString aRes(rtl::OUStringToOString(sURL, osl_getThreadTextEncoding()));
                CPPUNIT_ASSERT_MESSAGE(aRes.getStr(), bRes == bExpected);
            }
        }
        CPPUNIT_ASSERT(osl::FileBase::E_None == aDir.close());
    }

    void HwpFilterTest::test()
    {
        recursiveScan(m_aSrcRoot + rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("/clone/filters/hwpfilter/qa/cppunit/data/pass")), true);
        recursiveScan(m_aSrcRoot + rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("/clone/filters/hwpfilter/qa/cppunit/data/fail/")), false);

        printf("HwpFilter: tested %d files\n", m_nLoadedDocs);
    }

    CPPUNIT_TEST_SUITE_REGISTRATION(HwpFilterTest);
}

CPPUNIT_PLUGIN_IMPLEMENT();

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
