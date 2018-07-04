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
#include "qgegraph.h"

#include <fstream>
#include <iomanip>
#include <math.h>

#include <qptrlist.h>
#include <qdatetime.h>

#include <qgenode.h>
#include <qgeedge.h>
#include <qgeitemtype.h>

QgeGraph::QgeGraph( QCanvas* canvas ):
QgeItemData(), graph(),
x(0), y(0){
	this->canvas = canvas ;
	addNodeType( QgeItemType("Default", 1.0, 1.0) ) ;
	addEdgeType( QgeItemType("Default", 1.0, 1.0) ) ;
	movingGraph = FALSE ;
	
}


QgeGraph::~QgeGraph(){
	remove() ;
}

void QgeGraph::copy( QgeGraph* qgegraph ){
	remove() ;
	
	QgeItemData::copy( qgegraph ) ;
	
	qgegraph->is_directed() ? make_directed() : make_undirected() ;
		
	setNodeTypes( qgegraph->getNodeTypes() ) ;
	setEdgeTypes( qgegraph->getEdgeTypes() ) ;
	setExternalId( qgegraph->getExternalId() ) ;

	/*----------------------------------------------------
	* Contiene la informaci� del nodo equivalente
	*/
	QMap<node, QgeNode*> nodemapconvert;    
	
	QValueList<QgeNode*> listnodes ;
	for( graph::node_iterator it = qgegraph->nodes_begin(); it != qgegraph->nodes_end(); ++it )
		listnodes.append( qgegraph->nodeToQgeNode(*it) ) ;
	
	for( QValueList<QgeNode*>::iterator n_it = listnodes.begin(); n_it !=  listnodes.end(); ++n_it ){
		QgeNode* tmp_new = newNode() ;
		tmp_new->copy( *n_it ) ;
		nodemapconvert[ *(node*)(*n_it) ] = tmp_new ;
	}
	
	for( QValueList<QgeNode*>::iterator n_it = listnodes.begin(); n_it != listnodes.end(); ++n_it ){
		for (node::out_edges_iterator e_it = (*n_it)->out_edges_begin(); e_it != (*n_it)->out_edges_end(); ++e_it) {
			QgeEdge* edge_new = newEdge( nodemapconvert[ e_it->source() ], nodemapconvert[ e_it->target() ] ) ;
			edge_new->copy( qgegraph->edgeToQgeEdge(*e_it) ) ;
		}
	}
	
	updateCoordinates() ;
}

void QgeGraph::move(double x, double y){
	movingGraph = TRUE ;
	
	/*---------------------------------------------------------------------------
	* Busca los nodos que no est� ocultos y actualiza su posici�
	*/
	for( graph::node_iterator it = nodes_begin(); it != nodes_end(); ++it){
		double delta_x = nodeMap[*it]->x() - this->x ;
		double delta_y = nodeMap[*it]->y() - this->y ;
		
		moveNode( nodeMap[*it], x+delta_x , y+delta_y ) ;
	}
	//---------------------------------------------------------------------------
	
	/*---------------------------------------------------------------------------
	* Busca los nodos que est� ocultos y actualiza su posici�
	*/
	QValueList<QgeNode*> tmp = listNodeHidden.keys() ;
	for(QValueList<QgeNode*>::iterator it = tmp.begin(); it != tmp.end(); it++){
		double delta_x = (*it)->x() - this->x ;
		double delta_y = (*it)->y() - this->y ;

		moveNode( *it, x+delta_x , y+delta_y ) ;
	}
	//---------------------------------------------------------------------------
	
	movingGraph = FALSE ;
	this->x = x ;
	this->y = y ;

}

QRect QgeGraph::boundingRect() {
	QPoint max = QPoint(-1, -1) ;
	QPoint min = QPoint(123456, 123456) ;
	QPoint maxSizeNode = QPoint(-1, -1) ;
	
	/*---------------------------------------------------------------------------
	* Busca entre nodos que no est� ocultos el mayor valor de x e y
	*/
	for( graph::node_iterator it = nodes_begin(); it != nodes_end(); ++it){
		if( nodeMap[*it]->x() > max.x() )
			max.setX( (int)nodeMap[*it]->x() ) ;
		if( nodeMap[*it]->y() > max.y() )
			max.setY( (int)nodeMap[*it]->y() ) ;
		if( nodeMap[*it]->x() < min.x() )
			min.setX( (int)nodeMap[*it]->x() ) ;
		if( nodeMap[*it]->y() < min.y() )
			min.setY( (int)nodeMap[*it]->y() );
		if( nodeMap[*it]->getWidth() > maxSizeNode.x() )
			maxSizeNode.setX( (int)nodeMap[*it]->getWidth() ) ;
		if( nodeMap[*it]->getHeight() > maxSizeNode.y() )
			maxSizeNode.setY( (int)nodeMap[*it]->getHeight() ) ;
	}
	//---------------------------------------------------------------------------
	
	/*---------------------------------------------------------------------------
	* Busca entre nodos que est� ocultos el mayor valor de x e y
	*/
	QValueList<QgeNode*> tmp = listNodeHidden.keys() ;
	for(QValueList<QgeNode*>::iterator it = tmp.begin(); it != tmp.end(); it++){
		if( (*it)->x() > max.x() )
			max.setX( (int)(*it)->x() ) ;
		if( (*it)->y() > max.y() )
			max.setY( (int)(*it)->y() ) ;
		if( (*it)->x() < min.x() )
			min.setX( (int)(*it)->x() ) ;
		if( (*it)->y() < min.y() )
			min.setY( (int)(*it)->y() );
		if( (*it)->getWidth() > maxSizeNode.x() )
			maxSizeNode.setX( (int)(*it)->getWidth() ) ;
		if( (*it)->getHeight() > maxSizeNode.y() )
			maxSizeNode.setY( (int)(*it)->getHeight() ) ;
	}
	//---------------------------------------------------------------------------
	
	return QRect( min - maxSizeNode/2, max + maxSizeNode/2 ) ;
}

void QgeGraph::makeDirected()
{
	this->make_directed() ;
	canvas->update() ;
}

void QgeGraph::makeUndirected()
{
	this->make_undirected() ;
	canvas->update() ;
}


/**
 * Crea un nevo QgeNode del tipo "Default" con cooredenadas x e y
 * @return Un apuntador al nuevo nodo creado
 */
QgeNode* QgeGraph::newNode( double x, double y ){
	node nGTL = new_node() ;
	QgeNode* n = new QgeNode( canvas, x, y ) ;
	n->setNodeData( nGTL.get_node_data() ) ;
	n->show() ;
	
	nodeMap.insert( nGTL, n ) ;
	updateCoordinates() ;
	
	return n ;
}

/**
 * Oculta el nodo seleccionado
 * @param qgenode Nodo que va a ser ocultado
 */
void QgeGraph::hideNode( QgeNode* qgenode ){
	if( !qgenode->is_hidden() ){
		list< edge > listedge = hide_node( *qgenode ) ;
		canvas->setChanged( qgenode->boundingRect() ) ;
		
		listNodeHidden[ qgenode ] = listedge ;
		
		for( list<edge>::iterator it = listedge.begin(); it != listedge.end(); it++ )
			edgeMap[*it]->setPen( QPen( Qt::lightGray, 3 ) ) ;
	}else{
		restore_node( *qgenode ) ;
		canvas->setChanged( qgenode->boundingRect() ) ;
		
		for( list<edge>::iterator it = listNodeHidden[qgenode].begin() ; it != listNodeHidden[qgenode].end(); it++ ){
			restore_edge(*it) ;
			edgeMap[*it]->setPen( QPen( edgeMap[*it]->getColorUnSelected() , 3 ) ) ;
		}
		
		listNodeHidden.remove( qgenode ) ;
	}
}

/**
 * Crea una nueva arista del tipo "Default" usando como
 * coordenadas iniciales pertenecientes a from y como
 * finales las de to
 * @param from Nodo inicial
 * @param to Nodo de destino
 * @return Un apuntador a la nueva arista creada
 */
QgeEdge* QgeGraph::newEdge( QgeNode* from, QgeNode* to ){
	edge eGTL = new_edge( *from, *to) ;
	QgeEdge* e = new QgeEdge( canvas ) ;
	e->setEdgeData( eGTL.get_edge_data() ) ;
	e->setPoints( from->x(), from->y(), to->x(), to->y() ) ;
	e->show() ;
	
	edgeMap.insert( eGTL, e ) ;
	
	return e ;
}

QgeNode* QgeGraph::nodeToQgeNode( node n ){
	return nodeMap[n] ;
}

QgeEdge* QgeGraph::edgeToQgeEdge( edge e ){
	return edgeMap[e] ;
}

void QgeGraph::moveNode( QgeNode* qgenode, double x, double y ){
	qgenode->move(x, y) ;
	
	/*---------------------------------------------------------------------------
	* Actualiza la posici� de las aristas adjacentes al nodo
	*/
	for( node::out_edges_iterator it = qgenode->out_edges_begin(); it != qgenode->out_edges_end(); ++it )
		edgeMap[*it]->setFromPoint( qgenode->x(), qgenode->y() ) ;

	for( node::in_edges_iterator it = qgenode->in_edges_begin(); it != qgenode->in_edges_end(); ++it )
		edgeMap[*it]->setToPoint( qgenode->x(), qgenode->y() ) ;
	//---------------------------------------------------------------------------
	
	/*---------------------------------------------------------------------------
	* Actualiza la posici� de las aristas adjacentes al nodo que estan 
	* ocultas si el nodo esta oculto
	*/
	QValueList<QgeNode*> tmp = listNodeHidden.keys() ;
	for(QValueList<QgeNode*>::iterator it1 = tmp.begin(); it1 != tmp.end(); it1++){
		for(list<edge>::iterator it = listNodeHidden[*it1].begin(); it != listNodeHidden[*it1].end(); it++){
			edgeMap[*it]->setFromPoint( nodeMap[(*it).source()]->x(), nodeMap[(*it).source()]->y() ) ;
			edgeMap[*it]->setToPoint( nodeMap[(*it).target()]->x(), nodeMap[(*it).target()]->y() ) ;
		}
	}
	//---------------------------------------------------------------------------
	
	if( !movingGraph )
		updateCoordinates() ;
		
}

void QgeGraph::removeNode( QgeNode* qgenode ){
	
	if( !qgenode->is_hidden() ){
		for(node::inout_edges_iterator it = qgenode->inout_edges_begin(); it != qgenode->inout_edges_end(); ++it){
			if( edgeMap[*it] ){// for sanity
// 				delete edgeMap[*it] ;
				edgeMap.remove(*it) ;
			}
		}
	}else{
		for(list<edge>::iterator it = listNodeHidden[qgenode].begin(); it != listNodeHidden[qgenode].end(); it++){
			if( edgeMap[*it] ){// for sanity
// 				delete edgeMap[*it] ;
				edgeMap.remove(*it) ;
			}
		}
		listNodeHidden.remove(qgenode) ;
	}

	del_node( *qgenode ) ;
	nodeMap.remove( *qgenode ) ;
	
// 	if( qgenode )// for sanity
// 		delete qgenode ;
	
}

void QgeGraph::hideEdge( QgeEdge* qgeedge ){
	if( !qgeedge->is_hidden() )
		hide_edge( *qgeedge ) ;
	else
		restore_edge( *qgeedge ) ;
}

void QgeGraph::removeEdge( QgeEdge* qgeedge ){
	del_edge( *qgeedge ) ;
	edgeMap.remove( *qgeedge ) ;
	
// 	if( qgeedge )// for sanity
// 		delete qgeedge ;
	
}

void QgeGraph::setNodeTypes( QMap<QString, QgeItemType> nodeTypes ){ this->nodeTypes = nodeTypes ; }
void QgeGraph::setEdgeTypes( QMap<QString, QgeItemType> edgeTypes ){ this->edgeTypes = edgeTypes ; }
void QgeGraph::addNodeType( QgeItemType qgeitemtype ){ nodeTypes[qgeitemtype.getName()] = qgeitemtype ; }
void QgeGraph::addEdgeType( QgeItemType qgeitemtype ){ edgeTypes[qgeitemtype.getName()] = qgeitemtype ; }
QMap<QString, QgeItemType>& QgeGraph::getNodeTypes(){ return nodeTypes ; }
QMap<QString, QgeItemType>& QgeGraph::getEdgeTypes(){ return edgeTypes ; }

void QgeGraph::selectAll(bool select){

	for( graph::node_iterator it = nodes_begin(); it != nodes_end(); ++it)
			nodeToQgeNode(*it)->setSelected(select) ;
			
	for( graph::edge_iterator it = edges_begin(); it != edges_end(); ++it)
			edgeToQgeEdge(*it)->setSelected(select) ;
			
	canvas->update() ;
}

void QgeGraph::setProcessed( bool processed ){
	for( graph::node_iterator it = nodes_begin(); it != nodes_end(); ++it)
		nodeToQgeNode( *it )->setProcessed( processed ) ;
	
	for( graph::edge_iterator it = edges_begin(); it != edges_end(); ++it )
		edgeToQgeEdge( *it )->setProcessed( processed ) ;
		
	QgeItemData::setProcessed( processed ) ;
}

void QgeGraph::setSelected( bool selected ){
	for( graph::node_iterator it = nodes_begin(); it != nodes_end(); ++it)
		nodeToQgeNode( *it )->setSelected( selected ) ;
	
	for( graph::edge_iterator it = edges_begin(); it != edges_end(); ++it )
		edgeToQgeEdge( *it )->setSelected( selected ) ;
}

void QgeGraph::setDisableChangeType( bool changetype ){
	for( graph::node_iterator it = nodes_begin(); it != nodes_end(); ++it)
		nodeToQgeNode( *it )->setDisableChangeType( changetype ) ;
	
	for( graph::edge_iterator it = edges_begin(); it != edges_end(); ++it )
		edgeToQgeEdge( *it )->setDisableChangeType( changetype ) ;
}

bool QgeGraph::allNodesIsProcessed(){
	for( graph::node_iterator it = nodes_begin(); it != nodes_end(); ++it)
		if ( !nodeToQgeNode(*it)->isProcessed() )
			return FALSE ;
			
	return TRUE ;
}

void QgeGraph::resetNodeTypes(){
	for( graph::node_iterator it = nodes_begin(); it != nodes_end(); ++it){
		nodeToQgeNode(*it)->setType( QgeItemType("Default", 1.0, 1.0) ) ;
	}
}

void QgeGraph::resetEdgeTypes(){
	for( graph::edge_iterator it = edges_begin(); it != edges_end(); ++it){
		edgeToQgeEdge(*it)->setType( QgeItemType("Default", 1.0, 1.0) ) ;
	}
}

void QgeGraph::resetAllTypes(){
	resetNodeTypes() ;
	resetEdgeTypes() ;
}

QCanvas* QgeGraph::getCanvas(){
	return canvas ;
}

void QgeGraph::remove(){
	nodeTypes.clear() ;
	edgeTypes.clear() ;
	addNodeType( QgeItemType("Default", 1.0, 1.0) ) ;
	addEdgeType( QgeItemType("Default", 1.0, 1.0) ) ;
	
	/*---------------------------------------------------------------------------
	* Remueve los nodos que no estan ocultos, la manera pecualiar de realizarlo
	* es debido a que en la GTL, cuando se remueve un nodo, todos los nodos son
	* renumerados automaticamente, de esta manera cuando se remueva el primer 
	* nodo otro ocupar�su lugar antes de la siguiente llamda para remover
	* otro nodo
	*/
	int number = number_of_nodes() ;
	for(int i=0; i< number; i++)
		removeNode( nodeMap[*nodes_begin()] ) ;
	//---------------------------------------------------------------------------
	
	/*---------------------------------------------------------------------------
	* Remueve los nodos que estan ocultos
	*/
	QValueList<QgeNode*> tmp = listNodeHidden.keys() ;
	for(QValueList<QgeNode*>::iterator it = tmp.begin(); it != tmp.end(); it++ ){
		removeNode( *it ) ;
	}
	//---------------------------------------------------------------------------
	
	nodeMap.clear() ;
	edgeMap.clear() ;
	listNodeHidden.clear() ;
	clear() ;
}

void QgeGraph::load( const QString &filename ){
	remove() ;

	struct GML_pair* key_list;
	struct GML_stat* stat=(struct GML_stat*)malloc(sizeof(struct GML_stat));
	stat->key_list = NULL;
	
	FILE* file = fopen (filename.latin1(), "r");

	GML_init ();
	key_list = GML_parser (file, stat, 0);

	if (stat->err.err_num != GML_OK) {
		printf ("An error occured while reading line %d column %d of %s:\n", stat->err.line, stat->err.column, filename.latin1());
		
		switch (stat->err.err_num) {
			case GML_UNEXPECTED:
				printf ("UNEXPECTED CHARACTER");
				break;
			
			case GML_SYNTAX:
				printf ("SYNTAX ERROR"); 
				break;
			
			case GML_PREMATURE_EOF:
				printf ("PREMATURE EOF IN STRING");
				break;
			
			case GML_TOO_MANY_DIGITS:
				printf ("NUMBER WITH TOO MANY DIGITS");
				break;
			
			case GML_OPEN_BRACKET:
				printf ("OPEN BRACKETS LEFT AT EOF");
				break;
			
			case GML_TOO_MANY_BRACKETS:
				printf ("TOO MANY CLOSING BRACKETS");
				break;
			
			default:
				break;
		}
		
		printf ("\n");
		GML_free_list (key_list, stat->key_list);
		
		return ;
	}      
	
	while (key_list) {
		/*-------------------------------------------------------------------------------------
		* Como la GTL, no tiene el m�odo setId(), para la clase node, se generar�un mapa 
		* correspondiente a : el segundo o la clave es el id generado internamente y el 
		* primero o el tipo el obtenido del archivo
		*/
		QMap<int, int> ids ;
		//--------------------------------------------------------------------------------------
			
		if ( QString(key_list->key).compare("graph") == 0){
			GML_pair* list_graph = key_list->value.list ;
			
			while (list_graph) {
				if (QString(list_graph->key).compare("label") == 0)
					setLabel( list_graph->value.str ) ;
				if (QString(list_graph->key).compare("directed") == 0){
					if(list_graph->value.integer != 0)
						make_directed() ;
					else
						make_undirected() ;
				}
				if (QString(list_graph->key).compare("nodeTypes") == 0){
					GML_pair* list_graph_node_types = list_graph->value.list;
					
					while(list_graph_node_types){
						QString nameType = QString(list_graph_node_types->key) ;
						addNodeType( QgeItemType( nameType ) ) ;
						
						GML_pair* list_graph_node_types_list = list_graph_node_types->value.list ;
						while( list_graph_node_types_list ){
							nodeTypes[nameType].addValue( list_graph_node_types_list->value.floating ) ;
							list_graph_node_types_list = list_graph_node_types_list->next ;
						}
						delete list_graph_node_types_list ;
						list_graph_node_types = list_graph_node_types->next ;
					}
					delete list_graph_node_types ;
				}
				if (QString(list_graph->key).compare("edgeTypes") == 0){
					GML_pair* list_graph_edge_types = list_graph->value.list;
					
					while(list_graph_edge_types){
						QString nameType = QString(list_graph_edge_types->key) ;
						addEdgeType( QgeItemType( nameType ) ) ;
						
						GML_pair* list_graph_edge_types_list = list_graph_edge_types->value.list ;
						while( list_graph_edge_types_list ){
							edgeTypes[nameType].addValue( list_graph_edge_types_list->value.floating ) ;
							list_graph_edge_types_list = list_graph_edge_types_list->next ;
						}
						delete list_graph_edge_types_list ;
						list_graph_edge_types = list_graph_edge_types->next ;
					}
					delete list_graph_edge_types ;
				}
				
				if ( QString(list_graph->key).compare("node") == 0){
					QgeNode* node_tmp  = newNode() ;
					GML_pair* list_node = list_graph->value.list;
					
					while (list_node) {
						
						if (QString(list_node->key).compare("id") == 0)
							ids[node_tmp->id()] = list_node->value.integer;
						if (QString(list_node->key).compare("label") == 0)
							node_tmp->setLabel(list_node->value.str) ;
						if (QString(list_node->key).compare("type") == 0)
							node_tmp->setType( nodeTypes[list_node->value.str] ) ;
							
						if (QString(list_node->key).compare("selected") == 0){
							if(list_node->value.integer == 1)
								node_tmp->setSelected(TRUE) ;
							else
								node_tmp->setSelected(FALSE) ;
						}
						
						if (QString(list_node->key).compare("disableChangeType") == 0){
							if(list_node->value.integer == 1)
								node_tmp->setDisableChangeType(TRUE) ;
							else
								node_tmp->setDisableChangeType(FALSE) ;
						}
						
						if (QString(list_node->key).compare("isFirst") == 0){
							if(list_node->value.integer == 1)
								node_tmp->setFirst(TRUE) ;
							else
								node_tmp->setFirst(FALSE) ;
						}
						
						if (QString(list_node->key).compare("neighborRange") == 0)
							node_tmp->setNeighborRange( list_node->value.floating ) ;
							
						if (QString(list_node->key).compare("graphics") == 0){
							GML_pair* list_node_graphics = list_node->value.list;
							
							double x = 0, y = 0 ;
							int w = 0, h = 0 ;
							while(list_node_graphics){
								if (QString(list_node_graphics->key).compare("colorSelected") == 0)
									node_tmp->setColorSelected(list_node_graphics->value.str) ;
								if (QString(list_node_graphics->key).compare("colorUnselected") == 0)
									node_tmp->setColorUnSelected(list_node_graphics->value.str) ;
								if (QString(list_node_graphics->key).compare("colorName") == 0)
									node_tmp->setColorLabel(list_node_graphics->value.str) ;
								
								if (QString(list_node_graphics->key).compare("x") == 0){
									x = list_node_graphics->value.floating ;
									moveNode(node_tmp, x, y) ;
								}
								if (QString(list_node_graphics->key).compare("y") == 0){
									y = list_node_graphics->value.floating ;
									moveNode(node_tmp, x, y) ;
								}
								
								if (QString(list_node_graphics->key).compare("w") == 0){
									w = list_node_graphics->value.integer ;
									node_tmp->setSize( w, h ) ;
								}
								if (QString(list_node_graphics->key).compare("h") == 0){
									h = list_node_graphics->value.integer ;
									node_tmp->setSize( w, h ) ;
								}
									
								list_node_graphics = list_node_graphics->next ;
							}
							
							delete list_node_graphics ;
						}
						
						if (QString(list_node->key).compare("weights") == 0){
							GML_pair* list_node_weights = list_node->value.list;
							
							while(list_node_weights){
								node_tmp->addWeight(list_node_weights->key, list_node_weights->value.floating) ;
								list_node_weights = list_node_weights->next ;
							}
							
							delete list_node_weights ;
						}
		
						list_node = list_node->next;
					}
					
					delete list_node ;
				}
				
				if ( QString(list_graph->key).compare("edge") == 0){
					bool source_found = FALSE ;
					QgeEdge* edge_tmp = newEdge( nodeMap[*nodes_begin()], nodeMap[*nodes_begin()] ) ;
					
					GML_pair* list_edge = list_graph->value.list;
					
					while (list_edge) {
						if (QString(list_edge->key).compare("source") == 0){
							for( graph::node_iterator it = nodes_begin(); it != nodes_end(); ++it){
								if(list_edge->value.integer == ids[(*it).id()]){
									edge_tmp->change_source(*it) ;
									edge_tmp->setFromPoint( nodeMap[*it]->x(), nodeMap[*it]->y() ) ;
									source_found = TRUE ;
									break ;
								}
							}
						}
						if (QString(list_edge->key).compare("target") == 0  && source_found){
							for( graph::node_iterator it = nodes_begin(); it != nodes_end(); ++it){
								if(list_edge->value.integer == ids[(*it).id()]){
									edge_tmp->change_target(*it) ;
									edge_tmp->setToPoint( nodeMap[*it]->x(), nodeMap[*it]->y() ) ;
									source_found = FALSE ;
									break ;
								}
							}
						}
						if (QString(list_edge->key).compare("label") == 0)
							edge_tmp->setLabel(list_edge->value.str) ;
						if (QString(list_edge->key).compare("type") == 0)
							edge_tmp->setType( edgeTypes[list_edge->value.str] ) ;
							
						if (QString(list_edge->key).compare("selected") == 0){
							if(list_edge->value.integer == 1)
								edge_tmp->setSelected(TRUE) ;
							else
								edge_tmp->setSelected(FALSE) ;
						}
						
						if (QString(list_edge->key).compare("disableChangeType") == 0){
							if(list_edge->value.integer == 1)
								edge_tmp->setDisableChangeType(TRUE) ;
							else
								edge_tmp->setDisableChangeType(FALSE) ;
						}
							
						if (QString(list_edge->key).compare("graphics") == 0){
							GML_pair* list_edge_graphics = list_edge->value.list;
							
							while(list_edge_graphics){
								if (QString(list_edge_graphics->key).compare("colorSelected") == 0)
									edge_tmp->setColorSelected(list_edge_graphics->value.str) ;
								if (QString(list_edge_graphics->key).compare("colorUnselected") == 0)
									edge_tmp->setColorUnSelected(list_edge_graphics->value.str) ;
								if (QString(list_edge_graphics->key).compare("colorName") == 0)
									edge_tmp->setColorLabel(list_edge_graphics->value.str) ;
								if (QString(list_edge_graphics->key).compare("w") == 0)
									edge_tmp->setWidth(list_edge_graphics->value.integer) ;
									
								list_edge_graphics = list_edge_graphics->next ;
							}
							
							delete list_edge_graphics ;
						}
						
						if (QString(list_edge->key).compare("weights") == 0){
							GML_pair* list_edge_weights = list_edge->value.list;
							
							while(list_edge_weights){
								edge_tmp->addWeight(list_edge_weights->key, list_edge_weights->value.floating) ;
								list_edge_weights = list_edge_weights->next ;
							}
							
							delete list_edge_weights ;
						}
		
						list_edge = list_edge->next;
					}
					
					delete list_edge ;
				}
				
				list_graph = list_graph->next;
			}
			
			delete list_graph ;
		}
		
		key_list = key_list->next ;
	}
		
	GML_free_list (key_list, stat->key_list);
	updateCoordinates() ;
	
}

void QgeGraph::save( const QString &filename ){
	ofstream file(filename.latin1()) ;    
	if (!file) return ;
	
	file << "############################################" << "\n" ;
	file << "# This file was generated for NAcMoS v0.1" << "\n" ;
	file << "# Author: N�tor Aguirre" << "\n" ;
	file << "# email: nfaguirrec@unal.edu.co" << "\n" ;
	file << "# Date: " << QDate::currentDate().toString() << "\n" ;
	file << "############################################" << "\n" ;
	file << "graph [" << endl;
	file << "label \"" << getLabel() << "\"\n"  ;
	file << "directed " << (is_directed() ? "1" : "0") << "\n"; 
	
	file << "nodeTypes [\n" ;
	file << setiosflags(ios::fixed) ;
	QStringList nodeTypesKey = nodeTypes.keys() ;
	for(QStringList::iterator it = nodeTypesKey.begin(); it != nodeTypesKey.end(); ++it){
		file << "\t" << (*it).latin1() << " [" << "\n" ;
		
		for( uint i=0; i < nodeTypes[*it].getValues().size(); i++ )
			file << "\t\t" << char(97+i) << "  " << setprecision(6) << nodeTypes[*it].getValues()[i] << "\n" ;
		file << "\t]\n" ;
	}
	file << "]\n" ;
	
	file << "edgeTypes [\n" ;
	QStringList edgeTypesKey = edgeTypes.keys() ;
	for(QStringList::iterator it = edgeTypesKey.begin(); it != edgeTypesKey.end(); ++it){
		file << "\t" << (*it).latin1() << " [" << "\n" ;
		
		for( uint i=0; i < edgeTypes[*it].getValues().size(); i++ )
			file << "\t\t" << char(97+i) << "  " << setprecision(6) << edgeTypes[*it].getValues()[i] << "\n" ;
		
		file << "\t]\n" ;
	}
	file << "]\n" ;
	
	for (node_iterator it = nodes_begin(); it != nodes_end(); ++it) {
		file << "node [\n" ;
		file << "\t" << "id " << (*it).id() << "\n";
		
		file << "\t" << "label \"" << nodeMap[*it]->getLabel() << "\"\n" ;
		file << "\t" << "type \"" << nodeMap[*it]->getType().getName() << "\"\n" ;
		file << "\t" << "selected " << ( nodeMap[*it]->isSelected() ? "1" : "0" ) << "\n" ;
		file << "\t" << "neighborRange " << nodeMap[*it]->getNeighborRange() << "\n" ;
		file << "\t" << "disableChangeType " << ( nodeMap[*it]->isDisableChangeType() ? "1" : "0" ) << "\n" ;
		file << "\t" << "isFirst " << ( nodeMap[*it]->isFirst() ? "1" : "0" ) << "\n" ;
		
		file << "\t" << "graphics [\n" ;
		file << "\t\t" << "colorSelected \"" << nodeMap[*it]->getColorSelected().name() << "\"\n" ;
		file << "\t\t" << "colorUnselected \"" << nodeMap[*it]->getColorUnSelected().name() << "\"\n" ;
		file << "\t\t" << "colorName \"" << nodeMap[*it]->getColorLabel().name() << "\"\n" ;
		file << "\t\t" << "x " << setprecision(2) << nodeMap[*it]->x() << "\n" ;
		file << "\t\t" << "y " << setprecision(2) << nodeMap[*it]->y() << "\n" ;
		file << "\t\t" << "w " << nodeMap[*it]->getSize().width() << "\n" ;
		file << "\t\t" << "h " << nodeMap[*it]->getSize().height() << "\n" ;
		file << "\t" << "]\n" ;
		
		file << "\t" << "weights [\n" ;
		QStringList weightsNode = nodeMap[*it]->getWeights().keys() ;
		for(QStringList::iterator w_it = weightsNode.begin(); w_it != weightsNode.end(); ++w_it)
			file << "\t\t" << (*w_it).latin1() << " " << setw(10) << setprecision(8) << nodeMap[*it]->getWeights()[*w_it] << "\n" ;
		file << "\t" << "]\n" ;
		
		file << "]" << endl;
	}
	
	for (edge_iterator it = edges_begin(); it != edges_end(); ++it) {
		file << "edge [\n" ;
		file << "\t" << "source " << (*it).source().id() << "\n" ;
		file << "\t" << "target " << (*it).target().id() << "\n" ;
		
		file << "\t" << "label \"" << edgeMap[*it]->getLabel() << "\"\n" ;
		file << "\t" << "type \"" << edgeMap[*it]->getType().getName() << "\"\n" ;
		file << "\t" << "selected " << (edgeMap[*it]->isSelected() ? "1" : "0") << "\n" ;
		file << "\t" << "disableChangeType " << ( edgeMap[*it]->isDisableChangeType() ? "1" : "0" ) << "\n" ;
		
		file << "\t" << "graphics [\n" ;
		file << "\t\t" << "colorSelected \"" << edgeMap[*it]->getColorSelected().name() << "\"\n" ;
		file << "\t\t" << "colorUnselected \"" << edgeMap[*it]->getColorUnSelected().name() << "\"\n" ;
		file << "\t\t" << "colorName \"" << edgeMap[*it]->getColorLabel().name() << "\"\n" ;
		file << "\t\t" << "w " << edgeMap[*it]->getWidth() << "\n" ;
		file << "\t" << "]\n" ;
		
		file << "\t" << "weights [\n" ;
		QStringList weightsEdge = edgeMap[*it]->getWeights().keys() ;
		for(QStringList::iterator w_it = weightsEdge.begin(); w_it != weightsEdge.end(); ++w_it)
			file << "\t\t"  << (*w_it).latin1() << " " << setw(10) << setprecision(8) << edgeMap[*it]->getWeights()[*w_it] << "\n" ;
		file << "\t" << "]\n" ;
		
		file << "]" << endl;
	}
	
	file << "]" << endl;
}

void QgeGraph::updateCoordinates(){
	double xtmp_min = 1000000 ;
	double xtmp_max = 0.0 ;
	double ytmp_min = 1000000 ;
	double ytmp_max = 0.0 ;
	
	for( graph::node_iterator it = nodes_begin(); it != nodes_end(); ++it){
		
		if(nodeMap[*it]->x() <= xtmp_min)
			xtmp_min = nodeMap[*it]->x() ;
		
		if(nodeMap[*it]->x() > xtmp_max)
			xtmp_max = nodeMap[*it]->x() ;
			
		if(nodeMap[*it]->y() <= ytmp_min)
			ytmp_min = nodeMap[*it]->y() ;
		
		if(nodeMap[*it]->y() > ytmp_max)
			ytmp_max = nodeMap[*it]->y() ;
	}
	
	double xtmp = xtmp_min+fabs(xtmp_max-xtmp_min)/2.0 ;
	double ytmp = ytmp_min+fabs(ytmp_max-ytmp_min)/2.0 ;
	
	this->x = xtmp ;
	this->y = ytmp ;
}

