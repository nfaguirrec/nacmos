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
#include "SwapFile.h"

#include <qmap.h>

/**
 * @brief Constructor
 * @param filename Nombre del archivo que va a hacer el papel de swapfile
 * @param debug Flag que permite mostrar todo el procedimiento de busqueda y almacenamiento en modo de depuración
 */
SwapFile::SwapFile( const QString& filename, bool debug, bool fileNew )
{
	this->debug = debug ;
	this->filename = filename ;
	
	if( fileNew )
		system( "rm "+filename ) ;
	
	cout << "INPUT_FILE = " << filename << endl ;

	file = fopen( filename.latin1(), "a+" ) ;

	if ( file )
		cout << "Successfully open" << endl << endl ;
	else {
		cout << "Error opening file" << endl ;
		return ;
	}
	
	currentGraph = new QgeGraph(0) ;
}

/**
 * @brief Destructor
 */
SwapFile::~SwapFile()
{
}

/**
 * Adiciona un grafo al SwapFile
 * @param graph Grafo a ser adicionado
 * @return Identificador asignado al grafo adicionado
 */
int SwapFile::addGraph( QgeGraph* graph )
{
	currentGraph->remove() ;
	
	fpos_t position ;
	fgetpos( file, &position ) ;
	idToPosFile.append( position ) ;
	
	fprintf( file, "<graph>\n" ) ;
	
	fprintf( file, "\t<nodeTypes>\n") ;
	QMap<QString, QgeItemType> nodeTypes = graph->getNodeTypes() ;
	
	for( QMap<QString, QgeItemType>::iterator it = nodeTypes.begin(); it != nodeTypes.end(); it++ ){
		fprintf( file, "\t\t%s ", it.key().latin1() ) ;
		
		QValueList<double> values = it.data().getValues() ;
		for( QValueList<double>::iterator it2 = values.begin(); it2 != values.end(); it2++ ){
			fprintf( file, "%3.5f ", *it2 ) ;
		}
		
		fprintf( file, "\n" ) ;
	}
	fprintf( file, "\t</nodeTypes>\n") ;
	
	fprintf( file, "\t<edgeTypes>\n") ;
	QMap<QString, QgeItemType> edgeTypes = graph->getEdgeTypes() ;
	
	for( QMap<QString, QgeItemType>::iterator it = edgeTypes.begin(); it != edgeTypes.end(); it++ ){
		fprintf( file, "\t\t%s ", it.key().latin1() ) ;
		
		QValueList<double> values = it.data().getValues() ;
		for( QValueList<double>::iterator it2 = values.begin(); it2 != values.end(); it2++ ){
			fprintf( file, "%3.5f ", *it2 ) ;
		}
		
		fprintf( file, "\n" ) ;
	}
	fprintf( file, "\t</edgeTypes>\n") ;
	
	fprintf( file, "\tlabel=\"%s\"\n", graph->getLabel().latin1() ) ;
	fprintf( file, "\tdirected=%d\n", ( graph->is_directed() ? 1 : 0 ) ) ;
	fprintf( file, "\tnumberOfNodes=%d\n", graph->number_of_nodes() ) ;
	fprintf( file, "\tnumberOfEdges=%d\n", graph->number_of_edges() ) ;
	
	for( graph::node_iterator it = graph->nodes_begin(); it != graph->nodes_end(); ++it){
		fprintf( file, "\t<node>\n" ) ;
		
		fprintf( file, "\t\tid=%d\n", (*it).id() ) ;
		fprintf( file, "\t\tlabel=\"%s\"\n", graph->nodeToQgeNode(*it)->getLabel().latin1() ) ;
// 		fprintf( file, "\t\tlabel=\"%c\"\n", graph->nodeToQgeNode(*it)->getType().getName()[0].latin1() ) ;
		fprintf( file, "\t\ttype=\"%s\"\n", graph->nodeToQgeNode(*it)->getType().getName().latin1() ) ;
		fprintf( file, "\t\tselected=%d\n", ( graph->nodeToQgeNode(*it)->isSelected() ? 1 : 0 ) ) ;
		fprintf( file, "\t\tneighborRange=%3.5f\n", graph->nodeToQgeNode(*it)->getNeighborRange() ) ;
		fprintf( file, "\t\tdisableChangeType=%d\n", ( graph->nodeToQgeNode(*it)->isDisableChangeType() ? 1 : 0 ) ) ;
		fprintf( file, "\t\tisFirst=%d\n", ( graph->nodeToQgeNode(*it)->isFirst() ? 1 : 0 ) ) ;
		fprintf( file, "\t\texternalId=%.4f\n", graph->nodeToQgeNode(*it)->getExternalId() ) ;
		
		fprintf( file, "\t\t<graphics>\n" ) ;
		fprintf( file, "\t\t\tcolorSelected=\"%s\"\n", graph->nodeToQgeNode(*it)->getColorSelected().name().latin1() ) ;
		fprintf( file, "\t\t\tcolorUnselected=\"%s\"\n", graph->nodeToQgeNode(*it)->getColorUnSelected().name().latin1() ) ;
		fprintf( file, "\t\t\tcolorName=\"%s\"\n", graph->nodeToQgeNode(*it)->getColorLabel().name().latin1() ) ;
		fprintf( file, "\t\t\tx=%d\n", (int)graph->nodeToQgeNode(*it)->x() ) ;
		fprintf( file, "\t\t\ty=%d\n", (int)graph->nodeToQgeNode(*it)->y() ) ;
		fprintf( file, "\t\t\tw=%d\n", (int)graph->nodeToQgeNode(*it)->getWidth() ) ;
		fprintf( file, "\t\t\th=%d\n", (int)graph->nodeToQgeNode(*it)->getHeight() ) ;
		fprintf( file, "\t\t</graphics>\n" ) ;
		
		fprintf( file, "\t\t<weights>\n" ) ;
		for( QMap<QString, double>::iterator it2 = graph->nodeToQgeNode(*it)->getWeights().begin() ; it2 != graph->nodeToQgeNode(*it)->getWeights().end(); it2++ )
			fprintf( file, "\t\t\t%s=%f\n", it2.key().latin1(), it2.data() ) ;
		fprintf( file, "\t\t</weights>\n" ) ;
		
		fprintf( file, "\t</node>\n" ) ;
	}
	
	for( graph::edge_iterator it = graph->edges_begin(); it != graph->edges_end(); ++it){
		fprintf( file, "\t<edge>\n" ) ;
		
		fprintf( file, "\t\tsource=%d\n", (*it).source().id() ) ;
		fprintf( file, "\t\ttarget=%d\n", (*it).target().id() ) ;
		fprintf( file, "\t\tlabel=\"%s\"\n", graph->edgeToQgeEdge(*it)->getLabel().latin1() ) ;
		fprintf( file, "\t\ttype=\"%s\"\n", graph->edgeToQgeEdge(*it)->getType().getName().latin1() ) ;
		fprintf( file, "\t\tselected=%d\n", ( graph->edgeToQgeEdge(*it)->isSelected() ? 1 : 0 ) ) ;
		fprintf( file, "\t\tdisableChangeType=%d\n", ( graph->edgeToQgeEdge(*it)->isDisableChangeType() ? 1 : 0 ) ) ;
		
		fprintf( file, "\t\t<graphics>\n" ) ;
		fprintf( file, "\t\t\tcolorSelected=\"%s\"\n", graph->edgeToQgeEdge(*it)->getColorSelected().name().latin1() ) ;
		fprintf( file, "\t\t\tcolorUnselected=\"%s\"\n", graph->edgeToQgeEdge(*it)->getColorUnSelected().name().latin1() ) ;
		fprintf( file, "\t\t\tcolorName=\"%s\"\n", graph->edgeToQgeEdge(*it)->getColorLabel().name().latin1() ) ;
		fprintf( file, "\t\t\tw=%d\n", graph->edgeToQgeEdge(*it)->getWidth() ) ;
		fprintf( file, "\t\t</graphics>\n" ) ;
		
		fprintf( file, "\t\t<weights>\n" ) ;
		for( QMap<QString, double>::iterator it2 = graph->edgeToQgeEdge(*it)->getWeights().begin() ; it2 != graph->edgeToQgeEdge(*it)->getWeights().end(); it2++ )
			fprintf( file, "\t\t\t%s=%f\n", it2.key().latin1(), it2.data() ) ;
		fprintf( file, "\t\t</weights>\n" ) ;
		
		fprintf( file, "\t</edge>\n" ) ;
	}
	
	fprintf( file, "</graph>\n" ) ;
	
	fflush( file ) ;
	
	if( getSize() == 1 )
		emit hasChanged( TRUE ) ;
	else
		emit hasChanged( FALSE ) ;
	
	return size ;
}

/**
 * Retorna un apuntador al grafo cuyo identificador en el SwapFile corresponde a id.
 * Nota-Se debe copiar el grafo retornado, es decir
 * @code QgeGraph* g = new QgeGraph( new QCanvas(800, 600) ) ;
 * @code g->copy( swapFile.getGraph( 5 ) ) ;
 * @param id Identificador del grafo requerido
 * @return Apuntador al grafo
 */
QgeGraph* SwapFile::getGraph( uint id )
{
	if( id < idToPosFile.size() && idToPosFile.size() != 0 ){
		currentGraph->remove() ;
		loadGraph( id ) ;
	}
		
	QgeGraph* tmp = new QgeGraph(0) ;
	tmp->copy( currentGraph ) ;
	
	return tmp ;
}

/**
 * Retorna el número de grafos que existen en el SwapFile
 * @return Numero de grafos en el SwapFile
 */
int SwapFile::getSize()
{
	return idToPosFile.size() ;
}

/**
 * Almacena el dato del tipo QString a la clave asociada key, es decir:
 * Suponiendo que en el archivo se encuentra ...
 * @code type="Hola mundo"
 * ... entonces se cargará con el comando ...
 * @code QString data ;
 * @code loadPair( stream, "type", data, numberline ) ;
 * ... así a data se le asignará el valor de "Hola mundo"
 * @internal
 * @param stream Referencia al stream a ser leido
 * @param key Clave tipo QString a ser almacenada
 * @param data Referencia al dato para almacenar la información encontrada
 * @param numberline Referencia al número de linea, para llevar la cuenta de numero de lineas recorridas
 */
void SwapFile::loadPair( QString key, QString &data )
{
	char* token = new char[255];
	char* tmpKey ;
	
	QString tokenTmp ;
	
	do{
		fscanf( file, "%s", token ) ;
		
		tokenTmp = QString( token ) ;
		
		tmpKey = strtok( token, "=" ) ;
		if( strcmp( key.latin1(), tmpKey ) == 0 ){
			data = QString( strtok(NULL, "=") ) ;
		}else{
			data += QString(" ")+QString(token) ;
		}
	}while( tokenTmp[ tokenTmp.length() - 1 ] != '\"'  ) ;
	
	data.remove('\"') ;
	
	if ( debug ) cout << "\t"+key+" = " << data << endl ;
}

/**
 * @see loadPair( QTextStream &stream, QString key, QString &data, int &numberline )
 * @internal
 * @param stream Referencia al stream a ser leido
 * @param key Clave tipo QString a ser almacenada
 * @param data Referencia al dato para almacenar la información encontrada
 * @param numberline Referencia al número de linea, para llevar la cuenta de numero de lineas recorridas
 */
void SwapFile::loadPairInt( QString key, int &data )
{
	char* token = new char[255];
	fscanf( file, "%s", token ) ;
	char* tmpKey = strtok( token, "=" ) ;
	
	if( strcmp( key.latin1(), tmpKey ) == 0 )
		data = atoi( strtok(NULL, "=") ) ;
	
	if ( debug ) cout << "\t"+key+" = " << data << endl ;
}


/**
 * @see loadPair( QTextStream &stream, QString key, QString &data, int &numberline )
 * @param stream Referencia al stream a ser leido
 * @param key Clave tipo QString a ser almacenada
 * @param data Referencia al dato para almacenar la información encontrada
 * @param numberline Referencia al número de linea, para llevar la cuenta de numero de lineas recorridas
 */
void SwapFile::loadPairDouble( QString key, double &data )
{
	char* token = new char[255];
	fscanf( file, "%s", token ) ;
	char* tmpKey = strtok( token, "=" ) ;
	
	if( strcmp( key.latin1(), tmpKey ) == 0 )
		data = atof( strtok(NULL, "=") ) ;
		
	if ( debug ) cout << "\t"+key+" = " << setprecision(20) << data << endl ;
}

/**
 * @internal
 * Carga desde el SwapFile el grafo que tiene asociado el identificador id directamente
 * sobre el atributo de clase currentGraph
 * @param id Id asociado al grafo que será buscado en el archivo
 */
void SwapFile::loadGraph( uint id )
{
	char* token = new char[255] ;
	
	fsetpos( file, &idToPosFile[id] ) ;
	
	do {
		fscanf( file, "%s", token ) ;
			
		if ( strcmp( "<graph>", token ) == 0 ) {
			if ( debug ) {
				cout << "-------------------------------------------" << endl ;
				cout << " Loading Graph ...                          " << endl ;
				cout << "-------------------------------------------" << endl ;
			}
			
			ItemData ndata ;
			
			QValueList<QgeItemType> nTypes = loadTypes( NODE ) ;
			for( QValueList<QgeItemType>::iterator it = nTypes.begin(); it != nTypes.end(); it++ )
				currentGraph->addNodeType( *it ) ;
				
			QValueList<QgeItemType> eTypes = loadTypes( EDGE ) ;
			for( QValueList<QgeItemType>::iterator it = eTypes.begin(); it != eTypes.end(); it++ )
				currentGraph->addEdgeType( *it ) ;
			
			loadPair( "label", ndata.name ) ;
			loadPairInt( "directed", ndata.directed ) ;
			loadPairInt( "numberOfNodes", ndata.numberOfNodes ) ;
			loadPairInt( "numberOfEdges", ndata.numberOfEdges ) ;
			
			currentGraph->setLabel( ndata.name ) ;
			ndata.directed ? currentGraph->make_directed() : currentGraph->make_undirected() ;
			
			if ( debug ) cout << endl ;
			
			QMap<int, QgeNode*> nodeMap ;
			for( int i=0; i < ndata.numberOfNodes; i++ ){
				SwapFile::ItemData data = loadNode() ;
				
				QgeNode* n = currentGraph->newNode( data.graphicsdata.x, data.graphicsdata.y ) ;
				nodeMap[data.id] = n ;
				n->setLabel( data.name ) ;
				
				for( QValueList<QgeItemType>::iterator it = nTypes.begin(); it != nTypes.end(); it++ )
					if( (*it).getName().compare( data.type ) == 0 )
						n->setType( *it ) ;
						
				n->setSelected( ( ( data.selected == 1 ) ? TRUE : FALSE ) ) ;
				n->setNeighborRange( data.neighborRange ) ;
				n->setDisableChangeType( ( ( data.disableChangeType == 1 ) ? TRUE : FALSE ) ) ;
				n->setFirst( ( ( data.isFirst == 1 ) ? TRUE : FALSE ) ) ;
				n->setExternalId( data.externalId ) ;
				n->setColorSelected( QColor( data.graphicsdata.colorSelected ) ) ;
				n->setColorUnSelected( QColor( data.graphicsdata.colorUnselected ) ) ;
				n->setColorLabel( QColor( data.graphicsdata.colorName ) ) ;
				n->setWidth( data.graphicsdata.w ) ;
				n->setHeight( data.graphicsdata.h ) ;
				
				for( QMap<QString, double>::iterator it = data.weightsdata.data.begin(); it != data.weightsdata.data.end(); it++ )
					n->addWeight( it.key(), it.data() ) ;
			}
				
			if ( debug ) cout << endl ;
			
			for( int i=0; i < ndata.numberOfEdges; i++ ){
				SwapFile::ItemData data = loadEdge() ;
				QgeEdge* e = currentGraph->newEdge( nodeMap[data.source], nodeMap[data.target] ) ;
				
				e->setLabel( data.name ) ;
				
				for( QValueList<QgeItemType>::iterator it = eTypes.begin(); it != eTypes.end(); it++ )
					if( (*it).getName().compare( data.type ) == 0 )
						e->setType( *it ) ;

				e->setSelected( ( ( data.selected == 1 ) ? TRUE : FALSE ) ) ;
				e->setDisableChangeType( ( ( data.disableChangeType == 1 ) ? TRUE : FALSE ) ) ;
				e->setColorSelected( QColor( data.graphicsdata.colorSelected ) ) ;
				e->setColorUnSelected( QColor( data.graphicsdata.colorUnselected ) ) ;
				e->setColorLabel( QColor( data.graphicsdata.colorName ) ) ;
				e->setWidth( data.graphicsdata.w ) ;
				
				for( QMap<QString, double>::iterator it = data.weightsdata.data.begin(); it != data.weightsdata.data.end(); it++ )
					e->addWeight( it.key(), it.data() ) ;
			}

			if ( debug ) cout << endl ;
		}
		
	} while ( strcmp( "</graph>", token ) != 0 ) ;
}

/**
 * Remueve el grafo identidicado con el número id
 * @param id identificador del grafo a ser removido
 */
void SwapFile::remove( uint id )
{
	idToPosFile.remove( idToPosFile.at( id ) )  ;
	emit hasChanged() ;
}

void SwapFile::clear()
{
	idToPosFile.clear() ;
	fclose( file ) ;
	std::remove( filename ) ;
	
	cout << "Clean swapfile " << filename << endl ;

	file = fopen( filename.latin1(), "a+" ) ;

	if ( file )
		cout << "Successfully open" << endl << endl ;
	else {
		cout << "Error opening file" << endl ;
		return ;
	}
	
	emit hasChanged() ;
}

/**
 * Actualiza las referencias, es decir asocia al identicador de cada grafo
 * un apuntador a la sección del archivo
 */
void SwapFile::updateReferences()
{
	char tmp[255] ;
	fpos_t position ;
	
	fclose( file ) ;
	idToPosFile.clear() ;
	
	file = fopen( filename, "r" ) ;
	
	while( !feof( file ) ){
		fgetpos( file, &position ) ;
		fscanf( file, "%s", tmp ) ;
		
		if( string(tmp).compare("<graph>") == 0 ){
			idToPosFile.append( position ) ;
		}
	}
}

// void SwapFile::edit( uint id, QgeGraph* graph )
// {
// 	
// }

/**
 * Carga los atributos de un nodo desde el SwapFile
 * @internal
 * @param stream Referencia al stream a ser leido
 * @param numberline Referencia al número de linea, para llevar la cuenta de numero de lineas recorridas
 * @return Los datos asociados al nodo encapsulados en una estructura del tipo @see ItemData
 */
struct SwapFile::ItemData SwapFile::loadNode()
{
	ItemData ndata ;
	char* token = new char[255];
	
	do {
		fscanf( file, "%s", token ) ;

		if ( strcmp( "<node>", token ) == 0 ) {
			if ( debug ) {
				cout << "-------------------------------------------" << endl ;
				cout << " Loading Node ...                          " << endl ;
				cout << "-------------------------------------------" << endl ;
			}
			
			loadPairInt( "id", ndata.id ) ;
			loadPair( "label", ndata.name ) ;
			loadPair( "type", ndata.type ) ;
			
			if( ndata.name.length() == 0 || ndata.name.compare("Default") == 0 )
				ndata.name = ndata.type[0] ;
			
			loadPairInt( "selected", ndata.selected ) ;
			loadPairDouble( "neighborRange", ndata.neighborRange ) ;
			loadPairInt( "disableChangeType", ndata.disableChangeType ) ;
			loadPairInt( "isFirst", ndata.isFirst ) ;
			loadPairDouble( "externalId", ndata.externalId ) ;
			ndata.graphicsdata = loadGraphics( NODE ) ;
			ndata.weightsdata = loadWeights() ;
			
			if ( debug ) cout << endl ;
		}
		
	} while ( strcmp( "</node>", token ) != 0 ) ;
	
	return ndata ;
}

/**
 * Carga los atributos de una arista desde el SwapFile
 * @internal
 * @param stream Referencia al stream a ser leido
 * @param numberline Referencia al número de linea, para llevar la cuenta de numero de lineas recorridas
 * @return Los datos asociados a la arista encapsulados en una estructura del tipo @see ItemData
 */
struct SwapFile::ItemData SwapFile::loadEdge()
{
	ItemData ndata ;
	char* token = new char[255];
	
	do {
		fscanf( file, "%s", token ) ;

		if ( strcmp( "<edge>", token ) == 0 ) {
			if ( debug ) {
				cout << "-------------------------------------------" << endl ;
				cout << " Loading Edge ...                          " << endl ;
				cout << "-------------------------------------------" << endl ;
			}
			
			loadPairInt( "source", ndata.source ) ;
			loadPairInt( "target", ndata.target ) ;
			loadPair( "label", ndata.name ) ;
			loadPair( "type", ndata.type ) ;
			loadPairInt( "selected", ndata.selected ) ;
			loadPairInt( "disableChangeType", ndata.disableChangeType ) ;
			ndata.graphicsdata = loadGraphics( EDGE ) ;
			ndata.weightsdata = loadWeights() ;
			
			if ( debug ) cout << endl ;
		}
		
	} while ( strcmp( "</edge>", token ) != 0 ) ;
	
	return ndata ;
}

/**
 * Carga los atributos de una sección graphics, es decir:
 * @code <graphics>
 * @code 	x=25
 * @code 	y=36
 * @code 	.
 * @code 	.
 * @code 	.
 * @code </graphics>
 * @internal
 * @param stream Referencia al stream a ser leido
 * @param numberline Referencia al número de linea, para llevar la cuenta de numero de lineas recorridas
 * @return Los datos asociados son encapsulados en una estructura del tipo @see GraphicsData
 */
struct SwapFile::GraphicsData SwapFile::loadGraphics( TypeItem typeitem )
{
	GraphicsData ndata ;
	char* token = new char[255];
	
	do {
		fscanf( file, "%s", token ) ;

		if ( strcmp( "<graphics>", token ) == 0 ) {
			if ( debug ) {
				cout << "-------------------------------------------" << endl ;
				cout << " Loading Graphics ...                      " << endl ;
				cout << "-------------------------------------------" << endl ;
			}
			
			loadPair( "colorSelected", ndata.colorSelected ) ;
			loadPair( "colorUnselected", ndata.colorUnselected ) ;
			loadPair( "colorName", ndata.colorName ) ;
				
			if( typeitem == NODE ){
				loadPairInt( "x", ndata.x ) ;
				loadPairInt( "y", ndata.y ) ;
				loadPairInt( "w", ndata.w ) ;
				loadPairInt( "h", ndata.h ) ;
			}else if( typeitem == EDGE ){
				loadPairInt( "w", ndata.w ) ;
			}
		}
		
	} while ( strcmp( "</graphics>", token ) != 0 ) ;
	
	return ndata ;
}

/**
 * Carga los atributos de una sección typedata, es decir:
 * @param typeitem 
 * @param stream 
 * @param numberline 
 * @return 
 */
QValueList<QgeItemType> SwapFile::loadTypes( TypeItem typeitem )
{
	QValueList<QgeItemType> nDataList ;
	
	char* token = new char[255];
	
	fscanf( file, "%s", token ) ;
	
	if ( strcmp( "<nodeTypes>", token ) == 0  & typeitem == NODE ) {
		if ( debug ) {
			cout << "-------------------------------------------" << endl ;
			cout << " Loading nodeTypes ...                     " << endl ;
			cout << "-------------------------------------------" << endl ;
		}
		
		do {
			float data1 = 0.0 ;
			float data2 = 0.0 ;
			fscanf( file, "%s%f%f", token, &data1, &data2 ) ;
			
			if( strcmp( "</nodeTypes>", token ) != 0 ){
				QgeItemType ndata = QgeItemType( token, (double)data1, (double)data2 ) ;
				
				if( debug )
					cout << "Parsing: (" << token << ", " << data1 << ", " << data2 << " )"<< endl ;
					
				nDataList.append( ndata ) ;
			}
			
		} while ( strcmp( "</nodeTypes>", token ) != 0 ) ;
	}
	
	if ( strcmp( "<edgeTypes>", token ) == 0  & typeitem == EDGE ) {
		if ( debug ) {
			cout << "-------------------------------------------" << endl ;
			cout << " Loading edgeTypes ...                     " << endl ;
			cout << "-------------------------------------------" << endl ;
		}
		
		do {
			float data1 = 0.0 ;
			float data2 = 0.0 ;
			fscanf( file, "%s%f%f", token, &data1, &data2 ) ;
			
			if( strcmp( "</edgeTypes>", token ) != 0 ){
				QgeItemType ndata = QgeItemType( token, (double)data1, (double)data2 ) ;
				
				if( debug )
					cout << "Parsing: (" << token << ", " << data1 << ", " << data2 << " )"<< endl ;
					
				nDataList.append( ndata ) ;
			}
			
		} while ( strcmp( "</edgeTypes>", token ) != 0 ) ;
	}
	
	return nDataList ;
}

/**
 * Carga los atributos de una sección weights, es decir:
 * @code <weights>
 * @code 	PC1=3.4561
 * @code 	PC2=5.1561
 * @code 	.
 * @code 	.
 * @code 	.
 * @code </weights>
 * @internal
 * @param stream Referencia al stream a ser leido
 * @param numberline Referencia al número de linea, para llevar la cuenta de numero de lineas recorridas
 * @return Los datos asociados son encapsulados en una estructura del tipo @see WeightsData
 */
struct SwapFile::WeightsData SwapFile::loadWeights()
{
	WeightsData ndata ;
	
	char* token = new char[255];
	
	fscanf( file, "%s", token ) ;
	
	if ( strcmp( "<weights>", token ) == 0 ) {
		if ( debug ) {
			cout << "-------------------------------------------" << endl ;
			cout << " Loading Weights ...                      " << endl ;
			cout << "-------------------------------------------" << endl ;
		}
		
		do {
			fscanf( file, "%s", token ) ;
			
			if( strcmp( "</weights>", token ) != 0 ){
				char* key = strtok( token, "=" ) ;
				double data = atof( strtok(NULL, "=") ) ;
				ndata.data[ QString(key) ] = data ;
				
				if( debug )
					cout << "Parsing: (" << key << ", " << data << " )"<< endl ;
			}else{
				break ;
			}
			
		} while ( strcmp( "</weights>", token ) != 0 ) ;
	}
	
	return ndata ;
}

