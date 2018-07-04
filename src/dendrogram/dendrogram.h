/***************************************************************************
 *   Copyright (C) 2007 by Nestor Aguirre,,,   *
 *   nestor@meknes   *
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
#ifndef DENDROGRAM_H
#define DENDROGRAM_H

#include <qmap.h>

#include <qwt/qwt_plot.h>

#include <Set.h>


/**
	@author Nestor Aguirre,,, <nestor@meknes>
*/
class Dendrogram : public QwtPlot
{
	public:
		Dendrogram( QWidget* parent = 0, const char* name = 0 );
		~Dendrogram();
		
		void setEquivalenceClass( QMap< double, Set< Set<string> > > equivalenceClasses, Set<string>* universalSet = 0 ) ;
		void setMaxRadius( double maxRadius ) ;
		
	protected:
		QStringList sortNames( Set<string> setToOrder ) ;
		virtual void drawCanvas( QPainter* p );
		
	public:
		struct Point{
			double x ;
			double y ;
		} ;
		
		struct SetAndSubset{
			Set<string> current ;
			Set<string> previous ;
		} ;
		
	private:
		QMap< double, Set< Set<string> > > equivalenceClasses ;
		QMap< double, SetAndSubset > equivalenceClassesFiltered ;
		QStringList sortedNames ;
		QMap< Set<string>, Point > nameToPosition ;
		double maxRadius ;
};

#endif
