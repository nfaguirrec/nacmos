/***************************************************************************
*   Copyright (C) 2006 by Nestor Aguirre                                  *
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
#ifndef SWAPFILE_H
#define SWAPFILE_H

#include <string>
#include <iomanip>
#include <iostream>
using namespace std ;

#include <qstring.h>
#include <qcolor.h>
#include <qgegraph.h>
#include <qgenode.h>
#include <qgeedge.h>
#include <qgeitemtype.h>

/**
 *  @brief 
 *  @author N�tor Aguirre
 *
 *  <b> Fecha de creaci� : </b> $DATE
 *
 *  <b> Ultima modificaci� : </b> $DATE
 *
 *  <b> Historial de modificaciones : </b>
 *    - <tt>2006-05-15</tt>: N�tor Aguirre ( Dirac, diracn@gmail.com )
 *         -# Se adapt�el c�igo para soportar doxygen
 */
class SwapFile : public QObject
{
	Q_OBJECT
	
	private:
		enum TypeItem
		{
			NODE,
			EDGE
		};
		
		struct GraphicsData
		{
			QString colorSelected ;
			QString colorUnselected ;
			QString colorName ;
			int x ;
			int y ;
			int w ;
			int h ;
		};
		
		struct WeightsData
		{
			QMap<QString, double> data ;
		};
		
		struct ItemData
		{
			int id ;
			int source ;
			int target ;
			QString name ;
			QString type ;
			int selected ;
			double neighborRange ;
			int disableChangeType ;
			int isFirst ;
			double externalId ;
			int directed ;
			int numberOfNodes ;
			int numberOfEdges ;
			GraphicsData graphicsdata ;
			WeightsData weightsdata ;
		};
		
	public:
		SwapFile( const QString& filename, bool debug = FALSE, bool fileNew = TRUE );
		virtual ~SwapFile();
		
	public:
		int addGraph( QgeGraph* graph ) ;
		
		QgeGraph* getGraph( uint id ) ;
		int getSize() ;
		void remove( uint id ) ;
		void clear() ;
		void updateReferences() ;
// 		void edit( uint id, QgeGraph* graph ) ;
		
	signals:
		void hasChanged( bool firstTime = FALSE ) ;
		
	private:
		void loadGraph( uint id ) ;
		
		void loadPair( QString key, QString &data ) ;
		void loadPairInt( QString key, int &data ) ;
		void loadPairDouble( QString key, double &data ) ;
		
		struct ItemData loadNode() ;
		struct ItemData loadEdge() ;
		struct GraphicsData loadGraphics( TypeItem typeitem ) ;
		QValueList<QgeItemType> loadTypes( TypeItem typeitem ) ;
		struct WeightsData loadWeights() ;
		
	private:
		bool debug ;				///< Flag que activa la salida en modo debug
		int size ;				///< Nmero de grafos almacenados en el SwapFile
		QgeGraph* currentGraph ;		///< Grafos actualmente en edici�
		QString filename ;
		FILE* file ;
		QValueList<fpos_t> idToPosFile ;	///< convierte del id al bit de posici� en el archivo
};

#endif
