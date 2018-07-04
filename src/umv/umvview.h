/***************************************************************************
*   Copyright (C) 2005 by Nestor Aguirre                                  *
*   nestor@dirac                                                          *
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
#ifndef UMVVIEW_H
#define UMVVIEW_H

#include <stdlib.h>

#include <qcanvas.h>
#include <qwmatrix.h>
#include <qvaluelist.h>

#include <Pallet.h>

/**
@author Nestor Aguirre
*/
class UmvView : public QFrame{
	
	public:
		UmvView( QWidget *parent=0, const char *name=0 ) ;
		~UmvView() ;
		
		void buildGrid( uint xsize, uint ysize ) ;
		void setTitle( const QString& title ) ;
		void loadData( const QString& fileName ) ;
		
	protected:
// 		virtual void contentsMousePressEvent( QMouseEvent* e ) ;
// 		virtual void contentsMouseMoveEvent( QMouseEvent* e ) ;
// 		virtual void viewportResizeEvent( QResizeEvent *e ) ;
// 		virtual void contentsWheelEvent( QWheelEvent * e ) ;
// 		virtual void contentsMouseDoubleClickEvent( QMouseEvent * e ) ;
// 		virtual void drawContents( QPainter* p, int clipx, int clipy, int clipw, int cliph ) ;
		
		virtual void drawContents ( QPainter* p ) ;
		virtual void resizeEvent( QResizeEvent *e ) ;
		
	private:
		void centering( const QPoint& center ) ;
		void sortHexagons( const QPoint& beginPos ) ;
		void updateBoundingRect() ;
		
	private:
		struct TextItem{
			QString text ;
			QPoint positionIndex ;
		};
		
		struct HexagonData{
			QPoint position ;
			double colorValue ;
		};
		
		QValueList<HexagonData> hexagons ;
		QValueList<TextItem> labels ;
		int hexagonSize ;
		QRect bounding ;
		uint xSize ;
		uint ySize ;
		Pallet* pal ;
};

#endif  //UMVVIEW_H
