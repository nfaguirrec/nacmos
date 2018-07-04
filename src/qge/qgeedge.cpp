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
#include "qgeedge.h"

#include <math.h>

#include <GTL/edge_data.h>
#include <GTL/graph.h>

#include <iostream>
#include <stdlib.h>
#include <qpainter.h>

using namespace std ;

/**
 * Crea un nuevo objeto QgeEdge con x1 = y1 = x2 = y2 = 0
 * \brief Constructor
 */
QgeEdge::QgeEdge( QCanvas* canvas ):
QgeItemData(), QgeItem(canvas), edge(),
width(3){
	x1 = y1 = x2 = y2 = 0;
	width = 2 ;
	arrowBase = 5 ;
	arrowHeight = 10 ;
	setZ(5) ;
	setColorUnSelected( Qt::black ) ;
	setPen( QPen( getColorUnSelected(), width, Qt::SolidLine) ) ;
}

/**
 * Node que todas las bubclases deben llamar almetodo hide()
 * en su destructor, ya que hide() es necesario para habilitar 
 * el acceso al método areaPoints()
 * \brief Destructor
 */
QgeEdge::~QgeEdge(){
	hide() ;
}

void QgeEdge::copy( QgeEdge* qgeedge ){
	QgeItemData::copy( qgeedge ) ;

	setColorLabel( qgeedge->getColorLabel() ) ;
	setColorSelected( qgeedge->getColorSelected() ) ;
	setColorUnSelected( qgeedge->getColorUnSelected() ) ;
	setWidth( qgeedge->getWidth() ) ;
	setX( qgeedge->x() ) ;
	setY( qgeedge->y() ) ;
}

/**
* Retorna 7 ( QgeItem::Rtti_QgeEdge )
*/
int QgeEdge::rtti() const{
	return QgeItem::Rtti_Edge ;
}

/**
 * Reimplementada de QCanvasPoligonalItem, con la finalidad de asugar
 * que no se realice ninguna acción cuando se llame al metodo move(double x, double y)
 * de la misma. El movimineto de la clase QgeEdge será reducido a si el punto
 * inicial o final son movidos, y se llamarán de manera externa.
 * @param dx 
 * @param dy 
 */
void QgeEdge::moveBy(double, double){
	// nothing
}

void QgeEdge::setEdgeData( edge_data* data ){ set_edge_data( data ) ; }

/**
 * Selecciona el ancho de linea
 * @param width Ancho de linea
 */
void QgeEdge::setWidth( int width ){ 
	this->width = width ; 
	
	pen().setWidth(width) ;
}

void QgeEdge::setPoints( double xa, double ya, double xb, double yb ){
	if ( x1 != xa || x2 != xb || y1 != ya || y2 != yb ) {
		removeFromChunks() ;
		x1 = (int)xa ;
		y1 = (int)ya ;
		x2 = (int)xb ;
		y2 = (int)yb ;
		addToChunks();
	}
}

void QgeEdge::setFromPoint( double xa, double ya ){
	if ( x1 != xa || y1 != ya ) {
		removeFromChunks() ;
		x1 = (int)xa ;
		y1 = (int)ya ;
		addToChunks();
	}
}

void QgeEdge::setToPoint( double xb, double yb ){
	if ( x2 != xb || y2 != yb ) {
		removeFromChunks() ;
		x2 = (int)xb ;
		y2 = (int)yb ;
		addToChunks();
	}
}

int QgeEdge::getWidth(){
	return width ;
}

QPoint QgeEdge::getFromPoint(){
	return QPoint(x1, y1) ;
}

QPoint QgeEdge::getToPoint(){
	return QPoint(x2, y2) ;
}

/**
 * Dibuja la figura (actualmente una linea), centrada en x(), y() usando el painter p.\n \n
 * Reimplementado de QgeItem \n
 */
void QgeEdge::drawShape(QPainter& p){
// 	removeFromChunks() ;
// 	
// 	if( !isSelected() )
// 		p.setPen( QPen( getColorUnSelected(), getWidth() ) ) ;
// 	else
// 		p.setPen( QPen( getColorSelected(), getWidth() ) ) ;
// 		
// 	if( is_hidden() )
// 		p.setPen( QPen( Qt::lightGray, getWidth() ) ) ;
// 		
// 	p.drawLine((int)(x()+x1), (int)(y()+y1), (int)(x()+x2), (int)(y()+y2)) ;
// 	
// 	addToChunks();

	removeFromChunks() ;

	if ( !isSelected() )
		p.setPen( QPen( getColorUnSelected(), width ) ) ;
	else
		p.setPen( QPen( getColorSelected(), width ) ) ;

	if( this->is_hidden() )
		p.setPen( QPen( Qt::lightGray, width ) ) ;

// 	p.setBrush( QBrush( Qt::black, Qt::SolidPattern) ) ;
// 	p.setPen( QPen(Qt::black, 2) ) ;
	
	double deltaX = x2-x1 ;
	double deltaY = y2-y1 ;
	
	int h = arrowHeight ;
	int b = arrowBase ;
	int lineLong = (int)sqrt( pow(deltaX, 2) + pow(deltaY, 2) ) ;
	
	double angle ;
	double xc, yc ;
	if( x2 > x1 && y2 < y1 ){//primer cuadrante
		xc = x1+fabs(deltaX)/2.0 ;
		yc = y1-fabs(deltaY)/2.0 ;
		angle = atan( deltaY/deltaX )*180.0/3.1415926 + 90.0 ;
	}if( x2 < x1 && y2 < y1 ){//segundo cuadrante
		xc = x1-fabs(deltaX)/2.0 ;
		yc = y1-fabs(deltaY)/2.0 ;
		angle = atan( deltaY/deltaX )*180.0/3.1415926 - 90.0 ;
	}if( x2 < x1 && y2 > y1 ){//tercer cuadrante
		xc = x1-fabs(deltaX)/2.0 ;
		yc = y1+fabs(deltaY)/2.0 ;
		angle = atan( deltaY/deltaX )*180.0/3.1415926 - 90.0 ;
	}if( x2 > x1 && y2 > y1 ){//cuarto cuadrante
		xc = x1+fabs(deltaX)/2.0 ;
		yc = y1+fabs(deltaY)/2.0 ;
		angle = atan( deltaY/deltaX )*180.0/3.1415926 + 90.0 ;
	}if( x2 == x1 && y2 > y1 ){
		xc = x1 ;
		yc = y1+fabs(deltaY)/2.0 ;
		angle = 180.0 ;
	}if( x2 == x1 && y2 < y1 ){
		xc = x1 ;
		yc = y1-fabs(deltaY)/2.0 ;
		angle = 0.0 ;
	}if( x2 > x1 && y2 == y1 ){
		xc = x1+fabs(deltaX)/2.0 ;
		yc = y1 ;
		angle = 90.0 ;
	}if( x2 < x1 && y2 == y1 ){
		xc = x1-fabs(deltaX)/2.0 ;
		yc = y1 ;
		angle = -90.0 ;
	}
	
	QWMatrix m;
	m.translate( xc, yc ) ;
	m.rotate( angle );
	
	QPointArray points(3) ;
	points[0] = QPoint( int(-b/2) , int(h/2) ) ;
// 	points[1] = QPoint( 0, 0 ) ;
	points[1] = QPoint( int(b/2) , int(h/2) ) ;
	points[2] = QPoint( 0, int(-h/2) ) ;
	
	points = m.map( points ) ;
	
	if( get_edge_data()->owner->is_directed() )
		p.drawPolygon( points ) ;
	
	QPoint xBegin = QPoint( 0, int(-lineLong/2.0) ) ;
	QPoint xEnd = QPoint( 0, int(lineLong/2.0)) ;
	
	xBegin = m.map( xBegin ) ;
	xEnd = m.map( xEnd ) ;
	p.drawLine( xBegin, xEnd ) ;
	
	addToChunks();
}

/**
 * Esta función debe ser reimplementada en cada subclase. Éste debe retornar
 * las puntos de la frontera (i.e. outside and not touching) de la forma o
 * ocurriran errores al dibujar. \n \n
 * Reimplementado de QgeItem. \n
 * Nota: Éste metodo hay que revisarlo porque genera un bug, lo cual ha hecho
 * necesario agregar la linea "this->canvas()->retune( 500 )", en el 
 * constructor de QgeGraph()
 */
QPointArray QgeEdge::areaPoints() const{
// /*	QPointArray p(4);
// 	int xi = int(x());
// 	int yi = int(y());
// 	int pw = pen().width();
// 	int dx = QABS(x1-x2);
// 	int dy = QABS(y1-y2);
// 	pw = pw*4/3+2; // approx pw*sqrt(2)
// 	int px = x1<x2 ? -pw : pw ;
// 	int py = y1<y2 ? -pw : pw ;
// 	if ( dx && dy && (dx > dy ? (dx*2/dy <= 2) : (dy*2/dx <= 2)) ) {
// 		steep
// 		if ( px == py ) {
// 			p[0] = QPoint(x1+xi   ,y1+yi+py);
// 			p[1] = QPoint(x2+xi-px,y2+yi   );
// 			p[2] = QPoint(x2+xi   ,y2+yi-py);
// 			p[3] = QPoint(x1+xi+px,y1+yi   );
// 		} else {
// 			p[0] = QPoint(x1+xi+px,y1+yi   );
// 			p[1] = QPoint(x2+xi   ,y2+yi-py);
// 			p[2] = QPoint(x2+xi-px,y2+yi   );
// 			p[3] = QPoint(x1+xi   ,y1+yi+py);
// 		}
// 	} else if ( dx > dy ) {
// 		horizontal
// 		p[0] = QPoint(x1+xi+px,y1+yi+py);
// 		p[1] = QPoint(x2+xi-px,y2+yi+py);
// 		p[2] = QPoint(x2+xi-px,y2+yi-py);
// 		p[3] = QPoint(x1+xi+px,y1+yi-py);
// 	} else {
// 		vertical
// 		p[0] = QPoint(x1+xi+px,y1+yi+py);
// 		p[1] = QPoint(x2+xi+px,y2+yi-py);
// 		p[2] = QPoint(x2+xi-px,y2+yi-py);
// 		p[3] = QPoint(x1+xi-px,y1+yi+py);
// 	}
// 	return p;*/

	double deltaX = x2-x1 ;
	double deltaY = y2-y1 ;
	
	int h = arrowHeight ;
	int b = arrowBase ;
	int lineLong = (int)sqrt( pow(deltaX, 2) + pow(deltaY, 2) ) ;
	
	double angle ;
	double xc, yc ;
	if( x2 > x1 && y2 < y1 ){//primer cuadrante
		xc = x1+fabs(deltaX)/2.0 ;
		yc = y1-fabs(deltaY)/2.0 ;
		angle = atan( deltaY/deltaX )*180.0/3.1415926 + 90.0 ;
	}if( x2 < x1 && y2 < y1 ){//segundo cuadrante
		xc = x1-fabs(deltaX)/2.0 ;
		yc = y1-fabs(deltaY)/2.0 ;
		angle = atan( deltaY/deltaX )*180.0/3.1415926 - 90.0 ;
	}if( x2 < x1 && y2 > y1 ){//tercer cuadrante
		xc = x1-fabs(deltaX)/2.0 ;
		yc = y1+fabs(deltaY)/2.0 ;
		angle = atan( deltaY/deltaX )*180.0/3.1415926 - 90.0 ;
	}if( x2 > x1 && y2 > y1 ){//cuarto cuadrante
		xc = x1+fabs(deltaX)/2.0 ;
		yc = y1+fabs(deltaY)/2.0 ;
		angle = atan( deltaY/deltaX )*180.0/3.1415926 + 90.0 ;
	}if( x2 == x1 && y2 > y1 ){
		xc = x1 ;
		yc = y1+fabs(deltaY)/2.0 ;
		angle = 180.0 ;
	}if( x2 == x1 && y2 < y1 ){
		xc = x1 ;
		yc = y1-fabs(deltaY)/2.0 ;
		angle = 0.0 ;
	}if( x2 > x1 && y2 == y1 ){
		xc = x1+fabs(deltaX)/2.0 ;
		yc = y1 ;
		angle = 90.0 ;
	}if( x2 < x1 && y2 == y1 ){
		xc = x1-fabs(deltaX)/2.0 ;
		yc = y1 ;
		angle = -90.0 ;
	}

	int tolerance = 1 ;
	int lineWidth = pen().width() ;
	
	QPointArray points(10) ;
	points[0] = QPoint( int(-lineWidth/2.0-tolerance), int(-lineLong/2.0-tolerance) ) ;
	points[1] = QPoint( int( lineWidth/2.0+tolerance), int(-lineLong/2.0-tolerance) ) ;
	points[2] = QPoint( int( lineWidth/2.0+tolerance), int( -2*h-tolerance) ) ;
	points[3] = QPoint( int( 2*b+lineWidth/2.0+tolerance), int( 2*h+tolerance) ) ;
	points[4] = QPoint( int( lineWidth/2.0+tolerance), int(2*h+tolerance) ) ;
	points[5] = QPoint( int( lineWidth/2.0+tolerance), int( lineLong/2.0+tolerance) ) ;
	points[6] = QPoint( int(-lineWidth/2.0-tolerance), int( lineLong/2.0+tolerance) ) ;
	points[7] = QPoint( int( -lineWidth/2.0-tolerance), int( 2*h+tolerance) ) ;
	points[8] = QPoint( int(-2*b-lineWidth/2.0-tolerance), int(2*h+tolerance) ) ;
	points[9] = QPoint( int(-lineWidth/2.0-tolerance), int( -2*h-tolerance) ) ;
	
	QWMatrix m;
	m.translate( xc, yc ) ;
	m.rotate( angle );
	
	points = m.map( points ) ;
	
// 	poly->setPoints(points) ;
// 	poly->setBrush( QBrush(Qt::red) ) ;
// 	poly->setZ(-10) ;
// 	poly->show() ;

	return points ;
}

