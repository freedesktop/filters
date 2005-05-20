/************************************************************************
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

//Standard Java classes
import java.util.*;
import java.io.*;
import java.lang.ref.*;

// Imported TraX classes
import javax.xml.transform.*;
import javax.xml.transform.stream.*;
import org.xml.sax.*;
import org.xml.sax.helpers.*;

//StarOffice Interfaces and UNO
import com.sun.star.uno.*;
import com.sun.star.lang.*;
import com.sun.star.comp.loader.FactoryHelper;
import com.sun.star.registry.XRegistryKey;
import com.sun.star.io.*;
import com.sun.star.ucb.*;
import com.sun.star.beans.*;

//Uno to java Adaptor
import com.sun.star.lib.uno.adapter.*;

/** This outer class provides an inner class to implement the service
 * description, a method to instantiate the
 * component on demand (__getServiceFactory()), and a method to give
 * information about the component (__writeRegistryServiceInfo()).
 */
public class XSLTransformer
    implements XTypeProvider, XServiceName, XServiceInfo, XActiveDataSink,
        XActiveDataSource, XActiveDataControl, XInitialization

{

    /**
     * This component provides java based XSL transformations
     * A SAX based interface is not feasible when crossing language bordes
     * since too much time would be wasted by bridging the events between environments
     * example: 190 pages document, 82000 events 8seconds transform 40(!) sec. bridging
     *
     */

    private XInputStream xistream;
    private XOutputStream xostream;
    private BufferedOutputStream ostream;

    // private static HashMap templatecache;

    private static final int STREAM_BUFFER_SIZE = 4000;
    private static final String STATSPROP = "XSLTransformer.statsfile";
    private static PrintStream statsp;

    private String stylesheeturl;
    private String targeturl;
    private String targetbaseurl;
    private String sourceurl;
    private String sourcebaseurl;
    private String pubtype = new String();
    private String systype = new String();

    // processing thread
    private Thread t;

    // listeners
    private Vector listeners = new Vector();

    //
    private XMultiServiceFactory svcfactory;
    
    // cache for transformations by stylesheet
    private static Hashtable transformers = new Hashtable();

    // --- Initialization ---

    public XSLTransformer(XMultiServiceFactory msf) {
        svcfactory = msf;
    }

    public void initialize(Object[] values) throws com.sun.star.uno.Exception {
        NamedValue nv = null;
        for (int i=0; i<values.length; i++)
        {
            nv = (NamedValue)AnyConverter.toObject(new Type(NamedValue.class), values[i]);
            if (nv.Name.equals("StylesheetURL"))
                stylesheeturl = (String)AnyConverter.toObject(
                    new Type(String.class), nv.Value);
            else if (nv.Name.equals("SourceURL"))
                sourceurl = (String)AnyConverter.toObject(
                    new Type(String.class), nv.Value);
            else if (nv.Name.equals("TargetURL"))
                targeturl = (String)AnyConverter.toObject(
                    new Type(String.class), nv.Value);
            else if (nv.Name.equals("SourceBaseURL"))
                sourcebaseurl = (String)AnyConverter.toObject(
                    new Type(String.class), nv.Value);
            else if (nv.Name.equals("TargetBaseURL"))
                targetbaseurl = (String)AnyConverter.toObject(
                    new Type(String.class), nv.Value);
            else if (nv.Name.equals("SystemType"))
                systype = (String)AnyConverter.toObject(
                    new Type(String.class), nv.Value);
            else if (nv.Name.equals("PublicType"))
                pubtype = (String)AnyConverter.toObject(
                    new Type(String.class), nv.Value);
        }

        // some configurable debugging
        String statsfilepath = null;
        if ((statsfilepath = System.getProperty(STATSPROP)) != null) {
            try {
                File statsfile = new File(statsfilepath);
                statsp = new PrintStream(new FileOutputStream(statsfile.getPath(), false));
            } catch (java.lang.Exception e)
            {
                System.err.println("XSLTransformer: could not open statsfile'"+statsfilepath+"'");
                System.err.println("   "+e.getClass().getName()+": "+e.getMessage());
                System.err.println("   output disabled");
            }
        }
    }

    // --- XActiveDataSink        xistream = aStream;
    public void setInputStream(XInputStream aStream)
    {
        xistream = aStream;
    }

    public com.sun.star.io.XInputStream getInputStream()
    {
        return xistream;
    }

    // --- XActiveDataSource
    public void setOutputStream(XOutputStream aStream)
    {
        xostream = aStream;
        ostream = new BufferedOutputStream(
            new XOutputStreamToOutputStreamAdapter(xostream), STREAM_BUFFER_SIZE);

    }
    public com.sun.star.io.XOutputStream getOutputStream()
    {
        return xostream;
    }

    // --- XActiveDataControl
    public void addListener(XStreamListener aListener)
    {
        if (aListener != null && !listeners.contains(aListener)) {
            listeners.add(aListener);
        }
    }

    public void removeListener(XStreamListener aListener)
    {
        if (aListener != null ) {
            listeners.removeElement(aListener);
        }

    }

    public void start()
    {
        // notify listeners
        t = new Thread(){
            public void run() {
                try {
                    if (statsp != null) statsp.println("starting transformation...");
                    for (Enumeration e = listeners.elements(); e.hasMoreElements();)
                    {
                        XStreamListener l = (XStreamListener)e.nextElement();
                        l.started();
                    }
                    StreamSource stylesource = new StreamSource(stylesheeturl);

                    // buffer input and modify doctype declaration
                    // remove any dtd references but keep localy defined
                    // entities

                    ByteArrayOutputStream bufstream = new ByteArrayOutputStream();
                    final int bsize = 4000;
                    int rbytes = 0;
                    byte[][] byteBuffer = new byte[1][bsize];

                    XSeekable xseek = (XSeekable)UnoRuntime.queryInterface(XSeekable.class, xistream);
                    if (xseek != null) {
                        xseek.seek(0);
                    }
                    
                    while ((rbytes = xistream.readSomeBytes(byteBuffer, bsize)) != 0) {                        
                        bufstream.write(byteBuffer[0], 0, rbytes);
                    }
                    StringBuffer strbuf = new StringBuffer(bufstream.toString("UTF-8"));
                    bufstream = null;
                    
                    // close the input stream after we have transferred all data
                    // into the buffer so it won't keep the content open until it
                    // gets finalized by the java GC
                    xistream.closeInput();

                    // set stream to null, so the uno reference gets cleaned ASAP
                    xistream = null;

                    if (strbuf.indexOf("<!DOCTYPE")!=-1){
                        // document element tag name
                        String tag = strbuf.substring(strbuf.lastIndexOf("/")+1,
                                                      strbuf.lastIndexOf(">"));
                        String entities = new String();
                        // look for inline doctype/entities to preserve
                        if (strbuf.indexOf("[",strbuf.indexOf("<!DOCTYPE"))!=-1){
                            if (strbuf.indexOf("[",strbuf.indexOf("<!DOCTYPE")) <
                                strbuf.indexOf(">",strbuf.indexOf("<!DOCTYPE")))
                            {
                                // copy inline doctype/entities
                                entities = strbuf.substring(
                                    strbuf.indexOf("[",strbuf.indexOf("<!DOCTYPE")),
                                    strbuf.indexOf("]",strbuf.indexOf("<!DOCTYPE"))+1);
                            }
                        }
                        // construct a new doctype/header including only the saved inline information
                        String newDocType =
                            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!DOCTYPE "
                            +tag+" "+entities+">";
                        // replace the header of the original buffer (end index is exclusive)
                        strbuf.replace(0, strbuf.indexOf("<"+tag, 0), newDocType);
                    }
                    
                    StreamSource xmlsource = new StreamSource(new StringReader(strbuf.toString()));
                    
                    BufferedOutputStream output = new BufferedOutputStream(
                        new XOutputStreamToOutputStreamAdapter(xostream));
                    StreamResult xmlresult = new StreamResult(output);
                    
                    // in order to help performance and to remedy a a possible memory
                    // leak in xalan, where it seems, that Transformer instances cannot
                    // be reclaimed though they are no longer referenced here, we use
                    // a cache of weak references to transformers created for specific
                    // style sheet URLs see also #i48384#                    
                    Transformer transformer = null;
                    synchronized(transformers) {
                        java.lang.ref.WeakReference ref = null;
                        // try to get the transformer reference from the cache
                        if ((ref = (java.lang.ref.WeakReference)transformers.get(stylesheeturl)) == null || 
                            (transformer = (Transformer)ref.get()) == null) {
                            // we cannot find a valid reference for this stylesheet
                            if (ref != null) {
                                transformers.remove(stylesheeturl);
                            }
                            TransformerFactory tfactory = TransformerFactory.newInstance();
                            transformer = tfactory.newTransformer(stylesource);
                            transformer.setOutputProperty("encoding", "UTF-8");
                            ref = new java.lang.ref.WeakReference(transformer);
                            transformers.put(stylesheeturl, ref);
                        } 
                    }

                    // invalid to set 'null' as parameter as 'null' is not a valid Java object
                    if(sourceurl != null)
                        transformer.setParameter("sourceURL", sourceurl);
                    if(sourcebaseurl != null)
                        transformer.setParameter("sourceBaseURL", sourcebaseurl);                    
                    if(targeturl != null)
                        transformer.setParameter("targetURL", targeturl);
                    if(targetbaseurl != null)
                        transformer.setParameter("targetBaseURL", targetbaseurl);
                    if(pubtype != null)
                        transformer.setParameter("publicType", pubtype);
                    if(systype != null)
                        transformer.setParameter("systemType", systype);
                    if(svcfactory != null)
                        transformer.setParameter("XMultiServiceFactory",svcfactory);		

                    long tstart = System.currentTimeMillis();                                        
                    
                    StringWriter sw = new StringWriter();
                    StreamResult sr = new StreamResult(sw);
                    transformer.transform(xmlsource, sr);
                                        
                    String s = sw.toString();
                    OutputStreamWriter ow = new OutputStreamWriter(output, "UTF-8");
                    ow.write(s);
                    ow.close();
                    
                    long time = System.currentTimeMillis() - tstart;
                    if (statsp != null) {
                        statsp.println("finished transformation in "+time+"ms");
                    }
                    // dereference input buffer
                    xmlsource = null;
                    
                    output.close();
                    xostream.closeOutput();
                    // try to release reference asap...
                    xostream = null;

                    // notify any listeners about close
                    for (Enumeration e = listeners.elements(); e.hasMoreElements();)
                    {
                        XStreamListener l = (XStreamListener)e.nextElement();
                        l.closed();
                    }

                } catch (java.lang.Throwable ex)
                {
                    // notify any listeners about close
                    for (Enumeration e = listeners.elements(); e.hasMoreElements();)
                    {
                        XStreamListener l = (XStreamListener)e.nextElement();
                        l.error(new com.sun.star.uno.Exception(ex.getClass().getName()+": "+ex.getMessage()));
                    }
                    if (statsp != null)
                    {
                            statsp.println(ex.getClass().getName()+": "+ex.getMessage());
                            ex.printStackTrace(statsp);
                    }
                }
            }
        };
        t.start();
    }

    public void terminate()
    {
        try {
            if (statsp != null){
                statsp.println("terminate called");
            }
            if(t.isAlive()){
                t.interrupt();
                for (Enumeration e = listeners.elements(); e.hasMoreElements();)
                {
                    XStreamListener l = (XStreamListener)e.nextElement();
                    l.terminated();
                }
            }
        } catch (java.lang.Exception ex) {
            if (statsp != null){
                statsp.println(ex.getClass().getName()+": "+ex.getMessage());
                ex.printStackTrace(statsp);
            }
        }
    }

    // --- component management interfaces... ---
    private final static String _serviceName = "com.sun.star.comp.JAXTHelper";

    // Implement methods from interface XTypeProvider
    public byte[] getImplementationId() {
        byte[] byteReturn = {};
        byteReturn = new String( "" + this.hashCode() ).getBytes();
        return( byteReturn );
    }

    public com.sun.star.uno.Type[] getTypes() {
        Type[] typeReturn = {};
        try {
            typeReturn = new Type[] {
            new Type( XTypeProvider.class ),
            new Type( XServiceName.class ),
            new Type( XServiceInfo.class ),
            new Type( XActiveDataSource.class ),
            new Type( XActiveDataSink.class ),
            new Type( XActiveDataControl.class ),
            new Type( XInitialization.class )
            };
        }
        catch( java.lang.Exception exception ) {
        }

        return( typeReturn );
    }

    // --- Implement method from interface XServiceName ---
    public String getServiceName() {
        return( _serviceName );
    }

    // --- Implement methods from interface XServiceInfo ---
    public boolean supportsService(String stringServiceName) {
        return( stringServiceName.equals(_serviceName));
    }

    public String getImplementationName() {
        return( XSLTransformer.class.getName());
    }

    public String[] getSupportedServiceNames() {
        String[] stringSupportedServiceNames = { _serviceName };
        return stringSupportedServiceNames;
    }

    // --- component registration methods ---
    public static XSingleServiceFactory __getServiceFactory(
        String implName, XMultiServiceFactory multiFactory, XRegistryKey regKey)
    {
        XSingleServiceFactory xSingleServiceFactory = null;
        if (implName.equals(XSLTransformer.class.getName()) ) {
            xSingleServiceFactory = FactoryHelper.getServiceFactory(XSLTransformer.class,
            _serviceName, multiFactory, regKey);
        }
        return xSingleServiceFactory;
    }

    public static boolean __writeRegistryServiceInfo(XRegistryKey regKey) {
        return FactoryHelper.writeRegistryServiceInfo(XSLTransformer.class.getName(),
        _serviceName, regKey);
    }

}
