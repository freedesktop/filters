/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008 by Sun Microsystems, Inc.
 *
 * OpenOffice.org - a multi-platform office productivity suite
 *
 * $RCSfile: hstream.h,v $
 * $Revision: 1.3 $
 *
 * This file is part of OpenOffice.org.
 *
 * OpenOffice.org is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 3
 * only, as published by the Free Software Foundation.
 *
 * OpenOffice.org is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Lesser General Public License
 * version 3 along with OpenOffice.org.  If not, see
 * <http://www.openoffice.org/license.html>
 * for a copy of the LGPLv3 License.
 *
 ************************************************************************/

#ifndef _HSTREAM_H_
#define _HSTREAM_H_

typedef unsigned char byte;
/**
 * Stream class
 */
class HStream
{
    public:
        HStream();
        virtual ~HStream();

/**
 *
 */
        void addData( const byte *buf, int aToAdd);
/**
 * Read some byte to buf as given size
 */
        int readBytes( byte *buf, int aToRead);
/**
 * Skip some byte from stream as given size
 */
        int skipBytes( int aToSkip );
/**
 * @returns Size of remained stream
 */
        int available();
/**
 * remove the stream from this object.
 */
        void closeInput();
    private:
        int size;
        byte *seq;
        int pos;
};
#endif
