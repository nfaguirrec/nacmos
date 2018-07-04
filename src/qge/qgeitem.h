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
#ifndef QGEITEM_H
#define QGEITEM_H

#include <qcanvas.h>

/**
@author Nestor Aguirre
*/
class QgeItem : public QCanvasPolygonalItem {
	
	Q_PROPERTY( bool processed READ isProcessed WRITE setProcessed )
	Q_PROPERTY( QColor colorSelected READ getColorSelected WRITE setColorSelected )
	Q_PROPERTY( QColor colorUnSelected READ getColorUnSelected WRITE setColorUnSelected )
	Q_PROPERTY( QColor colorLabel READ getColorLabel WRITE setColorLabel )
	
	Q_ENUMS( QgeRttiValues )
	
	public:
		enum QgeRttiValue { Rtti_Item = 0, Rtti_Node = 6, Rtti_Edge = 7 } ;
		QgeItem(QCanvas* canvas = 0) ;
		~QgeItem() ;
		int rtti() const = 0 ;
		
// 		void setSelected( bool selected ) ;
		QColor getColorSelected() ;
		QColor getColorUnSelected() ;
		QColor getColorLabel() ;
		QFont getFont() ;
		
// 		bool isSelected() ;
		void setColorSelected( QColor colorSelected ) ;
		void setColorUnSelected( QColor colorUnSelected ) ;
		void setColorLabel( QColor colorLabel ) ;
		void setFont( QFont font ) ;
		
	protected:
		virtual void drawShape(QPainter &) = 0 ;
		virtual QPointArray areaPoints() const = 0 ;
		void addToChunks() ;
		void removeFromChunks() ;
		QPointArray chunks() const ;
		
	private:
// 		bool selected ;
		
		QColor colorSelected ;
		QColor colorUnSelected ;
		QColor colorLabel ;
		QFont font ;
};

#endif

