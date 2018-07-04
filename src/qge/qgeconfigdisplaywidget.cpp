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
#include "qgeconfigdisplaywidget.h"

#include <qspinbox.h>
#include <qcheckbox.h>
#include <styledbutton.h>

#include <qgeitem.h>
#include <qgeedge.h>
#include <qgenode.h>

QgeConfigDisplayWidget::QgeConfigDisplayWidget( QgeItem* qgeitem, QWidget* parent, const char* name ):
FormQgeConfigDisplayWidget(parent, name) {
	if( qgeitem )
		this->qgeitem = qgeitem ;
		
	if( qgeitem->rtti() == QgeItem::Rtti_Node )
		disableWidth() ;
	if( qgeitem->rtti() == QgeItem::Rtti_Edge )
		disableXY() ;
		
	readObject() ;
}

QgeConfigDisplayWidget::~QgeConfigDisplayWidget(){
}

/**
 * Actualiza los atributos del objeto usando los valores
 * asociados en la interface gráfica, es reimplementado
 * de la clase QWidget.\n \n
 * Warning !! \n
 *     Cuando la reimplemente en una clase hija, posterior
 *     a su código adicione return QWidget::close( FALSE ), a no ser
 *     que desee activar el flag \ref WDestructiveClose
 */
bool QgeConfigDisplayWidget::close( bool update ){

	if( update ){
		if( qgeitem->rtti() == QgeItem::Rtti_Edge )
			((QgeEdge*)qgeitem)->setWidth( spinBoxWidth->value() ) ;
			
		if( qgeitem->rtti() == QgeItem::Rtti_Node ){
			((QgeNode*)qgeitem)->setSize( spinBoxYSize->value(), spinBoxXSize->value() ) ;
			((QgeNode*)qgeitem)->setAutomaticResize( checkBoxAutoResize->isChecked() ) ;
		}
	
		qgeitem->setColorSelected( styledButtonSelected->color() ) ;
		qgeitem->setColorUnSelected( styledButtonUnSelected->color() ) ;
		qgeitem->setColorLabel( styledButtonLabel->color() ) ;
	}
	
	return QWidget::close( FALSE ) ;
}

/**
 * Carga los atributos del objeto y actauliza los campos asociados
 * en la interface gráfica
 */
void QgeConfigDisplayWidget::readObject(){
	
	if( qgeitem->rtti() == QgeItem::Rtti_Edge )
		spinBoxWidth->setValue( ((QgeEdge*)qgeitem)->getWidth() ) ;
		
	if( qgeitem->rtti() == QgeItem::Rtti_Node ){
		spinBoxXSize->setValue( ((QgeNode*)qgeitem)->getSize().height() ) ;
		spinBoxYSize->setValue( ((QgeNode*)qgeitem)->getSize().width() ) ;
		checkBoxAutoResize->setChecked( ((QgeNode*)qgeitem)->isEnableAutomaticResize() ) ;
	}
	
	styledButtonSelected->setColor( qgeitem->getColorSelected() ) ;
	styledButtonUnSelected->setColor( qgeitem->getColorUnSelected() ) ;
	styledButtonLabel->setColor( qgeitem->getColorLabel() ) ;
}

void QgeConfigDisplayWidget::disableWidth(){
	spinBoxWidth->setEnabled( FALSE )  ;
}

void QgeConfigDisplayWidget::disableXY(){
	spinBoxXSize->setEnabled( FALSE )  ;
	spinBoxYSize->setEnabled( FALSE )  ;
	checkBoxAutoResize->setEnabled( FALSE ) ;
}





