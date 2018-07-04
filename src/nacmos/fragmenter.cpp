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
#include "fragmenter.h"

#include <qprogressbar.h>
#include <GTL/dijkstra.h>

#include <qwt/qwt_data.h>

#include <qcanvas.h>


#include <qgegraph.h>
#include <qgeedge.h>
#include <qgenode.h>

#include <math.h>
#include <iostream>
#include <iomanip>
using namespace std ;

Fragmenter::Fragmenter( QgeGraph* qgegraph ){
	if( qgegraph )
		this->qgegraph = qgegraph ;
}

Fragmenter::~Fragmenter(){
	neighbor.clear() ;
	ids.clear() ;
}

void Fragmenter::setGraph( QgeGraph* qgegraph ){
	this->qgegraph = qgegraph ;
	
}

void Fragmenter::clear(){
	neighbor.clear() ;
	ids.clear() ;
}
/**
 * Realiza todo el proceso de extracción de vecindades, calculando el
 * externalId sobre cada nodo del grafo molde inicial, retorna el número
 * de motivos diferentes encontrados
 * @param progressbar pasado por referencia para mostrar el avance del calculo
 * @return numero de motivos encontrados
 */
int Fragmenter::run( QProgressBar* progressbar ){
	
	if( progressbar ){
		progressbar->setTotalSteps( qgegraph->number_of_nodes() ) ;
		progressbar->setProgress( 0 ) ;
	}
	
	list<node> listNode ;
	
	qgegraph->setProcessed( FALSE ) ;
	
	/*--------------------------------------------------------------------------
	* La forma particular de secuenciar los nodos, corresponde a un 
	* comportamiento interno de GTL, de renumerar los nodos y las aristas
	* despues de ocultar y restaurar nodos o aristas
	*/
	int i=1 ;
	for( graph::node_iterator it = qgegraph->nodes_begin() ; !qgegraph->allNodesIsProcessed() ; it++){
		
		if( !qgegraph->nodeToQgeNode(*it)->isProcessed() ){
			
			/*--------------------------------------------------------------------------
			* Se obtiene la lista de nodos que corresponden a la vecindad del nodo en
			* cuestion *it
			*/
			listNode = getListNodeFromNeighbor( qgegraph->nodeToQgeNode(*it) ) ;

			/*--------------------------------------------------------------------------
			* Se establece como procesado el nodo de interes sobre el grafo molde
			* original
			*/
			qgegraph->nodeToQgeNode(*it)->setProcessed(TRUE) ;
			
			/*--------------------------------------------------------------------------
			* Se crea el subgrafo inducido usando la lista de nodos que conforman
			* la vecindad del nodo, es decir que oculta todos los nodos que no
			* estan en la lista y mantiene las aristas asociadas
			*/
			qgegraph->induced_subgraph(listNode) ;
			
			/*--------------------------------------------------------------------------
			* Se establece como seleccionado el nodo en cuestion sobre el grafo molde
			* Nota: No se observa en el qgewidget principal ya que el cambio es muy rápido
			*/
			qgegraph->nodeToQgeNode(*it)->setSelected(TRUE) ;

			/*--------------------------------------------------------------------------
			* Se crea un nuevo grafo inicialmente con canvas nulo, posteriormente
			* se le copian todos los atributos del subgrafo inducido ( en qge cuando )
			* no se copia de un grafo se selecciona el canvas se le adiciona sobre el
			* cual se encuentra el grafo original.
			*/
			QgeGraph* tmp = new QgeGraph( new QCanvas() ) ;
			tmp->copy( qgegraph ) ;
			
			/*--------------------------------------------------------------------------
			* Se obtiene el id y se le asigna al nodo respectivo en el nodo original
			* haciendo uso del atributo "externalId", el cual será visualizado en el 
			* dialogo de propiedades
			*/
			double id_tmp = getSubgraphId(tmp) ;
			qgegraph->nodeToQgeNode(*it)->setExternalId( id_tmp ) ;
			
			/*--------------------------------------------------------------------------
			* Se verifica que el subgrafo obtenido no haya sido conciderado previamente
			* discriminandolo por su id, si ya fue conciderado aumenta en uno el contador
			* que representa la frecuencia de aparición del motivo, si no se inicializa 
			* su respectivo contador a 1, se almacena el motivo en el mapa "neighbor" y
			* y se le selecciona el respectivo ExternalId al motivo asociado
			* original
			*/
			if( ids.contains( id_tmp ) ){
				ids[ id_tmp ] += 1 ;
			}else{
				ids[ id_tmp ] = 1 ;
// 				neighbor[ id_tmp ] = tmp ;
				neighbor.insert( id_tmp, tmp ) ;
				neighbor[ id_tmp ]->setExternalId( id_tmp ) ;
			}
			
			/*--------------------------------------------------------------------------
			* Se establece como no seleccionado el nodo en cuestion sobre el grafo molde
			* Nota: No se observa en el qgewidget principal ya que el cambio es muy rápido
			*/
			qgegraph->nodeToQgeNode(*it)->setSelected(FALSE) ;
			
			/*--------------------------------------------------------------------------
			* Se restaura el grafo molde, es decir los nodos y las aristas que fueron
			* ocultados, se restablecen
			*/
			qgegraph->restore_graph() ;
			
			/*--------------------------------------------------------------------------
			* Finalmente se incrementa el contador y el progreso de la barra de progreso
			*/
			i++ ;
			if( progressbar ){
				progressbar->setProgress( i ) ;
				progressbar->update() ;
			}
			
		}
		
	}
	
	qgegraph->setProcessed( FALSE ) ;
	
	return ids.size() ;
}

QgeGraph* Fragmenter::getNeighbor( QgeNode* qgenode ){
	if( neighbor.getKeys().contains( qgenode->getExternalId() ) ){
		return neighbor[ qgenode->getExternalId() ] ;
	}else{
		cout << "Warning !!! - Fragmenter::getNeighbor( QgeNode* qgenode ) " << endl ;
		cout << "QgeNode not found" << endl ;
		return 0 ;
	}
}

QgeGraph* Fragmenter::find( double id ){
	if( neighbor.getKeys().contains( id ) )
		return neighbor[ id ] ;
	else{
		cout << "Warning !!! - Fragmenter::find( double ) " << endl ;
		cout << "Graph not found" << endl ;
		return 0 ;
	}
}

uint Fragmenter::numberOfSubgraphs(){
	return ids.size() ;
}

QValueList<double> Fragmenter::getListIds(){
	return ids.keys() ;
}

QgeGraph* Fragmenter::getGraph() 
{
	return qgegraph ;
}

list<node> Fragmenter::getListNodeFromNeighbor( QgeNode* n ){
	/*------------------------------------------------------------------
	* Si el grafo inicial es dirigido, éste es convertido a uno no
	* diriguido, por requerimientos del algoritmo
	*/
	bool directed = qgegraph->is_directed () ;
	if( directed )
		qgegraph->make_undirected() ;
	//------------------------------------------------------------------
	
	edge_map<double> w( (graph)*qgegraph );
	for( graph::edge_iterator it = qgegraph->edges_begin() ; it != qgegraph->edges_end(); it++){
		QString type = qgegraph->edgeToQgeEdge(*it)->getType().getName() ;
		w[*it] = qgegraph->getEdgeTypes()[type].getValues()[1] ;
	}
	
	GTL::dijkstra B ;
	B.source( *(node*)n ) ;
	B.weights(w) ;
	
	if ( B.check( *(graph*)qgegraph ) != algorithm::GTL_OK ){
		cout << "check dijkstra in fragmenter FAILED" << endl;
		exit(1);
	}

	if ( B.run( *(graph*)qgegraph ) != algorithm::GTL_OK ){
		cout << "run dijkstra in fragmenter FAILED" << endl;
		exit(1);
	}    
	
	list<node> nodeList ;
	
	for( graph::node_iterator it = qgegraph->nodes_begin() ; it != qgegraph->nodes_end(); it++){
		if( B.distance(*it) >= 0.0 && B.distance(*it) <= n->getNeighborRange() )
			nodeList.push_back( *it ) ;
	}
	
	/*------------------------------------------------------------------
	* Si el grafo inicial era diriguido, éste será reanudado a ése tipo
	*/
	if( directed )
		qgegraph->make_directed() ;
	//------------------------------------------------------------------
	
	return nodeList ;
}

double Fragmenter::getSubgraphId( QgeGraph* qgegraph, bool* ok ){
	if (ok != 0)
		*ok = TRUE ;

	edge_map<double> w( *(graph*)qgegraph );
	for( graph::edge_iterator it = qgegraph->edges_begin() ; it != qgegraph->edges_end(); it++ ){
		QString type = qgegraph->edgeToQgeEdge(*it)->getType().getName() ;
		w[*it] = qgegraph->getEdgeTypes()[type].getValues()[0] ;
	}
	
	QValueList<double> valuesrep ;
	QValueList<double> allvalues ;
	QMap<node, double> locValues ;
	QMap<node, double> selfValues ;
	QMap<node, double> nameValues ;
	
	double value = 0.0 ;
	for( graph::node_iterator it1 = qgegraph->nodes_begin(); it1 != qgegraph->nodes_end(); ++it1){
		/*-----------------------------------------------------------------------------
		* Tercero, se adiciona su corespondiente valor asci, para caracterizar
		* su nombre
		*/
		double namevalue = 0.0 ;
		
		if( qgegraph->nodeToQgeNode(*it1)->getLabel().compare("") != 0 ){
			namevalue = qgegraph->nodeToQgeNode(*it1)->getLabel().at(0).latin1() ;
		}else{
			namevalue = 1.0 ;
		}
		namevalue *= 10.0 ;
		//-----------------------------------------------------------------------------	
	
		/*-----------------------------------------------------------------------------
		* Inicialmente se calcula el valor otorgado segun la posición que ocupa
		* dentro del grafo, como función de la suma de sus distancias al resto
		* de nodos
		*/
		double locvalue = 0.0 ;
		
		for( graph::node_iterator it2 = qgegraph->nodes_begin(); it2 != qgegraph->nodes_end(); ++it2){
			GTL::dijkstra B;
			B.source(*it2);
			B.weights(w);
			
			if ( B.check( *(graph*)qgegraph ) != algorithm::GTL_OK ){
// 				cout << "check dijkstra in getId FAILED" << endl;
				exit(1);
			}
		
			if ( B.run( *(graph*)qgegraph ) != algorithm::GTL_OK ){
// 				cout << "run dijkstra in getId FAILED" << endl;
				exit(1);
			}    
			
			locvalue += B.distance(*it1) ;
		}
		locvalue *= 5000.0*namevalue ;
		//-----------------------------------------------------------------------------
		
		/*-----------------------------------------------------------------------------
		* Segundo, se caracteriza cada nodo con la suma de los valores de las aristas 
		* que inciden en el y la resta de la que salen de el
		*/
		double selfvalue = 0.0 ;
		
		for( node::in_edges_iterator it2 = (*it1).in_edges_begin(); it2 != (*it1).in_edges_end(); ++it2){
			selfvalue += w[*it2] ;
		}
		
		for( node::in_edges_iterator it2 = (*it1).out_edges_begin(); it2 != (*it1).out_edges_end(); ++it2){
			selfvalue -= w[*it2] ;
		}
		selfvalue *= 300.0*namevalue ;
		//-----------------------------------------------------------------------------
		
		/*-----------------------------------------------------------------------------
		* Tercero, se adiciona su corespondiente valor asci, para caracterizar
		* su nombre
		*/
// 		double namevalue = 0.0 ;
// 		
// 		if( qgegraph->nodeToQgeNode(*it1)->getLabel().compare("") != 0 )
// 			namevalue = qgegraph->nodeToQgeNode(*it1)->getLabel().at(0).latin1() ;
// 		namevalue *= 100.0*locvalue ;
		//-----------------------------------------------------------------------------	
		
		/*-----------------------------------------------------------------------------
		* Finalmente se suman las tres contribuciones, teniendo en cuenta que si se 
		* trata del nodo seleccionado se le da arbitrariamente el doble de peso
		*/
		if( qgegraph->nodeToQgeNode(*it1)->isSelected() )
			value += 2.0*(locvalue + selfvalue + namevalue) ;
		else
			value += locvalue + selfvalue + namevalue;
		//-----------------------------------------------------------------------------
		
		/*-----------------------------------------------------------------------------
		* Se almacenan los valores encontrados
		*/
		locValues[*it1] = locvalue ;
		selfValues[*it1] = selfvalue ;
		nameValues[*it1] = namevalue ;
		//-----------------------------------------------------------------------------
		
		/*-----------------------------------------------------------------------------
		* Se almacenan los valores que han sido repetidos para posteriormente
		* reportarlos como errores
		*/
		for(uint i=0; i< allvalues.size(); i++){
			if( fabs(allvalues[i] - (locvalue + selfvalue + namevalue)) < 1e-4 )
				valuesrep.push_back(locvalue + selfvalue + namevalue) ;
		}
		allvalues.push_back(locvalue + selfvalue + namevalue) ;
		//-----------------------------------------------------------------------------
		
	}
	
	
	node nodeSelected = *qgegraph->nodes_begin() ;
	for( graph::node_iterator it1 = qgegraph->nodes_begin(); it1 != qgegraph->nodes_end(); ++it1){
		if( qgegraph->nodeToQgeNode(*it1)->isSelected()){
			nodeSelected = *it1 ;
			break ;
		}
	}
	
	double locvalue = 0.0 ;
	if( qgegraph->number_of_edges() != 0 ){

		double minDist = 100000.0 ;
		for( node::inout_edges_iterator it = nodeSelected.inout_edges_begin(); it != nodeSelected.inout_edges_end(); ++it){
			if(w[*it] < minDist)
				minDist = w[*it] ;
		}
		
		QValueList<node> nodesinmed ;
		for( node::inout_edges_iterator it = nodeSelected.inout_edges_begin(); it != nodeSelected.inout_edges_end(); ++it){
			if( fabs(minDist -w[*it]) < 1e-4 )
				nodesinmed.push_back( (*it).opposite(nodeSelected) ) ;
		}
		
		for( graph::node_iterator it1 = qgegraph->nodes_begin(); it1 != qgegraph->nodes_end(); ++it1){
			if(nodesinmed.contains(*it1) == 0 && *it1 != nodeSelected){
				for( QValueList<node>::iterator it2 = nodesinmed.begin(); it2 != nodesinmed.end(); ++it2){
					GTL::dijkstra B;
					B.source(*it1);
					B.weights(w);
					
					if (B.check( *(graph*)qgegraph ) != algorithm::GTL_OK){
// 						cout << "check dijkstra in getId FAILED" << endl;
						exit(1);
					}
				
					if (B.run( *(graph*)qgegraph ) != algorithm::GTL_OK){
// 						cout << "run dijkstra in getId FAILED" << endl;
						exit(1);
					}    
					
					locvalue += B.distance(*it2) ;
				}
			}
		}
		value += locvalue ;
	}
	
	return value ;
}

/**
 * \internal
 */
QwtArrayData Fragmenter::toQwtArrayData( QMap<double, int> dates ){

	QwtArray<double> x( dates.size() ) ;
	QwtArray<double> y( dates.size() ) ;
	
	QValueList<double> tmp = dates.keys() ;
	
	for( uint i=0; i < dates.size(); i++ ){
		x[i] = tmp[i] ;
		y[i] = (double)dates[ tmp[i] ] ;
	}
	
	return QwtArrayData(x, y) ;
}
