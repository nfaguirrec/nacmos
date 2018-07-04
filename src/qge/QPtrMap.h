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
#ifndef PTRMAP_H
#define PTRMAP_H

#include <qptrvector.h>
#include <qmap.h>

/**
 *  @brief 
 *  @author Néstor Aguirre
 *
 *  <b> Fecha de creación : </b> $DATE
 *
 *  <b> Ultima modificación : </b> $DATE
 *
 *  <b> Historial de modificaciones : </b>
 */
template< class KEY, class TYPE >
class QPtrMap
{
	public:
		QPtrMap();
		~QPtrMap();
		
		void clear() ;
		void insert( const KEY& key, TYPE* type ) ;
		TYPE* operator[] ( const KEY& k ) ;
		TYPE* at( const KEY& key ) ;
		void remove(  const KEY& key ) ;
		uint size() ;
		QMap<KEY, uint> getKeys() const ;
		
	private:
		QMap<KEY, uint> keys ;
		QPtrVector<TYPE> types ;
};

template< class KEY, class TYPE >
QPtrMap<KEY,TYPE>::QPtrMap()
{
	types.setAutoDelete( TRUE ) ;
}

template< class KEY, class TYPE >
QPtrMap<KEY,TYPE>::~QPtrMap()
{
	clear() ;
}

template< class KEY, class TYPE >
void QPtrMap<KEY,TYPE>::clear()
{
	keys.clear() ;
	types.clear() ;
}

template< class KEY, class TYPE >
void QPtrMap<KEY,TYPE>::insert( const KEY& k, TYPE* t )
{
	if( !keys.contains(k) ){
		keys[k] = types.size() ;
		types.resize( keys[k] + 1 ) ;
		types.insert( keys[k], t ) ;
	}
}

template< class KEY, class TYPE >
TYPE* QPtrMap<KEY,TYPE>::operator[] ( const KEY& k )
{
	return types.at( keys[k] ) ;
}

template< class KEY, class TYPE >
TYPE* QPtrMap<KEY,TYPE>::at( const KEY& k )
{
	return types.at( keys[k] ) ;
}

template< class KEY, class TYPE >
void QPtrMap<KEY,TYPE>::remove(  const KEY& k )
{
	types.remove( keys[ k ] ) ;
	keys.remove( k ) ;
}

template< class KEY, class TYPE >
uint QPtrMap<KEY,TYPE>::size()
{
	return keys.size() ;
}

template< class KEY, class TYPE >
QMap<KEY, uint> QPtrMap<KEY,TYPE>::getKeys() const
{
	return keys ;
}

#endif
