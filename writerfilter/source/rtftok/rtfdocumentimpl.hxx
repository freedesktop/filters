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
 *       Miklos Vajna <vmiklos@frugalware.org>
 * Portions created by the Initial Developer are Copyright (C) 2011 the
 * Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 3 or later (the "GPLv3+"), or
 * the GNU Lesser General Public License Version 3 or later (the "LGPLv3+"),
 * in which case the provisions of the GPLv3+ or the LGPLv3+ are applicable
 * instead of those above.
 */

#ifndef _RTFDOCUMENTIMPL_HXX_
#define _RTFDOCUMENTIMPL_HXX_

#include <memory>
#include <stack>
#include <vector>

#include <rtl/strbuf.hxx>
#include <oox/helper/graphichelper.hxx>
#include <oox/helper/storagebase.hxx>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>

#include <resourcemodel/WW8ResourceModel.hxx>
#include <rtftok/RTFDocument.hxx>
#include <rtfcontrolwords.hxx>
#include <rtfreferencetable.hxx>
#include <rtfsprm.hxx>

class SvStream;

namespace writerfilter {
    namespace rtftok {
        enum RTFInternalState
        {
            INTERNAL_NORMAL,
            INTERNAL_BIN,
            INTERNAL_HEX
        };

        enum RTFDesitnationState
        {
            DESTINATION_NORMAL,
            DESTINATION_SKIP,
            DESTINATION_FONTTABLE,
            DESTINATION_FONTENTRY,
            DESTINATION_COLORTABLE,
            DESTINATION_STYLESHEET,
            DESTINATION_STYLEENTRY,
            DESTINATION_FIELDINSTRUCTION,
            DESTINATION_FIELDRESULT,
            DESTINATION_LISTTABLE,
            DESTINATION_LISTENTRY,
            DESTINATION_LISTOVERRIDETABLE,
            DESTINATION_LISTOVERRIDEENTRY,
            DESTINATION_LISTLEVEL,
            DESTINATION_LEVELTEXT,
            DESTINATION_LEVELNUMBERS,
            DESTINATION_SHPPICT,
            DESTINATION_PICT,
            DESTINATION_PICPROP,
            DESTINATION_SHAPEPROPERTY,
            DESTINATION_SHAPEPROPERTYNAME,
            DESTINATION_SHAPEPROPERTYVALUE,
            DESTINATION_SHAPE,
            DESTINATION_SHAPEINSTRUCTION,
            DESTINATION_SHAPEPROPERTYVALUEPICT,
            DESTINATION_NESTEDTABLEPROPERTIES,
            DESTINATION_FOOTNOTE,
            DESTINATION_BOOKMARKSTART,
            DESTINATION_BOOKMARKEND,
            DESTINATION_REVISIONTABLE,
            DESTINATION_REVISIONENTRY
        };

        enum RTFBorderState
        {
            BORDER_NONE,
            BORDER_PARAGRAPH,
            BORDER_CELL,
            BORDER_PAGE
        };

        enum RTFErrors
        {
            ERROR_OK,
            ERROR_GROUP_UNDER,
            ERROR_GROUP_OVER,
            ERROR_EOF,
            ERROR_HEX_INVALID
        };

        enum RTFControlTypes
        {
            CONTROL_FLAG, // eg \sbknone takes no parameter
            CONTROL_DESTINATION, // eg \fonttbl, if ignored, the whole group should be skipped
            CONTROL_SYMBOL, // eg \tab
            CONTROL_TOGGLE, // eg \b (between on and off)
            CONTROL_VALUE // eg \fs (requires parameter)
        };

        /// Minimalistic buffer elements for nested cells.
        enum RTFBufferTypes
        {
            BUFFER_PROPS,
            BUFFER_CELLEND,
            BUFFER_STARTRUN,
            BUFFER_UTEXT,
            BUFFER_ENDRUN,
            BUFFER_PAR
        };

        /// An entry in the color table.
        class RTFColorTableEntry
        {
            public:
                RTFColorTableEntry();
                sal_uInt8 nRed;
                sal_uInt8 nGreen;
                sal_uInt8 nBlue;
        };

        /// State of the parser, which gets saved / restored when changing groups.
        class RTFParserState
        {
            public:
                RTFParserState();
                RTFInternalState nInternalState;
                RTFDesitnationState nDestinationState;
                RTFBorderState nBorderState;
                // font table, stylesheet table
                RTFSprms_t aTableSprms;
                RTFSprms_t aTableAttributes;
                // reset by plain
                RTFSprms_t aCharacterSprms;
                RTFSprms_t aCharacterAttributes;
                // reset by pard
                RTFSprms_t aParagraphSprms;
                RTFSprms_t aParagraphAttributes;
                // reset by sectd
                RTFSprms_t aSectionSprms;
                RTFSprms_t aSectionAttributes;
                // reset by trowd
                RTFSprms_t aTableRowSprms;
                RTFSprms_t aTableRowAttributes;
                // reset by cellx
                RTFSprms_t aTableCellSprms;
                RTFSprms_t aTableCellAttributes;
                // reset by tx
                RTFSprms_t aTabAttributes;

                RTFReferenceTable::Entries_t aFontTableEntries;
                int nCurrentFontIndex;

                RTFColorTableEntry aCurrentColor;

                RTFReferenceTable::Entries_t aStyleTableEntries;
                int nCurrentStyleIndex;

                rtl_TextEncoding nCurrentEncoding;

                /// Current \uc value.
                int nUc;
                /// Characters to skip, set to nUc by \u.
                int nCharsToSkip;

                /// Next list level index to use when parsing list table.
                int nListLevelNum;
                /// List level entries, which will form a list entry later.
                RTFSprms_t aListLevelEntries;

                /// List of character positions in leveltext to replace.
                std::vector<sal_Int32> aLevelNumbers;

                float nPictureScaleX;
                float nPictureScaleY;
                std::vector<std::pair<rtl::OUString, rtl::OUString>> aShapeProperties;

                /// Current cellx value.
                int nCellX;
                std::deque<RTFSprms::Pointer_t> aTableCellsSprms;
                std::deque<RTFSprms::Pointer_t> aTableCellsAttributes;

                /// CJK or CTL?
                bool bIsCjk;
        };

        /// Implementation of the RTFDocument interface.
        class RTFDocumentImpl
            : public RTFDocument
        {
            public:
                typedef ::boost::shared_ptr<RTFDocumentImpl> Pointer_t;
                RTFDocumentImpl(com::sun::star::uno::Reference<com::sun::star::uno::XComponentContext> const& xContext,
                                com::sun::star::uno::Reference<com::sun::star::io::XInputStream> const& xInputStream,
                                com::sun::star::uno::Reference<com::sun::star::lang::XComponent> const& xDstDoc,
                                com::sun::star::uno::Reference<com::sun::star::frame::XFrame> const& xFrame);
                virtual ~RTFDocumentImpl();
                virtual void resolve(Stream & rHandler);
                virtual std::string getType() const;

                SvStream& Strm();
                Stream& Mapper();
                sal_uInt32 getColorTable(sal_uInt32 nIndex);
                sal_uInt32 getEncodingTable(sal_uInt32 nFontIndex);
                void skipDestination(bool bParsed);
                RTFSprms_t mergeSprms();
                RTFSprms_t mergeAttributes();
                void setSubstream(bool bIsSubtream);
                void setIgnoreFirst(rtl::OUString& rIgnoreFirst);
                void resolveSubstream(sal_uInt32 nPos, Id nId);
                void resolveSubstream(sal_uInt32 nPos, Id nId, rtl::OUString& rIgnoreFirst);
                void seek(sal_uInt32 nPos);
            private:
                int resolveParse();
                int resolveKeyword();
                void resolveShapeProperties(std::vector<std::pair<rtl::OUString, rtl::OUString>>& rShapeProperties);

                int dispatchKeyword(rtl::OString& rKeyword, bool bParam, int nParam);
                int dispatchFlag(RTFKeyword nKeyword);
                int dispatchDestination(RTFKeyword nKeyword);
                int dispatchSymbol(RTFKeyword nKeyword);
                int dispatchToggle(RTFKeyword nKeyword, bool bParam, int nParam);
                int dispatchValue(RTFKeyword nKeyword, int nParam);

                int resolveChars(char ch);
                /// Resolve a picture: If not inline, then anchored.
                int resolvePict(char ch, bool bInline);
                int pushState();
                int popState();
                void text(rtl::OUString& rString);
                void parBreak();
                void sectBreak(bool bFinal);
                void replayBuffer(std::deque<std::pair<RTFBufferTypes, RTFValue::Pointer_t>>& rBuffer);

                com::sun::star::uno::Reference<com::sun::star::uno::XComponentContext> const& m_xContext;
                com::sun::star::uno::Reference<com::sun::star::io::XInputStream> const& m_xInputStream;
                com::sun::star::uno::Reference<com::sun::star::lang::XComponent> const& m_xDstDoc;
                com::sun::star::uno::Reference<com::sun::star::frame::XFrame> const& m_xFrame;
                com::sun::star::uno::Reference<com::sun::star::lang::XMultiServiceFactory> m_xModelFactory;
                SvStream* m_pInStream;
                Stream* m_pMapperStream;
                /// Same as m_aStates.size(), except that this can be negative for invalid input.
                int m_nGroup;
                std::stack<RTFParserState> m_aStates;
                /// Read by RTF_PARD.
                RTFParserState m_aDefaultState;
                bool m_bSkipUnknown;
                /// For debugging purposes, where int value would not be enough
                rtl::OString* m_pCurrentKeyword;
                /// Font index <-> encoding map, *not* part of the parser state
                std::map<int, rtl_TextEncoding> m_aFontEncodings;
                /// Color index <-> RGB color value map
                std::vector<sal_uInt32> m_aColorTable;
                bool m_bFirstRun;
                /// If paragraph properties should be emitted on next run.
                bool m_bNeedPap;
                /// The list table and list override table combined.
                RTFSprms_t m_aListTableSprms;
                /// The settings table.
                RTFSprms_t m_aSettingsTableSprms;

                oox::StorageRef m_xStorage;
                oox::GraphicHelper* m_pGraphicHelper;

                /// Buffered table cells, till cell definitions are not reached.
                std::deque<std::pair<RTFBufferTypes, RTFValue::Pointer_t>> m_aTableBuffer;
                bool m_bTable;
                /// Buffered superscript, till footnote is reached (or not).
                std::deque<std::pair<RTFBufferTypes, RTFValue::Pointer_t>> m_aSuperBuffer;
                bool m_bSuper;
                bool m_bHasFootnote;
                /// If this is a substream.
                bool m_bIsSubstream;
                std::deque<std::pair<Id, sal_uInt32>> m_nHeaderFooterPositions;
                sal_uInt32 m_nGroupStartPos;
                /// Ignore the first occurrence of this text.
                rtl::OUString m_aIgnoreFirst;
                /// Bookmark name <-> index map.
                std::map<rtl::OUString, int> m_aBookmarks;
                /// Revision index <-> author map.
                std::map<int, rtl::OUString> m_aAuthors;
                /// Text from special destinations.
                rtl::OUStringBuffer m_aDestinationText;
        };
    } // namespace rtftok
} // namespace writerfilter

#endif // _RTFDOCUMENTIMPL_HXX_

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
