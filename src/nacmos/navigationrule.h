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
#ifndef NAVIGATIONRULE_H
#define NAVIGATIONRULE_H

// #include <qvaluelist.h>
#include <qptrlist.h>
#include <qfile.h>

class QgeGraph ;
class QgeNode ;
class QString ;

/**
@author Nestor Aguirre
*/
class NavigationRule{
	public:
		NavigationRule( QgeGraph* qgegraph, QString edgeType = "Default" ) ;
		~NavigationRule() ;
		
		static QPtrList<QgeNode> getSecuence( QgeGraph* qgegraph, QString edgeType = "Default" ) ;
		static void loadSecuence( QgeGraph* qgegraph, const QString& fileName, QString edgeType = "Default" ) ;
		static bool loadSecuence( QFile& file, QgeGraph* qgegraph, QString edgeType = "Default" ) ;
		static void saveSecuence( QgeGraph* qgegraph, const QString& fileName, QString edgeType = "Default" ) ;
	private:
		QgeGraph* qgegraph ;
		QString edgeType ;

};

#endif
