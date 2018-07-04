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
#include "PretopologicalSpace.h"

#include "PretopologicalSpace.h"

/**
 *  
 */
PretopologicalSpace::PretopologicalSpace()
{
}

/**
 * 
 * @param basisSet  
 */
PretopologicalSpace::PretopologicalSpace( vector< Set<string> > basisSet )
{
	this->basisSet = basisSet ;
}

/**
 * 
 * @return 
 */
PretopologicalSpace::~PretopologicalSpace()
{
}

/**
 * 
 * @return 
 */
vector< Set<string> > PretopologicalSpace::getBasisSet() const
{
	return basisSet ;
}

/**
 * 
 * @param basisSet 
 */
void PretopologicalSpace::setBasisSet( vector< Set<string> > basisSet )
{
	this->basisSet = basisSet ;
}

/**
 * 
 * @param e 
 * @param debug 
 * @return 
 */
Set<string> PretopologicalSpace::getMinimalClosedSet( string e, bool debug )
{
	Set<string> unitary ;
	unitary.insert(e) ;
	Set<string> output = pseudoclausure(e) ;
	
	if( debug ){
		cerr << endl ;
		cerr << "Convergence" << endl ;
	}
	
	while( unitary != output ){
		
// 		if( debug )
// 			cerr << output << endl ;
		
		unitary = output;
		output = pseudoclausure( output ) ;
	}
	
	if( debug )
		cerr << endl ;
	
	output.setName("F( "+ e +" )") ;
	return output ;
}

/**
 * 
 * @param s 
 * @return 
 */
Set<string> PretopologicalSpace::pseudoclausure( Set<string> s )
{
	Set<string> output ;
	
	for( Set<string>::iterator it = s.begin(); it != s.end(); it++ ){
		
		for( unsigned int i=0; i<basisSet.size(); i++ ){
			if( basisSet[i].getName().compare(*it) == 0 ){
				output = output.unionTo( pseudoclausure(basisSet[i].getName()) ) ;
				break ;
			}
		}
	}
	
	output.setName("a( "+ s.getName() +" )") ;
	return output ;
}


