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
#include "editinputvalues.h"

#include <qlistbox.h>

EditInputValues::EditInputValues( QWidget* parent , const char* name ):
FormEditInputValues( parent, name ){
}

EditInputValues::~EditInputValues(){
}

void EditInputValues::setAvailableValues( QStringList availablevalues ){
	listBoxAvailable->insertStringList( availablevalues ) ;
}

QStringList EditInputValues::getValues(){
	uint num = listBoxSelected->count() ;
	
	QStringList tmp ;
	
	for( uint i=0; i<num; i++ )
		tmp.append( listBoxSelected->text(i) ) ;
	
	return tmp ;
}

QStringList EditInputValues::getInputValues( QStringList availablevalues ){
	EditInputValues* editinputvalues = new EditInputValues() ;
	editinputvalues->setAvailableValues( availablevalues ) ;
	
	if ( editinputvalues->exec() == QDialog::Accepted ){
		return editinputvalues->getValues() ;
	}else{
		return QStringList() ;
	}
	
}

void EditInputValues::remove(){
	listBoxSelected->removeItem( listBoxSelected->currentItem() ) ;
}

void EditInputValues::add(){
	uint num = listBoxSelected->count() ;
	
	QStringList tmp ;
	for( uint i=0; i<num; i++ )
		tmp.append( listBoxSelected->text(i) ) ;

	if( tmp.contains( listBoxAvailable->currentText() ) == 0 )
		listBoxSelected->insertItem( listBoxAvailable->currentText() ) ;
}



