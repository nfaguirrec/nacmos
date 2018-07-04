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
#ifndef QGEWIDGET_H
#define QGEWIDGET_H

#include <GTL/dijkstra.h>

#include <qcanvas.h>

class QActionGroup ;
class QAction ;
class QPopupMenu ;

class QgeGraph ;
class QgeNode ;
class QgeGeneralConfigDialog ;

/**
@author Nestor Aguirre
*/
class QgeWidget : public QCanvasView {
	Q_OBJECT
	
	Q_ENUMS( States )
	
	public:
		QgeWidget( QWidget* parent, const char* name ) ;
		QgeWidget( QCanvas* canvas, QWidget* parent, const char* name ) ;
		~QgeWidget() ;
		
		enum State {
			MOVE_GRAPH,
			INSERT_NODE,
			HIDE_NODE,
			REMOVE_NODE,
			MOVE_NODE,
			INSERT_EDGE,
			HIDE_EDGE,
			REMOVE_EDGE,
			GET_DIJKSTRA_PATH
		};
		
		QActionGroup* getStateComponentActions() ;
		QActionGroup* getFileOperations() ;
		QgeGraph* getGraph() ;
		
		void setGraph( QgeGraph* qgegraph ) ;
		void setCanvas( QCanvas* canvas ) ;
		
		void dijkstra( QgeNode nInit, QgeNode nEnd ) ;
		
	private:
		void initComponents() ;
		void initStateComponentActions() ;
		void initFileOperations() ;
		
	public slots:
		void changeState( QAction* action ) ;
		void changeState( int state ) ;
		void centerGraph() ;
			
		void newGraph() ;
		void load() ;
		void load( QString fileName ) ;
		void save() ;
		void saveAs() ;
		void reload() ;
		
		void showConfigDialogGraph() ;
			
	protected slots:
		virtual void contentsMousePressEvent( QMouseEvent* e ) ;
		virtual void contentsMouseMoveEvent( QMouseEvent* e ) ;
		virtual void contentsMouseReleaseEvent( QMouseEvent* e ) ;
		virtual void viewportResizeEvent( QResizeEvent *e ) ;
		virtual void contentsWheelEvent( QWheelEvent * e ) ;
		
	private:
		QgeGraph* qgegraph ;
		
		QActionGroup* stateComponentActionGroup ;
		QActionGroup* fileOperationsActionGroup ;
		QAction* graphProperties ;
		QPopupMenu* popupmenu ;
		
		int state ;
		QgeNode* nodeMoving ;
		QgeNode* nodeFrom ;
		
		QCanvasLine* line ;
		QCanvasRectangle* rect ;
		QString actualFileName ;
};

#endif
