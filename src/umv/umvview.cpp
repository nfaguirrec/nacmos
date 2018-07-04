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
#include "umvview.h"

#include <math.h>
#include <fstream>
#include <iostream>
using namespace std ;

#include <qsize.h>
#include <qpaintdevice.h>
#include <qpainter.h>

int aprox( double value ){
	double fractpart, intpart;
	fractpart = modf( value , &intpart );
	
	if( fractpart >= 0.5 ){
		return ceil( value ) ;
	}else{
		return floor( value ) ;
	}
}

UmvView::UmvView( QWidget *parent, const char *name ):
QFrame(parent, name)
{
	hexagonSize = 10 ;
	
	pal = new Pallet( Pallet::BLACK_ORANGE_YELLOW ) ;
}

UmvView::~UmvView()
{
	hexagons.clear() ;
	delete pal ;
}

void UmvView::setTitle( const QString& )
{
}

void UmvView::loadData( const QString& fileName )
{
	ifstream file( fileName.latin1() ) ;
	
	string type ;
	int i = -1, j = -1 ;
	string value ;
	int lasti = -1, lastj = -1 ;
	
	do{
		file >> type >> i >> j >> value ;
		
		if( i == -1 && j == -1 )
			break ;
		
		if( i>=0 && i<(int)xSize && j>=0 &&j<(int)ySize ){
			
			if( type.compare("Hexagon") == 0 ){
				
				hexagons[ i+xSize*j ].colorValue = QString( value ).toDouble() ;
				
			}
			
			if( type.compare("Label") == 0 ){
				
				if( ( lasti != i ) || ( lastj != j ) ){
					
					TextItem label ;
					label.text = QString( value ) ;
					label.positionIndex.setX( i ) ;
					label.positionIndex.setY( j ) ;
					
					labels.append( label ) ;
				}else{
					QString lastText = labels[ labels.size() - 1 ].text ;
					lastText.append( "," ) ;
					labels[ labels.size() - 1 ].text = lastText.append( QString(value) ) ;
				}
				
				lasti = i ;
				lastj = j ;
				
				i=-1 ;
				j=-1 ;
					
			}
		}else{
			cout << "Warning !!!: index out of bounds (i, j) = ( " << i << ", " << j << " )" << endl ;
		}
		
	}while( !file.eof() ) ;
	
	double hexagonSize1 = double(0.9*width())/double(1.0+2.0*xSize) ;
	double hexagonSize2 = double(0.7*2.0*height())/double(1.0+3.0*ySize) ;
		
// 	hexagonSize = int(0.5*( hexagonSize1 + hexagonSize2 )) ;
// 	hexagonSize = hexagonSize1 ;
	hexagonSize = ( hexagonSize1 < hexagonSize2 ? hexagonSize1 : hexagonSize2 ) ;

	centering( QPoint( geometry().width()/2, geometry().height()/2 ) ) ;
}

void UmvView::buildGrid( uint xSize, uint ySize )
{
	hexagons.clear() ;
	labels.clear() ;
	
	this->xSize = xSize ;
	this->ySize = ySize ;
		
	HexagonData hd ;
	hd.position = QPoint( 0, 0 ) ;
	hd.colorValue = 1.0 ;
	for( uint i=0; i < xSize*ySize; i++ )    hexagons.append( hd ) ;
}

void UmvView::centering( const QPoint& center )
{
// 	bounding.moveCenter( center ) ;
	
	double boxWidth = xSize*(2.0*hexagonSize) + 2.0*hexagonSize ;
	
	double boxHeight ;
// 	if( ((ySize-1)/2)%2 < 1.0e-4 ){
// 		boxHeight = ySize*(0.65*2.0*hexagonSize) + hexagonSize ;
// 		cout << "Entro a 1" << endl ;
// 	}else{
// 		boxHeight = (2.0*ySize-1.0)*( 0.5*hexagonSize ) ;
// 		boxHeight = ySize*(0.7*2.0*hexagonSize) + 3.2*hexagonSize/2.0 ;
		if( hexagonSize%2 == 0 )
			boxHeight = ((ySize-1.0)/2.0+1.0)*(2.0*hexagonSize) + ((ySize-1.0)/2.0)*(2.0*hexagonSize)/2.0 ;
		else
			boxHeight = ((ySize-1.0)/2.0+1.0)*(2.0*hexagonSize) + ((ySize-1.0)/2.0)*(2.0*hexagonSize)/2.0 - 2.0*2.0*hexagonSize ;
// 		cout << "Entro a 2" << endl ;
// 	}
	
	QPoint begin( center.x() - boxWidth/2.0, center.y() - boxHeight/2.0 ) ;
	sortHexagons( begin ) ;
	
	bounding.setX( (*hexagons.begin()).position.x() - hexagonSize ) ;
	bounding.setY( (*hexagons.begin()).position.y() - hexagonSize ) ;
	bounding.setWidth( aprox(boxWidth) ) ;
	bounding.setHeight( aprox(boxHeight) ) ;
}

void UmvView::sortHexagons( const QPoint& beginPos )
{
	uint depth = 0 ;
	bool rightDirection = TRUE ;
	uint index = 0 ;
	int xCurrent = 0 ;
	int yCurrent = 0 ;
	
	for( uint j=0; j < ySize; j++ ){
		for( uint i=0; i < xSize; i++, index++ ){
			
			if( i == xSize-1 ){
				if( rightDirection )
					depth++ ;
				else
					depth-- ;
				
				if( depth == 2 && rightDirection )
					rightDirection = ( rightDirection ? FALSE : TRUE ) ;
				
				if( depth == 0 && !rightDirection )
					rightDirection = ( rightDirection ? FALSE : TRUE ) ;
			}
			
			hexagons[ index ].position.setX( beginPos.x() + xCurrent + hexagonSize ) ;
			hexagons[ index ].position.setY( beginPos.y() + yCurrent + hexagonSize ) ;
			
			xCurrent += uint(2.0*hexagonSize) ;
		}
		
		yCurrent += uint(3.0*hexagonSize/2.0) ;
		
		xCurrent = depth*hexagonSize;

	}
	
}

// void UmvView::contentsMousePressEvent(QMouseEvent* )
// {
// 	cout << "###Error:### UmvView::contentsMousePressEvent(QMouseEvent* e) Not implement !!!!!!" << endl ;
// }
// 
// void UmvView::contentsMouseMoveEvent(QMouseEvent* )
// {
// 	cout << "###Error:### UmvView::contentsMouseMoveEvent(QMouseEvent* e) Not implement !!!!!!" << endl ;
// }
// 
// void UmvView::contentsMouseDoubleClickEvent( QMouseEvent * e )
// {
// 	if( e->button() == Qt::LeftButton ){
// 		QWMatrix m = worldMatrix();
// 		m.reset() ;
// 		setWorldMatrix( m );
// 	}
// 	
// 	canvas()->update() ;
// }
// 
// void UmvView::viewportResizeEvent ( QResizeEvent *e )
// {
// 	canvas()->resize( e->size().width(), e->size().height() ) ;
// 	
// 	int hexagonSize1 = int( double(0.9*canvas()->width())/double(1.0+2.0*xSize) ) ;
// 	int hexagonSize2 = int( double(0.7*2.0*canvas()->height())/double(1.0+3.0*ySize) ) ;
// 	
// // 	hexagonSize = int(0.5*( hexagonSize1 + hexagonSize2 )) ;
// // 	hexagonSize = hexagonSize1 ;
// 	hexagonSize = ( hexagonSize1 < hexagonSize2 ? hexagonSize1 : hexagonSize2 ) ;
// 	
// 	updateBoundingRect() ;
// 	centering( QPoint( canvas()->width()/2, canvas()->height()/2 ) ) ;
// 	
// 	canvas()->update();
// }
// 
// void UmvView::contentsWheelEvent( QWheelEvent* e )
// {
// 	if ( e->delta() > 0 ){
// 		
// 		QWMatrix m = worldMatrix();
// 		m.scale( 0.9, 0.9 );
// 		setWorldMatrix( m );
// 		
// 	}else if ( e->delta() < 0 ){
// 		
// 		QWMatrix m = worldMatrix();
// 		m.scale( 1.11111, 1.11111 );
// 		setWorldMatrix( m );
// 		
// 	}
// 	
// 	setContentsPos( e->pos().x(), e->pos().y() ) ;
// 	center( e->pos().x(), e->pos().y() ) ;
// 	
// // 	canvas()->update();
// // 	update() ;
// }

void UmvView::drawContents ( QPainter* p )
{
	p->setPen( Qt::black ) ;
	p->setBrush( Qt::black ) ;
	p->drawRect( 0, 0, width(), height() ) ;
	
// 	p->setPen( QColor( 101, 144, 154 ) ) ;
// 	p->setBrush( QBrush( QColor( 101, 144, 154 ), Qt::DiagCrossPattern ) ) ;
// 	p->drawRect( bounding ) ;
	
	QPoint center ;
	
	for( int i = 0; i < xSize; i++ ){
		for( int j = 0; j < ySize; j++ ){
			
			p->setPen( pal->getColor( hexagons[i+xSize*j].colorValue ) ) ;
			p->setBrush( pal->getColor( hexagons[i+xSize*j].colorValue ) ) ;
			
			center = QPoint( hexagons[i+xSize*j].position.x(), hexagons[i+xSize*j].position.y() ) ;
			QPointArray pa(6);
			pa[0] = QPoint(center.x()+hexagonSize, center.y()+hexagonSize/2);
			pa[1] = QPoint(center.x(), center.y()+hexagonSize);
			pa[2] = QPoint(center.x()-hexagonSize, center.y()+hexagonSize/2);
			pa[3] = QPoint(center.x()-hexagonSize, center.y()-hexagonSize/2);
			pa[4] = QPoint(center.x(), center.y()-hexagonSize);
			pa[5] = QPoint(center.x()+hexagonSize, center.y()-hexagonSize/2);
			
			p->drawPolygon( pa ) ;
		}
	}
	
	p->setPen( Qt::blue ) ;
	p->setBrush( Qt::blue ) ;
	
	QFont font( "TypeWriter", 7, QFont::Normal ) ;
	p->setFont( font ) ;
	for( QValueList<TextItem>::iterator it = labels.begin(); it != labels.end(); it++ ){
		int x = hexagons[2*(*it).positionIndex.x()+xSize*2*(*it).positionIndex.y()].position.x() ;
		int y = hexagons[2*(*it).positionIndex.x()+xSize*2*(*it).positionIndex.y()].position.y() ;
		
		p->drawEllipse( x-5/2, y-5/2, 5, 5 ) ;
		p->drawText( x+5, y+5, (*it).text, -1, QPainter::LTR ) ;
	}
	
}

void UmvView::resizeEvent( QResizeEvent *e )
{
	double hexagonSize1 = double(0.9*width())/double(1.0+2.0*xSize) ;
	double hexagonSize2 = double(0.7*2.0*height())/double(1.0+3.0*ySize) ;
		
// 	hexagonSize = int(0.5*( hexagonSize1 + hexagonSize2 )) ;
// 	hexagonSize = hexagonSize1 ;
	hexagonSize = ( hexagonSize1 < hexagonSize2 ? hexagonSize1 : hexagonSize2 ) ;
	
	centering( QPoint( geometry().width()/2, geometry().height()/2 ) ) ;
}
