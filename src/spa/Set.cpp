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

/**
 * 
 * @param name  
 */
template< class TYPE >
Set<TYPE>::Set( string name )
{
	this->name = name ;
}

/**
 * 
 * @param name 
 * @param data 
 * @param size  
 */
template< class TYPE >
Set<TYPE>::Set( string name, TYPE* data, int size ){
	this->name = name ;
	
	for( int i=0; i<size; i++ ){
		insert( data[i] ) ;
	}
}

/**
 * 
 * @return 
 */
template< class TYPE >
Set<TYPE>::~Set()
{
}

/**
 * 
 * @return 
 */
template< class TYPE >
string Set<TYPE>::getName() const
{
	return name ;
}

/**
 * 
 * @param name 
 */
template< class TYPE >
void Set<TYPE>::setName( string name )
{
	this->name = name ;
}

/**
 * 
 * @param s 
 * @return 
 */
template< class TYPE >
bool Set<TYPE>::contains( const TYPE& s )
{
	return !( find(s) == this->end() ) ;
}

/**
 * 
 * @param s 
 * @return 
 */
template< class TYPE >
bool Set<TYPE>::isSubsetOf( const Set<TYPE>& s )
{
	if( this->size() < s.size() ){
		for( typename Set<TYPE>::iterator it = this->begin(); it != this->end(); it++ ){
			if( s.find(*it) == s.end() ){
				return false ;
			}
		}
	}else{
		throw SpaException("SetException : The set "+this->getName()+" is less than "+s.getName()) ;
	}
	
	return true ;
}


/**
 * 
 * @param s 
 * @return 
 */
template< class TYPE >
Set<TYPE> Set<TYPE>::unionTo( const Set<TYPE>& s )
{
	Set<TYPE> tmp = *this ;
	
	for( typename Set<TYPE>::iterator it = s.begin(); it != s.end(); it++ ){
		tmp.insert(*it) ;
	}
	
	tmp.setName(this->getName()+"u"+s.getName()) ;
	
// 	set_difference(this->begin(), this->end(), s.begin(), s.end(), t);

	return tmp ;
}

/**
 * 
 * @param s 
 * @return 
 */
template< class TYPE >
Set<TYPE> Set<TYPE>::differenceWith( const Set<TYPE>& s )
{
	Set<string> output ;
	
	for ( Set<string>::iterator it = this->begin(); it != this->end(); it++ ){
		if( s.find( *it ) == s.end() ){
			output.insert( *it );
		}
	}
	
	return output ;
}

/**
 * 
 * @param s 
 * @return 
 */
template< class TYPE >
Set<TYPE> Set<TYPE>::intersectionWith( const Set<TYPE>& s ){
	Set<TYPE> tmp ;	
	
	for( typename  Set<TYPE>::iterator it = s.begin(); it != s.end(); it++ ){
		if( this->find(*it) != this->end() ){
			tmp.insert(*it) ;
		}
	}
	
	tmp.setName(this->getName()+"n"+s.getName()) ;
	return tmp ;
}

/**
 * 
 * @param s 
 * @return 
 */
template< class TYPE >
Set<TYPE> Set<TYPE>::operator + ( const Set<TYPE>& s )
{
	return unionTo( s ) ;
}

/**
 * 
 * @param s 
 * @return 
 */
template< class TYPE >
Set<TYPE> Set<TYPE>::operator - ( const Set<TYPE>& s )
{
	return intersectionWith( s ) ;
}

/**
 * 
 * @param os 
 * @param s 
 * @return 
 */
template< class TYPE >
ostream& operator << ( ostream& os, const Set<TYPE>& s )
{
	os << s.getName() << " = " ;
	os << "{ " ;
	unsigned int i = 0 ;
	for( typename Set<TYPE>::iterator it = s.begin(); it != s.end(); it++, i++ ){
		if( i != (s.size() - 1) )
			os << *it << ", "  ;
		else
			os << *it << "" ;
	}
	os << " }" ;
	
	return os ;
}

/**
 * 
 * @param is 
 * @param s 
 * @return 
 */
template< class TYPE >
istream& operator >> ( istream& is, Set<TYPE>& s )
{
	string tmp ;
	getline( is, tmp ) ;
	
	istringstream stream1;
	stream1.str( tmp );
	
	string data ;
	
	int i = 0 ;
	while( stream1 >> data ) {
		if( i == 0 )
			s.setName( data ) ;
		else
			s.insert( data ) ;
		i++ ;
	}
	
	return is ;
}

