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
#include "verifingfragmentation.h"

#include <qtable.h>
#include <qvaluelist.h>

#include <qcanvas.h>
#include <qgewidget.h>
#include <qgegraph.h>
#include <qgenode.h>
#include <fragmenter.h>

VerifingFragmentation::VerifingFragmentation( QgeGraph* qgegraph, QWidget* parent, const char* name ):
VerifingFragmentationBase(parent, name)
{
	if( qgegraph ){
		this->qgegraph = qgegraph ;
		fragmenter = new Fragmenter( qgegraph ) ;
	}

	qgeWidget->setCanvas( new QCanvas(198, 198) ) ;
	table->setEnabled( FALSE ) ;
	
	table->setColumnStretchable(0, TRUE) ;
	
	run() ;
}

VerifingFragmentation::~VerifingFragmentation()
{
}

void VerifingFragmentation::showFrecuencyDiagram(){
//  	if( fragmenter ){
// 		fragmenter->showStatictics() ;
// 	}
}

void VerifingFragmentation::run(){
	if( qgegraph ){
// 		progressBar->reset() ;
// 		clear() ;
			
		qgegraph->setSelected( FALSE ) ;
		table->setEnabled( TRUE ) ;
		
// 		table->setNumRows( fragmenter->run( progressBar ) ) ;
		table->setNumRows( fragmenter->run() ) ;
		
		/*-----------------------------------------------------------------------------------------
		* Se coloca en el componente visor de grafos ( qgeWidget ), el grafo vecindad
		* correspondiente al primer nodo del grafo actualmente en edición.
		*/
		qgeWidget->setGraph( fragmenter->find( *fragmenter->getListIds().begin() ) ) ;
		qgeWidget->getGraph()->move(100, 100) ;
		qgeWidget->canvas()->update() ;
		
		/*-----------------------------------------------------------------------------------------
		* Finalmente se actualiza la tabla con los valores encontrados durante el proceso
		*/
		updateTable() ;
	}
}

void VerifingFragmentation::viewGraph( int row ){
	/*------------------------------------------------------------------------------------------
	* Muestra el subgrafo dado leyendo el externalId de la tabla, luego lo busca en el 
	* fragmentador, lo centra en eñ componente ( 100, 100; porque el componente mide 200,200 )
	* y actualiza el canvas
	*/
	double id = table->text( row, 0 ).toDouble() ;
	
	qgeWidget->setGraph( fragmenter->find( id ) ) ;
	qgeWidget->getGraph()->move(100, 100) ;
	qgeWidget->canvas()->update() ;
	
	/*------------------------------------------------------------------------------------------
	* Resalta con la activación de la propiedad selected a los nodos del grafo original
	* que tienen el valor dado de externalId y finalmente actualiza el canvas
	*/
	qgegraph->setSelected( FALSE ) ;
	for( graph::node_iterator it = qgegraph->nodes_begin(); it != qgegraph->nodes_end(); it++ ){
		if( qgegraph->nodeToQgeNode(*it)->getExternalId() == id ){
			qgegraph->nodeToQgeNode(*it)->setSelected( TRUE ) ;
		}
	}
		
	qgegraph->getCanvas()->update() ;
}

void VerifingFragmentation::updateTable(){
	for( int i=0; i < table->numRows(); i++ )
		table->removeRow( i ) ;
		
	table->setNumRows( fragmenter->numberOfSubgraphs() ) ;
		
	QValueList<double> listids = fragmenter->getListIds() ;
	int i=0 ;
	for( QValueList<double>::iterator it = listids.begin(); it != listids.end(); it++, i++ ){
		table->setText( i, 0, QString::number( *it, 'f', 1 ) ) ;
		
		QgeGraph* graphtmp = fragmenter->find(*it) ;
		table->setText( i, 1, QString::number( graphtmp->number_of_nodes() ) ) ;
		table->setText( i, 2, QString::number( graphtmp->number_of_edges() ) ) ;
		table->setText( i, 3, !qgegraph->is_acyclic() ? "TRUE" : "FALSE" ) ;
	}
}


