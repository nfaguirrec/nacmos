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
#include "qgewidget.h"
#include <pixmaps.h>
#include <pixmapsconfig.h>

#include <qaction.h>
#include <qimage.h>
#include <qfiledialog.h>
#include <qpopupmenu.h>
#include <qcursor.h>
#include <qerrormessage.h>
#include <qwmatrix.h>

#include <qgegraph.h>
#include <qgenode.h>
#include <qgeedge.h>
#include <qgeconfigdialognode.h>
#include <qgeconfigdialogedge.h>
#include <qgeconfigdialoggraph.h>

QgeWidget::QgeWidget( QWidget* parent, const char* name ):
QCanvasView(parent, name){
	state = INSERT_NODE ;
	
	actualFileName = QString::null ;
	
	nodeFrom = 0 ;
	nodeMoving = 0 ;
}

QgeWidget::QgeWidget( QCanvas* canvas, QWidget* parent, const char* name ):
QCanvasView(canvas, parent, name){
	state = INSERT_NODE ;
	
	actualFileName = QString::null ;
	
	nodeFrom = 0 ;
	nodeMoving = 0 ;
	
	line = new QCanvasLine( this->canvas() ) ;
	line->setPen( QPen( Qt::green, 2, Qt::DashLine ) ) ;
	
	rect = new QCanvasRectangle( this->canvas() ) ;
	rect->setPen( QPen( Qt::green, 2, Qt::DashLine ) ) ;
	
	QImage img ;
	img.load( "logo/banner2.png", "PNG") ;
	this->canvas()->setBackgroundPixmap( img ) ;
	
	initComponents() ;
}

QgeWidget::~QgeWidget(){
	delete nodeFrom ;
	delete nodeMoving ;
	delete line ;
	delete rect ;
}

/**
 * Retorna el grupo de acciones que correspondesn a :\n
 * 1- Mover el grafo \n
 * 2- Insertar un nodo \n
 * 3- Remover un nodo \n
 * 4- Mover un Nodo \n
 * 5- Insertar una arista \n
 * 6- Remover una arista \n
 * Como los anteriores corresponden a los posibles estados 
 * del componente y por lo tanto son mutuamente excluyentes 
 * tienen activada la opcion de toggled()
 * @return 
 */
QActionGroup* QgeWidget::getStateComponentActions(){
	return stateComponentActionGroup ;
}

/**
 * Retorna el grupo de acciones que corresponden a: \n
 * 1- Nuevo grafo ( New ) \n
 * 2- Cargar un grafo desde archivo ( Load ... ) \n
 * 3- Salvar el grafo en edición ( Save .. ) \n
 * 4- Salvar el grafo con un nuevo nomber ( Save As ...) \n
 * 5- Recargar el grafo ( Reload ) \n
 * @return 
 */
QActionGroup* QgeWidget::getFileOperations(){
	return fileOperationsActionGroup ;
}

QgeGraph* QgeWidget::getGraph(){
	return qgegraph ;
}

/**
 * Crea una copia del grafo dado y lo muestra en el componente
 * @param qgegraph 
 */
void QgeWidget::setGraph( QgeGraph* qgegraph ){
	this->qgegraph->remove() ;
	this->qgegraph->copy( qgegraph ) ;
// 	canvas()->update() ;
}

void QgeWidget::setCanvas( QCanvas* canvas ){
	QCanvasView::setCanvas( canvas ) ;
	
	line = new QCanvasLine( this->canvas() ) ;
	line->setPen( QPen( Qt::green, 2, Qt::DashLine ) ) ;
	
	rect = new QCanvasRectangle( this->canvas() ) ;
	rect->setPen( QPen( Qt::green, 2, Qt::DashLine ) ) ;
	
	initComponents() ;
}

void QgeWidget::initComponents(){
	qgegraph = new QgeGraph( canvas() ) ;
	
	initStateComponentActions() ;
	initFileOperations() ;
	
	popupmenu = new QPopupMenu( this, "Qge Options" ) ;
	stateComponentActionGroup->addTo( popupmenu ) ;
	
	QImage img ;
	img.loadFromData( image_config_data, sizeof( image_config_data ), "PNG" );
	graphProperties = new QAction( "Graph Properties", QKeySequence(tr("p")), this, "Graph Properties" ) ;
	graphProperties->setIconSet( QIconSet( img ) ) ;
	connect( graphProperties, SIGNAL( activated() ), this, SLOT( showConfigDialogGraph() ) ) ;
	graphProperties->addTo( popupmenu ) ;
	
	QPopupMenu *tmp = new QPopupMenu( popupmenu, "tmp" ) ;
	tmp->insertTearOffHandle();
	fileOperationsActionGroup->addTo( tmp ) ;
	popupmenu->insertItem( "File Actions", tmp ) ;	
}

void QgeWidget::initStateComponentActions(){

	QImage img ;
	
	img.loadFromData( image_movegraph_data, sizeof( image_movegraph_data ), "PNG" );
	QAction* moveGraph = new QAction( "Move Graph", QKeySequence(tr("Shift+g")), this, "Move Graph" ) ;
	moveGraph->setIconSet( QIconSet( img ) ) ;
	moveGraph->setToggleAction( TRUE ) ;
		
	img.loadFromData( image_movenode_data, sizeof( image_movenode_data ), "PNG" );
	QAction* moveNode = new QAction( "Move Node", QKeySequence(tr("Shift+m")), this, "Move Node" ) ;
	moveNode->setIconSet( QIconSet( img ) ) ;
	moveNode->setToggleAction( TRUE ) ;
	
	img.loadFromData( image_insertnode_data, sizeof( image_insertnode_data ), "PNG" );
	QAction* insertNode = new QAction( "Insert Node", QKeySequence(tr("n")), this, "Insert Node" ) ;
	insertNode->setIconSet( QIconSet( img ) ) ;
	insertNode->setToggleAction( TRUE ) ;
	insertNode->setOn( TRUE ) ;
	
	img.loadFromData( image_hidenode_data, sizeof( image_hidenode_data ), "PNG" );
	QAction* hideNode = new QAction( "Hide Node", QKeySequence(tr("h")), this, "Hide Node" ) ;
	hideNode->setIconSet( QIconSet( img ) ) ;
	hideNode->setToggleAction( TRUE ) ;

	img.loadFromData( image_removenode_data, sizeof( image_removenode_data ), "PNG" );
	QAction* removeNode = new QAction( "Remove Node", QKeySequence(tr("Shift+n")), this, "Remove Node" ) ;
	removeNode->setIconSet( QIconSet( img ) ) ;
	removeNode->setToggleAction( TRUE ) ;
	
	img.loadFromData( image_insertedge_data, sizeof( image_insertedge_data ), "PNG" );
	QAction* insertEdge = new QAction( "Insert Edge", QKeySequence(tr("e")), this, "Insert Edge" ) ;
	insertEdge->setIconSet( QIconSet( img ) ) ;
	insertEdge->setToggleAction( TRUE ) ;
	
	img.loadFromData( image_hideedge_data, sizeof( image_hideedge_data ), "PNG" );
	QAction* hideEdge = new QAction( "Hide Edge", QKeySequence(tr("Shift+h")), this, "Hide Edge" ) ;
	hideEdge->setIconSet( QIconSet( img ) ) ;
	hideEdge->setToggleAction( TRUE ) ;
	
	img.loadFromData( image_removeedge_data, sizeof( image_removeedge_data ), "PNG" );
	QAction* removeEdge = new QAction( "Remove Edge", QKeySequence(tr("Shift+e")), this, "Remove Edge" ) ;
	removeEdge->setIconSet( QIconSet( img ) ) ;
	removeEdge->setToggleAction( TRUE ) ;
	
	img.loadFromData( image_removeedge_data, sizeof( image_removeedge_data ), "PNG" );
	QAction* dijkstraPath = new QAction( "Get Dijkstra Path", QKeySequence(tr("d")), this, "Get Dijkstra Path" ) ;
	dijkstraPath->setIconSet( QIconSet( img ) );
	dijkstraPath->setToggleAction( TRUE ) ;

	
	stateComponentActionGroup = new QActionGroup( this, "State Component Actions") ;
	
	/*--------------------------------------------------------------------------------------------------
	* Distribución del stateComponentActionGroup
	*/
	stateComponentActionGroup->add( moveGraph ) ;
	stateComponentActionGroup->addSeparator() ;
	stateComponentActionGroup->add( moveNode ) ;
	stateComponentActionGroup->add( insertNode ) ;
	stateComponentActionGroup->add( hideNode) ;
	stateComponentActionGroup->add( removeNode ) ;
	stateComponentActionGroup->addSeparator() ;
	stateComponentActionGroup->add( insertEdge ) ;
	stateComponentActionGroup->add( hideEdge) ;
	stateComponentActionGroup->add( removeEdge ) ;
	stateComponentActionGroup->addSeparator() ;
	stateComponentActionGroup->add( dijkstraPath ) ;
	
	stateComponentActionGroup->setExclusive(TRUE) ;
	connect( stateComponentActionGroup, SIGNAL( selected( QAction* ) ), this, SLOT( changeState(QAction*) ) ) ;
	//--------------------------------------------------------------------------------------------------
}

void QgeWidget::initFileOperations(){

	QImage img ;
	
	img.loadFromData( image_new_data, sizeof( image_new_data ), "PNG" );
	QAction* newGraph = new QAction( "New Graph", QKeySequence(tr("Ctrl+n")), this, "New Graph" ) ;
	newGraph->setIconSet( QIconSet( img ) ) ;
	
	img.loadFromData( image_load_data, sizeof( image_load_data ), "PNG" );
	QAction* loadGraph = new QAction( "Load Graph ... ", QKeySequence(tr("Ctrl+l")), this, "Load Graph" ) ;
	loadGraph->setIconSet( QIconSet( img ) ) ;
	
	img.loadFromData( image_save_data, sizeof( image_save_data ), "PNG" );
	QAction* saveGraph = new QAction( "Save Graph", QKeySequence(tr("Ctrl+s")), this, "Save Graph" ) ;
	saveGraph->setIconSet( QIconSet( img ) ) ;
	
	img.loadFromData( image_saveas_data, sizeof( image_saveas_data ), "PNG" );
	QAction* saveGraphAs = new QAction( "Save Graph As ...", QKeySequence( QString::null ), this, "Save Graph As" ) ;
	saveGraphAs->setIconSet( QIconSet( img ) ) ;
	
	img.loadFromData( image_reload_data, sizeof( image_reload_data ), "PNG" );
	QAction* reloadGraph = new QAction( "Reload Graph", QKeySequence(tr("F5")), this, "Reload Graph" ) ;
	reloadGraph->setIconSet( QIconSet( img ) ) ;

	fileOperationsActionGroup = new QActionGroup( this, "File Operation Actions") ;
	
	/*--------------------------------------------------------------------------------------------------
	* Distribución del stateComponentActionGroup
	*/
	fileOperationsActionGroup->add(newGraph) ;
	fileOperationsActionGroup->add(loadGraph) ;
	fileOperationsActionGroup->addSeparator() ;
	fileOperationsActionGroup->add(saveGraph) ;
	fileOperationsActionGroup->add(saveGraphAs) ;
	fileOperationsActionGroup->addSeparator() ;
	fileOperationsActionGroup->add(reloadGraph) ;
	fileOperationsActionGroup->addSeparator() ;
	
	connect( newGraph, SIGNAL( activated() ), this, SLOT( newGraph() ) ) ;
	connect( loadGraph, SIGNAL( activated() ), this, SLOT( load() ) ) ;
	connect( saveGraph, SIGNAL( activated() ), this, SLOT( save() ) ) ;
	connect( saveGraphAs, SIGNAL( activated() ), this, SLOT( saveAs() ) ) ;
	connect( reloadGraph, SIGNAL( activated() ), this, SLOT( reload() ) ) ;
	//--------------------------------------------------------------------------------------------------
}

/**
 * Dependiendo del nombre del objeto QAction recibido, el metodo se
 * encarga de cambiar el estado del componente ( \ref state )
 * @param action 
 */
void QgeWidget::changeState(QAction* action){
// 	qgegraph->selectAll(FALSE) ;
	
	if( QString(action->name()).compare("Move Graph") == 0 ){
		cout << "Change to state: Move Graph" << endl ;
		state = MOVE_GRAPH ;
	}
	
	if( QString(action->name()).compare("Move Node") == 0 ){
		cout << "Change to state: Move Node" << endl ;
		state = MOVE_NODE ;
	}
	
	if( QString(action->name()).compare("Insert Node") == 0 ){
		cout << "Change to state: Insert Node" << endl ;
		state = INSERT_NODE ;
	}
		
	if( QString(action->name()).compare("Hide Node") == 0 ){
		cout << "Change to state: Hide Node" << endl ;
		state = HIDE_NODE ;
	}
		
	if( QString(action->name()).compare("Remove Node") == 0 ){
		cout << "Change to state: Remove Node" << endl ;
		state = REMOVE_NODE ;
	}
		
	if( QString(action->name()).compare("Insert Edge") == 0 ){
		cout << "Change to state: Insert edge" << endl ;
		state = INSERT_EDGE ;
	}
		
	if( QString(action->name()).compare("Hide Edge") == 0 ){
		cout << "Change to state: Hide edge" << endl ;
		state = HIDE_EDGE ;
	}
		
	if( QString(action->name()).compare("Remove Edge") == 0 ){
		cout << "Change to state: Remove edge" << endl ;
		state = REMOVE_EDGE ;
	}
	
	if( QString(action->name()).compare("Get Dijkstra Path") == 0 ){
		cout << "Change to state: Get Dijkstra Path" << endl ;
		state = GET_DIJKSTRA_PATH ;
	}
	
	qgegraph->setSelected(FALSE) ;
}

/**
 * @param state
 */
void QgeWidget::changeState( int state ){
	this->state = state ;
}

void QgeWidget::centerGraph(){
	qgegraph->move( canvas()->width()/2, canvas()->height()/2 ) ;
	canvas()->update() ;
} 
 
/**
 * Remueve el grafo en edicion y actualiza el canvas
 */
void QgeWidget::newGraph(){
	cout << "Removing graph ... " ;
	qgegraph->remove() ;
	canvas()->update() ;
	cout << "OK" << endl ;
}

void QgeWidget::load(){
	QString fileName = QFileDialog::getOpenFileName(
			".",
			"GML (*.gml)",
			this,
			"open file dialog",
			"Choose a file" );
	if( fileName != QString::null ){
		actualFileName = fileName ; 
		
		cout << "Loading graph from " << actualFileName << " ... " ;
		qgegraph->load( fileName ) ;
		qgegraph->move( canvas()->width()/2, canvas()->height()/2 ) ;
		canvas()->setAllChanged() ;
		canvas()->update() ;
		cout << "OK" << endl ;
	}
}

void QgeWidget::load( QString fileName ){
	actualFileName = fileName ;
	
	cout << "Loading graph from " << actualFileName << " ... " ;
	qgegraph->load( fileName ) ;
	qgegraph->move( canvas()->width()/2, canvas()->height()/2 ) ;
	canvas()->setAllChanged() ;
	canvas()->update() ;
	cout << "OK" << endl ;
}

void QgeWidget::save(){
	if( actualFileName.isNull() )
		saveAs() ;
	else{
		cout << "Saving graph to " << actualFileName << " ... " ;
		qgegraph->save( actualFileName ) ;
		cout << "OK" << endl ;
	}
}

void QgeWidget::saveAs(){
	QString fileName = QFileDialog::getSaveFileName(
			".",
			"GML (*.gml)",
			this,
			"save file dialog",
			"Choose a filename to save under" );
	if( fileName != QString::null ){
		actualFileName = fileName ;
		
		cout << "Saving graph to " << actualFileName << " ... " ;
		qgegraph->save( fileName ) ;
		cout << "OK" << endl ;
	}
}

void QgeWidget::reload(){
	if( !actualFileName.isNull() ){
		cout << "Reloading graph from ... " << actualFileName ;
		qgegraph->load( actualFileName ) ;
		qgegraph->move( canvas()->width()/2, canvas()->height()/2 ) ;
		canvas()->setAllChanged() ;
		canvas()->update() ;
		cout << "OK" << endl ;
	}else{
		QErrorMessage* qerrormessage = new QErrorMessage(0, "qerrormessage") ;
		qerrormessage->message("You need select a file name") ;
		qerrormessage->exec() ;
		
		load() ;
	}
}

void QgeWidget::showConfigDialogGraph(){
	QgeConfigDialogGraph* qgeconfigdialoggraph = new QgeConfigDialogGraph( qgegraph, this ) ;
	if ( qgeconfigdialoggraph->exec() == QDialog::Accepted ){
		qgeconfigdialoggraph->updateObject() ;
	}
}

void QgeWidget::contentsMousePressEvent( QMouseEvent* e ){

	if(e->button() == Qt::LeftButton){
		QCanvasItemList l = canvas()->collisions(e->pos());
		
		switch(state){
			case MOVE_GRAPH:
				rect->show() ;
				break ;
			case MOVE_NODE:
				for ( QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it ) {
					if( (*it)->rtti() == QgeItem::Rtti_Node ){
						nodeMoving = (QgeNode*)*it;
						break ;
					}
				}
				break ;
			case INSERT_NODE:
				qgegraph->newNode(e->pos().x(), e->pos().y()) ;
				cout << "Inserting new Node (" << e->x() << ", " << e->y() << ") ... OK" << endl ;
				break ;
			case HIDE_NODE:
				for ( QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it ) {
					if( (*it)->rtti() == QgeItem::Rtti_Node ){
						qgegraph->hideNode( (QgeNode*)*it ) ;
						cout << "Hidding Node (" << ((QgeNode*)*it)->x() << ", " << ((QgeNode*)*it)->y() << ") ... OK" << endl ;
						break ;
					}
				}
				break ;
			case REMOVE_NODE:
				for ( QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it ) {
					if( (*it)->rtti() == QgeItem::Rtti_Node ){
						qgegraph->removeNode((QgeNode*)*it) ;
						cout << "Removing Node (" << ((QgeNode*)*it)->x() << ", " << ((QgeNode*)*it)->y() << ") ... OK" << endl ;
						break ;
					}
				}
				break ;
			case INSERT_EDGE:
				for ( QCanvasItemList::Iterator it = l.begin(); it != l.end(); ++it ){
					if( (*it)->rtti() == QgeItem::Rtti_Node ){
						if( !nodeFrom ){
							nodeFrom = (QgeNode*)*it ;
							line->show() ;
						}
					}
				}
				break ;
				
			case HIDE_EDGE:
				for ( QCanvasItemList::Iterator it = l.begin(); it != l.end(); ++it ){
					if( (*it)->rtti() == QgeItem::Rtti_Edge ){
						qgegraph->hideEdge( (QgeEdge*)*it ) ;
					}
				}
				break ;
				
			case REMOVE_EDGE:
				for ( QCanvasItemList::Iterator it = l.begin(); it != l.end(); ++it ) {
					if( (*it)->rtti() == QgeItem::Rtti_Edge ){
						qgegraph->removeEdge( (QgeEdge*)*it );
						break ;
					}
				}
				break ;
				
			case GET_DIJKSTRA_PATH:
				for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it){
					if((*it)->rtti() == QgeItem::Rtti_Node){
						if( !nodeFrom ){
							qgegraph->selectAll(FALSE) ;
							
							nodeFrom = (QgeNode*)*it ;
							nodeFrom->setSelected(TRUE) ;
							
							line->show() ;
						}
					}
				}
				break ;
		}
		canvas()->update() ;
	}
	
	if( e->button() == Qt::RightButton ){
		bool in = FALSE ;
	
		QCanvasItemList l = canvas()->collisions( e->pos() );
		
		for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) {
		
			in = TRUE ;
			if( (*it)->rtti() == QgeItem::Rtti_Node ){
				QgeConfigDialogNode* qgeconfigdialognode = new QgeConfigDialogNode( (QgeNode*)*it, qgegraph->getNodeTypes().keys(), this ) ;
				if (qgeconfigdialognode->exec() == QDialog::Accepted){
					qgeconfigdialognode->updateObject() ;
					canvas()->update() ;
				}
				break ;
			}
			
			if( (*it)->rtti() == QgeItem::Rtti_Edge ){
				QgeConfigDialogEdge* qgeconfigdialogedge = new QgeConfigDialogEdge( (QgeEdge*)*it, qgegraph->getEdgeTypes().keys(), this ) ;
				if (qgeconfigdialogedge->exec() == QDialog::Accepted){
					qgeconfigdialogedge->updateObject() ;
					canvas()->update() ;
				}
				break ;
			}
		}
		
		if( !in )
			popupmenu->exec( QCursor::pos() ) ;

	}
	
}

void QgeWidget::contentsMouseMoveEvent( QMouseEvent* e ){
	if ( nodeMoving ){
		qgegraph->moveNode( nodeMoving, e->x(), e->y() ) ;
		canvas()->update() ;
	}
	
	if( state == MOVE_GRAPH ){
		QRect rectTmp = qgegraph->boundingRect() ;
		rect->setSize( rectTmp.width(), rectTmp.height() ) ;
		rect->setX( e->x() - rectTmp.width()/2 ) ;
		rect->setY( e->y() - rectTmp.height()/2 ) ;

		canvas()->update() ;
	}
		
	if( nodeFrom ){
		line->setPoints( nodeFrom->x(), nodeFrom->y(), e->x(), e->y() ) ;
		canvas()->update() ;
	}
}

void QgeWidget::contentsMouseReleaseEvent( QMouseEvent* e){
	if ( nodeMoving ) 
		nodeMoving = 0 ;
		
	if( state == MOVE_GRAPH ){
		qgegraph->move( e->x(), e->y() ) ;
		
		rect->hide() ;
		canvas()->update() ;
	}
	
	if( nodeFrom ){
		QCanvasItemList l = canvas()->collisions(e->pos());
		
		for ( QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it ){
			if( (*it)->rtti() == QgeItem::Rtti_Node ){
				if( nodeFrom != (QgeNode*)*it ){
					if( state == INSERT_EDGE ){
						cout << "Inserting new Edge ... OK" << endl ;
						qgegraph->newEdge( nodeFrom,  (QgeNode*)*it ) ;
					}
					
					if( state == GET_DIJKSTRA_PATH ){
						cout << "Getting get dijkstra path ... OK" << endl ;
						dijkstra( *nodeFrom, *(QgeNode*)*it ) ;
					}
					
					nodeFrom = 0 ;
					
					line->hide() ;
					setMouseTracking( FALSE ) ;
				}
			}else{
				nodeFrom = 0 ;
				
				line->hide() ;
				setMouseTracking( FALSE ) ;
			}
		}
		canvas()->update() ;
	}
}

void QgeWidget::viewportResizeEvent( QResizeEvent * ){
	if( canvas() ){
		canvas()->resize( width() - 4, height() - 4 ) ;
		qgegraph->move( canvas()->width()/2, canvas()->height()/2 ) ;
		canvas()->update() ;
	}
}

void QgeWidget::dijkstra( QgeNode nInit, QgeNode nEnd ){

	edge_map<double> w(*qgegraph);
	for( graph::edge_iterator it = qgegraph->edges_begin() ; it != qgegraph->edges_end(); it++)
		w[*it] = 1.0 ;
	
	GTL::dijkstra B;
	B.store_preds(true);
	B.source(nInit);
	B.weights(w);
	
	if (B.check(*qgegraph) != algorithm::GTL_OK){
		cout << "check FAILED" << endl;
		return ;
	}

	if (B.run(*qgegraph) != algorithm::GTL_OK){
		cout << "run FAILED" << endl;
		return ;
	}    

	dijkstra::shortest_path_node_iterator itNode = B.shortest_path_nodes_begin(nEnd) ;
	for( dijkstra::shortest_path_node_iterator it = itNode; it != B.shortest_path_nodes_end(nEnd); ++it){
		qgegraph->nodeToQgeNode(*it)->setSelected(TRUE) ;
	}

	dijkstra::shortest_path_edge_iterator itEdge = B.shortest_path_edges_begin(nEnd) ;
	for( dijkstra::shortest_path_edge_iterator it = itEdge; it != B.shortest_path_edges_end(nEnd); ++it){
		qgegraph->edgeToQgeEdge(*it)->setSelected(TRUE) ;
	}
	
}

void QgeWidget::contentsWheelEvent( QWheelEvent* e )
{
	if ( e->delta() > 0 ){
		
		QWMatrix m = worldMatrix();
		m.scale( 0.9, 0.9 );
		setWorldMatrix( m );
		
	}else if ( e->delta() < 0 ){
		
		QWMatrix m = worldMatrix();
		m.scale( 1.11111, 1.11111 );
		setWorldMatrix( m );
		
	}
	
	setContentsPos( e->pos().x(), e->pos().y() ) ;
	center( e->pos().x(), e->pos().y() ) ;
	
// 	canvas()->update();
// 	update() ;
}
