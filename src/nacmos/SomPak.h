/***************************************************************************
 *   Copyright (C) 2007 by Universidad Nacional de Colombia   *
 *   http://www.unal.edu.co   *
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
#ifndef SOMPAK_H
#define SOMPAK_H

#include <iostream>
using namespace std ;

#include <qstring.h>

/**
	@author Universidad Nacional de Colombia <http://www.unal.edu.co>
*/
class SomPak{
	public:
		SomPak();
		~SomPak();
		
		enum NeighborhoodFunction{
			BUBBLE,
			GAUSSIAN
		} ;
		
		enum Topology{
			HEXAGONAL,
			RECTANGULAR
		} ;
		
		void setNeighborhoodFunction( NeighborhoodFunction neighborhoodFunction ) ;
		void setInputDataFile( QString inputDataFile ) ;
		void setOutputDataFile( QString outputDataFile ) ;
		void setInputVectorDataFile( QString inputVectorDataFile ) ;
		void setOutputVectorDataFile( QString outputVectorDataFile ) ;
		void setTrainingRunningLength( int trainingRunningLength ) ;
		void setInitialLearningRate( double initialLearningRate ) ;
		void setInitialTrainingRadius( int initialTrainingRadius ) ;
		void setXSize( int xSize ) ;
		void setYSize( int ySize ) ;
		void setTopology( Topology topology ) ;
		
		void initMap() ;
		double trainMap() ;
		
	private:
		QString neighborhoodFunction ;
		QString inputDataFile ;
		QString outputDataFile ;
		QString inputVectorDataFile ;
		QString outputVectorDataFile ;
		int trainingRunningLength ;
		double initialLearningRate ;
		int initialTrainingRadius ;
		int xSize ;
		int ySize ;
		Topology topology ;
};

#endif
