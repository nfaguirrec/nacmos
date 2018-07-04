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
#include "randomtypegenerator.h"

#include <qgegraph.h>
#include <qgenode.h>
#include <qgeedge.h>

RandomTypeGenerator::RandomTypeGenerator( QgeGraph* principalqgegraph ){
	
	if( principalqgegraph )
		this->principalqgegraph = principalqgegraph ;
}

RandomTypeGenerator::~RandomTypeGenerator(){
}

/**
 * Por cada llamada a run(), se almacena en buffer el grafo aleatorio
 * generado
 */
QgeGraph* RandomTypeGenerator::run( int seed ){
	generateRandomNodeTypes( seed ) ;
	
	return principalqgegraph ;
}

void RandomTypeGenerator::generateRandomNodeTypes( int seed ){
	srand( time(NULL)+seed ) ;
	
	QMap<QString, QStringList> labelsHidrogen ;
	QMap<QString, QStringList> labelsPhosphate ;
	
	/*--------------------------------------------------------------
	* A traves de aristas de hidrogeno solo se aceptan los pares
	* G-C , A-U y G-U (probable)
	*/
	labelsHidrogen["Adenine"].append("Uracil") ;
	
	labelsHidrogen["Citosine"].append("Guanine") ;
	
	labelsHidrogen["Guanine"].append("Citosine") ;
	labelsHidrogen["Guanine"].append("Uracil") ;
	
	labelsHidrogen["Uracil"].append("Adenine") ;
	labelsHidrogen["Uracil"].append("Guanine") ;
	//---------------------------------------------------------------

	/*---------------------------------------------------------------
	* El grupo completo de lables
	*/
	QStringList labels ;
	labels.push_back("Adenine") ;
	labels.push_back("Citosine") ;
	labels.push_back("Guanine") ;
	labels.push_back("Uracil") ;
	//---------------------------------------------------------------
	
	principalqgegraph->setProcessed( FALSE ) ;
	
	for( graph::node_iterator it = principalqgegraph->nodes_begin(); it != principalqgegraph->nodes_end(); it++ ){
		if( !principalqgegraph->nodeToQgeNode(*it)->isProcessed() ){
			principalqgegraph->nodeToQgeNode(*it)->setType( principalqgegraph->getNodeTypes()[labels[ RAND(labels.size()) ]] ) ;
			
			if( principalqgegraph->nodeToQgeNode(*it)->getType().getName().compare("Default") != 0 )
				principalqgegraph->nodeToQgeNode(*it)->setLabel( QString(principalqgegraph->nodeToQgeNode(*it)->getType().getName()[0]) ) ;
				
			principalqgegraph->nodeToQgeNode(*it)->setProcessed(TRUE) ;
			
			for( node::inout_edges_iterator it2 = (*it).inout_edges_begin(); it2 != (*it).inout_edges_end(); it2++ ){
				if( !principalqgegraph->nodeToQgeNode( (*it2).opposite(*it))->isProcessed() ){
					QString currentType = principalqgegraph->nodeToQgeNode(*it)->getType().getName() ;
					if( principalqgegraph->edgeToQgeEdge(*it2)->getType().getName().compare("Hydrogen") == 0  || 
						principalqgegraph->edgeToQgeEdge(*it2)->getType().getName().compare("Hydrogen2") == 0 ){
						principalqgegraph->nodeToQgeNode( (*it2).opposite(*it) )->setType(
							principalqgegraph->getNodeTypes()[ 
									labelsHidrogen[currentType][ RAND(labelsHidrogen[currentType].size()) ] 
								]
							) ;
							
						if( principalqgegraph->nodeToQgeNode( (*it2).opposite(*it) )->getType().getName().compare("Default") != 0 )
							principalqgegraph->nodeToQgeNode( (*it2).opposite(*it) )->setLabel( QString( principalqgegraph->nodeToQgeNode( (*it2).opposite(*it) )->getType().getName()[0] ) ) ;
							
						principalqgegraph->nodeToQgeNode((*it2).opposite(*it))->setProcessed(TRUE) ;
					}
				}
			}
		}
	}
	
	principalqgegraph->setProcessed( FALSE ) ;
	principalqgegraph->setLabel("Random "+QString::number(seed) ) ;
}


