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
#include "ViennaRNA.h"

#include <qfile.h>
#include <qtextstream.h>

#include <swapmanager.h>

ViennaRNA::ViennaRNA()
{
}

ViennaRNA::~ViennaRNA()
{
}

void ViennaRNA::setDefaultParameters( bool defaultParameters )
{
	this->defaultParameters = defaultParameters ;
}

void ViennaRNA::setFastaFile( QString fastaFileName )
{
	fastaFileNames.clear() ;
	fastaFileNames.append( fastaFileName ) ;
}

void ViennaRNA::setFastaFiles( QStringList fastaFileNames )
{
	this->fastaFileNames = fastaFileNames ;
}

void ViennaRNA::findLabels()
{
// 	QString command = QString("rm tmp/fastaLabels.dat" ) ;
// 	cout << "Executing " << command << " ... "  ;
// 	system( command.latin1() ) ;
// 	cout << "OK" << endl ;
// 	
// 	for( QStringList::iterator it = fastaFileNames.begin(); it != fastaFileNames.end(); it++ ){
// 		command = QString("gawk 'BEGIN{ RS=\">\" ; FS=\"\\n\" } { if( NF!= 0 ) print $1 }' " + *it + ">> tmp/fastaLabels.dat" ) ;
// 		cout << "Executing " << command << " ... " ;
// 		system( command.latin1() ) ;
// 		cout << "OK" << endl ;
// 	}
// 	
	QFile fileIn( "tmp/fastaLabels.dat" ) ;
	fileIn.open( IO_ReadOnly ) ;
	QTextStream streamIn( &fileIn ) ;
	
	fastaFileNames.clear() ;
	
	QString tmpLabel ;
	while( !fileIn.atEnd() ){
		streamIn >> tmpLabel ;
		streamIn >> tmpLabel ;
		labels.append( tmpLabel ) ;
	}
}

QgeGraph* ViennaRNA::execFirst()
{
	QString command = QString("rm tmp/secondary.dat tmp/fastaLabels.dat") ;
	cout << "Executing " << command << " ... " ;
	system( command.latin1() ) ;
	cout << "OK" << endl ;

	for( QStringList::iterator it = fastaFileNames.begin(); it != fastaFileNames.end(); it++ ){
		command = QString("ViennaRNA/Progs/RNAfold < ") + *it + QString(" >> ")+QString("tmp/secondary.dat") ;
		cout << "Executing " << command << " ... " ;
		system( command.latin1() ) ;
		cout << "OK" << endl ;
	}
	
	command = "ViennaRNA/Progs/RNAplot -o gml < tmp/secondary.dat >> tmp/fastaLabels.dat" ;
	cout << "Executing " << command << " ... " ;
	system( command.latin1() ) ;
	cout << "OK" << endl ;
	
	command = QString("rm *.ps") ;
	cout << "Executing " << command << " ... " ;
	system( command.latin1() ) ;
	cout << "OK" << endl ;
	
	findLabels() ;
	
	for( QStringList::iterator it = labels.begin(); it != labels.end(); it++ ){
		command = QString("mv ")+ *it + QString("*.gml tmp/") ;
		cout << "Executing " << command << " ... " ;
		system( command.latin1() ) ;
		cout << "OK" << endl ;
	}
	
	for( QStringList::iterator it = labels.begin(); it != labels.end(); it++ ){
		command = QString("gawk -f utils/vienna2nacmos.awk tmp/") + *it + QString("*.gml > tmp/") + *it + QString("_NACMOS.gml") ;
		cout << "Executing " << command << " ... " ;
		system( command.latin1() ) ;
		cout << "OK" << endl ;
	}
			
// 	command = "rm tmp/rna.gml" ;
// 	cout << "Executing " << command << " ... " ;
// 	system( command.latin1() ) ;
// 	cout << "OK" << endl ;
	
	QgeGraph* output = new QgeGraph( 0 ) ;
	cout << "Loading graph from: " << QString("tmp/")+(*labels.begin())+QString("_NACMOS.gml") << endl ;
	
	output->load( QString("tmp/")+(*labels.begin())+QString("_NACMOS.gml")  ) ;
	
// 	output->load( fastaFileName  ) ;
	
	return output ;	
}
void ViennaRNA::exec()
{
	QString command = QString("rm tmp/secondary.dat tmp/fastaLabels.dat") ;
	cout << "Executing " << command << " ... " ;
	system( command.latin1() ) ;
	cout << "OK" << endl ;

	for( QStringList::iterator it = fastaFileNames.begin(); it != fastaFileNames.end(); it++ ){
		command = QString("ViennaRNA/Progs/RNAfold < ") + *it + QString(" >> ")+QString("tmp/secondary.dat") ;
		cout << "Executing " << command << " ... " ;
		system( command.latin1() ) ;
		cout << "OK" << endl ;
	}
	
	command = "ViennaRNA/Progs/RNAplot -o gml < tmp/secondary.dat >> tmp/fastaLabels.dat" ;
	cout << "Executing " << command << " ... " ;
	system( command.latin1() ) ;
	cout << "OK" << endl ;
	
	command = QString("rm *.ps") ;
	cout << "Executing " << command << " ... " ;
	system( command.latin1() ) ;
	cout << "OK" << endl ;
	
	findLabels() ;
	
	for( QStringList::iterator it = labels.begin(); it != labels.end(); it++ ){
		command = QString("mv ")+ *it + QString("*.gml tmp/") ;
		cout << "Executing " << command << " ... " ;
		system( command.latin1() ) ;
		cout << "OK" << endl ;
	}
	
	for( QStringList::iterator it = labels.begin(); it != labels.end(); it++ ){
		command = QString("gawk -f utils/vienna2nacmos.awk tmp/") + *it + QString("*.gml > tmp/") + *it + QString("_NACMOS.gml") ;
		cout << "Executing " << command << " ... " ;
		system( command.latin1() ) ;
		cout << "OK" << endl ;
	}
			
// 	command = "rm tmp/rna.gml" ;
// 	cout << "Executing " << command << " ... " ;
// 	system( command.latin1() ) ;
// 	cout << "OK" << endl ;
	
	QgeGraph* output = new QgeGraph( 0 ) ;
	cout << "Loading graph from: " << QString("tmp/")+(*labels.begin())+QString("_NACMOS.gml") << endl ;
	
	for( QStringList::iterator it = labels.begin(); it != labels.end(); it++ )
	{
		output->load( QString("tmp/")+(*it)+QString("_NACMOS.gml")) ;
		SwapManager::Instance( SwapManager::PRINCIPAL )->addGraph( output ) ;
	}
}
