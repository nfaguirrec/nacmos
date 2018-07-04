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
#include "qgeconfigweightswidget.h"

#include <qtable.h>
#include <qgeitemdata.h>
#include <qlineedit.h>

QgeConfigWeightsWidget::QgeConfigWeightsWidget( QgeItemData* qgeitemdata, QWidget* parent, const char* name ):
FormQgeConfigWeightsWidget(parent, name) {
	if( qgeitemdata )
		this->qgeitemdata = qgeitemdata ;
		
	table->setColumnStretchable( 0, TRUE ) ;
	table->setColumnStretchable( 1, TRUE ) ;
	
	readObject() ;
}

QgeConfigWeightsWidget::~QgeConfigWeightsWidget(){
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
bool QgeConfigWeightsWidget::close( bool update ){
	return QWidget::close( FALSE ) ;
}

/**
 * Carga los atributos del objeto y actauliza los campos asociados
 * en la interface gráfica
 */
void QgeConfigWeightsWidget::readObject(){
	updateTable() ;
}

void QgeConfigWeightsWidget::datesFromTable( int row ){
	lineEditName->setText( table->text(row, 0) ) ;
	lineEditValue->setText( table->text(row, 1) ) ;
}

void QgeConfigWeightsWidget::remove(){
	QString key = table->text( table->currentRow() , 0 ) ;
	
	qgeitemdata->getWeights().erase( key ) ;
	updateTable() ;
}

void QgeConfigWeightsWidget::add(){
	QString name = lineEditName->text() ;
	double value = lineEditValue->text().toDouble() ;
	
	qgeitemdata->addWeight( name, value ) ;
	updateTable() ;
}

void QgeConfigWeightsWidget::updateTable(){

	for(int i = 0; i< table->numRows(); i++)
		for(int j = 0; j< table->numCols(); j++)
			table->clearCell(i, j) ;
		
	QStringList keys = qgeitemdata->getWeights().keys() ; ;
	table->setNumRows( keys.size() ) ;
	
	for ( uint i = 0 ; i < keys.size(); i++){
		table->setText( i, 0, keys[i] )  ;
		table->setText( i, 1, QString::number( qgeitemdata->getWeights()[keys[i]], 'f', 6 ) )  ;
	}
}

