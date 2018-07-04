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
#include "qgeitem.h"

#include <iostream>

#include <qpainter.h>

using namespace std ;

/**
 * \brief Constructor
 */
QgeItem::QgeItem(QCanvas* canvas):
QCanvasPolygonalItem(canvas){
// 	selected = FALSE ;
	colorSelected = Qt::blue ;
	colorUnSelected = Qt::red ;
	colorLabel = Qt::white ;
	
	setBrush( colorUnSelected ) ;
	
	font = QFont("Times", 12, 5, TRUE) ;
}

/**
 * Note que todas las bubclases deben llamar almetodo hide()
 * en su destructor, ya que hide() es necesario para habilitar 
 * el acceso al método areaPoints()
 * \brief Destructor
 */
QgeItem::~QgeItem(){
	hide();
}

/**
* Returns 0 (QgeItem::Rtti_Item)
*/
int QgeItem::rtti() const {
	return 0 ;
}

// void QgeItem::setSelected( bool selected ){ 
// 	QCanvasPolygonalItem::setSelected( selected ) ;
// 	this->selected = selected ; 
// }

void QgeItem::setColorSelected( QColor colorSelected ){ this->colorSelected = colorSelected ; }
void QgeItem::setColorUnSelected( QColor colorUnSelected ){ this->colorUnSelected = colorUnSelected ; }
void QgeItem::setColorLabel( QColor colorLabel ){ this->colorLabel = colorLabel ; }
void QgeItem::setFont( QFont font ){ this->font = font ; }

// bool QgeItem::isSelected(){ return selected ; }
QColor QgeItem::getColorSelected(){ return colorSelected ; }
QColor QgeItem::getColorUnSelected(){ return colorUnSelected ; }
QColor QgeItem::getColorLabel(){ return colorLabel ; }
QFont QgeItem::getFont(){ return font ; }

/**
 * Dibuja la figura, centrada en x(), y() usando el painter p.\n \n
 * Reimplementado de QCanvasPolygonalItem. \n
 * Reimplementado en QgeNode, QgeEdge and QgeGraph. 
 */
void QgeItem::drawShape(QPainter& ){
}

/**
 * Esta función debe ser reimplementada en cada subclase. Éste debe retornar
 * las puntos de la frontera (i.e. outside and not touching) de la forma o
 * ocurriran errores al dibujar. \n \n
 * Reimplementado de QCanvasPolygonalItem. \n
 * Reimplementado en QgeNode, QgeEdge and QgeGraph. 
 */
QPointArray QgeItem::areaPoints() const{
	return 0 ;
}

/*!
  \internal
  Adds the item to all the chunks it covers.
*/
void QgeItem::addToChunks(){
    if (isVisible() && canvas()) {
	QPointArray pa = chunks();
	for (int i=0; i<(int)pa.count(); i++)
	    canvas()->addItemToChunk(this,pa[i].x(),pa[i].y());
// 	val=(uint)TRUE;
    }
}

/*!
  \internal
  Removes the item from all the chunks it covers.
*/
void QgeItem::removeFromChunks(){
    if (isVisible() && canvas()) {
	QPointArray pa = chunks();
	for (int i=0; i<(int)pa.count(); i++)
	    canvas()->removeItemFromChunk(this,pa[i].x(),pa[i].y());
    }
}

/*!
  \internal
  Returns the chunks covered by the item.
*/
QPointArray QgeItem::chunks() const{
    QPointArray r;
    int n=0;
    QRect br = boundingRect();
    if (isVisible() && canvas()) {
	int chunksize=canvas()->chunkSize();
	br &= QRect(0,0,canvas()->width(),canvas()->height());
	if ( br.isValid() ) {
	    r.resize((br.width()/chunksize+2)*(br.height()/chunksize+2));
	    for (int j=br.top()/chunksize; j<=br.bottom()/chunksize; j++) {
		for (int i=br.left()/chunksize; i<=br.right()/chunksize; i++) {
		    r[n++] = QPoint(i,j);
		}
	    }
	}
    }
    r.resize(n);
    return r;
}
