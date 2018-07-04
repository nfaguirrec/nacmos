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
#ifndef RANDOMTYPEGENERATOR_H
#define RANDOMTYPEGENERATOR_H

#define RAND(x) int( x*double(rand() )/double(2147483647))

#include <qmap.h>
class QgeGraph ;
#include <SwapFile.h>

/**
@author Nestor Aguirre
*/
class RandomTypeGenerator{
	public:
		RandomTypeGenerator( QgeGraph* principalqgegraph );
		~RandomTypeGenerator();
		
		QgeGraph* run( int seed = 0 ) ;
		
	private:
		void generateRandomNodeTypes( int seed = 0 ) ;
	
	private:
		QgeGraph* principalqgegraph ;
};

#endif
