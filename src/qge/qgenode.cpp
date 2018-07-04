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
#include "qgenode.h"

#include <qpainter.h>
#include <qgeitemtype.h>

#include <iostream>
using namespace std ;

/**
 * Crea un nuevo objeto QgeNode con width = 32 y height = 32, 
 * con brush = Qt::red y coordenadas x e y
 * @param x Posición en x
 * @param y Posición en y
 * \brief Constructor
 */
QgeNode::QgeNode( QCanvas* canvas, double x, double y ):
QgeItemData(), QgeItem(canvas), node(),
width(23), height(23){
	
	removeFromChunks() ;
	setX(x) ;
	setY(y) ;
	setZ(10) ;
	addToChunks() ;
	
	automaticLabel = TRUE ;
	automaticResize = TRUE ;
	first = FALSE ;
	
	/*------------------------------------------------------
	* Esta variable hay darle otro valor por defecto
	* el valor actual es solo para satisfacer los 
	* requerimientos de Nams
	*/
	neighborRange = 12.0 ;
}

/**
 * Note que todas las bubclases deben llamar almetodo hide()
 * en su destructor, ya que hide() es necesario para habilitar 
 * el acceso al método areaPoints()
 * \brief Destructor
 */
QgeNode::~QgeNode(){
	hide() ;
}

void QgeNode::copy( QgeNode* qgenode ){
	QgeItemData::copy( qgenode ) ;
	
	setAutomaticResize( qgenode->isEnableAutomaticResize() ) ;
	
	///@TODO Esto deberia estar en el constructor de copia de QgeItem
	setWidth( qgenode->getWidth() ) ;
	setHeight( qgenode->getHeight() ) ;
	/////////////////////////////////////////////
	
	setAutomaticLabel( qgenode->isEnableAutomaticLabel() ) ;
	setColorLabel( qgenode->getColorLabel() ) ;
	setColorSelected( qgenode->getColorSelected() ) ;
	setColorUnSelected( qgenode->getColorUnSelected() ) ;
	setSize( qgenode->getSize().width(), qgenode->getSize().height() ) ;
	setNeighborRange( qgenode->getNeighborRange() ) ;
	qgenode->isFirst() ? setFirst( TRUE ) : setFirst( FALSE ) ;
	move( qgenode->x(), qgenode->y() ) ;
	
	if( qgenode->isSelected() )
		setSelected( TRUE ) ;
	else
		setSelected( FALSE ) ;
}

/**
* Retorna 6 ( QgeItem::Rtti_QgeNode )
*/
int QgeNode::rtti() const {
	return QgeItem::Rtti_Node ;
}

int QgeNode::getWidth(){ return width ; }
int QgeNode::getHeight(){ return height ; }
QSize QgeNode::getSize(){ return QSize(width, height) ; }
double QgeNode::getNeighborRange(){ return neighborRange ; } ;

void QgeNode::setNodeData( node_data* data ){ set_node_data( data ) ; }

void QgeNode::setWidth( int width ){ 
	removeFromChunks() ;
	this->width = width ; 
	addToChunks() ;
}

void QgeNode::setHeight( int height ){ 
	removeFromChunks() ;
	this->height = height ; 
	addToChunks() ;
}

void QgeNode::setSize( int width, int height ){ 
	removeFromChunks() ;
	this->width = width; 
	this->height = height ; 
	addToChunks() ;
}

void QgeNode::setLabel( QString label ){
	QgeItemData::setLabel(label) ;
	
	if( automaticResize ){
		if( label.compare("") != 0 ) {
			removeFromChunks() ;
			width = getLabel().length()*getFont().pointSize() + 10 ;
			height = getFont().pointSize() + 0.1*getLabel().length()*getFont().pointSize() + 10 ;
			addToChunks() ;
		}else{
			removeFromChunks() ;
			width = 23 ;
			height = 23 ;
			addToChunks() ;
		}
	}
}

void QgeNode::setAutomaticResize( bool automaticResize ){
	this->automaticResize = automaticResize ;
}

void QgeNode::setAutomaticLabel( bool automaticLabel ){
	this->automaticLabel = automaticLabel ;
}

void QgeNode::setFirst( bool first ){
	this->first = first ;
}

void QgeNode::setNeighborRange( double neighborRange ){
	this->neighborRange = neighborRange ;
}

bool QgeNode::isEnableAutomaticResize(){
	return automaticResize ;
}

bool QgeNode::isEnableAutomaticLabel(){
	return automaticLabel ;
}

bool QgeNode::isFirst(){
	return first ;
}

/**
 * Dibuja la figura (actualmente un circulo), centrada en x(), y() usando el painter p.\n \n
 * Reimplementado de QgeItem \n
 */
void QgeNode::drawShape(QPainter& p){

	if( automaticLabel ){
		if( getType().getName().compare("Default") != 0 )
			setLabel( QString( getType().getName()[0] ) ) ;
// 		else
// 			setLabel("") ;
	}

// 	p.setPen( NoPen ); // since QRegion(QPointArray) excludes outline :-(  )-:
	p.setPen( QPen( Qt::black, 2 ) ) ;
	
	if( !isSelected() )
		p.setBrush( getColorUnSelected() ) ;
	else
		p.setBrush( getColorSelected() ) ;
		
	if( is_hidden() ) {
		p.setPen( QPen( Qt::lightGray, 2 ) ) ;
		p.setBrush( Qt::lightGray ) ;
	}
		
	p.drawEllipse( int(x()-width/2.0+0.5), int(y()-height/2.0+0.5), width, height );
	
	if( is_hidden() )
		p.setPen( QPen( Qt::darkGray, 2 ) ) ;
	else
		p.setPen( getColorLabel() ) ;
		
	if( isEnableShowLabel() )
		p.drawText( x() - getLabel().length()*getFont().pointSizeFloat()/2.9, y() + getFont().pointSize()/2, getLabel() ) ;
}

/**
 * Esta función debe ser reimplementada en cada subclase. Éste debe retornar
 * las puntos de la frontera (i.e. outside and not touching) de la forma o
 * ocurriran errores al dibujar. \n \n
 * Reimplementado de QgeItem. \n
 */
QPointArray QgeNode::areaPoints() const{

	QPointArray r;
	
	r.makeEllipse(int(x()-width/2.0+0.5)-1, int(y()-height/2.0+0.5)-1, width+3, height+3);
	r.resize(r.size()+1);
	r.setPoint(r.size()-1,int(x()),int(y()));
	return r;
}

