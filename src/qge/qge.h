/***************************************************************************
 *   Copyright (C) 2005 by Nestor Aguirre                                  *
 *   nfaguirrec@unal.edu.co                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
 
/*--------------------------------------------------------------------------
 *  This file contains NO source code, just some documentation for doxygen to
 *  parse.
 */

/*!
  \mainpage Qge - Qt Graphs Editor 

The Qwt library contains GUI Components and utility classes which are primarily
useful for programs with a technical background. Beside a 2D plot widget
it provides scales, sliders, dials, compasses, thermometers, wheels and knobs 
to control or display values, arrays, or ranges of type double.

  \image html "../screenshots/principal.png"

  \section homepage Project page
  The official project page is hosted at 
	<a href="http://qge.sourceforge.net">sourceforge</a>

  \section license License

  Qge is distributed under the terms of the \ref qwtlicense.

  \section mailinglist Mailing list 
  For all kind of Qge related questions there is a Qwt <a href="http://sourceforge.net/mail/?group_id=13693">mailing list</a>.

  \section supportedplatforms Supported platforms

  Qge might be usable in all environments where you find a
  <a href="http://www.trolltech.com/products/qt/index.html">Qt</a> >= 2.3.0. 
  Unfortunately the resources of the Qge team are limited. All we are using is:
  - Qt/X11 with gcc on Linux\n
  - Qt/Win 2.3.0 non commercial

  \section screenshotsonmainpage Screenshots

  \latexonly Screenshots are only available in the HTML docs.\endlatexonly

  \section downloads Downloads
  Stable releases, prereleases and snapshots are available at the Qge <a href="http://sourceforge.net/projects/qwt">project page</a>.

  Qge doesn´t distribute binary packages, but you find some
  for the following Linux distributions: 
  <a href="http://packages.qa.debian.org/libq/libqwt.html">Debian</a>,
  <a href="http://packages.gentoo.org/packages/?category=x11-libs;name=qwt">Gentoo</a>. Please
  report links that are not listed here.

  For anonymous CVS (a couple of hours old) try:
  \code
cvs -d:pserver:anonymous@cvs.sourceforge.net:/cvsroot/qge login
cvs -z3 -d:pserver:anonymous@cvs.sourceforge.net:/cvsroot/qge co qwt
  \endcode

  \section installonmainpage Installation
 
  Have a look at the qge.pro project file. It is prepared for building
  dynamic libraries in Win32 and Unix/X11 environments. 
  If you don't know what to do with it, read the file \ref qgeinstall and/or 
  Trolltechs 
  <a href="http://www.trolltech.com/developer/documentation/index.html">tmake/qmake</a> documentation. Once you have build the library you have to install 
  all files from the lib, include and doc directories.

  On RPM based systems you can adjust \ref qwtspec and build your own
  RPM package. Read the comments in qwt.spec for the details.

 
  \section relatedprojects Related Projects

  <a href="http://pyqwt.sourceforge.net">PyQwt</a>, a set of Qwt Python bindings.\n

  \section credits Credits:
  \par Authors: 
    Nestor Aguirre 
  \par Project admin:
    Nestor Aguirre \<nfaguirrec@unal.edu.co\>
  \par Developers:
    Nestor Aguirre
  \par Contributors:
*/

