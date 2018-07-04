/***************************************************************************
 *   Copyright (C) 2005 by Nestor Aguirre                                  *
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
#include "Pallet.h"

#include <pallets.h>
#include <math.h>
#include <iostream>
using namespace std ;

Pallet::Pallet( uint id )
{
	this->id = id ;
}

Pallet::~Pallet()
{
}

QColor Pallet::getColor( double value )
{
	if( value <= 0.0 )
		return QColor( blackOrangeYellow[0] ) ;
	if( value >= 1.0 )
		return QColor( blackOrangeYellow[100] ) ;
	
	return QColor( blackOrangeYellow[ int(floor(value*100)) ] ) ;
}

