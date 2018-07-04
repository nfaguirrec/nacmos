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
#ifndef CLUSTERINGWITHPS_H
#define CLUSTERINGWITHPS_H

#include "clusteringwithpsbase.h"

#include <qmap.h>

#include <qwt/qwt_slider.h>

#include <qgegraph.h>
#include <qgewidget.h>
#include <swapfileviewer.h>

#include <Set.h>

#define MAX_DOT 8.14291

/**
@author Nestor Aguirre
*/
class ClusteringWithPS : public ClusteringWithPSBase
{
	private:
		enum{
			EUCLIDEAN_DISTANCE,
			FIRST_DIFFERENCE_DISTANCE,
			ALL_DIFFERENCES_DISTANCE,
		};
		
	public:
		ClusteringWithPS( QWidget* parent = 0, const char* name = 0 );
		~ClusteringWithPS();

		QMap< double, Set< Set<string> > > getEquivalenceClasses() ;
		
		void updateListViewClasses( double currentRadius ) ;
		
		static QgeGraph* getTree( QgeGraph* qgegraph, QgeWidget* qgewidget = 0 ) ;
		static double getEuclideanDistance( QgeGraph* tree1, QgeGraph* tree2 ) ;
		static double getFirstDifferenceDistance( QgeGraph* tree1, QgeGraph* tree2 ) ;
		static double getAllDifferencesDistance( QgeGraph* tree1, QgeGraph* tree2 ) ;
		
	public slots:
		virtual void showResults();
		virtual void run();
		virtual void transform();
		virtual void viewTrees();
    		virtual void setDistance( int currentButton );
    		virtual void radiusChanged( double currentRadius );
		
	private:
		struct dotType{
			int pos ;
			double dotValue ;
		};
		
		static QValueList<int> searchUsingLabel( QgeGraph* tree1, QgeGraph* tree2 ) ;
		static double dot( QgeGraph* tree1, QgeGraph* tree2, int pos ) ;
		
	private:
		QMap< int, QMap<int, double> > distance ;
		int currentButton ;
		QMap< double, Set< Set<string> > > equivalenceClasses ;
		double maxRadius ;
		
		SwapFileViewer* vcsf ;
};

#endif
