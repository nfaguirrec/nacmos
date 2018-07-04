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
#include "dendrogram.h"

#include <qpainter.h>
#include <qbrush.h>
#include <qpen.h>

#include <qwt/qwt_painter.h>
#include <qwt/qwt_scale_draw.h>
#include <qwt/qwt_scale_widget.h>

#define tx(x) transform(Dendrogram::xBottom, x)
#define ty(x) transform(Dendrogram::yLeft, x)

Dendrogram::Dendrogram( QWidget* parent, const char* name )
: QwtPlot( parent, name )
{
	axisWidget(Dendrogram::xBottom)->setLabelAlignment( Qt::AlignHCenter | Qt::AlignBottom ) ;
	axisScaleDraw( Dendrogram::xBottom )->setLabelRotation( 0.0 ) ;
	
	setAxisScale( Dendrogram::yLeft, 100.0, 0.0, 10.0 ) ;
	enableAxis( Dendrogram::yLeft, false) ;
	setAxisScale( Dendrogram::xBottom, -0.2, 1.0, 0.1 ) ;
}

Dendrogram::~Dendrogram()
{
}

#include <iostream>
using namespace std ;

void Dendrogram::drawCanvas(QPainter* p)
{
	QwtPlot::drawCanvas( p ) ;
	
	int ypos = 0 ;
	double stepSize = 100.0/sortedNames.size() ;
	for ( QStringList::Iterator itqstr = sortedNames.begin();  itqstr != sortedNames.end(); itqstr++, ypos++ ){
		QwtPainter::drawText( p, tx(-0.2), ty(ypos*stepSize), tx(-0.02), ty(100), Qt::AlignAuto | Qt::BreakAnywhere | Qt::SingleLine , *itqstr) ;
		Set<string> setTMP ;
		setTMP.insert( *itqstr ) ;
		
		Point pointTMP ;
		pointTMP.x = 0 ;
		pointTMP.y = ypos*stepSize ;
		
		nameToPosition[ setTMP ] = pointTMP ;
	}
	
	for( QMap< double, SetAndSubset >::iterator it = equivalenceClassesFiltered.begin(); 
		    it != equivalenceClassesFiltered.end(); it++ ){
		
		if( it.data().current.size() == 2 ){
			
			double yorder = 0.0 ;
			for ( Set<string>::iterator itset = it.data().current.begin(); 
						   itset != it.data().current.end(); itset++ ){
				Set<string> setTMP ;
				setTMP.insert( *itset ) ;
				yorder = yorder + nameToPosition[ setTMP ].y ;
			}
			yorder /= 2.0 ;
		
			Point pointTMP ;
			pointTMP.x = it.key() ;
			pointTMP.y = yorder ;
			
			nameToPosition[ it.data().current ] = pointTMP ;
			
		}
		else{
			
			double yorder = 0.0 ;
			yorder = nameToPosition[ it.data().current.differenceWith(it.data().previous) ].y
					+ nameToPosition[ it.data().previous ].y ;
			yorder /= 2.0 ;
			
			Point pointTMP ;
			pointTMP.x = it.key() ;
			pointTMP.y = yorder ;
			
			nameToPosition[ it.data().current ] = pointTMP ;
			
		}
	}
	
	
	double x, y ;
	for( QMap< double, SetAndSubset >::iterator it = equivalenceClassesFiltered.begin(); 
		    it != equivalenceClassesFiltered.end(); it++ ){

			if( it.data().current.size() != 1 ){
				if( it.data().current.size() != 2 ){
					QwtPainter::drawLine( p, 
						tx(nameToPosition[it.data().previous].x/maxRadius),
						ty(nameToPosition[ it.data().previous].y), 
						tx(nameToPosition[ it.data().current ].x/maxRadius),
						ty(nameToPosition[ it.data().previous].y ) ) ;
					
					QwtPainter::drawLine( p, 
						tx(nameToPosition[ it.data().current.differenceWith( it.data().previous) ].x/maxRadius),
						ty(nameToPosition[ it.data().current.differenceWith( it.data().previous) ].y), 
						tx(nameToPosition[ it.data().current ].x/maxRadius),
						ty(nameToPosition[ it.data().current.differenceWith( it.data().previous) ].y ) ) ;

					QwtPainter::drawLine( p, 
						tx(nameToPosition[ it.data().current ].x/maxRadius),
						ty(nameToPosition[ it.data().current.differenceWith( it.data().previous) ].y), 
						tx(nameToPosition[ it.data().current ].x/maxRadius),
						ty(nameToPosition[ it.data().previous ].y ) ) ;
					
				}
				else{
					Set<string> setTMP ;
					for ( Set<string>::iterator itset = it.data().current.begin(); itset != it.data().current.end(); itset++ ){
						
						setTMP.clear() ;
						setTMP.insert( *itset ) ;
						QwtPainter::drawLine( p, 
						tx(nameToPosition[ setTMP ].x/maxRadius),
						ty(nameToPosition[ setTMP ].y), 
						tx(nameToPosition[ it.data().current ].x/maxRadius),
						ty(nameToPosition[ setTMP ].y ) ) ;
					}
					
					QwtPainter::drawLine( p, 
						tx(nameToPosition[ it.data().current ].x/maxRadius),
						ty(nameToPosition[ it.data().current.differenceWith( setTMP ) ].y), 
						tx(nameToPosition[ it.data().current ].x/maxRadius),
						ty(nameToPosition[ setTMP ].y ) ) ;
				}
			}
// 			
// 			QwtPainter::drawLine( p,
// 				  tx(nameToPosition[ it.data().current.differenceWith(it.data().previous) ].x),
// 				  ty(nameToPosition[ it.data().current.differenceWith(it.data().previous) ].y),
// 				  tx(nameToPosition[ it.data().current ].x),
// 				  ty(nameToPosition[ it.data().current ].y) ) ;
			x = nameToPosition[ it.data().current ].x/maxRadius ;
			y = nameToPosition[ it.data().current ].y ;
			
			QwtPainter::drawPoint( p, tx(x), ty(y) ) ;
			
	}
	
	cout << "Conjuntos" << endl ;
	for( QMap< Set<string>, Point >::iterator it = nameToPosition.begin(); it != nameToPosition.end(); it++ ){
		cout << it.key() << ": ( " << it.data().x << ", " << it.data().y << " )" << endl ;
	}
}

void Dendrogram::setEquivalenceClass( QMap< double, Set< Set<string> > > equivalenceClasses, Set<string>* universalSet )
{
	this->equivalenceClasses = equivalenceClasses ;
	
	if( !universalSet ){
		///@todo Hay que sacar el conjunto universal desde las clases de equivalencia
// 		(*universalSet) = (equivalenceClasses.end()--).data().begin() ;
// 		cout << "Universal = " << (*universalSet) << endl ;
// 		cout << "Key = " << equivalenceClasses[].key() << endl ;
	}
	
	sortedNames = sortNames( *universalSet ) ;
}

QStringList Dendrogram::sortNames( Set<string> setOrder )
{
	QValueList<double> keys = equivalenceClasses.keys() ;
	// Asumiendo que el qmap organiza los valores en orden ascendente
	QStringList nameorder ;
	Set<string> group ;
	group.clear() ;
	int i=1 ;

	for ( QValueList<double>::iterator it = keys.begin(); it != keys.end(); it++){
		if( nameorder.size() == setOrder.size () ){
			break ;
		}
		for( Set< Set<string> >::iterator it1 = equivalenceClasses[ *it ].begin(); it1 != equivalenceClasses[ *it ].end(); it1++ ){
			if( it1->size() != 1 & i==1 ){
				group = *it1 ;
				group.setName( "Orden" ) ;
				for( Set<string>::iterator it2 = it1->begin(); it2 != it1->end(); it2++ ){
					if( setOrder.find( *it2 ) != setOrder.end() ){
						nameorder.append( *it2 ) ;
					}
				}
				if(nameorder.size() == 0 ){
					group.clear() ;
				}
				else{
					SetAndSubset setandsubetTMP ;
					setandsubetTMP.current = *it1 ;
					setandsubetTMP.previous = group ;
					equivalenceClassesFiltered[ *it ] = setandsubetTMP ;
					
					i++ ;
					break ;
				}
			}
			
			Set<string> tmp = *it1 ;
			
			if ( group.intersectionWith( tmp ) == group & i != 1 ){
				if( tmp.size() <= group.size() + 2 ){
					bool in = false ;
					for ( Set<string>::iterator ittmp = tmp.begin(); ittmp != tmp.end(); ittmp++ ){
						if( group.find( *ittmp ) == group.end()){
							nameorder.append( *ittmp );
							in = true ;
						}
					}
					
					if( in ){
						SetAndSubset setandsubetTMP ;
						setandsubetTMP.current = *it1 ;
						setandsubetTMP.previous = group ;
						equivalenceClassesFiltered[ *it ] = setandsubetTMP ;
					}
					if( tmp.size() == group.size() + 2){
						Set<string> nuevo = tmp.differenceWith( group ) ;
						sortNames( nuevo ) ; 
					}
					group = tmp ;
					group.setName( "Orden" ) ;
				}
				else{
					Set<string> nuevo = tmp.differenceWith( group ) ;
					
					QStringList seudoOrden = sortNames( nuevo ) ;
					
					nameorder += seudoOrden ;
					
					SetAndSubset setandsubetTMP ;
					setandsubetTMP.current = *it1 ;
					setandsubetTMP.previous = group ;
					equivalenceClassesFiltered[ *it ] = setandsubetTMP ;
					
					group = tmp ;
					group.setName( "Orden" ) ;
				}
			}
		}
	}
	
	return nameorder ;
}

void Dendrogram::setMaxRadius( double maxRadius )
{
	this->maxRadius = maxRadius ;
}

