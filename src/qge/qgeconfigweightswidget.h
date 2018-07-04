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
#ifndef QGECONFIGWEIGHTSWIDGET_H
#define QGECONFIGWEIGHTSWIDGET_H

#include <formqgeconfigweightswidget.h>

class QgeItemData ;

/**
@author Nestor Aguirre
*/
class QgeConfigWeightsWidget : public FormQgeConfigWeightsWidget{
	public:
		QgeConfigWeightsWidget( QgeItemData* qgeitemdata, QWidget* parent = 0, const char* name = 0 ) ; 
		~QgeConfigWeightsWidget() ;
		
		bool close( bool update ) ;
		
	private:
		void readObject() ;
		
	private:
		QgeItemData* qgeitemdata ;
		
	private slots:
		virtual void datesFromTable( int row );
		virtual void remove();
		virtual void add();
		
		virtual void updateTable() ;
};

#endif
