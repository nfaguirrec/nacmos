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
#include "qgeconfiggeneralgraphwidget.h"

#include <qlineedit.h>
#include <qcheckbox.h>

#include <qgegraph.h>

QgeConfigGeneralGraphWidget::QgeConfigGeneralGraphWidget( QgeGraph* qgegraph, QWidget* parent ):
FormQgeConfigGeneralGraphWidget( parent ){
	if( qgegraph )
		this->qgegraph = qgegraph ;
		
	readObject() ;
}

QgeConfigGeneralGraphWidget::~QgeConfigGeneralGraphWidget(){
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
bool QgeConfigGeneralGraphWidget::close( bool update ){
	if( update ){
		qgegraph->setLabel( lineEditLabel->text() ) ;
		
		if ( checkBoxMakeDirected->isChecked() )
			qgegraph->makeDirected() ;
		else
			qgegraph->makeUndirected() ;
	}
	
	return QWidget::close( FALSE ) ;
}

/**
 * Carga los atributos del objeto y actauliza los campos asociados
 * en la interface gráfica
 */
void QgeConfigGeneralGraphWidget::readObject(){
	lineEditLabel->setText( qgegraph->getLabel() ) ;
// 	checkBoxSelected->setDown( qgegraph->isSelected() ) ;
	checkBoxMakeDirected->setChecked( qgegraph->is_directed() ) ;
	checkBoxIsCyclic->setChecked( !qgegraph->is_acyclic() ) ;
	checkBoxIsDirected->setChecked( qgegraph->is_directed() ) ;
	checkBoxIsConnected->setChecked( qgegraph->is_connected() ) ;
	lineEditNumberOfNodes->setText( QString::number( qgegraph->number_of_nodes() ) ) ;
	lineEditNumberOfEdges->setText( QString::number( qgegraph->number_of_edges() ) ) ;
}

