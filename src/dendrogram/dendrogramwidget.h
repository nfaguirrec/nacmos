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
#ifndef DENDROGRAMWIDGET_H
#define DENDROGRAMWIDGET_H

#include <dendrogram.h>

#include <qvariant.h>
#include <qpixmap.h>
#include <qstringlist.h> 
#include <qdialog.h>

#include <Set.h>

class QwtPlotZoomer ;
class QwtPlotPicker ;
class Dendrogram ;
class QwtArrayData ;
class QGridLayout ;
class QSpacerItem ;
class QPushButton ;

/**
@author Nestor Aguirre
*/
class DendrogramWidget : public QWidget{
	Q_OBJECT
	
	public:
		DendrogramWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 ) ;
		~DendrogramWidget() ;
		
		void setXTitle( const QString& text ) ;
		void setYTitle( const QString& text ) ;
		
		void setData( QwtArrayData qwtarraydata ) ;
		
		void setEquivalenceClass( QMap< double, Set< Set<string> > > equivalenceClasses, Set<string>* universalSet = 0 ) ;
		void setMaxRadius( double maxRadius ) ;
		
	private slots:
		void moved( const QPoint& point );
		
		virtual void print();
		virtual void zoomIn( bool zoom );
		
		virtual void languageChange();
		
	protected:
		QGridLayout* FormSpaceDendrogramWidgetLayout;
		QSpacerItem* spacer1;
		
	private:
		Dendrogram* dendrogram;
		QPushButton* pushButtonZoomIn;
		QPushButton* pushButtonPrint;
		QPixmap image0;
		QPixmap image1;
		
		QwtPlotZoomer *d_zoomer ;
		QwtPlotPicker *d_picker ;
};

#endif
