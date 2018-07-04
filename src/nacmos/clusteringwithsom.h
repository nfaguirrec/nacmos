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
#ifndef CLUSTERINGWITHSOM_H
#define CLUSTERINGWITHSOM_H

#include <clusteringwithsombase.h>

#define RAND(x) int( x*double(rand() )/double(2147483647))

class UMatrixView ;
class EditInputValues ;
class RandomTypeGenerator ;
class Secuence ;
class QgeGraph ;
class Fragmenter ;

/**
@author Nestor Aguirre
 */
class ClusteringWithSOM : public ClusteringWithSOMBase
{
	public:
		ClusteringWithSOM( QWidget* parent = 0, const char* name = 0 );
		~ClusteringWithSOM();
		
	public slots:
		virtual void view();
		virtual void mapping();
		virtual void initMap();
		virtual void train();
		virtual void editInputValues() ;
		virtual void clear();
		virtual void setRandomType( int state );
		
	private:
		QMap<QString, QString> optionToCommand ;
		QStringList valuesToUse ;
};

#endif
