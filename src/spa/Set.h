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
#ifndef SET_H
#define SET_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>

using namespace std ;

#include <SpaException.h>

template< class TYPE >
class Set : public set<TYPE>
{
	private:
		string name ;
	public:
		Set( string name = "X" ) ;
		Set( string name, TYPE* data, int size ) ;
		~Set() ;
		
		string getName() const ;
		void setName( string name ) ;
		
		bool contains( const TYPE& s ) ;
		bool isSubsetOf( const Set<TYPE>& s ) ;
		
		Set<TYPE> unionTo( const Set<TYPE>& s ) ;
		Set<TYPE> differenceWith( const Set<TYPE>& s ) ;
		Set<TYPE> intersectionWith( const Set<TYPE>& s ) ;
		Set<TYPE> getComplement() ;
		
		Set<TYPE> operator + ( const Set& s ) ;
		Set<TYPE> operator - ( const Set& s ) ;
		
// 		friend ostream& operator << ( ostream& os, const Set<TYPE>& s ) ;
// 		friend istream& operator >> ( istream& is, Set<TYPE>& s ) ;
};

#include <Set.cpp>

#endif
