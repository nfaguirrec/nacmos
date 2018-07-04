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
#ifndef QGECONFIGTYPESGRAPHWIDGET_H
#define QGECONFIGTYPESGRAPHWIDGET_H

#include <formqgeconfigtypesgraphwidget.h>

#include <qgeitem.h>

class QgeGraph ;

/**
@author Nestor Aguirre
*/
class QgeConfigTypesGraphWidget : public FormQgeConfigTypesGraphWidget {
	public:
		QgeConfigTypesGraphWidget( QgeGraph* qgegraph, QgeItem::QgeRttiValue rtti, QWidget* parent = 0, const char* name = 0 );
		~QgeConfigTypesGraphWidget() ;
		
		bool close( bool update ) ;
		
	private:
		void readObject() ;
		
	private:
		QgeGraph* qgegraph ;
		QgeItem::QgeRttiValue rtti ;
		
	private slots:
		virtual void datesFromTable( int row ) ;
		virtual void remove() ;
		virtual void add() ;
		
		virtual void updateTable() ;
};

#endif
