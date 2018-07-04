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
#include "swapmanager.h"

SwapManager::SwapManager()
{
}

SwapManager::~SwapManager()
{
	if( principal != NULL )
		principal = NULL ;
	
	if( secondary != NULL )
		secondary = NULL ;
		
	if( templates != NULL )
		templates = NULL ;
		
	if( temporary != NULL )
		temporary = NULL ;
}

SwapFile* SwapManager::principal = NULL ;
SwapFile* SwapManager::secondary = NULL ;
SwapFile* SwapManager::templates = NULL ;
SwapFile* SwapManager::temporary = NULL ;
SwapFile* SwapManager::Instance( int number )
{
	switch( number )
	{
		case PRINCIPAL:
			if( principal == NULL )
				principal = new SwapFile("principal.swp", FALSE) ;
			return principal ;
		
		case SECONDARY:
			if( secondary == NULL )
				secondary = new SwapFile("secondary.swp", FALSE) ;
			return secondary ;
			
		case TEMPLATES:
			if( templates == NULL )
				templates = new SwapFile("templates.swp", FALSE) ;
			return templates ;
		
		case TEMPORARY:
			if( temporary == NULL )
				temporary = new SwapFile("temporary.swp", FALSE) ;
			return temporary ;
			
		default:
			return 0 ;
	}
}

