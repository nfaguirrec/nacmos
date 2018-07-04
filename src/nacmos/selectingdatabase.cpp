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
#include "selectingdatabase.h"

#include <qcheckbox.h>

SelectingDatabase::SelectingDatabase( QWidget * parent, const char * name, bool modal, WFlags f ):
SelectingDatabaseBase( parent, name, modal, f )
{
}


SelectingDatabase::~SelectingDatabase()
{
}

int SelectingDatabase::getSelectedDatabase()
{
	if( checkBoxPrincipalDatabase->isChecked() && checkBoxSecondaryDatabase->isChecked() )
		return BOTH;
	
	
	else if( checkBoxPrincipalDatabase->isChecked() )
		return PRINCIPAL;
	
	else if( checkBoxSecondaryDatabase->isChecked() )
		return SECONDARY;
	
	return PRINCIPAL;
}

void SelectingDatabase::help()
{
}



