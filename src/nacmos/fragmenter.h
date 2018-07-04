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
#ifndef FRAGMENTER_H
#define FRAGMENTER_H

#include <qvaluelist.h>
#include <QPtrMap.h>
#include <GTL/node.h>

class QgeGraph ;
class QgeNode ;
class QProgressBar ;
class QCanvas ;
class QwtArrayData ;
class QwtPlotZoomer ;

/**
@author Nestor Aguirre
*/
class Fragmenter{
	public:
		Fragmenter( QgeGraph* qgegraph ) ;
		~Fragmenter() ;
		
		void setGraph( QgeGraph* qgegraph ) ;
		int run( QProgressBar* progressbar = 0 ) ;
		void clear() ;
		QgeGraph* getNeighbor( QgeNode* qgenode ) ;
		
		QgeGraph* find( double id ) ;
		uint numberOfSubgraphs() ;
		QValueList<double> getListIds() ;
		QgeGraph* getGraph() ;
		
		static double getSubgraphId( QgeGraph* qgegraph, bool* ok = 0 ) ;
		static QwtArrayData toQwtArrayData( QMap<double, int> dates) ;
		
	private:
		list<node> getListNodeFromNeighbor( QgeNode* n ) ;
		
	private:
		QgeGraph* qgegraph ;
		QPtrMap< double, QgeGraph> neighbor ;
		
		//----------------------------------------------------------------------
		// Esto es para contar coantas veces aparece cada id, el key<double>
		// es el valor del id, y el type<int> es el numero de veces que
		// aparece el valor durante la fragmentación
		QMap<double, int> ids ;
		//----------------------------------------------------------------------
};

#endif
