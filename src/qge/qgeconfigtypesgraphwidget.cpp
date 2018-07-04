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
#include "qgeconfigtypesgraphwidget.h"

#include <qtable.h>
#include <qlineedit.h>
#include <qgegraph.h>
#include <qgeitemtype.h>

QgeConfigTypesGraphWidget::QgeConfigTypesGraphWidget( QgeGraph* qgegraph, QgeItem::QgeRttiValue rtti, QWidget* parent, const char* name ):
FormQgeConfigTypesGraphWidget( parent ){
	if( qgegraph )
		this->qgegraph = qgegraph ;
	
	this->rtti = rtti ;
	
	table->setColumnStretchable( 0, TRUE ) ;
	table->setColumnStretchable( 1, TRUE ) ;
	table->setColumnStretchable( 2, TRUE ) ;
	
	readObject() ;
}

QgeConfigTypesGraphWidget::~QgeConfigTypesGraphWidget(){
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
bool QgeConfigTypesGraphWidget::close( bool update ){
	return QWidget::close( FALSE ) ;
}

/**
 * Carga los atributos del objeto y actauliza los campos asociados
 * en la interface gráfica
 */
void QgeConfigTypesGraphWidget::readObject(){
	updateTable() ;
}

void QgeConfigTypesGraphWidget::datesFromTable( int row ){
	lineEditType->setText( table->text(row, 0) ) ;
	lineEditValue1->setText( table->text(row, 1) ) ;
	lineEditValue2->setText( table->text(row, 2) ) ;
}

void QgeConfigTypesGraphWidget::remove(){
	QString key = table->text( table->currentRow() , 0 ) ;
	
	if( key.compare("Default") != 0 ){
		if( rtti == QgeItem::Rtti_Node )
			qgegraph->getNodeTypes().erase( key ) ;
		if( rtti == QgeItem::Rtti_Edge )
			qgegraph->getEdgeTypes().erase( key ) ;
			
		updateTable() ;
	}
}

void QgeConfigTypesGraphWidget::add(){
	QString type = lineEditType->text() ;
	double value1 = lineEditValue1->text().toDouble() ;
	double value2 = lineEditValue2->text().toDouble() ;
	
	if( rtti == QgeItem::Rtti_Node )
		qgegraph->addNodeType( QgeItemType(type, value1, value2) ) ;
	if( rtti == QgeItem::Rtti_Edge )
		qgegraph->addEdgeType( QgeItemType(type, value1, value2) ) ;
	
	updateTable() ;
}

void QgeConfigTypesGraphWidget::updateTable(){

	for(int i = 0; i< table->numRows(); i++)
		for(int j = 0; j< table->numCols(); j++)
			table->clearCell(i, j) ;
		
	QStringList keys ;
	
	if( rtti == QgeItem::Rtti_Node )
		keys = qgegraph->getNodeTypes().keys() ;
	if( rtti == QgeItem::Rtti_Edge )
		keys = qgegraph->getEdgeTypes().keys() ;

	table->setNumRows( keys.size() ) ;
	
	for ( uint i = 0 ; i < keys.size(); i++){
		table->setText( i, 0, keys[i] )  ;
		
		if( rtti == QgeItem::Rtti_Node ){
			table->setText( i, 1, QString::number( qgegraph->getNodeTypes()[keys[i]].getValues()[0], 'f', 6 ) )  ;
			table->setText( i, 2, QString::number( qgegraph->getNodeTypes()[keys[i]].getValues()[1], 'f', 6 ) )  ;
		}
		if( rtti == QgeItem::Rtti_Edge ){
			table->setText( i, 1, QString::number( qgegraph->getEdgeTypes()[keys[i]].getValues()[0], 'f', 6 ) )  ;
			table->setText( i, 2, QString::number( qgegraph->getEdgeTypes()[keys[i]].getValues()[1], 'f', 6 ) )  ;
		}
	}
}

