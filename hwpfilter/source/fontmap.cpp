/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: fontmap.cpp,v $
 *
 *  $Revision: 1.5 $
 *
 *  last change: $Author: rt $ $Date: 2007-05-29 15:52:53 $
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

#include <stdio.h>
#ifdef SOLARIS
#include <strings.h>
#else
#include <string.h>
#endif

// #i42367# prevent MS compiler from using system locale for parsing
#ifdef _MSC_VER
#pragma setlocale("C")
#endif


struct FontEntry
{
    const char *familyname;
    int key;
     double ratio;
};
/**
 * ratio\xb4\xc2 \xc7\xd1\xb1\xdb 70%, \xbc\xfd\xc0\xda 10% \xbf\xb5\xb9\xae 20%\xc0\xc7 \xba\xf1\xc0\xb2\xb7\xce \xb1\xb8\xbc\xba\xb5\xc7\xbe\xfa\xb4\xd9\xb4\xc2 \xb0\xa1\xc1\xa4\xc7\xcf\xbf\xa1 \xc1\xa4\xc7\xd8\xc1\xf8\xb4\xd9.
 */
const struct FontEntry FontMapTab[] =
{
    {"\xb8\xed\xc1\xb6",0, 0.97},
    {"\xb0\xed\xb5\xf1",1, 0.97},
    {"\xbb\xf9\xb9\xb0",1, 0.97},
    {"\xc7\xca\xb1\xe2",0, 0.97},
    {"\xbd\xc3\xbd\xba\xc5\xdb",1, 0.84},
    {"\xbd\xc3\xbd\xba\xc5\xdb \xbe\xe0\xc0\xda",1, 0.84},
    {"\xbd\xc3\xbd\xba\xc5\xdb \xb0\xa3\xc0\xda",1, 0.84},
    {"HY\xb5\xd5\xb1\xd9 \xb0\xed\xb5\xf1",2, 0.97},
    {"\xbf\xbe\xc7\xd1\xb1\xdb",0, 0.97},
    {"\xb0\xa1\xb4\xc2\xb0\xf8\xc7\xd1",0, 0.72},
    {"\xc1\xdf\xb0\xa3\xb0\xf8\xc7\xd1",0, 0.72},
    {"\xb1\xbd\xc0\xba\xb0\xf8\xc7\xd1",0, 0.72},
    {"\xb0\xa1\xb4\xc2\xc7\xd1",0, 0.72},
    {"\xc1\xdf\xb0\xa3\xc7\xd1",0, 0.72},
    {"\xb1\xbd\xc0\xba\xc7\xd1",0, 0.72},
    {"\xc8\xde\xb8\xd5\xb8\xed\xc1\xb6",0, 0.97},
    {"\xc8\xde\xb8\xd5\xb0\xed\xb5\xf1",1, 0.97},
    {"\xb0\xa1\xb4\xc2\xbe\xc8\xbb\xf3\xbc\xf6\xc3\xbc",0, 0.55},
    {"\xc1\xdf\xb0\xa3\xbe\xc8\xbb\xf3\xbc\xf6\xc3\xbc",0, 0.637},
    {"\xb1\xbd\xc0\xba\xbe\xc8\xbb\xf3\xbc\xf6\xc3\xbc",0, 0.63},
    {"\xc8\xde\xb8\xd5\xb0\xa1\xb4\xc2\xbb\xf9\xc3\xbc",0, 0.666},
    {"\xc8\xde\xb8\xd5\xc1\xdf\xb0\xa3\xbb\xf9\xc3\xbc",0, 0.685},
    {"\xc8\xde\xb8\xd5\xb1\xbd\xc0\xba\xbb\xf9\xc3\xbc",0, 0.727},
    {"\xc8\xde\xb8\xd5\xb0\xa1\xb4\xc2\xc6\xd4\xc3\xbc",0, 0.666},
    {"\xc8\xde\xb8\xd5\xc1\xdf\xb0\xa3\xc6\xd4\xc3\xbc",0, 0.685},
    {"\xc8\xde\xb8\xd5\xb1\xbd\xc0\xba\xc6\xd4\xc3\xbc",0, 0.727},
    {"\xc8\xde\xb8\xd5\xbf\xbe\xc3\xbc",3, 0.97},
    {"\xc7\xd1\xbe\xe7\xbd\xc5\xb8\xed\xc1\xb6",0, 0.97},
    {"\xc7\xd1\xbe\xe7\xb0\xdf\xb8\xed\xc1\xb6",1, 0.97},
    {"\xc7\xd1\xbe\xe7\xc1\xdf\xb0\xed\xb5\xf1",1, 0.97},
    {"\xc7\xd1\xbe\xe7\xb0\xdf\xb0\xed\xb5\xf1",1, 0.97},
    {"\xc7\xd1\xbe\xe7\xb1\xd7\xb7\xa1\xc7\xc8",0, 0.97},
    {"\xc7\xd1\xbe\xe7\xb1\xc3\xbc\xad",3, 0.97},
    {"\xb9\xae\xc8\xad\xb9\xd9\xc5\xc1",0, 0.97},
    {"\xb9\xae\xc8\xad\xb9\xd9\xc5\xc1\xc1\xa6\xb8\xf1",1, 0.97},
    {"\xb9\xae\xc8\xad\xb5\xb8\xbf\xf2",1, 0.97},
    {"\xb9\xae\xc8\xad\xb5\xb8\xbf\xf2\xc1\xa6\xb8\xf1",0, 0.97},
    {"\xb9\xae\xc8\xad\xbe\xb2\xb1\xe2",0, 0.97},
    {"\xb9\xae\xc8\xad\xbe\xb2\xb1\xe2\xc8\xea\xb8\xb2",0, 0.97},
    {"\xc6\xe6\xc8\xea\xb8\xb2",0, 0.97},
    {"\xba\xb9\xbc\xfe\xbe\xc6",0, 0.827},
    {"\xbf\xc1\xbc\xf6\xbc\xf6",0, 0.97},
    {"\xbf\xc0\xc0\xcc",0, 0.97},
    {"\xb0\xa1\xc1\xf6",0, 0.97},
    {"\xb0\xad\xb3\xb6\xc4\xe1",2, 0.97},
    {"\xb5\xfe\xb1\xe2",3, 0.97},
    {"\xc5\xb8\xc0\xcc\xc7\xc1",0, 0.987},
    {"\xc5\xc2 \xb3\xaa\xb9\xab",1, 0.97},
    {"\xc5\xc2 \xc7\xec\xb5\xe5\xb6\xf3\xc0\xce",0, 0.97},
    {"\xc5\xc2 \xb0\xa1\xb4\xc2 \xc7\xec\xb5\xe5\xb6\xf3\xc0\xce",0, 0.97},
    {"\xc5\xc2 \xc7\xec\xb5\xe5\xb6\xf3\xc0\xceT",0, 0.97},
    {"\xc5\xc2 \xb0\xa1\xb4\xc2 \xc7\xec\xb5\xe5\xb6\xf3\xc0\xceT",0, 0.97},
    {"\xbe\xe7\xc0\xe7 \xb4\xd9\xbf\xee\xb8\xed\xc1\xb6M",0, 0.97},
    {"\xbe\xe7\xc0\xe7 \xba\xbb\xb8\xf1\xb0\xa2M",0, 0.97},
    {"\xbe\xe7\xc0\xe7 \xbc\xd2\xbd\xbd",1, 0.97},
    {"\xbe\xe7\xc0\xe7 \xb8\xc5\xc8\xad",1, 0.987},
    {"\xbe\xe7\xc0\xe7 \xc6\xb0\xc6\xb0",0, 0.97},
    {"\xbe\xe7\xc0\xe7 \xc2\xfc\xbd\xa1",1, 0.97},
    {"\xbe\xe7\xc0\xe7 \xb5\xd1\xb1\xe2",0, 0.97},
    {"\xbe\xe7\xc0\xe7 \xbb\xfe\xb3\xda",1, 0.97},
    {"\xbe\xe7\xc0\xe7 \xbf\xcd\xb4\xe7",1, 0.97},
    {"\xbd\xc5\xb8\xed \xbc\xbc\xb8\xed\xc1\xb6",0, 0.97},
    {"\xbd\xc5\xb8\xed \xbd\xc5\xb8\xed\xc1\xb6",0, 0.97},
    {"\xbd\xc5\xb8\xed \xbd\xc5\xbd\xc5\xb8\xed\xc1\xb6",0, 0.97},
    {"\xbd\xc5\xb8\xed \xc1\xdf\xb8\xed\xc1\xb6",0, 0.97},
    {"\xbd\xc5\xb8\xed \xc5\xc2\xb8\xed\xc1\xb6",0, 0.97},
    {"\xbd\xc5\xb8\xed \xb0\xdf\xb8\xed\xc1\xb6",0, 0.97},
    {"\xbd\xc5\xb8\xed \xbd\xc5\xb9\xae\xb8\xed\xc1\xb6",0, 0.97},
    {"\xbd\xc5\xb8\xed \xbc\xf8\xb8\xed\xc1\xb6",0, 0.97},
    {"\xbd\xc5\xb8\xed \xbc\xbc\xb0\xed\xb5\xf1",1, 0.97},
    {"\xbd\xc5\xb8\xed \xc1\xdf\xb0\xed\xb5\xf1",1, 0.97},
    {"\xbd\xc5\xb8\xed \xc5\xc2\xb0\xed\xb5\xf1",1, 0.97},
    {"\xbd\xc5\xb8\xed \xb0\xdf\xb0\xed\xb5\xf1",1, 0.97},
    {"\xbd\xc5\xb8\xed \xbc\xbc\xb3\xaa\xb7\xe7",2, 0.97},
    {"\xbd\xc5\xb8\xed \xb5\xf0\xb3\xaa\xb7\xe7",2, 0.97},
    {"\xbd\xc5\xb8\xed \xbd\xc5\xb1\xd7\xb7\xa1\xc7\xc8",2, 0.97},
    {"\xbd\xc5\xb8\xed \xc5\xc2\xb1\xd7\xb7\xa1\xc7\xc8",2, 0.97},
    {"\xbd\xc5\xb8\xed \xb1\xc3\xbc\xad",3, 0.97}
};

#define FONTCOUNT 4
#ifndef WIN32
#if defined(LINUX)
char* RepFontTab[] =
{
    "\xb9\xe9\xb9\xac \xb9\xd9\xc5\xc1",                                     /* 0 */
    "\xb9\xe9\xb9\xac \xb5\xb8\xbf\xf2",                                      /* 1 */
    "\xb9\xe9\xb9\xac \xb1\xbc\xb8\xb2",                                      /* 2 */
    "\xb9\xe9\xb9\xac \xc7\xec\xb5\xe5\xb6\xf3\xc0\xce"                                      /* 3 */
};
#else
const char* RepFontTab[] =
{
    "Batang",                                     /* 0 */
    "Dotum",                                      /* 1 */
    "Gulim",                                      /* 2 */
    "Gungso"                                      /* 3 */
};
#endif
#else
char* RepFontTab[] =
{
    "\xb9\xd9\xc5\xc1",                                       /* 0 */
    "\xb5\xb8\xbf\xf2",                                       /* 1 */
    "\xb1\xbc\xb8\xb2",                                       /* 2 */
    "\xb1\xc3\xbc\xad"                                        /* 3 */
};
#endif

int getRepFamilyName(const char* orig, char *buf, double &ratio)
{
    int i;
    int size = sizeof(FontMapTab)/sizeof(FontEntry);
    for( i = 0 ; i < size ; i++)
    {
        if( !strcmp(orig, FontMapTab[i].familyname) ){
                ratio = FontMapTab[i].ratio;
            return strlen( strcpy(buf,RepFontTab[FontMapTab[i].key]) );
          }
    }
     ratio = FontMapTab[0].ratio;
    return strlen( strcpy(buf, RepFontTab[0] ) );
}
