/***************************************************************************
*   Copyright (C) 2006 by Nestor Aguirre                                  *
*   nestor@dirac                                                          *
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
#include "PretopologicalMetricSpace.h"

/**
 *  
 */
PretopologicalMetricSpace::PretopologicalMetricSpace():
PretopologicalSpace()
{
}

/**
 * 
 * @param basisSet 
 * @return 
 */
PretopologicalMetricSpace::PretopologicalMetricSpace( vector< Set<string> > basisSet ):
PretopologicalSpace( basisSet )
{
}

/**
 * 
 * @return 
 */
PretopologicalMetricSpace::~PretopologicalMetricSpace()
{
}

/**
 * 
 * @param e 
 * @return 
 */
Set<string> PretopologicalMetricSpace::pseudoclausure( string e )
{
	Set<string> output ;
	Set<string> unitary ;
	unitary.insert(e) ;
	
	for( unsigned int i=0; i<getBasisSet().size(); i++ ){
		if( !unitary.intersectionWith( getBasisSet()[i] ).empty() ){
			output.insert( getBasisSet()[i].getName() ) ;
		}
	}
	
	output.setName("a( "+ e +" )") ;
	return output ;
}


