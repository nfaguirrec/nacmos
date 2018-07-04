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
#ifndef QGENODE_H
#define QGENODE_H

#include <GTL/node.h>
#include <qgeitem.h>
#include <qgeitemdata.h>

/**
@author Nestor Aguirre
*/
class QgeNode : public QgeItemData, public QgeItem, public node {
	
	public:
		QgeNode( QCanvas* canvas = 0, double x = 0, double y = 0 ) ;
		~QgeNode() ;
		
		void copy( QgeNode* qgenode ) ;
		int rtti() const ;
		
		int getWidth() ;
		int getHeight() ;
		QSize getSize() ;
		double getNeighborRange() ;
		
		void setNodeData( node_data* data ) ;
		void setWidth( int width ) ;
		void setHeight( int height ) ;
		void setSize( int width, int height ) ;
		void setLabel( QString label ) ;
		void setAutomaticResize( bool automaticResize ) ;
		void setAutomaticLabel( bool automaticLabel ) ;
		void setFirst( bool first ) ;
		void setNeighborRange( double neighborRange ) ;
		
		bool isEnableAutomaticResize() ;
		bool isEnableAutomaticLabel() ;
		bool isFirst() ;
		
	protected:
		void drawShape(QPainter &p) ;
		QPointArray areaPoints() const ;
		
	private:
		int width ;
		int height ;
		bool automaticResize ;
		bool automaticLabel ;
		bool first ;
		double neighborRange ;
};

#endif
