/***************************************************************************
 *   Copyright (C) 2007 by Universidad Nacional de Colombia   *
 *   http://www.unal.edu.co   *
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
#ifndef SELECTINGDATABASE_H
#define SELECTINGDATABASE_H

#include <qbuttongroup.h> 

#include <selectingdatabasebase.h>

/**
	@author Universidad Nacional de Colombia <http://www.unal.edu.co>
*/
class SelectingDatabase : public SelectingDatabaseBase{
	public:
		enum{
			PRINCIPAL,
			SECONDARY,
			BOTH
		};
		
		SelectingDatabase( QWidget * parent = 0, const char * name = 0, bool modal = FALSE, WFlags f = 0 );
    		~SelectingDatabase();
		
		int getSelectedDatabase();
	
	public slots:
    		virtual void help();
		
	private:
		QButtonGroup* box ;
};

#endif
