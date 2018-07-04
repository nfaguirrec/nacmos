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
#include "graphseditor.h"

#include <qcanvas.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qerrormessage.h>
#include <qmessagebox.h>
#include <qtextstream.h>

#include <qgewidget.h>
#include <qgegraph.h>

#include <navigationrule.h>
#include <verifingfragmentation.h>
#include <ViennaRNA.h>

GraphsEditor::GraphsEditor( QWidget* parent, const char* name ):
GraphsEditorBase( parent, name )
{
	qgewidget->setCanvas( new QCanvas( 800 ,600 ) ) ;
}

GraphsEditor::~GraphsEditor()
{
}

void GraphsEditor::callViennaRNAPackage( QString fastaFileName )
{
	QString command = QString("ViennaRNA/Progs/RNAfold < ") + fastaFileName + QString(" > ")+QString("tmp/secondary.dat") ;
	cout << "Executing " << command << " ... " ;
	system( command.latin1() ) ;
	cout << "OK" << endl ;
			
	command = "ViennaRNA/Progs/RNAplot -o gml < tmp/secondary.dat" ;
	cout << "Executing " << command << " ... " ;
	system( command.latin1() ) ;
	cout << "OK" << endl ;
			
	command = "mv rna* tmp/" ;
	cout << "Executing " << command << " ... " ;
	system( command.latin1() ) ;
	cout << "OK" << endl ;
			
	command = "gawk -f utils/vienna2nacmos.awk tmp/rna.gml > tmp/salida.gml" ;
	cout << "Executing " << command << " ... " ;
	system( command.latin1() ) ;
	cout << "OK" << endl ;
			
	command = "rm tmp/rna.gml" ;
	cout << "Executing " << command << " ... " ;
	system( command.latin1() ) ;
	cout << "OK" << endl ;
	
	qgewidget->load( "tmp/salida.gml" ) ;
}

void GraphsEditor::removeEdge()
{
	qgewidget->changeState( QgeWidget::REMOVE_EDGE ) ;
}


void GraphsEditor::removeNode()
{
	qgewidget->changeState( QgeWidget::REMOVE_NODE ) ;
}


void GraphsEditor::hideEdge()
{
	qgewidget->changeState( QgeWidget::HIDE_EDGE ) ;
}


void GraphsEditor::hideNode()
{
	qgewidget->changeState( QgeWidget::HIDE_NODE ) ;
}


void GraphsEditor::moveNode()
{
	qgewidget->changeState( QgeWidget::MOVE_NODE ) ;
}


void GraphsEditor::insertEdge()
{
	qgewidget->changeState( QgeWidget::INSERT_EDGE ) ;
}


void GraphsEditor::insertNode()
{
	qgewidget->changeState( QgeWidget::INSERT_NODE ) ;
}


void GraphsEditor::moveGraph()
{
	qgewidget->changeState( QgeWidget::MOVE_GRAPH ) ;
}

void GraphsEditor::getDijsktraPath()
{
	qgewidget->changeState( QgeWidget::GET_DIJKSTRA_PATH ) ;
}

void GraphsEditor::fileSaveAsFasta()
{
	QString fileName = QFileDialog::getSaveFileName(
			".",
	"Fasta (*.fasta)",
	this,
	"save file dialog",
	"Choose a filename to save under" );
	if( fileName != QString::null ){
		cout << "Saving secuence to " << fileName << " ... " ;
		NavigationRule::saveSecuence( qgewidget->getGraph(), fileName, "Phosphate" ) ;
		cout << "OK" << endl ;
	}
		
	fileNameFasta = fileName ;
}


void GraphsEditor::fileSaveFasta()
{
	if( fileNameFasta.length() != 0 ){
		NavigationRule::saveSecuence( qgewidget->getGraph(), fileNameFasta, "Phosphate" ) ;
	}else{
		fileSaveAsFasta() ;
	}
}


void GraphsEditor::fileOpenFasta()
{
	QString fileName = QFileDialog::getOpenFileName(
			".",
	"Fasta (*.fasta)",
	this,
	"open file dialog",
	"Choose a file" );
	if( fileName != QString::null ){
		QFile file( fileName ) ;
		file.open( IO_ReadOnly ) ;
		
		cout << "Loading secuence from " << fileName << " ... " ;
		NavigationRule::loadSecuence( file, qgewidget->getGraph(), "Phosphate" ) ;
		cout << "OK" << endl ;
	}
	
	fileNameFasta = fileName ;
}


void GraphsEditor::reload()
{
	qgewidget->reload() ;
}


void GraphsEditor::newGraph()
{
	qgewidget->newGraph() ;
}


void GraphsEditor::print()
{
}


void GraphsEditor::verifingFragmentation()
{
	if ( qgewidget->getGraph()->number_of_nodes() == 0 ){
		QMessageBox::warning( this, tr("Warning !!!!"), tr("Graph not found" ), QMessageBox::Ok, 0 ) ;
		return ;
	}
	
	VerifingFragmentation* verifingfragmentation = new VerifingFragmentation( qgewidget->getGraph() ) ;
	verifingfragmentation->show() ;
}


void GraphsEditor::fileSaveAs()
{
	qgewidget->saveAs() ;
}


void GraphsEditor::fileSave()
{
	qgewidget->save() ;
}


void GraphsEditor::fileOpen()
{
	qgewidget->load() ;
}

void GraphsEditor::secondaryStructureViennaPackage()
{
	if( qgewidget->getGraph()->number_of_nodes() != 0 ){
		QString label = qgewidget->getGraph()->getLabel() ;
		qgewidget->getGraph()->setLabel("") ;
		NavigationRule::saveSecuence( qgewidget->getGraph(), "tmp/fasta.fasta", "Phosphate" ) ;
		qgewidget->getGraph()->setLabel( label ) ;
		
		callViennaRNAPackage( "tmp/fasta.fasta" ) ;
	}else{
		QStringList fileNames = QFileDialog::getOpenFileNames( "Fasta (*.fasta)", ".", this, "fileNames", "Opening Fasta Files" ) ;
		
		if( !fileNames.empty() ){
			
			ViennaRNA vienna ;
			vienna.setFastaFiles( fileNames ) ;
			vienna.setDefaultParameters( TRUE ) ;
			qgewidget->setGraph( vienna.execFirst() ) ;
			qgewidget->centerGraph();
		}
	}
}

// #include <qtoolbar.h>
// #include <qaction.h>
// 
// GraphsEditor::GraphsEditor(QWidget * parent, const char * name)
// 	:QMainWindow(parent, name){
// 	qgewidget = new QgeWidget( new QCanvas(798, 598), this, "qgewidget") ;
// 	
// 	setCentralWidget(qgewidget) ;
// 	
// 	QToolBar *tb1 = new QToolBar( this );
// 	addDockWindow( tb1, tr( "Menubar" ), Left, FALSE );
// 	setDockEnabled( tb1, Right, FALSE );
// 	qgewidget->getStateComponentActions()->addTo( tb1 ) ;
// 	
// 	QToolBar *tb2 = new QToolBar( this );
// 	addDockWindow( tb2, tr( "Menubar" ), Top, FALSE );
// 	setDockEnabled( tb2, Left, FALSE );
// 	setDockEnabled( tb2, Right, FALSE );
// 	qgewidget->getFileOperations()->addTo( tb2 ) ;
// 	
// 	setUsesBigPixmaps( TRUE ) ;
// }
// 
// GraphsEditor::~GraphsEditor(){
// 	delete qgewidget ;
// }
