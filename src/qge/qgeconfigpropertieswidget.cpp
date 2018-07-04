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
#include "qgeconfigpropertieswidget.h"

#include <qlineedit.h>
#include <qcheckbox.h>
#include <qcombobox.h>

#include <qgeitem.h>
#include <qgeedge.h>
#include <qgenode.h>
#include <qgeitemtype.h>

QgeConfigPropertiesWidget::QgeConfigPropertiesWidget( QgeItem* qgeitem, QWidget* parent, const char* name ):
FormQgeConfigPropertiesWidget( parent, name ){
	if( qgeitem )
		this->qgeitem = qgeitem ;
	
	readObject() ;
}


QgeConfigPropertiesWidget::~QgeConfigPropertiesWidget(){
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
bool QgeConfigPropertiesWidget::close( bool update ){

	if( update ){
		qgeitem->setSelected( checkBoxSelected->isChecked() ) ;
		
		if( qgeitem->rtti() == QgeItem::Rtti_Edge ){
			((QgeEdge*)qgeitem)->setLabel( lineEditLabel->text() ) ;
			
			((QgeEdge*)qgeitem)->setType( comboBoxType->currentText() ) ;
			((QgeEdge*)qgeitem)->setDisableChangeType( checkBoxChangeType->isChecked() ) ;
			((QgeEdge*)qgeitem)->setEnableShowLabel( checkBoxShowLabel->isChecked() ) ;
		}
			
		if( qgeitem->rtti() == QgeItem::Rtti_Node ){
			((QgeNode*)qgeitem)->setLabel( lineEditLabel->text() ) ;
			((QgeNode*)qgeitem)->setAutomaticLabel( checkBoxAutoLabel->isChecked() ) ;
			((QgeNode*)qgeitem)->setFirst( checkBoxSetFirst->isChecked() ) ;
			((QgeNode*)qgeitem)->setNeighborRange( lineEditNeighborRange->text().toDouble() ) ;
			
			((QgeNode*)qgeitem)->setType( comboBoxType->currentText() ) ;
			((QgeNode*)qgeitem)->setDisableChangeType( checkBoxChangeType->isChecked() ) ;
			((QgeNode*)qgeitem)->setEnableShowLabel( checkBoxShowLabel->isChecked() ) ;
		}
	}
	
	return QWidget::close( FALSE ) ;
}

void QgeConfigPropertiesWidget::setTypes( QStringList list ){
	comboBoxType->insertStringList( list ) ;
	
	if( qgeitem->rtti() == QgeItem::Rtti_Edge )
		comboBoxType->setCurrentText( ((QgeEdge*)qgeitem)->getType().getName() ) ;
		
	if( qgeitem->rtti() == QgeItem::Rtti_Node )
		comboBoxType->setCurrentText( ((QgeNode*)qgeitem)->getType().getName() ) ;
}

/**
 * Carga los atributos del objeto y actauliza los campos asociados
 * en la interface gráfica
 */
void QgeConfigPropertiesWidget::readObject(){

	if( qgeitem->rtti() == QgeItem::Rtti_Edge ){
		lineEditInternalId->setText( QString::number( ((QgeEdge*)qgeitem)->id() ) ) ;
		lineEditExternalId->setText( QString::number( ((QgeEdge*)qgeitem)->getExternalId() )) ;
		lineEditLabel->setText( ((QgeEdge*)qgeitem)->getLabel() ) ;
		checkBoxAutoLabel->setEnabled( FALSE ) ;
		checkBoxSetFirst->setEnabled( FALSE ) ;
		lineEditNeighborRange->setEnabled( FALSE ) ;
		checkBoxChangeType->setChecked( ((QgeEdge*)qgeitem)->isDisableChangeType() ) ;
		checkBoxShowLabel->setChecked( ((QgeEdge*)qgeitem)->isEnableShowLabel() ) ;
	}
		
	if( qgeitem->rtti() == QgeItem::Rtti_Node ){
		lineEditInternalId->setText( QString::number( ((QgeNode*)qgeitem)->id() ) ) ;
		lineEditExternalId->setText( QString::number( ((QgeNode*)qgeitem)->getExternalId() )) ;
		lineEditLabel->setText( ((QgeNode*)qgeitem)->getLabel() ) ;
		checkBoxAutoLabel->setChecked( ((QgeNode*)qgeitem)->isEnableAutomaticLabel() ) ;
		
		lineEditLabel->setEnabled( !((QgeNode*)qgeitem)->isEnableAutomaticLabel() ) ;
		checkBoxSetFirst->setChecked( ((QgeNode*)qgeitem)->isFirst() ) ;
		
		lineEditNeighborRange->setText( QString::number( ((QgeNode*)qgeitem)->getNeighborRange() ) ) ;
		checkBoxChangeType->setChecked( ((QgeNode*)qgeitem)->isDisableChangeType() ) ;
		checkBoxShowLabel->setChecked( ((QgeEdge*)qgeitem)->isEnableShowLabel() ) ;
	}
		
	checkBoxSelected->setChecked( qgeitem->isSelected() ) ;
}



