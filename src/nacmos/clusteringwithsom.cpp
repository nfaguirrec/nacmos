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
#include "clusteringwithsom.h"

#include <stdlib.h>
#include <fstream>
using namespace std ;

#include <qspinbox.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qcanvas.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qbuttongroup.h>
#include <qbutton.h>

#include <SwapFile.h>
#include <swapmanager.h>
#include <navigationrule.h>
#include <randomtypegenerator.h>
#include <qgegraph.h>
#include <qgenode.h>
// #include <secuence.h>
#include <navigationrule.h>

#include <editinputvalues.h>
// #include <editsecuenceslabel.h>

#include <fragmenter.h>
// #include <translationidtoname.h>

#include <umvview.h>



 ClusteringWithSOM::ClusteringWithSOM( QWidget* parent, const char* name )
: ClusteringWithSOMBase( parent, name )
{
// 	umvView->setCanvas( new QCanvas( 800, 600 ) ) ;

	optionToCommand[ "Hexagonal" ] = "hexa" ;
	optionToCommand[ "Rectangular" ] = "rect" ;
	optionToCommand[ "Gaussian" ] = "gaussian" ;
	optionToCommand[ "Bubble" ] = "bubble" ;
	
	QButtonGroup* buttongroup = new QButtonGroup() ;
	buttongroup->insert( (QButton*)checkBoxRandom ) ;
	buttongroup->insert( (QButton*)checkBoxLinear ) ;
	buttongroup->setExclusive( TRUE ) ;
	checkBoxRandom->setChecked( TRUE ) ;
	
	spinBoxSeed->setValue( 0 ) ;
	
	spinBoxXDim->setValue( 40 ) ;
	spinBoxYDim->setValue( 30 ) ;
	
	comboBoxNeighborhood->insertItem( "Hexagonal" ) ;
	comboBoxNeighborhood->insertItem( "Rectangular" ) ;
	
	comboBoxTopology->insertItem( "Bubble" ) ;
	comboBoxTopology->insertItem( "Gaussian" ) ;
}


ClusteringWithSOM::~ClusteringWithSOM()
{}


void ClusteringWithSOM::view()
{
	string command = "som_pak/umat -cin tmp/ex.cod > tmp/salida.ps" ;
	
	cout << "Executing: " << command << "... " ;
	if( system( command.c_str() ) == 0 )
		cout << "OK" << endl ;
	else
		cout << "Failed" << endl ;
	
	int xDim = 2*spinBoxXDim->value()-1 ;
	int yDim = 2*spinBoxYDim->value()-1 ;
	
	umvView->buildGrid( xDim, yDim ) ;
	umvView->loadData( "tmp/umatrixdata.dat" ) ;
	
	umvView->update() ;
}

/**
 * 
 */
void ClusteringWithSOM::train()
{
	cout << "Creating data vector ... " ;
	
	QFile output( "tmp/data.dat" );
	if ( output.open( IO_WriteOnly ) ) {
		QTextStream stream( &output ) ;
	
		stream <<  SwapManager::Instance( SwapManager::SECONDARY )->getGraph(0)->number_of_nodes()*valuesToUse.size() << endl ;
		
		for( int i=0; i < SwapManager::Instance( SwapManager::SECONDARY )->getSize(); i++ ){
		
			QgeGraph* qgegraph = SwapManager::Instance( SwapManager::SECONDARY )->getGraph( i ) ;
			
			QPtrList<QgeNode> listtmp = NavigationRule::getSecuence( qgegraph, "Phosphate" ) ;
	
			for ( QPtrList<QgeNode>::iterator it = listtmp.begin(); it != listtmp.end(); it++ ) {
				
				for ( QStringList::iterator it2 = valuesToUse.begin(); it2 != valuesToUse.end(); it2++ )
					stream << QString::number( ( *it ) ->getWeights() [ *it2 ], 'f', 6 ) << "  " ;
				
			}
			stream << endl ;

		}
	
		output.close();
	}
	
	cout << "OK" << endl ;
	
	QString command = QString( "som_pak/vsom -din tmp/data.dat -cin tmp/ex.cod -cout tmp/ex.cod" ) +
	                  QString( " -rlen " ) + QString::number( spinBoxNumberOfSteps->value() ) +
	                  QString( " -alpha " ) + lineEditAlpha->text() +
	                  QString( " -radius " ) + lineEditRadius->text() ;
// 			  QString(" &") ;

	cout << "Executing: " << command << "... " ;
	if( system( command.latin1() ) == 0 )
		cout << "OK" << endl ;
	else
		cout << "Failed" << endl ;
	
	command = "som_pak/qerror -din tmp/data.dat -cin tmp/ex.cod > tmp/qerror" ;
	
	cout << "Executing: " << command << "... " ;
	if( system( command.latin1() ) == 0 )
		cout << "OK" << endl ;
	else
		cout << "Failed" << endl ;
	
	QFile input( "tmp/qerror" );
	if ( input.open( IO_ReadOnly ) ) {
		QTextStream stream( &input ) ;
	
		QString item ;
	
		for ( int i = 0; i < 9; i++ )
			stream >> item ;
	
		lineEditQError->setText( item ) ;
	
		input.close();
	}
	
	int xDim = 2*spinBoxXDim->value()-1 ;
	int yDim = 2*spinBoxYDim->value()-1 ;
	
	umvView->buildGrid( xDim, yDim ) ;
	umvView->loadData( "tmp/umatrixdata.dat" ) ;
	
	view() ;
}

void ClusteringWithSOM::editInputValues()
{
	QgeGraph * graphtmp = SwapManager::Instance( SwapManager::SECONDARY )->getGraph( 0 ) ;

	if ( graphtmp ) {
		QStringList availablevalues = graphtmp->nodeToQgeNode( *graphtmp->nodes_begin() )->getWeights().keys() ;
		QStringList values = EditInputValues::getInputValues( availablevalues ) ;

		valuesToUse = values ;

		QString tmp ;
		uint num = values.size() ;
		for ( uint i = 0; i < num; i++ ) {
			if ( i == num - 1 )
				tmp.append( valuesToUse[ i ] ) ;
			else
				tmp.append( valuesToUse[ i ] + ", " ) ;
		}

		lineEditWeightingInputValues->setText( tmp ) ;
	}
}

void ClusteringWithSOM::initMap()
{
	cout << "Creating reference vectors ... " ;
	
	ofstream file("tmp/data.dat") ;
	
	int sizeVectors = SwapManager::Instance( SwapManager::SECONDARY )->getGraph(0)->number_of_nodes()*valuesToUse.size() ;
	
	file << sizeVectors << endl ;
	
// 	for( uint i=0; i < 20; i++ ){
// 		for( uint j=0; j < sizeVectors; j++ ){
// 			file << (double(RAND(100))/100.0) << "  " ;
// 		}
// 		file << endl ;
// 	}
	
	for( int i=0; i < SwapManager::Instance( SwapManager::SECONDARY )->getSize(); i++ ){
		
		QgeGraph* graphtmp = SwapManager::Instance( SwapManager::SECONDARY )->getGraph(i) ;
		
		QPtrList<QgeNode> listtmp = NavigationRule::getSecuence( graphtmp, "Phosphate" ) ;
		
		for ( QPtrList<QgeNode>::iterator it = listtmp.begin(); it != listtmp.end(); it++ ) {
			for ( QStringList::iterator it2 = valuesToUse.begin(); it2 != valuesToUse.end(); it2++ )
				file << QString::number( ( *it )->getWeights() [ *it2 ], 'f', 6 ) << "  " ;
		}
		file << endl ;
		
	}
		
	cout << "OK" << endl ;
	
	QString command = QString( "som_pak/randinit -din tmp/data.dat -cout tmp/ex.cod" ) +
	                  QString( " -xdim " ) + QString::number( spinBoxXDim->value() ) +
	                  QString( " -ydim " ) + QString::number( spinBoxYDim->value() ) +
			  QString( " -topol " ) + optionToCommand[ comboBoxNeighborhood->currentText() ] +
			  QString( " -neigh " ) + optionToCommand[ comboBoxTopology->currentText() ] +
			  QString( " -rand " ) + QString::number( spinBoxSeed->value() ) ;
// 				QString(" &") ;
	
	cout << "Executing: " << command << "... " ;
	system( command.latin1() ) ;
	cout << "OK" << endl ;
	
	umvView->buildGrid( spinBoxXDim->value(), spinBoxYDim->value() ) ;
	
	view() ;
}

void ClusteringWithSOM::mapping()
{
	cout << "Creating calibration vectors ... " ;
	
	ofstream file("tmp/calib.dat") ;
	
	int sizeVectors = SwapManager::Instance( SwapManager::PRINCIPAL )->getGraph(0)->number_of_nodes()*valuesToUse.size() ;
	
	file << sizeVectors << endl ;
	
	for( int i=0; i < SwapManager::Instance( SwapManager::PRINCIPAL )->getSize(); i++ ){
		
		QgeGraph* graphtmp = SwapManager::Instance( SwapManager::PRINCIPAL )->getGraph(i) ;
		
		QPtrList<QgeNode> listtmp = NavigationRule::getSecuence( graphtmp, "Phosphate" ) ;
		
		for ( QPtrList<QgeNode>::iterator it = listtmp.begin(); it != listtmp.end(); it++ ) {
			for ( QStringList::iterator it2 = valuesToUse.begin(); it2 != valuesToUse.end(); it2++ )
				file << QString::number( ( *it )->getWeights() [ *it2 ], 'f', 6 ) << "  " ;
		}
		
		file << graphtmp->getLabel().replace( " ", "_" ) << endl ;
		
	}
		
	cout << "OK" << endl ;

	QString command = QString( "som_pak/vcal -din tmp/calib.dat -cin tmp/ex.cod -cout tmp/ex.cod" ) ;
	
	cout << "Executing: " << command << "... " ;
	system( command.latin1() ) ;
	cout << "OK" << endl ;
	
	view() ;
}

void ClusteringWithSOM::clear()
{
}

void ClusteringWithSOM::setRandomType( int state )
{
	if( state == QButton::On ){
		spinBoxSeed->setDisabled( TRUE ) ;
	}else{
		spinBoxSeed->setDisabled( FALSE ) ;
	}
}



