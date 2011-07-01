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

#ifndef _RTFTYPES_HXX_
#define _RTFTYPES_HXX_

#include <rtfcontrolwords.hxx>

namespace writerfilter {
    namespace rtftok {
        /// Respresents an RTF Control Word
        typedef struct
        {
            const char *sKeyword;
            int nControlType;
            RTFKeyword nIndex;
        } RTFSymbol;
        extern RTFSymbol aRTFControlWords[];
        extern int nRTFControlWords;

        /// RTF legacy charsets
        typedef struct
        {
            int charset;
            int codepage;
        } RTFEncoding;
        extern RTFEncoding aRTFEncodings[];
        extern int nRTFEncodings;
    } // namespace rtftok
} // namespace writerfilter

#endif // _RTFTYPES_HXX_

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
