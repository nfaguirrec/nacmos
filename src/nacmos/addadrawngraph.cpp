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
#include "addadrawngraph.h"

#include <qfiledialog.h>

#include <qgewidget.h>

#include <navigationrule.h>
#include <swapmanager.h>

AddADrawnGraph::AddADrawnGraph( bool withLoadSequences, QWidget* parent, const char* name )
 : AddADrawnGraphBase( parent, name )
{
	this->withLoadSequences = withLoadSequences ;
	qgeWidget->setCanvas( new QCanvas(800, 600) ) ;
}

AddADrawnGraph::~AddADrawnGraph()
{
}

void AddADrawnGraph::help()
{
}

void AddADrawnGraph::cancel()
{
	this->close() ;
}

void AddADrawnGraph::ok()
{
	this->close() ;
}

void AddADrawnGraph::add()
{
	if( !withLoadSequences ){
	
		SwapManager::Instance( SwapManager::TEMPLATES )->addGraph( qgeWidget->getGraph() ) ;
		
	}else{
	
		QStringList fileNames = QFileDialog::getOpenFileNames( "Fasta Files (*.fasta)", ".", this, "fileNames", "Open Files fasta" ) ;
		
		if( fileNames.size() != 0 ){
		
			QgeGraph* tmp = qgeWidget->getGraph() ;
			
			for( QStringList::iterator it = fileNames.begin(); it != fileNames.end(); it++ ){
				
				QFile file( *it ) ;
				file.open( IO_ReadOnly ) ;
				
				if( file.exists() ){
				
					bool arriveEOF = FALSE ;
					
					while( !arriveEOF ){
						arriveEOF = NavigationRule::loadSecuence( file, tmp, "Phosphate" ) ;
						SwapManager::Instance( SwapManager::PRINCIPAL )->addGraph( tmp ) ;
					}
					
					file.close() ;
						
				}
				
			}
			
		}

	}
}


