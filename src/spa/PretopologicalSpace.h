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
#ifndef PRETOPOLOGICALSPACE_H
#define PRETOPOLOGICALSPACE_H

#include <vector>
using namespace std ;

#include <Set.h>

class PretopologicalSpace
{
	public:
		PretopologicalSpace() ;
		PretopologicalSpace( vector< Set<string> > basisSet ) ;
		virtual ~PretopologicalSpace() ;
		
		vector< Set<string> > getBasisSet() const ;
		void setBasisSet( vector< Set<string> > basisSet ) ;
		
		Set<string> getMinimalClosedSet( string e, bool debug = false ) ;
		Set<string> pseudoclausure( Set<string> s ) ;
		virtual Set<string> pseudoclausure( string e ) = 0 ;
		
	private:
		vector< Set<string> > basisSet ;
};

#endif
