/***************************************************************************
 *   Copyright (C) 2007 by Nestor Aguirre   *
 *   nfaguirrec@unal.edu.co   *
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
#include "outputstream.h"

OutputStream::OutputStream()
{
}

OutputStream::~OutputStream()
{
	if( logs != NULL )
		logs = NULL ;
		
	if( messages != NULL )
		messages = NULL ;
		
	if( outputs != NULL )
		outputs = NULL ;
}

stringstream* OutputStream::logs = NULL ;
stringstream* OutputStream::messages = NULL ;
stringstream* OutputStream::outputs = NULL ;
stringstream& OutputStream::Instance( int number, QTextEdit* textEdit )
{
	switch( number )
	{
		case LOGS:
			if( logs == NULL ){
				logs = new stringstream() ;
			}
			
			textEdit->insert( logs->str() ) ;
			
			return *logs ;
			
		case MESSAGES:
			if( messages == NULL )
				messages = new stringstream() ;
			
			textEdit->insert( messages->str() ) ;
			
			return *messages ;
		
		case OUTPUTS:
			if( outputs == NULL )
				outputs = new stringstream() ;
			
			textEdit->insert( outputs->str() ) ;
			
			return *outputs ;
			
		default:
			if( outputs == NULL )
				outputs = new stringstream() ;
			
			textEdit->insert( outputs->str() ) ;
			
			return *outputs ;
	}
}

