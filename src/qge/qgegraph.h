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
#ifndef QGEGRAPH_H
#define QGEGRAPH_H

#include <qptrdict.h>
#include <qcanvas.h>
#include <qobject.h>

#include <GTL/graph.h>
#include <qgeitemdata.h>
#include <QPtrMap.h>

class QgeNode ;
class QgeEdge ;

/**
@author Nestor Aguirre
*/
class QgeGraph : public QgeItemData, public graph {
	public:
		QgeGraph( QCanvas* canvas ) ;
		~QgeGraph() ;
		
		void copy( QgeGraph* qgegraph ) ;
		void move( double x, double y ) ;
		QRect boundingRect() ;
		
		void makeDirected() ;
		void makeUndirected() ;
		
		QgeNode* newNode( double x = 0, double y = 0 ) ;
		QgeEdge* newEdge( QgeNode* from, QgeNode* to ) ;
		
		QgeNode* nodeToQgeNode( node n ) ;
		QgeEdge* edgeToQgeEdge( edge e ) ;
		
		void moveNode( QgeNode* qgenode, double x, double y ) ;
		void hideNode( QgeNode* qgenode ) ;
		void removeNode( QgeNode* qgenode ) ;
		void hideEdge( QgeEdge* qgeedge ) ;
		void removeEdge( QgeEdge* qgeedge ) ;
		
		void setNodeTypes( QMap<QString, QgeItemType> nodeTypes ) ;
		void setEdgeTypes( QMap<QString, QgeItemType> edgeTypes ) ;
		void addNodeType( QgeItemType qgeitemtype ) ;
		void addEdgeType( QgeItemType qgeitemtype ) ;
		QMap<QString, QgeItemType>& getNodeTypes() ;
		QMap<QString, QgeItemType>& getEdgeTypes() ;
		
		void selectAll(bool select) ;
		
		void setProcessed( bool processed ) ;
		void setSelected( bool selected ) ;
		void setDisableChangeType( bool changetype ) ;
		
		bool allNodesIsProcessed() ;
		void resetNodeTypes() ;
		void resetEdgeTypes() ;
		void resetAllTypes() ;
		QCanvas* getCanvas() ;
		
	public slots:
		void remove() ;
		void load( const QString &filename ) ;
		void save( const QString &filename ) ;
		
	private:
		void updateCoordinates() ;
		
	private:
		QPtrMap<node, QgeNode> nodeMap ;
		QPtrMap<edge, QgeEdge> edgeMap ;
	
		QMap<QString, QgeItemType> nodeTypes ;
		QMap<QString, QgeItemType> edgeTypes ;
		QMap<QgeNode*, list<edge> > listNodeHidden ;
		bool movingGraph ;
		
		QCanvas* canvas ;
		double x ;
		double y ;
};

#endif
