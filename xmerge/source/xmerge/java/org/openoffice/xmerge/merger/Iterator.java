/*************************************************************************
 *
 *  OpenOffice.org - a multi-platform office productivity suite
 *
 *  $RCSfile: Iterator.java,v $
 *
 *  $Revision: 1.3 $
 *
 *  last change: $Author: ihi $ $Date: 2006-08-01 13:34:15 $
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

package org.openoffice.xmerge.merger;

/**
 *  This is an interface used by the {@link
 *  org.openoffice.xmerge.merger.DiffAlgorithm
 *  DiffAlgorithm} and {@link
 *  org.openoffice.xmerge.merger.MergeAlgorithm
 *  MergeAlgorithm} to access a <code>Document</code>.
 *
 *  @author smak
 */
public interface Iterator {


    /**
     *  Move to next element in the sequence.
     *
     *  @return  The <code>Object</code> of the next element in the sequence.
     *           If there is no next element, then return null.
     */
    public Object next();


    /**
     *  Move to previous element in the sequence.
     *
     *  @return  The <code>Object</code> of the previous element in the sequence.
     *           If there is no previous element, then return null.
     */
    public Object previous();


    /**
     * Move to the beginning of the sequence.
     *
     * @return  The <code>Object</code> of the first element in the sequence.
     *          If it is empty, then return null.
     */
    public Object start();


    /**
     * Move to the end of the sequence.
     *
     * @return  The <code>Object</code> of the last element in the sequence.
     *          If it is empty, then return null.
     */
    public Object end();


    /**
     * Return the current element <code>Object</code> content.
     *
     * @return  The <code>Object</code> at current position.
     */
    public Object currentElement();


    /**
     * Return the total element count in the sequence.
     *
     * @return  The total element count.
     */
    public int elementCount();


    /**
     *  A method to allow the difference algorithm to test whether the
     *  <code>obj1</code> and <code>obj2</code> in the
     *  <code>Iterator</code> are considered equal.  As not every
     *  <code>Object</code> in the <code>Iterator</code> can implement its
     *  own equal method, with this equivalent method, we can allow
     *  flexibility for the <code>Iterator</code> to choose a custom way
     *  to compare two objects.  Two objects can even be compared based on
     *  the position in the <code>Iterator</code> rather than by 
     *  the content via this option.
     *
     *  @param  obj1  The first <code>Object</code>.
     *  @param  obj2  The second <code>Object</code>.
     *
     *  @return  true if equal, false otherwise.
     */
    public boolean equivalent(Object obj1, Object obj2);


    /**
     *  <p>A method to force the <code>Iterator</code> to transverse the tree
     *  again to refresh the content.</p>
     *
     *  <p>It is used mainly for <code>Iterator</code> objects which take a snap
     *  shot instead of dynamically transversing the tree.  The current
     *  position will be set to the beginning.</p>
     */
    public void refresh();
}

