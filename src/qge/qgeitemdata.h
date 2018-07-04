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
#ifndef QGEITEMDATA_H
#define QGEITEMDATA_H

#include <qmap.h>
#include <qgeitemtype.h>

class QString ;

/**
@author Nestor Aguirre
*/
class QgeItemData {

	public:
		QgeItemData();
		~QgeItemData();
		
		void copy( QgeItemData* qgeitemdata ) ;
		void setProcessed( bool processed ) ;
		void setType( QgeItemType type ) ;
		void setLabel( QString label ) ;
		void setExternalId( double externalId ) ;
		void setDisableChangeType( bool changetype ) ;
		void setEnableShowLabel( bool showLabel ) ;
		
		bool isProcessed() ;
		QgeItemType getType() ;
		QString getLabel() ;
		double getExternalId() ;
		bool isDisableChangeType() ;
		bool isEnableShowLabel() ;
		
		void addWeight(QString name, double value) ;
		QMap<QString, double>& getWeights() ;
		
	private:
		bool processed ;
		double externalId ;
		QgeItemType type ;
		QString label ;
		QMap<QString, double> weight ;
		bool changetype ;
		bool showLabel ;
};

#endif
