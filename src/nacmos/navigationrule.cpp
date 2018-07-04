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
#include "navigationrule.h"

#include <qstring.h>
#include <qtextstream.h>

#include <qgegraph.h>
#include <qgenode.h>
#include <qgeedge.h>

#include <iostream>
using namespace std ;

NavigationRule::NavigationRule(  QgeGraph* qgegraph, QString edgeType ){
	if( qgegraph )
		this->qgegraph = qgegraph ;
	
	this->edgeType = edgeType ;
}

NavigationRule::~NavigationRule(){
}

QPtrList<QgeNode> NavigationRule::getSecuence( QgeGraph* qgegraph, QString edgeType ){
// QValueList<QgeNode*> NavigationRule::getSecuence( QgeGraph* qgegraph, QString edgeType ){	   
// 	QValueList<QgeNode*> tmplist ;
	QPtrList<QgeNode> tmplist ;
	
	/*------------------------------------------------------------------------------------------------
	* Primero se busca el nodo que el usuario declaro como first
	*/
	node first ;
	for( graph::node_iterator it = qgegraph->nodes_begin(); it != qgegraph->nodes_end() ; it++ ){
		if( qgegraph->nodeToQgeNode(*it)->isFirst() )
			first = *it ;
	}
	
	tmplist.append( qgegraph->nodeToQgeNode(first) ) ;
	
	qgegraph->setProcessed(FALSE) ;
	
	node tmp = first ;
	qgegraph->nodeToQgeNode( tmp )->setProcessed( TRUE ) ;

	do{
		for( node::out_edges_iterator it = tmp.out_edges_begin(); it != tmp.out_edges_end(); it++){
			if( qgegraph->edgeToQgeEdge(*it)->getType().getName().contains(edgeType) != 0 ){
				tmplist.append( qgegraph->nodeToQgeNode( (*it).opposite( tmp ) ) ) ;
				qgegraph->nodeToQgeNode( (*it).opposite( tmp ) )->setProcessed( TRUE ) ;
				tmp = (*it).opposite( tmp ) ;
				break ;
			}
		}
	}while( !qgegraph->allNodesIsProcessed() ) ;
	
	qgegraph->setProcessed(FALSE) ;
	
// 	for( QValueList<QgeNode*>::iterator it = tmplist.begin(); it != tmplist.end(); it++ )
// 		cout << (*it)->getLabel() ;
// 	cout << endl ;
	
	return tmplist ;
}

void NavigationRule::loadSecuence( QgeGraph* qgegraph, const QString& fileName, QString edgeType ){
	QPtrList<QgeNode> nodelist = getSecuence( qgegraph, edgeType ) ;
	
	QMap<QString, QString> typesMap ;
	typesMap["A"] = "Adenine" ;
	typesMap["C"] = "Citosine" ;
	typesMap["G"] = "Guanine" ;
	typesMap["U"] = "Uracil" ;
	
	if( fileName != QString::null ){
		cout << "Loading Secuence from " << fileName << " ... " ;
		
		QFile file( fileName );
		if ( file.open( IO_ReadOnly ) ) {
			
			QTextStream streamIn( &file ) ;
			
			QString nameSecuence ;
			QString secuence ;
			
			nameSecuence = streamIn.readLine() ;
			secuence = streamIn.readLine() ;
				
			file.close();
			
			nameSecuence = nameSecuence.remove(">").stripWhiteSpace() ;
			secuence = secuence.stripWhiteSpace() ;
			
			/*-----------------------------------------------------------------
			* Una vez cargados el nombre y la secuencia se pondera el grafo
			*/
			qgegraph->setLabel( nameSecuence ) ;
			
			int i = 0 ;
			for( QPtrList<QgeNode>::iterator it = nodelist.begin(); it != nodelist.end(); it++, i++ ){
				(*it)->setType( typesMap[ QString(secuence[i]) ] ) ;
			}
		}    
		
		cout << "OK" << endl ;
	}

}

/**
 * Carga las secuencias de un archivo, manteniendo el apuntador a �te abierto
 * @param file 
 * @param qgegraph 
 * @param edgeType 
 * @return TRUE si se ha llegado al bit EOF, FALSE de otra manera
 */
bool NavigationRule::loadSecuence( QFile& file, QgeGraph* qgegraph, QString edgeType )
{
	QPtrList<QgeNode> nodelist = getSecuence( qgegraph, edgeType ) ;
		
	QMap<QString, QString> typesMap ;
	typesMap["A"] = "Adenine" ;
	typesMap["C"] = "Citosine" ;
	typesMap["G"] = "Guanine" ;
	typesMap["U"] = "Uracil" ;
		
	cout << "Loading Secuence from " << file.name() << " ... " ;
	
	if ( file.isOpen() ) {
		
		QTextStream streamIn( &file ) ;
		
		QString nameSecuence ;
		QString secuence ;
		
		nameSecuence = streamIn.readLine() ;
		secuence = streamIn.readLine() ;
			
		nameSecuence = nameSecuence.remove(">").stripWhiteSpace() ;
		secuence = secuence.stripWhiteSpace() ;
		
		/*-----------------------------------------------------------------
		* Una vez cargados el nombre y la secuencia se pondera el grafo
		*/
		qgegraph->setLabel( nameSecuence ) ;
		
		int i = 0 ;
		for( QPtrList<QgeNode>::iterator it = nodelist.begin(); it != nodelist.end(); it++, i++ ){
			(*it)->setType( typesMap[ QString(secuence[i]) ] ) ;
		}
	}    
	
	cout << "OK" << endl ;
	
	return file.atEnd() ;
}

void NavigationRule::saveSecuence( QgeGraph* qgegraph, const QString& fileName, QString edgeType ){
	QPtrList<QgeNode> nodelist = getSecuence( qgegraph, edgeType ) ;
	
	QMap<QString, QString> typesMap ;
	typesMap["Adenine"] = "A" ;
	typesMap["Citosine"] = "C" ;
	typesMap["Guanine"] = "G" ;
	typesMap["Uracil"] = "U" ;
	
	if( fileName != QString::null ){
		cout << "Saving Secuence to " << fileName << " ... " ;
		
		QFile file( fileName );
		if ( file.open( IO_WriteOnly ) ) {
			
			QTextStream streamOut( &file ) ;
			
			streamOut << "> " << qgegraph->getLabel() << "\n" ;
			
			for( QPtrList<QgeNode>::iterator it = nodelist.begin(); it != nodelist.end(); it++ ){
				streamOut << typesMap[ (*it)->getType().getName() ] ;
			}
			
			streamOut << "\n" ;
			
			file.close();
		}    
		
		cout << "OK" << endl ;
	}
}

