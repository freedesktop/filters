/*************************************************************************
 *
 *  $RCSfile: shellio.hxx,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: rt $ $Date: 2004-05-05 16:30:02 $
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
#ifndef _SHELLIO_HXX
#define _SHELLIO_HXX

#ifndef _STRING_HXX //autogen
#include <tools/string.hxx>
#endif
#ifndef _DATE_HXX //autogen
#include <tools/date.hxx>
#endif
#ifndef _TIME_HXX //autogen
#include <tools/time.hxx>
#endif
#ifndef _DATETIME_HXX //autogen
#include <tools/datetime.hxx>
#endif
#ifndef _REF_HXX //autogen
#include <tools/ref.hxx>
#endif
#ifndef _SOT_FORMATS_HXX //autogen
#include <sot/formats.hxx>
#endif

#ifndef _SWTYPES_HXX
#include <swtypes.hxx>
#endif
#ifndef _DOCFAC_HXX
#include <docfac.hxx>	// SwDocFac
#endif
#include <tools/debug.hxx>
class SfxItemPool; 
class SfxItemSet; 
class SvPtrarr; 
class SvStorage; 
class SvStorageStreamRef; 
class SvStream; 
class SvStrings; 
class SvStringsSortDtor; 
class SvxMacroTableDtor; 
namespace binfilter {

// einige Forward - Deklarationen
class SfxFactoryFilterContainer;
class SfxFilter;
class SfxMedium;
class SvxFontItem;

class Sw3Io;
class SwCntntNode;
class SwCrsrShell;
class SwDoc;
class SwPaM;
class SwTextBlocks;
struct SwPosition;
struct Writer_Impl;

// ab so vielen chars wird ein mit einem ASCII/W4W-Reader eingelesener
// Absatz zwangsweise umgebrochen. Muss immer groesser als 200 sein !!!
#define MAX_ASCII_PARA 10000


class SwAsciiOptions
{
    String sFont;
    rtl_TextEncoding eCharSet;
    USHORT nLanguage;
    LineEnd eCRLF_Flag;

public:

    const String& GetFontName() const { return sFont; }
    void SetFontName( const String& rFont ) { sFont = rFont; }

    rtl_TextEncoding GetCharSet() const { return eCharSet; }
    void SetCharSet( rtl_TextEncoding nVal ) { eCharSet = nVal; }

    USHORT GetLanguage() const { return nLanguage; }
    void SetLanguage( USHORT nVal ) { nLanguage = nVal; }

    LineEnd GetParaFlags() const { return eCRLF_Flag; }
    void SetParaFlags( LineEnd eVal ) { eCRLF_Flag = eVal; }

    void Reset()
    {
        sFont.Erase();
        eCRLF_Flag = GetSystemLineEnd();
        eCharSet = ::gsl_getSystemTextEncoding();
        nLanguage = 0;
    }
    // for the automatic conversion (mail/news/...)
//STRIP001 	void ReadUserData( const String& );
    void WriteUserData( String& );

    SwAsciiOptions() { Reset(); }
};

/**************** SwReader/Reader ************************/
// Basisklasse der moeglichen Optionen fuer einen speziellen Reader
class Reader;
// Ruft den Reader mit seinen Optionen, Dokument, Cursor etc.
class SwReader;
// SwRead ist der Pointer auf die Read-Optionen-Basisklasse
typedef Reader *SwRead;

class SwgReaderOption
{
    SwAsciiOptions aASCIIOpts;
    union
    {
        BOOL bFmtsOnly;
        struct
        {
            BOOL bFrmFmts: 1;
            BOOL bPageDescs: 1;
            BOOL bTxtFmts: 1;
            BOOL bNumRules: 1;
            BOOL bMerge:1;
        }  Fmts;
    } What;

public:
    void ResetAllFmtsOnly() { What.bFmtsOnly = 0; }
    void SetAllFmtsOnly() { What.bFmtsOnly = (BOOL)0xff; }
    BOOL IsFmtsOnly() const { return What.bFmtsOnly; }

    BOOL IsFrmFmts() const { return What.Fmts.bFrmFmts; }
    void SetFrmFmts( const BOOL bNew) { What.Fmts.bFrmFmts = bNew; }

    BOOL IsPageDescs() const { return What.Fmts.bPageDescs; }
    void SetPageDescs( const BOOL bNew) { What.Fmts.bPageDescs = bNew; }

    BOOL IsTxtFmts() const { return What.Fmts.bTxtFmts; }
    void SetTxtFmts( const BOOL bNew) { What.Fmts.bTxtFmts = bNew; }

    BOOL IsNumRules() const { return What.Fmts.bNumRules; }
    void SetNumRules( const BOOL bNew) { What.Fmts.bNumRules = bNew; }

    BOOL IsMerge() const { return What.Fmts.bMerge; }
    void SetMerge( const BOOL bNew ) { What.Fmts.bMerge = bNew; }

    const SwAsciiOptions& GetASCIIOpts() const { return aASCIIOpts; }
    void SetASCIIOpts( const SwAsciiOptions& rOpts ) { aASCIIOpts = rOpts; }
    void ResetASCIIOpts() { aASCIIOpts.Reset(); }

    SwgReaderOption()
    {	ResetAllFmtsOnly(); aASCIIOpts.Reset(); }
};

class SwReader: public SwDocFac
{
    SvStream* pStrm;
    SvStorage* pStg;
    SfxMedium* pMedium;		// wer ein Medium haben will (W4W)

    SwPaM* pCrsr;
    String aFileName;

public:
    /*
     * Initiales Einlesen. Dokument wird erst beim Read(..) angelegt.
     * JP 25.04.95: oder falls es mitgegeben wird, in dieses.
     *				Sonderfall fuer Load mit Sw3Reader
     */
//STRIP001 	SwReader( SvStream&, const String& rFilename, SwDoc *pDoc = 0 );
    SwReader( SvStorage&, const String& rFilename, SwDoc *pDoc = 0 );
//STRIP001 	SwReader( SfxMedium&, const String& rFilename, SwDoc *pDoc = 0 );
    /*
     * In ein existierendes Dokument einlesen, Dokument und
     * Position im Dokument werden aus dem SwPaM uebernommen.
     */
//STRIP001 	SwReader( SvStream&, const String& rFilename, SwPaM& );
//STRIP001 	SwReader( SvStorage&, const String& rFilename, SwPaM& );
//STRIP001 	SwReader( SfxMedium&, const String& rFilename, SwPaM& );

    /*
     * Nur SwReader::Read(...) ist die Export-Schnittstelle!!!
     */
//STRIP001 	BOOL NeedsPasswd( const Reader& );
//STRIP001 	BOOL CheckPasswd( const String&, const Reader& );
    ULONG Read( const Reader& );

    // ask for glossaries
//STRIP001 	BOOL HasGlossaries( const Reader& );
//STRIP001 	BOOL ReadGlossaries( const Reader&, SwTextBlocks&, BOOL bSaveRelFiles );
};



/*  */
/****************  SPEZIELLE Reader ************************/

// spezielle - Reader koennen beides sein !! (Excel, W4W, .. )
#define SW_STREAM_READER	1
#define SW_STORAGE_READER	2

class Reader
{
    friend class SwReader;
    SwDoc* pTemplate;
    String aTemplateNm;
    Date aDStamp;
    Time aTStamp;
    DateTime aChkDateTime;

protected:
    SvStream* pStrm;
    SvStorage* pStg;
    SfxMedium* pMedium;		// wer ein Medium haben will (W4W)

    SwgReaderOption aOpt;
    BOOL bInsertMode : 1;
    BOOL bTmplBrowseMode : 1;
    BOOL bReadUTF8: 1;		// Stream als UTF-8 interpretieren
    BOOL bBlockMode: 1;
    BOOL bOrganizerMode : 1;
    BOOL bHasAskTemplateName : 1;
    BOOL bIgnoreHTMLComments : 1;

    virtual String GetTemplateName() const;

public:
    Reader();
    virtual ~Reader();

//STRIP001 	virtual int GetReaderType();
    SwgReaderOption& GetReaderOpt() { return aOpt; }

//STRIP001 	virtual void SetFltName( const String& rFltNm );
//STRIP001 	static void SetNoOutlineNum( SwDoc& rDoc );

    // den Item-Set eines Frm-Formats an das alte Format anpassen
//STRIP001 	static void ResetFrmFmtAttrs( SfxItemSet &rFrmSet );

    // die Rahmen-/Grafik-/OLE-Vorlagen an das alte Format (ohne
    // Umrandung etc.) anpassen
//STRIP001 	static void ResetFrmFmts( SwDoc& rDoc );

    // Die Filter-Vorlage laden, setzen und wieder freigeben
    SwDoc* GetTemplateDoc();
    BOOL SetTemplate( SwDoc& rDoc );
    void ClearTemplate();
    void SetTemplateName( const String& rDir );
//STRIP001 	void MakeHTMLDummyTemplateDoc();

    BOOL IsReadUTF8() const { return bReadUTF8; }
    void SetReadUTF8( BOOL bSet ) { bReadUTF8 = bSet; }

    BOOL IsBlockMode() const { return bBlockMode; }
    void SetBlockMode( BOOL bSet ) { bBlockMode = bSet; }

    BOOL IsOrganizerMode() const { return bOrganizerMode; }
    void SetOrganizerMode( BOOL bSet ) { bOrganizerMode = bSet; }

    BOOL IsIgnoreHTMLComments() const { return bIgnoreHTMLComments; }
    void SetIgnoreHTMLComments( BOOL bSet ) { bIgnoreHTMLComments = bSet; }

//STRIP001 	virtual BOOL HasGlossaries() const;
//STRIP001 	virtual BOOL ReadGlossaries( SwTextBlocks&, BOOL bSaveRelFiles ) const;

    // read the sections of the document, which is equal to the medium.
    // returns the count of it
//STRIP001 	virtual USHORT GetSectionList( SfxMedium& rMedium,
//STRIP001 									SvStrings& rStrings ) const;
private:
    virtual ULONG Read(SwDoc &,SwPaM &,const String &)=0;

    // alle die die Streams / Storages nicht geoeffnet brauchen,
    // muessen die Methode ueberladen (W4W!!)
//STRIP001 	virtual int SetStrmStgPtr();
};

//STRIP001 class RtfReader: public Reader
//STRIP001 {
//STRIP001 	virtual ULONG Read( SwDoc &,SwPaM &,const String &);
//STRIP001 };


class AsciiReader: public Reader
{
    friend class SwReader;
    virtual ULONG Read( SwDoc &,SwPaM &,const String &){DBG_BF_ASSERT(0, "STRIP"); return 0;} //STRIP001 virtual ULONG Read( SwDoc &,SwPaM &,const String &);
public:
    AsciiReader(): Reader() {}
};

class SwgReader: public Reader
{
    virtual ULONG Read( SwDoc &,SwPaM &,const String &){DBG_BF_ASSERT(0, "STRIP"); return 0;} //STRIP001 virtual ULONG Read( SwDoc &,SwPaM &,const String &);
};

class StgReader : public Reader
{
    String aFltName;

protected:
//STRIP001 	ULONG OpenMainStream( SvStorageStreamRef& rRef, USHORT& rBuffSize );

public:
//STRIP001 	virtual int GetReaderType();
    const String& GetFltName() { return aFltName; }
//STRIP001 	virtual void SetFltName( const String& r );
};


class Sw3Reader : public StgReader
{
    Sw3Io* pIO;
    virtual ULONG Read( SwDoc &,SwPaM &,const String &);
public:
    Sw3Reader() : pIO( 0 ) {}

          Sw3Io* GetSw3Io() 				{ return pIO; }
    const Sw3Io* GetSw3Io() const			{ return pIO; }
          void   SetSw3Io( Sw3Io* pIo ) 	{ pIO = pIo; }

    // read the sections of the document, which is equal to the medium.
    // returns the count of it
//STRIP001 	virtual USHORT GetSectionList( SfxMedium& rMedium,
//STRIP001 								SvStrings& rStrings ) const;
};

/*  */
////////////////////////////////////////////////////////////////////////////

// Der uebergebene Stream muss dynamisch angelegt werden und
// vor dem Loeschen der Instanz per Stream() angefordert
// und geloescht werden!

class SwImpBlocks;

class SwTextBlocks
{
    friend class Sw2TextBlocks;
    friend class Sw3IoImp;
    SwImpBlocks* pImp;
    ULONG 		 nErr;

public:
    SwTextBlocks( const String& );
    SwTextBlocks( SvStorage& );
    ~SwTextBlocks();

    void Flush(){}

    SwDoc* GetDoc();
    void   ClearDoc();					// Doc-Inhalt loeschen
    const  String& GetName();
    void   SetName( const String& );
    ULONG GetError() const { return nErr; }

    BOOL   IsOld() const;
    ULONG  ConvertToNew();				// Textbausteine konvertieren

    USHORT GetCount() const;						// Anzahl Textbausteine ermitteln
    USHORT GetIndex( const String& ) const;			// Index fuer Kurznamen ermitteln
    USHORT GetLongIndex( const String& ) const;		//Index fuer Langnamen ermitteln
    const  String& GetShortName( USHORT ) const; 	// Kurzname fuer Index zurueck
    const  String& GetLongName( USHORT ) const;  	// Langname fuer Index zurueck

    BOOL   Delete( USHORT );			// Loeschen
    USHORT Rename( USHORT, const String*, const String* ); // Umbenennen
//STRIP001 	ULONG  CopyBlock( SwTextBlocks& rSource, String& rSrcShort,
//STRIP001 									const String& rLong ); // Block kopieren

//STRIP001 	BOOL   BeginGetDoc( USHORT );			// Textbaustein einlesen
//STRIP001 	void   EndGetDoc();						// Textbaustein wieder loslassen

    BOOL   BeginPutDoc( const String&, const String& ); // Speichern Beginn
    USHORT PutDoc(); 								// Speichern Ende

//STRIP001 	const String& GetText( USHORT );				// Textinhalt einlesen
    USHORT PutText( const String&, const String&, const String& ); // Speichern( Kurzn., Text)

//STRIP001 	BOOL IsOnlyTextBlock( USHORT ) const;
//STRIP001 	BOOL IsOnlyTextBlock( const String& rShort ) const;

    const String& GetFileName() const;		// Dateiname von pImp
//STRIP001 	BOOL IsReadOnly() const;  				// ReadOnly-Flag von pImp

    BOOL GetMacroTable( USHORT nIdx, SvxMacroTableDtor& rMacroTbl );
    BOOL SetMacroTable( USHORT nIdx, const SvxMacroTableDtor& rMacroTbl );

//STRIP001 	String GetValidShortCut( const String& rLong,
//STRIP001 							 BOOL bCheckInBlock = FALSE ) const;

//STRIP001 	BOOL StartPutMuchBlockEntries();
//STRIP001 	void EndPutMuchBlockEntries();
};


extern void _InitFilter();
extern void _FinitFilter();
extern SwRead ReadRtf, ReadAscii, ReadSwg, ReadSw3, ReadHTML, ReadXML;

extern BOOL SetHTMLTemplate( SwDoc &rDoc ); //Fuer Vorlagen aus HTML.vor laden shellio.cxx


/*  */
/////////////////////////////////////////////////////////////////////////////

/*
 * Schreiben, Writer
 */


/* Basis-Klasse aller Writer */

class Writer : public SvRefBase
{
    SwAsciiOptions aAscOpts;

    void _AddFontItem( SfxItemPool& rPool, const SvxFontItem& rFont );
    void _AddFontItems( SfxItemPool& rPool, USHORT nWhichId );

protected:
    Writer_Impl* pImpl;

    SvStream* pStrm;
    SwPaM* pOrigPam;			// der letze zu bearbeitende Pam
    const String* pOrigFileName;

    void ResetWriter();
    BOOL CopyNextPam( SwPaM ** );

    void PutNumFmtFontsInAttrPool();
    void PutEditEngFontsInAttrPool( BOOL bIncl_CJK_CTL = TRUE );
//STRIP001 	void PutCJKandCTLFontsInAttrPool();

    virtual ULONG WriteStream() = 0;

public:
    SwDoc* pDoc;
    SwPaM* pCurPam;
    BOOL bWriteAll : 1;
    BOOL bShowProgress : 1;
    BOOL bWriteClipboardDoc : 1;
    BOOL bWriteOnlyFirstTable : 1;
    BOOL bASCII_ParaAsCR : 1;
    BOOL bASCII_ParaAsBlanc : 1;
    BOOL bASCII_NoLastLineEnd : 1;
    BOOL bUCS2_WithStartChar : 1;

    BOOL bBlock : 1;
    BOOL bOrganizerMode : 1;

    Writer();
    virtual ~Writer();

//STRIP001 	virtual ULONG Write( SwPaM&, SfxMedium&, const String* = 0 );
            ULONG Write( SwPaM&, SvStream&,  const String* = 0 );
    virtual ULONG Write( SwPaM&, SvStorage&, const String* = 0 );

//STRIP001 	virtual void SetPasswd( const String& );
//STRIP001 	virtual void SetVersion( const String&, long );
    virtual BOOL IsStgWriter() const;
    virtual BOOL IsSw3Writer() const;

    BOOL ShowProgress() const 					{ return bShowProgress; }
    void SetShowProgress( BOOL bFlag = FALSE )	{ bShowProgress = bFlag; }

    const String* GetOrigFileName() const 		{ return pOrigFileName; }

    const SwAsciiOptions& GetAsciiOptions() const { return aAscOpts; }
    void SetAsciiOptions( const SwAsciiOptions& rOpt ) { aAscOpts = rOpt; }

    // suche die naechste Bookmark-Position aus der Bookmark-Tabelle
//STRIP001 	USHORT FindPos_Bkmk( const SwPosition& rPos ) const;
    // build a bookmark table, which is sort by the node position. The
    // OtherPos of the bookmarks also inserted.
//STRIP001 	void CreateBookmarkTbl();
    // search alle Bookmarks in the range and return it in the Array
//STRIP001 	USHORT GetBookmarks( const SwCntntNode& rNd,
//STRIP001 						xub_StrLen nStt, xub_StrLen nEnd,
//STRIP001 						SvPtrarr& rArr );
//STRIP001 
    // lege einen neuen PaM an der Position an
//STRIP001 	SwPaM* NewSwPaM( SwDoc & rDoc, ULONG nStartIdx, ULONG nEndIdx,
//STRIP001 									BOOL bNodesArray = TRUE ) const;

    // kopiere ggfs. eine lokale Datei ins Internet
//STRIP001 	BOOL CopyLocalFileToINet( String& rFileNm );

    // Stream-spezifische Routinen, im Storage-Writer NICHT VERWENDEN!

    // Optimierung der Ausgabe auf den Stream.
//STRIP001 	SvStream& OutLong( SvStream& rStrm, long nVal );
//STRIP001 	SvStream& OutULong( SvStream& rStrm, ULONG nVal );

    // Hex-Zahl ausgeben, default ist 2.stellige Zahl
//STRIP001 	SvStream& OutHex( SvStream& rStrm, ULONG nHex, BYTE nLen = 2 );
    // 4-st. Hex-Zahl ausgeben
//STRIP001 	inline SvStream& OutHex4( SvStream& rStrm, USHORT nHex )
//STRIP001 		{   return OutHex( rStrm, nHex, 4 ); }
    // 8-st. Hex-Zahl ausgeben
//STRIP001 	inline SvStream& OutHex8( SvStream& rStrm, ULONG nHex )
//STRIP001 		{   return OutHex( rStrm, nHex, 8 ); }

//STRIP001 	inline SvStream& OutHex( USHORT nHex, BYTE nLen = 2 )      { return OutHex( Strm(), nHex, nLen ); }
//STRIP001 	inline SvStream& OutHex4( USHORT nHex )     { return OutHex( Strm(), nHex, 4 ); }
//STRIP001 	inline SvStream& OutHex8( ULONG nHex )      { return OutHex( Strm(), nHex, 8 ); }
//STRIP001 	inline SvStream& OutLong( long nVal )       { return OutLong( Strm(), nVal ); }
//STRIP001 	inline SvStream& OutULong( ULONG nVal )		{ return OutULong( Strm(), nVal ); }

    void SetStrm( SvStream& rStrm ) { pStrm = &rStrm; }
#ifdef PRODUCT
    SvStream& Strm() { return *pStrm; }
#else
    SvStream& Strm();
#endif

    BOOL IsOrganizerMode() const { return bOrganizerMode; }
    void SetOrganizerMode( BOOL bSet ) { bOrganizerMode = bSet; }
};

#ifndef SW_DECL_WRITER_DEFINED
#define SW_DECL_WRITER_DEFINED
SV_DECL_REF(Writer)
#endif
SV_IMPL_REF(Writer)

// Basisklasse fuer alle Storage-Writer

class StgWriter : public Writer
{
protected:
    String aFltName;
    SvStorage* pStg;

    // Fehler beim Aufruf erzeugen
    virtual ULONG WriteStream(){DBG_BF_ASSERT(0, "STRIP"); return 0;} //STRIP001 virtual ULONG WriteStream();
    virtual ULONG WriteStorage() = 0;

public:
    StgWriter() : Writer(), pStg( 0 ) {}

//STRIP001 	virtual BOOL IsStgWriter() const;
    virtual ULONG Write( SwPaM&, SvStorage&, const String* = 0 );

    SvStorage& GetStorage() const		{ return *pStg; }
    const String& GetFltName() const	{ return aFltName; }
    void SetFltName( const String& r )	{ aFltName = r; }
};

class Sw3Writer : public StgWriter
{
    Sw3Io* pIO;
    BOOL bSaveAs : 1;

    virtual ULONG WriteStorage();

public:
    Sw3Writer() : pIO( 0 ), bSaveAs( FALSE ) {}

          Sw3Io* GetSw3Io() 				{ return pIO; }
    const Sw3Io* GetSw3Io() const			{ return pIO; }
    void SetSw3Io( Sw3Io* pIo, BOOL bSvAs = FALSE )
        { pIO = pIo; bSaveAs = bSvAs; }

//STRIP001 	virtual BOOL IsSw3Writer() const;
};



// Schnittstellenklasse fuer den allgemeinen Zugriff auf die
// speziellen Writer

class SwWriter
{
    SvStream* pStrm;
    SvStorage* pStg;
    SfxMedium* pMedium;

    SwPaM* pOutPam;
    SwCrsrShell *pShell;
    SwDoc &rDoc;
    BOOL bWriteAll;

public:
    ULONG Write( WriterRef& rxWriter, const String* = 0);

//STRIP001 	SwWriter( SvStream&, SwCrsrShell &,BOOL bWriteAll = FALSE );
//STRIP001 	SwWriter( SvStream&, SwDoc & );
    SwWriter( SvStream&, SwPaM &, BOOL bWriteAll = FALSE );

//	SwWriter( SvStorage&, SwCrsrShell &,BOOL bWriteAll = FALSE );
    SwWriter( SvStorage&, SwDoc & );
//	SwWriter( SvStorage&, SwPaM&, BOOL bWriteAll = FALSE );

//STRIP001 	SwWriter( SfxMedium&, SwCrsrShell &,BOOL bWriteAll = FALSE );
//STRIP001 	SwWriter( SfxMedium&, SwDoc & );
//	SwWriter( SfxMedium&, SwPaM&, BOOL bWriteAll = FALSE );
};


/*  */
/////////////////////////////////////////////////////////////////////////////

void GetRTFWriter( const String&, WriterRef& );
void GetASCWriter( const String&, WriterRef& );
void GetSw3Writer( const String&, WriterRef& );
//STRIP001 void GetHTMLWriter( const String&, WriterRef& );
void GetXMLWriter( const String&, WriterRef& );

// Die folgende Klasse ist ein Wrappe fuer die Basic-I/O-Funktionen
// des Writer 3.0. Alles ist statisch. Alle u.a. Filternamen sind die
// Writer-internen Namen, d.h. die namen, die in INSTALL.INI vor dem
// Gleichheitszeichen stehen, z.b. SWG oder ASCII.

class SwIoSystem
{
public:
        // suche ueber den Filtertext den Filtereintrag
    static const SfxFilter* GetFilterOfFilterTxt( const String& rFilterNm,
                                const SfxFactoryFilterContainer* pCnt = 0 );
        // suche ueber den internen FormatNamen den Filtereintrag
    static const SfxFilter* GetFilterOfFormat( const String& rFormat,
                                const SfxFactoryFilterContainer* pCnt = 0 );

    // Feststellen des zu verwendenden Filters fuer die uebergebene
    // Datei. Der Filtername wird zurueckgeliefert. Konnte kein Filter
    // zurueckgeliefert werden, wird der Name des ASCII-Filters geliefert!
    static const SfxFilter* GetFileFilter( const String& rFileName,
                                            const String& rPrefFltName,
                                            SfxMedium* pMedium = 0 );

        // Feststellen ob das File in dem vorgegebenen Format vorliegt.
        // Z.z werden nur unsere eigene Filter unterstuetzt!!
    static FASTBOOL IsFileFilter( SfxMedium& rMedium, const String& rFmtName,
                                    const SfxFilter** ppFlt = 0 );

    static FASTBOOL IsValidStgFilter( SvStorage& , const SfxFilter& );

        static bool IsDetectableText(const sal_Char* pBuf, ULONG &rLen, 
        CharSet *pCharSet=0, bool *pSwap=0, LineEnd *pLineEnd=0);
    static bool IsDetectableW4W(const String& rFileName);

    static const SfxFilter* GetTextFilter(const sal_Char* pBuf, ULONG nLen);
    // gebe einen bestimmten Reader zurueck
//STRIP001 	static Reader* GetReader( const String& rFltName );
    // gebe einen bestimmten Writer zurueck
    static void GetWriter( const String& rFltName, WriterRef& xWrt );

    static const String GetSubStorageName( const SfxFilter& rFltr );
};


// ----------------------------------
// diese Filter sind immer vorhanden und koennen ueber die
// Formatnamen gesucht werden. Alle anderen Filter sind nur intern
// bekannt. Die UI-Seite benutzt die GetReader()/GetWriter() -Funktionen,
// um die speziellen zu erhalten.

extern sal_Char __FAR_DATA FILTER_SWG[];	// SWG-Filter
extern sal_Char __FAR_DATA FILTER_RTF[];	// RTF-Filter
extern sal_Char __FAR_DATA FILTER_TEXT[];	// Text-Filter mit Default-CodeSet
extern sal_Char __FAR_DATA FILTER_BAS[];	// StarBasic (identisch mit ANSI)
extern sal_Char __FAR_DATA FILTER_W4W[];	// W4W-Filter
extern sal_Char __FAR_DATA FILTER_WW8[];	// WinWord 97-Filter
extern sal_Char __FAR_DATA FILTER_SW3[];	// SW3-Storage Filter
extern sal_Char __FAR_DATA FILTER_SW4[];	// SW4-Storage Filter
extern sal_Char __FAR_DATA FILTER_SW4[];	// SW4-Storage Filter
extern sal_Char __FAR_DATA FILTER_SW5[];	// SW5-Storage Filter
extern sal_Char __FAR_DATA FILTER_SWGV[];	// SWG-Vorlagen Filter
extern sal_Char __FAR_DATA FILTER_SW3V[];	// SW3-Storage Vorlagen Filter
extern sal_Char __FAR_DATA FILTER_SW4V[];	// SW4-Storage Vorlagen Filter
extern sal_Char __FAR_DATA FILTER_SW5V[];	// SW5-Storage Vorlagen Filter
extern sal_Char __FAR_DATA FILTER_SWW4V[];	// SW/Web Storage Vorlagen Filter
extern sal_Char __FAR_DATA FILTER_SWW5V[];	// SW/Web Storage Vorlagen Filter
extern sal_Char __FAR_DATA FILTER_TEXT_DLG[];	// text filter with encoding dialog
extern sal_Char __FAR_DATA FILTER_XML[];	// XML filter
extern sal_Char __FAR_DATA FILTER_XMLV[];	// XML filter
extern sal_Char __FAR_DATA FILTER_XMLVW[];	// XML filter


} //namespace binfilter
#endif
