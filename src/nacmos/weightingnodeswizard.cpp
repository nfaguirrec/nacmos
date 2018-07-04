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
#include "weightingnodeswizard.h"

#include <iostream>
using namespace std ;

#include <qspinbox.h>
#include <qlineedit.h>
#include <randomtypegenerator.h>
#include <qprogressbar.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qfile.h>
#include <qtable.h>

#include <qgegraph.h>
#include <qgewidget.h>

#include <navigationrule.h>
#include <swapmanager.h>
#include <ViennaRNA.h>
#include <selectingdatabase.h>
			 
/**
 * @brief Constructor
 * @param parent 
 * @param name 
 */
WeightingNodesWizard::WeightingNodesWizard( QWidget* parent, const char* name ):
WeightingNodesWizardBase(parent, name)
{
	qgeWidget->setCanvas( new QCanvas(128, 128) ) ;
	
	tableMotifName->setColumnStretchable(0, TRUE) ;
	tableMotifName->setColumnStretchable(1, TRUE) ;
	
// 	current = currentPage()->name() ;
	current = "Graph Source" ;
	
	viewPrincipalSwapFile = new SwapFileViewer() ;
	viewPrincipalSwapFile->setSwapFileId( SwapManager::PRINCIPAL ) ;
	viewPrincipalSwapFile->setCaption( "NAcMoS - Principal RNAs database" ) ;
	viewPrincipalSwapFile->resize(680, 720) ;
	
	viewTemplatesSwapFile = new SwapFileViewer() ;
	viewTemplatesSwapFile->setSwapFileId( SwapManager::TEMPLATES ) ;
	viewTemplatesSwapFile->setCaption( "NAcMoS - Templates RNAs database" ) ;
	viewTemplatesSwapFile->resize(680, 720) ;
}

/**
 * @brief Destructor
 */
WeightingNodesWizard::~WeightingNodesWizard()
{
}

/**
 * 
 * @param pageName 
 */
void WeightingNodesWizard::currentChanged( const QString& pageName )
{
// 	if( tmpGraph ){
// 		tmpGraph->remove() ;
// 		delete tmpGraph ;
// 	}
	
	if( pageName.compare("Motif Name") == 0 
		&& ( current.compare("Graph Source") == 0 ) 
		   && SwapManager::Instance( SwapManager::PRINCIPAL )->getSize() > 0 ){
		
		fragmenter = new Fragmenter( 0 ) ;
		
		for( int i=0; i < SwapManager::Instance( SwapManager::PRINCIPAL )->getSize(); i++ ){
			
			tmpGraph = new QgeGraph( 0 ) ;
			tmpGraph = SwapManager::Instance( SwapManager::PRINCIPAL )->getGraph( i ) ;
			
			if( tmpGraph ){
				fragmenter->setGraph( tmpGraph ) ;
				fragmenter->run() ;
				SwapManager::Instance( SwapManager::TEMPORARY )->addGraph( fragmenter->getGraph() ) ;
			}
			
		}
		
		/*------------------------------------------------------------------------------------
		* Se actualiza el mapa detraducciï¿½ inicialmente obteniendo los ids del fragmentador y
		* se les asigne a cada uno el nombre ""
		*/
		QValueList<double> listids = fragmenter->getListIds() ;
		for( QValueList<double>::iterator it = listids.begin(); it != listids.end(); it++ ){
			translationMap[*it] = "" ;
		}
		
		/*------------------------------------------------------------------------------------
		* Se busca el primer subgrafo y se muestra en el qgewidget
		*/
		qgeWidget->setGraph( fragmenter->find( *fragmenter->getListIds().begin() ) ) ;
		qgeWidget->getGraph()->move(100, 100) ;
		qgeWidget->canvas()->update() ;
		
		/*------------------------------------------------------------------------------------
		* Finalmente se actualiza la tabla, que lee los valores del mapa de traducciï¿½
		*/
		updateTableMotifNames() ;
		
	}
	
	if( pageName.compare("Weighting Nodes From Motives") == 0 && ( current.compare("Motif Name") == 0 ) ){
		setFinishEnabled( currentPage(), TRUE ) ;
		updateTableProperties() ;
	}
	/*------------------------------------------------------------------------------------
	* Se actualiza el numbre de la pagina en ediciï¿½
	*/
	
	current = pageName ;
}

/**
 * 
 */
void WeightingNodesWizard::run()
{
// 	if( tmpGraph ){
// 		tmpGraph->remove() ;
// 		delete tmpGraph ;
// 	}
	
	if( SwapManager::Instance( SwapManager::TEMPLATES )->getSize() > 0 ){
			
		progressBarSequenceGenerator->reset() ;
		progressBarSequenceGenerator->setTotalSteps( spinBoxHowManySequences->value()*SwapManager::Instance( SwapManager::TEMPLATES )->getSize() ) ;
		progressBarSequenceGenerator->setProgress( 0 ) ;
			
		for( int i = 0; i < SwapManager::Instance( SwapManager::TEMPLATES )->getSize(); i++ ){
			
			tmpGraph = new QgeGraph( 0 ) ;
			tmpGraph = SwapManager::Instance( SwapManager::TEMPLATES )->getGraph(i) ;
			
			if ( tmpGraph ){
				
				RandomTypeGenerator* random = new RandomTypeGenerator( tmpGraph ) ;
			
				for( int j=0; j< spinBoxHowManySequences->value() ; j++ ){
					SwapManager::Instance( SwapManager::PRINCIPAL )->addGraph( random->run( j ) ) ;
					
					progressBarSequenceGenerator->setProgress( (i+1)*(j+1) ) ;
					progressBarSequenceGenerator->update() ;
				}
			
			}
			
		}
	}
}

/**
 * 
 */
void WeightingNodesWizard::viewPrincipalGraphs()
{
	viewPrincipalSwapFile->show() ;
	viewPrincipalSwapFile->update( TRUE ) ;
	lineEditGraphWithSequences->setText( QString::number( SwapManager::Instance( SwapManager::PRINCIPAL )->getSize() ) ) ;
}

/**
 * 
 */
void WeightingNodesWizard::viewTemplateGraphs()
{
	viewTemplatesSwapFile->show() ;
	lineEditDrawGraph->setText( QString::number( SwapManager::Instance( SwapManager::TEMPLATES )->getSize() ) ) ;
	viewTemplatesSwapFile->update( TRUE ) ;
}

/**
 * 
 */
void WeightingNodesWizard::drawGraphs()
{
	AddADrawnGraph* addGraph = new AddADrawnGraph() ;
	addGraph->exec() ;
	lineEditDrawGraph->setText( QString::number( SwapManager::Instance( SwapManager::TEMPLATES )->getSize() ) ) ;
}

/**
 * 
 */
void WeightingNodesWizard::loadGraphs()
{
	QStringList fileNames = QFileDialog::getOpenFileNames( "Graph Modeling Language (*.gml)", ".", this, "fileNames", "Open Files gml" ) ;
	
	if( fileNames.size() != 0 ){
		QgeGraph* tmp = new QgeGraph(0) ;
		
		for( QStringList::iterator it = fileNames.begin(); it != fileNames.end(); it++ ){
			tmp->remove() ;
			tmp->load( *it ) ;
			SwapManager::Instance( SwapManager::TEMPLATES )->addGraph( tmp ) ;
		}
		
		lineEditDrawGraph->setText( QString::number( SwapManager::Instance( SwapManager::TEMPLATES )->getSize() ) ) ;
		delete tmp ;
	}
	
}

/**
 * 
 */
void WeightingNodesWizard::loadLabelGraphs()
{
	QStringList fileNames = QFileDialog::getOpenFileNames( "Graph Modeling Language (*.gml)", ".", this, "fileNames", "Open Files gml with Labels" ) ;
	
	if( fileNames.size() != 0 ){
		QgeGraph* tmp = new QgeGraph(0) ;
		
		for( QStringList::iterator it = fileNames.begin(); it != fileNames.end(); it++ ){
			tmp->remove() ;
			tmp->load( *it ) ;
			SwapManager::Instance( SwapManager::PRINCIPAL )->addGraph( tmp ) ;
		}
		
		lineEditLoadLabelGraphs->setText( QString::number( SwapManager::Instance( SwapManager::PRINCIPAL )->getSize() ) ) ;
		
		delete tmp ;
	}
}

/**
 * 
 */
void WeightingNodesWizard::viewGraphsLoad()
{
	viewPrincipalSwapFile->show() ;
	this->lineEditLoadLabelGraphs->setText( QString::number( SwapManager::Instance( SwapManager::PRINCIPAL )->getSize() ) ) ;
// 	viewPrincipalSwapFile->update( TRUE ) ;
}

/**
 * 
 */
void WeightingNodesWizard::loadGraphsAndSequences()
{
	if ( QMessageBox::warning( this, tr("Warning !!!!"), tr("This option is for massive files.\n\n"
						"If you want to use this option you need to have two type files:\n"
						"First file most have extention \"name\".gml and Second File \n"
						"must have the same name and \"name_?.gml\" where ? is a sequential\n" 
						"number begin one, but if you have only one file by default is \"name\".fasta.\n"
						"Do you want to continue?" ), QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes )
		return ;
	
	/***********************************************************************************
	* Se captura la lista de nombre de archivos en formato gml que contienen los
	* grafos no etiquetados
	*/
	QStringList fileNamesTemplates = QFileDialog::getOpenFileNames( "Graph Modeling Language (*.gml)", ".", this, "fileNames", "Open Files gml" ) ;
	
	// Si el numero de archivos es direferente de cero ...
	if( fileNamesTemplates.size() != 0 ){
	
		// Se crea un grafo temporal
		QgeGraph* tmp = new QgeGraph(0) ;
		
		// Para cada archivo seleccionado en formato gml ...
		for( QStringList::iterator it = fileNamesTemplates.begin(); it != fileNamesTemplates.end(); it++ ){
			
			/**************************************************************************
			* Se remoeven los nodos y las aristas del grafo y se carga el nuevo
			* grafo desde el archivo actualmente en seleccion
			*/
			tmp->remove() ;
			tmp->load( *it ) ;
			
			/**************************************************************************
			* Se le elimina la extensiï¿½ al archivo, para
			* comenzar a generar las posibles extensiones _?.fasta
			*/
			(*it).remove(".gml") ;
			
			for( int i=1; true; i++ ){
			
				QFile file( (*it)+"_"+QString::number(i)+".fasta" ) ;
				
				if( file.exists() ){
				
					file.open( IO_ReadOnly ) ;
					bool arriveEOF = FALSE ;
					
					while( !arriveEOF ){
						arriveEOF = NavigationRule::loadSecuence( file, tmp, "Phosphate" ) ;
						SwapManager::Instance( SwapManager::PRINCIPAL )->addGraph( tmp ) ;
					}
					
					file.close() ;
						
				}else if( i == 1 ){
					
					QFile file2( (*it)+".fasta" ) ;
					
					if( file2.exists() ){
						file2.open( IO_ReadOnly ) ;
						bool arriveEOF = FALSE ;
						
						while( !arriveEOF ){
							arriveEOF = NavigationRule::loadSecuence( file2, tmp, "Phosphate" ) ;
							SwapManager::Instance( SwapManager::PRINCIPAL )->addGraph( tmp ) ;
						}
						
						file2.close() ;
						break ;
					}else{
						file.close() ;
						return ;
					}

				}else{
					file.close() ;
					return ;
				}
				
			}
			
		}
		
		delete tmp ;
		
		lineEditDrawGraph->setText( QString::number( SwapManager::Instance( SwapManager::PRINCIPAL )->getSize() ) ) ;
		lineEditGraphWithSequences->setText( QString::number( SwapManager::Instance( SwapManager::PRINCIPAL )->getSize() ) ) ;
	}
}

/**
 * 
 */
void WeightingNodesWizard::drawGraphsAndSequences()
{
	if ( QMessageBox::warning( this, tr("Warning !!!!"), tr("This option is for massive files.\n\n"
						"If you want to use this option you need to have a file\n"
						"with fasta secuences for draw graph. \n"
						"Do you want continue?" ), QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes )
		return ;
		
	AddADrawnGraph* addGraph = new AddADrawnGraph( TRUE ) ;
	addGraph->exec() ;
	
	lineEditGraphWithSequences->setText( QString::number( SwapManager::Instance( SwapManager::PRINCIPAL )->getSize() ) ) ;
}

/**
 * 
 * @param row 
 */
void WeightingNodesWizard::viewSubgraph( int row )
{
	/*------------------------------------------------------------------------------------------
	* Muestra el subgrafo dado leyendo el externalId de la tabla, luego lo busca en el 
	* fragmentador, lo centra en eï¿½componente ( 100, 100; porque el componente mide 200,200 )
	* y actualiza el canvas
	*/
	double id = tableMotifName->text( row, 0 ).toDouble() ;
	
	qgeWidget->setGraph( fragmenter->find( id ) ) ;
	qgeWidget->getGraph()->move(100, 100) ;
	qgeWidget->canvas()->update() ;
}

/**
 * 
 */
void WeightingNodesWizard::loadNames()
{
	QString fileName = QFileDialog::getOpenFileName(
			".",
			"Translation (*.trans)",
			this,
			"open file dialog",
			"Choose a file" );
	if( fileName != QString::null ){
		cout << "Loading translation id2name from " << fileName << " ... " ;
		
		QFile file( fileName );
		if ( file.open( IO_ReadOnly ) ) {
			QTextStream stream( &file ) ;
			
			double tmpdouble = 0.0 ;
			QString tmpstr ;
			
			bool ok ;
			while( !stream.atEnd() ){
				stream >> tmpdouble ;
				stream >> tmpstr ;
				
				tmpstr.toDouble(&ok) ;
				
				if( translationMap.contains( tmpdouble ) && !ok )
					translationMap[tmpdouble] = tmpstr ;
			}
				
			file.close();
			updateTableMotifNames() ;
		}
		
		cout << "OK" << endl ;
	}
}

/**
 * 
 */
void WeightingNodesWizard::updateTableMotifNames()
{
	for( int i=0; i < tableMotifName->numRows(); i++ )
		tableMotifName->removeRow( i ) ;
		
	tableMotifName->setNumRows( translationMap.size() ) ;
		
	int i=0 ;
	for( QMap<double, QString>::iterator it = translationMap.begin(); it != translationMap.end(); it++, i++ ){
		tableMotifName->setText( i, 0, QString::number( it.key(), 'f', 1 ) ) ;
		tableMotifName->setText( i, 1, it.data() ) ;
	}
}

/**
 * 
 */
void WeightingNodesWizard::getNameGQT(){
	
	QValueList<double> listids = fragmenter->getListIds() ;
	
	int i = 0 ;
	for( QValueList<double>::iterator it = listids.begin(); it != listids.end(); it++, i++ ){
		translationMap[*it] = getNameGQT( fragmenter->find(*it) ) ;
	}
	
	updateTableMotifNames() ;
}

/**
 * 
 * @param qgegraph 
 * @todo Borrar todas las lineas que contienen hidrogeno2 que están comentadas, una vez se esté seguro
 * @return 
 */
QString WeightingNodesWizard::getNameGQT( QgeGraph* qgegraph )
{
	QString name = "" ;
	
	/*----------------------------------------------------------------------------------------------
	* Se busca el nodo seleccionado
	*/
	QgeNode* nodeSelected ;
	for( graph::node_iterator it = qgegraph->nodes_begin(); it != qgegraph->nodes_end(); it++ ){
		if( qgegraph->nodeToQgeNode(*it)->isSelected() ){
			nodeSelected = qgegraph->nodeToQgeNode(*it) ;
		}
	}
	
	/*----------------------------------------------------------------------------------------------
	* Se recorren los nodos del subgrafo y se almacenas enlas siguientes variables
	*/
	QgeNode* backInP ;
	QgeNode* backInPH ;
	QgeNode* nextOutP ;
	QgeNode* nextOutPH ;
	QgeNode* outH ;
	bool hidrogenOut ;
	
	/*----------------------------------------------------------------------------------------------
	* Se busacan los nodos antrior y siguiente del nodo seleccionado por la ruta fosfato
	*/
	backInP = getNode( nodeSelected, qgegraph, "Phosphate", 1 ) ;
	nextOutP = getNode( nodeSelected, qgegraph, "Phosphate", -1 ) ;
	//----------------------------------------------------------------------------------------------
	
	/*----------------------------------------------------------------------------------------------
	* Se pregunta si las aristas de hidrogeno salen o entran, primero pregunatando al nodo
	* seleccionado y posteriormente a los nodos de hidrogeneo anteriores y siguiente de la ruta
	* fosfato. para cada uno primero preguntando por los dos tipos de aristas hidrogeno
	*/
	outH =  getNode( nodeSelected, qgegraph, "Hydrogen", -1 ) ;
	outH ? (hidrogenOut = TRUE) : (hidrogenOut = FALSE) ;
	
	if( outH == 0 ){
		backInPH =  getNode( backInP, qgegraph, "Hydrogen", -1 ) ;
		backInPH ? (hidrogenOut = TRUE) : (hidrogenOut = FALSE) ;
		
// 		if( backInPH == 0 ){
// 			backInPH =  getNode( backInP, qgegraph, "Hydrogen2", -1 ) ;
// 			backInPH ? (hidrogenOut = TRUE) : (hidrogenOut = FALSE) ;
			
			if( backInPH ==0 ){
				nextOutPH =  getNode( nextOutP, qgegraph, "Hydrogen", -1 ) ;
				nextOutPH ? (hidrogenOut = TRUE) : (hidrogenOut = FALSE) ;
				
// 				if( nextOutPH == 0 ){
// 					nextOutPH =  getNode( nextOutP, qgegraph, "Hydrogen2", -1 ) ;
// 					nextOutPH ? (hidrogenOut = TRUE) : (hidrogenOut = FALSE) ;
// 				}
			}
// 		}
	}
	//----------------------------------------------------------------------------------------------
	
	// Si el nodo de intereés tiene un enlace de hidrogeno ...
	if( hidrogenOut ){
		outH =  getNode( nodeSelected, qgegraph, "Hydrogen", -1 ) ;
// 		if( outH == 0 )
// 			outH =  getNode( nodeSelected, qgegraph, "Hydrogen2", -1 ) ;
		
		if( backInP ){
			backInPH = getNode( backInP, qgegraph, "Hydrogen", -1 ) ;
		
		
		
// 			if( backInPH == 0 )
// 				backInPH = getNode( backInP, qgegraph, "Hydrogen2", -1 ) ;
		}
		else{ 
			backInPH = getNode( outH, qgegraph, "Phosphate", -1 ) ;
		}
		
		if( nextOutP ){
			nextOutPH = getNode( nextOutP, qgegraph, "Hydrogen", -1 ) ;
// 			if( nextOutPH == 0 )
// 				nextOutPH = getNode( nextOutP, qgegraph, "Hydrogen2", -1 ) ;
		}
		else{ 
			backInPH = getNode( outH, qgegraph, "Phosphate", 1 ) ;
		}
	}else{
		outH =  getNode( nodeSelected, qgegraph, "Hydrogen", 1 ) ;
// 		if( outH == 0 )
// 			outH =  getNode( nodeSelected, qgegraph, "Hydrogen2", 1 ) ;
		
		if( backInP ){
			backInPH = getNode( backInP, qgegraph, "Hydrogen", 1 ) ;
// 			if( backInPH == 0 )
// 				backInPH = getNode( backInP, qgegraph, "Hydrogen2", 1 ) ;
		}
			
		if( nextOutP ){
			nextOutPH = getNode( nextOutP, qgegraph, "Hydrogen", 1 ) ;
// 			if( nextOutPH == 0 )
// 				nextOutPH = getNode( nextOutP, qgegraph, "Hydrogen2", 1 ) ;
		}
	}
	
	QString value[6] ;
	backInP ? ( value[0] = QString( backInP->getLabel()) ) : ( value[0] = "x" ) ;
	nodeSelected ? ( value[1] = QString( nodeSelected->getLabel()) ) : ( value[1] = "x" ) ;
	nextOutP ? ( value[2] = QString( nextOutP->getLabel()) ) : ( value[2] = "x" ) ;
	nextOutPH ? ( value[3] = QString( nextOutPH->getLabel()) ) : ( value[3] = "x" ) ;
	outH ? ( value[4] = QString( outH->getLabel()) ) : ( value[4] = "x" ) ;
	backInPH ? ( value[5] = QString( backInPH->getLabel()) ) : ( value[5] = "x" ) ;
	
	switch( qgegraph->number_of_nodes() ){
		case 6:
			for( int i=0; i<6; i++ ) name += value[i] ;
			
			if( hidrogenOut ){
				name += QString::number(2) ;
			}else{
				name = QString(name[3])+QString(name[4])+QString(name[5])+
					QString(name[0])+QString(name[1])+QString(name[2])+QString::number(5) ;
			}
			break ;
		case 5:
			for( int i=0; i<6; i++ ){
				name += value[i] ;
			}
			
			if( hidrogenOut ){
				name += QString::number(2) ;
			}else{
				name = QString(name[3])+QString(name[4])+QString(name[5])+
					QString(name[0])+QString(name[1])+QString(name[2])+QString::number(5) ;
			}
			break ;
		case 4:
			for( int i=0; i<6; i++ ) name += value[i] ;
			
			if( hidrogenOut ){
				name += QString::number(2) ;
			}else{
				name = QString(name[3])+QString(name[4])+QString(name[5])+
					QString(name[0])+QString(name[1])+QString(name[2])+QString::number(5) ;
			}
			break ;
		case 3:
			name = value[0]+value[1]+value[2]+"xxx2" ;
			break ;
			
		default:
			name = "" ;
			break ;
	}
	
	if( ( name.stripWhiteSpace().length() == 6 ) && ( QString(name[0]).compare("x") == 0 ) ){
		name = QString::null ;
		if( outH ){
			backInPH = getNode( outH, qgegraph, "Phosphate", -1 ) ;
			
			if( backInPH ){
				value[5] = backInPH->getLabel() ;
			
				for( int i=0; i<6; i++ ) name += value[i] ;
				name += QString::number(2) ;
			}
		}
	}
	
	if( name.contains("xxxx") > 0 ){
		name = QString::null ;
		
		QgeNode* nextInPInP = getNode( backInP, qgegraph, "Phosphate", 1 ) ;
		QString tmp = nextInPInP->getLabel() ;
		
		name = tmp+value[0]+value[1]+"xxx3" ;
	}
	
	return name ;
}

/**
 * 
 * @param qgenode 
 * @param qgegraph 
 * @param itemtypeName 
 * @param direction 
 * @return 
 */
QgeNode* WeightingNodesWizard::getNode( QgeNode* qgenode, QgeGraph* qgegraph, QString itemtypeName, int direction )
{
	if( direction == 1 && qgenode ){
		for( node::in_edges_iterator it = qgenode->in_edges_begin(); it != qgenode->in_edges_end(); it++ ){
			if( qgegraph->edgeToQgeEdge(*it)->getType().getName().contains( itemtypeName ) != 0 ){
				return qgegraph->nodeToQgeNode( (*it).opposite( *(node*)qgenode ) ) ;
			}
		}
	}else if( direction == -1 && qgenode ){
		for( node::out_edges_iterator it = qgenode->out_edges_begin(); it != qgenode->out_edges_end(); it++ ){
			if( qgegraph->edgeToQgeEdge(*it)->getType().getName().contains( itemtypeName ) != 0 ){
				return qgegraph->nodeToQgeNode( (*it).opposite( *(node*)qgenode ) ) ;
			}
		}
	}
	
	return 0 ;
}

/**
 * 
 */
void WeightingNodesWizard::loadProperties()
{
	weightsMap.clear() ;
	nameColumns.clear() ;
	
	QString fileName = QFileDialog::getOpenFileName(
			".",
			"Weight dates (*.dat)",
			this,
			"open file dialog",
			"Choose a file" );
	if( fileName != QString::null ){
		cout << "Loading Weight dates from " << fileName << " ... " ;
		
		QFile file( fileName );
		if ( file.open( IO_ReadOnly ) ) {
			QTextStream stream( &file ) ;
			
			/*-----------------------------------------------------------------
			* Se lee el numero de columnas y el numbre para tales columnas
			*/
			int numberColumns ;
			stream >> numberColumns ;
			
			QString tmpstr ;
			for( int i=0; i<= numberColumns; i++ ){
				stream >> tmpstr ;
				nameColumns.append( tmpstr.remove("<").remove(">") ) ;
			}
			//-----------------------------------------------------------------
			
			QValueList<double> tmpdoublelist ;
			double tmpdouble ;
			while( !stream.atEnd() ){
				stream >> tmpstr ;
				
				tmpdoublelist.clear() ;
				for( int i=0; i< numberColumns; i++ ){
					stream >> tmpdouble ;
					tmpdoublelist.append( tmpdouble ) ;
				}
				
				QStringList namelist = translationMap.values() ;
				if( namelist.contains( tmpstr.stripWhiteSpace() ) > 0 ){
					weightsMap[ tmpstr.stripWhiteSpace() ] = tmpdoublelist ;
				}
			}
				
			file.close();
			
			nameColumns.push_front("Internal Id") ;
			tableProperties->setNumCols( nameColumns.size() ) ;
			tableProperties->setColumnLabels( nameColumns ) ;
			
			updateTableProperties() ;
		}
		
		cout << "OK" << endl ;
	}
}

/**
 * 
 */
void WeightingNodesWizard::removeProperty()
{
}

/**
 * 
 */
void WeightingNodesWizard::addProperty()
{
}

/**
 * Este SLOT es llamado cuando el boton finish es presionado
 */
void WeightingNodesWizard::accept()
{
	
	SelectingDatabase* selectingdatabaseDialog = new SelectingDatabase() ;
	
	if( selectingdatabaseDialog->exec() == SelectingDatabase::Accepted ){
		
		int option = selectingdatabaseDialog->getSelectedDatabase() ;
		delete selectingdatabaseDialog ;
		
		if( option == SelectingDatabase::PRINCIPAL || option == SelectingDatabase::BOTH )
			SwapManager::Instance( SwapManager::PRINCIPAL )->clear() ;
		
		if( option == SelectingDatabase::SECONDARY || option == SelectingDatabase::BOTH )
			SwapManager::Instance( SwapManager::SECONDARY )->clear() ;
			
		QgeGraph* graphtmp = new QgeGraph(0) ;
		
		for( int i=0; i<SwapManager::Instance( SwapManager::TEMPORARY )->getSize(); i++ ){
			graphtmp = SwapManager::Instance( SwapManager::TEMPORARY )->getGraph(i) ;
			
			if( graphtmp ){
				for( graph::node_iterator it2 = graphtmp->nodes_begin(); it2 != graphtmp->nodes_end(); it2++ ){
					
					if( translationMap.contains( graphtmp->nodeToQgeNode(*it2)->getExternalId() ) ){
						
						for( uint i = 0; i < nameColumns.size()-2; i++ ){
							QString name = translationMap[ graphtmp->nodeToQgeNode(*it2)->getExternalId() ] ;
							
							if( weightsMap.contains( name ) ){
								graphtmp->nodeToQgeNode(*it2)->addWeight( nameColumns[i+2], weightsMap[name][i]) ;
							}else{
								graphtmp->nodeToQgeNode(*it2)->addWeight( nameColumns[i+2], 0.0) ;
							}
						}
						
					} 
				}
				if( option == SelectingDatabase::PRINCIPAL || option == SelectingDatabase::BOTH )
					SwapManager::Instance( SwapManager::PRINCIPAL )->addGraph( graphtmp ) ;
				
				if( option == SelectingDatabase::SECONDARY || option == SelectingDatabase::BOTH )
					SwapManager::Instance( SwapManager::SECONDARY )->addGraph( graphtmp ) ;
				
			}
			
		}
		
		close() ;
		
		emit finished() ;
	}
}

/**
 *
 */
void WeightingNodesWizard::updateTableProperties()
{
	for( int i=0; i < tableProperties->numRows(); i++ )
		tableProperties->removeRow( i ) ;
		
	tableProperties->setNumRows( translationMap.size() ) ;
		
	int i=0 ;
	for( QMap<double, QString>::iterator it = translationMap.begin(); it != translationMap.end(); it++, i++ ){
		tableProperties->setText( i, 0, QString::number( it.key(), 'f', 1 ) ) ;
		tableProperties->setText( i, 1, it.data() ) ;
		
		if( weightsMap.contains(it.data()) > 0 ){
			int j=0 ;
			for( QValueList<double>::iterator it2 = weightsMap[it.data()].begin(); it2 != weightsMap[it.data()].end(); it2++, j++ ){
				tableProperties->setText( i, 2+j, QString::number(*it2, 'f', 5) ) ;
			}
		}
	}
}

void WeightingNodesWizard::viewGraphsViennaRNA()
{
	viewPrincipalSwapFile->show() ;
	viewPrincipalSwapFile->update( TRUE ) ;
	this->lineEditLoadFastaViennaRNA->setText( QString::number( SwapManager::Instance( SwapManager::PRINCIPAL )->getSize() ) ) ;
}

void WeightingNodesWizard::loadFastaViennaRNA()
{
	QMessageBox* message = new QMessageBox() ;
	message->setTextFormat( Qt::RichText ) ;
	message->setIcon( QMessageBox::Information ) ;
	
	message->setText(
			tr(
			"<div style=\"text-align: center;\">"
				"<big><big><big>"
					"<span style=\"font-weight: bold;\">"
						"Vienna RNA Package"
					"</span>"
				"</big></big></big>"
				"<br>"
				"<big>"
					"<span style=\"font-weight: bold;\">"
						"RNA Secondary Structure Prediction and Comparison"
					"</span>"
				"</big>"
			"</div>"
			"<br>"
			"These secondary structures will be calculated.\n\n"
			"whit Vienna Package by using default parameters.\n"
			  )
		) ;
	
	message->exec() ;
		
	QStringList fileNames = QFileDialog::getOpenFileNames( "Fasta (*.fasta)", ".", this, "fileNames", "Opening Fasta Files" ) ;
	
	
	if( fileNames.size() != 0 ){
		ViennaRNA vienna ;
		vienna.setFastaFiles( fileNames) ;
		vienna.exec() ;
	}
	this->lineEditLoadFastaViennaRNA->setText( QString::number( SwapManager::Instance( SwapManager::PRINCIPAL )->getSize() ) ) ;
}

