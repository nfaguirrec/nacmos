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
#include "SomPak.h"

SomPak::SomPak()
{
}

SomPak::~SomPak()
{
}

void SomPak::setNeighborhoodFunction( NeighborhoodFunction neighborhoodFunction )
{
	this->neighborhoodFunction = neighborhoodFunction ;
}

void SomPak::setInputDataFile( QString inputDataFile )
{
	this->inputDataFile = inputDataFile ;
}

void SomPak::setOutputDataFile( QString outputDataFile )
{
	this->outputDataFile = outputDataFile ;
}

void SomPak::setInputVectorDataFile( QString inputVectorDataFile )
{
	this->inputVectorDataFile = inputVectorDataFile ;
}

void SomPak::setOutputVectorDataFile( QString outputVectorDataFile )
{
	this->outputVectorDataFile = outputVectorDataFile ;
}

void SomPak::setTrainingRunningLength( int trainingRunningLength )
{
	this->trainingRunningLength = trainingRunningLength ;
}

void SomPak::setInitialLearningRate( double initialLearningRate )
{
	this->initialLearningRate = initialLearningRate ;
}

void SomPak::setInitialTrainingRadius( int initialLearningRate )
{
	this->initialLearningRate = initialLearningRate ;
}

void SomPak::setXSize( int xSize )
{
	this->xSize = xSize ;
}

void SomPak::setYSize( int ySize )
{
	this->ySize = ySize ;
}

void SomPak::setTopology( Topology topology )
{
	this->topology = topology ;
}

void SomPak::initMap()
{
	QString command = ("som_pak/umat -cin ") + inputVectorDataFile +
			  (" -cout ") + outputVectorDataFile +
			  (" > tmp/salida.ps") ;
	
	cout << "Executing " << command << " ... " ;
	system( command.latin1() ) ;
	cout << "OK" << endl ;
}

double SomPak::trainMap()
{
	QString command = ( "som_pak/vsom -din ") + inputDataFile + 
			  (" -cin ") + inputVectorDataFile +
			  (" -cout ")+ outputVectorDataFile +
			  ( " -rlen " ) + QString::number( trainingRunningLength ) +
			  ( " -alpha " ) + QString::number( initialLearningRate ) +
			  ( " -radius " ) + QString::number( initialTrainingRadius ) ;
	
	cout << "Executing " << command << " ... " ;
	system( command.latin1() ) ;
	cout << "OK" << endl ;
}

