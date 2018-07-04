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
#ifndef SPAEXCEPTION_H
#define SPAEXCEPTION_H

#include <string>
using namespace std ;

class SpaException
{
	public:
		SpaException( string message = "### SpaException: Undefined" )
		{
			this->message = message ;
		}

		~SpaException()
		{
		}
		
		void setMessage( string message )
		{
			this->message = message ;
		}

		string getMessage()
		{
			return message ;
		}
		
	private:
		string message ;

};

#endif
