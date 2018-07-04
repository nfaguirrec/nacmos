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
#ifndef QGEEDGE_H
#define QGEEDGE_H

#include <GTL/edge.h>
#include <qgeitem.h>
#include <qgeitemdata.h>

/**
@author Nestor Aguirre
*/
class QgeEdge : public QgeItemData, public QgeItem, public edge {

	public:
		QgeEdge( QCanvas* canvas = 0) ;
		~QgeEdge() ;
		
		void copy( QgeEdge* qgeedge ) ;
		int rtti() const ;
		void moveBy( double dx, double dy ) ;
		
		void setEdgeData( edge_data* data ) ;
		void setWidth( int width ) ;
		void setPoints( double xa, double ya, double xb, double yb ) ;
		void setFromPoint( double xa, double ya ) ;
		void setToPoint( double xb, double yb ) ;
		
		int getWidth() ;
		QPoint getFromPoint() ;
		QPoint getToPoint() ;
		
	protected:
		virtual void drawShape(QPainter& p) ;
		virtual QPointArray areaPoints() const ;
		
	private:
		int width ;
		int x1 ;
		int x2 ;
		int y1 ;
		int y2 ;
		int arrowBase ;
		int arrowHeight ;
};

#endif
